#include "StdAfx.h"
#include "CandidateRegionSelection.h"

#include "QEstimator.h"

CandidateRegionSelection::CandidateRegionSelection(void)
{
}


CandidateRegionSelection::~CandidateRegionSelection(void)
{
}
/*
	sortAscendingSubImages

	Purpose: sort function for sorting the array of mse values mentioned at section III-B-1 => s3

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
inline bool sortAscendingSubImages(const SubImageInfo& i, const SubImageInfo& j)
{
	if(i.MSE < j.MSE )
		return true;
	else
		return false;
}
/*
	SelectSeedRegion

	Purpose: finds the initial seed mentioned at section III-B

	Author: Sadegh Soleimani

	Inputs:							
		srcImage : the source image
		
	outputs:
		arrSeedSubImages : the final seed region contains 4 subimages
		arrUnRefineSubimages : remaining subimages of original image which must be process in next step of algorithm
*/
int CandidateRegionSelection::SelectSeedRegion(CvMat* srcImage, vector<SubImageInfo>& arrSeedSubImages, vector<SubImageInfo>& arrUnRefineSubimages)
{
	const int SubimageWidth = 128;
	const int SubimageHeight = 128;
	const int ThresholdT = 48;// threshold for Tc mentioned at section III-B-1 =>S3
	const int NS = 4;//threshold for minimum number of subimages in seed
	int xDirSubImageCount = srcImage->cols / SubimageWidth;
	int yDirSubImageCount = srcImage->rows / SubimageHeight;
	int seedImagesCount = 0;
	SubImageInfo selectedSeedSubimage;
	vector<SubImageInfo> arrSubimagesInfo;
	QEstimator QE;
	SubImageInfo subImageInfo;
	for(int i = 0 ; i < yDirSubImageCount ; i++)//calculates the Q Table of every subimage
	{
		for(int j = 0 ; j < xDirSubImageCount; j++)
		{
			CvMat* subImage = cvCreateMatHeader(SubimageHeight, SubimageWidth, CV_32F);
			cvGetSubRect(srcImage, subImage, cvRect(j * SubimageWidth, i * SubimageHeight, SubimageWidth, SubimageHeight));
			subImageInfo.i = i;
			subImageInfo.j = j;
			subImageInfo.QTable = cvCreateMat(8, 8, CV_32F);
			cvZero(subImageInfo.QTable);
			QE.ExtractQTable(subImage, subImageInfo.QTable);
			subImageInfo.MSE = CalculateMSE(subImage, subImageInfo.QTable); 
			subImageInfo.T = CalculateT(subImageInfo.QTable); 
			subImageInfo.image = subImage;
			arrSubimagesInfo.push_back(subImageInfo);
		}
	}
	for(int i = 0 ; i < arrSubimagesInfo.size(); i++)//eliminates each subimage which have t greater than 48(section III-B-1 => s3)
	{
		if(arrSubimagesInfo[i].T > ThresholdT)
		{
			arrSubimagesInfo.erase(arrSubimagesInfo.begin() + i);
			i--;
		}
	}
	sort(arrSubimagesInfo.begin(), arrSubimagesInfo.end(), sortAscendingSubImages);
	if(arrSubimagesInfo.size() < 4)//if the original image be bmp or high guality jpeg then after previous step, 
								   //just tampered subimages will be remain
	{
		for(int i = 0;  i < arrSubimagesInfo.size() ; i++)
		{
			arrSeedSubImages.push_back(arrSubimagesInfo[i]);
		}
		
		return -1;
	}
	int firstSelectedSeedCounter = 0;
	CvMat* seedQTable = cvCreateMat(8, 8, CV_32F);
	while(seedImagesCount < NS && firstSelectedSeedCounter < arrSubimagesInfo.size() )//section III-B-1 =>S5 - S7
	{
		arrSeedSubImages.clear();
		selectedSeedSubimage = arrSubimagesInfo[firstSelectedSeedCounter];
		seedImagesCount = 1;
		arrSeedSubImages.push_back(selectedSeedSubimage);
		arrUnRefineSubimages.clear();
		int i = 0;
		seedQTable = selectedSeedSubimage.QTable;
		for( ; i < arrSubimagesInfo.size() && seedImagesCount < NS; i++)
		{
			if(i == firstSelectedSeedCounter)continue;
			if(CalculateMAE(seedQTable, arrSubimagesInfo[i].QTable) < 1)
			{
				seedImagesCount++;
				arrSeedSubImages.push_back(arrSubimagesInfo[i]);
				CvMat* tempVersion = cvCreateMat(SubimageHeight , seedImagesCount * SubimageWidth, CV_32F);

				CvMat* subImage = cvCreateMatHeader(SubimageHeight, SubimageWidth, CV_32F);
				for(int m = 0; m < seedImagesCount ; m++)//creating the initial version
				{
					cvGetSubRect(tempVersion, subImage, cvRect(m * SubimageWidth, 0, SubimageWidth, SubimageHeight));
					cvCopy(arrSubimagesInfo[i].image, subImage);
				}
				QE.ExtractQTable(tempVersion, seedQTable);
				cvReleaseMat(&tempVersion);
				cvReleaseMatHeader(&subImage);
			}
			else
				arrUnRefineSubimages.push_back(arrSubimagesInfo[i]);
		}
		if(seedImagesCount >= NS && i < arrSubimagesInfo.size() )//add remain subimages to arrSubimagesInfo as U'
		{
			while(i < arrSubimagesInfo.size())
			{
				arrUnRefineSubimages.push_back(arrSubimagesInfo[i]);
				i++;
			}
		}
		firstSelectedSeedCounter++;
	}
	cvReleaseMat(&seedQTable);
	return 0;
}

/*
	CalculateT

	Purpose: calculate number of step size = 1

	Author: Sadegh Soleimani

	Inputs:							
		QTable : the estimated Quantization table
		
	outputs:
		return the number of step size = 1
			 
*/
int CandidateRegionSelection::CalculateT(CvMat* QTable)
{
	int tCounter = 0;
	for(int i = 0 ; i < QTable->rows; i++)
	{
		for(int j = 0 ; j < QTable->cols; j++)
		{
			if(QTable->data.fl[i * QTable->cols + j] == 1)
				tCounter++;
		}
	}
	return tCounter;
}
/*
	CalculateMAE

	Purpose: calculates the absolute difference between two the estimated Q Table(furmula (18))

	Author: Sadegh Soleimani

	Inputs:							
		QTable1 : the first input the estimated Q table
		QTable2 : the second input the estimated Q table
	outputs:
		returns mean absolute error of two inputs	 
*/
double CandidateRegionSelection::CalculateMAE(CvMat* QTable1, CvMat* QTable2)
{
	int QCounter = 0;
	double sumOfQes = 0;
	int x1 , x2;
	for(int i = 0 ; i < QTable1->rows; i++)
	{
		for(int j = 0 ; j < QTable1->cols; j++)
		{
			if(!i && !j)continue;
			x1 = QTable1->data.fl[i * QTable1->cols + j];
			x2 = QTable2->data.fl[i * QTable1->cols + j];
			if(x1 > 0 && x2 > 0 )
			{
				QCounter++;
				if(!(x1 % x2) || !(x2 % x1))continue;
				sumOfQes += abs(x1 - x2);
			}
		}
	}
	if(QCounter)
		return (sumOfQes / QCounter);
	else
		return 0;
}
/*
	CalculateMSE

	Purpose: calculates the mean squared error mentioned at formula (17)

	Author: Sadegh Soleimani

	Inputs:							
		srcSubImage : the source subimage
		QTable : the estimated Q Table of input subimage
	outputs:
		returns the mean squared error mentioned at formula (17)
*/
double CandidateRegionSelection::CalculateMSE(CvMat* srcSubImage, CvMat* QTable)
{
	QEstimator QE;
	CvMat* dstDCT = cvCreateMat(srcSubImage->rows, srcSubImage->cols, CV_32F);
	QE.ExtractDCTCoefficients(srcSubImage, dstDCT);
	int xdirBlockCount = srcSubImage->cols / 8;
	int ydirBlockCount = srcSubImage->rows / 8;
	int totalBlockCount = xdirBlockCount * ydirBlockCount;
	int xQ = 0;
	int xR = 0;
	float x = 0;
	double sumOfQes = 0;
	
	for(int k = 0 ; k < 8 ; k++)
	{
		for(int l = 0 ; l < 8 ; l++)
		{
			if(!k && !l)continue;
			if(QTable->data.fl[k * QTable->cols + l] > 0)
			{
				for(int i = 0 ; i < ydirBlockCount; i++)
				{
					for(int j = 0 ; j < xdirBlockCount; j++)
					{
						x = int(abs(dstDCT->data.fl[(i * 8 + k) * dstDCT->cols + (j * 8 + l)]) + 0.5);
						xQ = int(x / QTable->data.fl[k * QTable->cols + l]);
						xR = int(xQ * QTable->data.fl[k * QTable->cols + l]);
						sumOfQes += (xR - x) * (xR - x);						
					}
				}
			}
		}
	}
	
	cvReleaseMat(&dstDCT);
	return sumOfQes / totalBlockCount;
}

/*
	GrowingAndRefinement

	Purpose: increases the number of genuine sub-images by randomly permuting and re-combining blocks.


	Author: Sadegh Soleimani

	Inputs:							
		arrUnRefineSubimages : the remaining subimages for growing and merging with the seed region
		
	Inputs/outputs:
		seedRegion : the input seed and final condidate region
*/
int CandidateRegionSelection::GrowingAndRefinement(vector<SubImageInfo>& seedRegion, vector<SubImageInfo>& arrUnRefineSubimages)
{
	const int NK = 20;
	int NS = seedRegion.size();
	const int SubimageWidth = 128;
	const int SubimageHeight = 128;
	int totalExpectedSubImages = NS * NK;
	double R1 = 0;
	double finalMAE = 0;
	double sumOfSuitables = 0;
	int randomaizeSteps = 2;
	QEstimator QE;
	CvMat* QTableOfSeed  = cvCreateMat(8, 8, CV_32F); 
	CvMat* tmpVersionHeader = 0;
	vector<SubImageInfo> arrSuspects;
	for(int i = arrUnRefineSubimages.size() -1 ; i >= 0 ; i--)//checks condition of merging and growing for each remain subimage
	{
		randomaizeSteps = 2;
		sumOfSuitables = 0;
		NS++;
		totalExpectedSubImages = NS * NK;
		CvMat* tempVersion = cvCreateMat(SubimageHeight , NS * SubimageWidth, CV_32F);
		tmpVersionHeader = cvCreateMatHeader(SubimageHeight , (NS-1) * SubimageWidth, CV_32F);
		
		CvMat* subImage = cvCreateMatHeader(SubimageHeight, SubimageWidth, CV_32F);
		for(int m = 0; m < NS - 1 ; m++)//creating the initial version
		{
			cvGetSubRect(tempVersion, subImage, cvRect(m * SubimageWidth, 0, SubimageWidth, SubimageHeight));
			cvCopy(seedRegion[m].image, subImage);
		}

		cvGetSubRect(tempVersion, tmpVersionHeader, cvRect(0, 0, (NS-1) *SubimageWidth, SubimageHeight));
		//IplImage* tmpVersionimg = cvCreateImage(cvSize( (NS-1) * SubimageWidth,  SubimageHeight), 8,1);
		//cvScale(tmpVersionHeader, tmpVersionimg);
		//cvShowImage("",tmpVersionimg);cvWaitKey();
		
		QE.ExtractQTable(tmpVersionHeader, QTableOfSeed);
		cvGetSubRect(tempVersion, subImage, cvRect((NS - 1) * SubimageWidth, 0, SubimageWidth, SubimageHeight));
		cvCopy(arrUnRefineSubimages[i].image, subImage);
		vector<SubImageInfo> newRandomSubImages;
		MakeNewTemporaryVersion(tempVersion, newRandomSubImages, totalExpectedSubImages, randomaizeSteps);//recursive function for creating totalExpectedSubImages subimages
		//cvScale(tmpVersionHeader, tmpVersionimg);
		//cvShowImage("",tmpVersionimg);cvWaitKey();
		for(int n = 0 ; n < totalExpectedSubImages ; n++)//calculates Q table for every new random image
		{
			newRandomSubImages[n].QTable = cvCreateMat(8, 8, CV_32F);
			QE.ExtractQTable(newRandomSubImages[n].image, newRandomSubImages[n].QTable);
			
			sumOfSuitables += (CalculateMAE(QTableOfSeed, newRandomSubImages[n].QTable) - 1 > 0)?1:0;
		}
		if(sumOfSuitables / (100) < 0.1)
		{
			seedRegion.push_back(arrUnRefineSubimages[i]);
		}
		else
		{
			NS--;
//CalculateMAE(QTableOfSeed, arrUnRefineSubimages[i].QTable);
			arrSuspects.push_back(arrUnRefineSubimages[i]);
		}

		for(int n = 0 ; n < totalExpectedSubImages ; n++)
		{
			cvReleaseMat(&newRandomSubImages[n].QTable);
			cvReleaseMat(&newRandomSubImages[n].image);
		}
		cvReleaseMat(&tempVersion);
		cvReleaseMatHeader(&subImage);
		cvReleaseMatHeader(&tmpVersionHeader);
	}
	for(int i = 0 ; i < arrSuspects.size();i++)
	{
		arrSuspects[i].MSE = CalculateMAE(QTableOfSeed, arrSuspects[i].QTable);
	}
	sort(arrSuspects.begin(), arrSuspects.end(), sortAscendingSubImages);
	if(arrSuspects.size())	finalMAE = arrSuspects[arrSuspects.size() - 1].MSE;
	else finalMAE = 12;
	cvReleaseMat(&QTableOfSeed);
	return finalMAE * 10;
}

/*
	MakeNewTemporaryVersion

	Purpose: creates recursively new subimages which are create by permuting of blocks in initial temporary version contains (Ns + 1) subimages 

	Author: Sadegh Soleimani

	Inputs:							
		tempVersion : new temporary version of (Ns + 1) subimages 
		totalExpectedSubImages : the final expected number of new random subimages
	outputs:
		newRandomSubImages : contains (Ns + 1) * (Nk + 1) subimages
*/
int CandidateRegionSelection::MakeNewTemporaryVersion(CvMat* tempVersion, vector<SubImageInfo>& newRandomSubImages, int totalExpectedSubImages, int randomaizeSteps)
{
	const int NK = 20;
	const int SubimageWidth = 128;
	const int SubimageHeight = 128;
	
	int xdirBlockCount = SubimageWidth * (totalExpectedSubImages / NK) / 8;
	int ydirBlockCount = SubimageHeight  / 8;

	if(newRandomSubImages.size() == totalExpectedSubImages)
		return 0;
	CvMat* subImageHeader = cvCreateMatHeader(SubimageHeight, SubimageWidth, CV_32F);
	CvMat* tmpSubimage = cvCreateMat(8, 8, CV_32F);
	CvMat* subImage1 = cvCreateMatHeader(8, 8, CV_32F);
	CvMat* subImage2 = cvCreateMatHeader(8, 8, CV_32F);
	SubImageInfo subimageInfo;
	int iRandom1 = 0, iRandom2 = 0, jRandom1 = 0, jRandom2 = 0;
	for(int i = 0; i < totalExpectedSubImages / NK ; i++)//split final created random sub images
	{
		subimageInfo.image = cvCreateMat(SubimageHeight, SubimageWidth, CV_32F);
		cvGetSubRect(tempVersion, subImageHeader, cvRect(i * SubimageWidth, 0, SubimageWidth, SubimageHeight));
		cvCopy(subImageHeader, subimageInfo.image);
		newRandomSubImages.push_back(subimageInfo);
	}
	//creating random version
	int permutCount = randomaizeSteps;
	while(permutCount)
	{
		iRandom1 = rand() % ydirBlockCount;
		iRandom2 = rand() % ydirBlockCount;
		jRandom1 = rand() % xdirBlockCount;
		jRandom2 = rand() % xdirBlockCount;
		cvGetSubRect(tempVersion, subImage1, cvRect(jRandom1 * 8, iRandom1 * 8, 8, 8));
		cvGetSubRect(tempVersion, subImage2, cvRect(jRandom2 * 8, iRandom2 * 8, 8, 8));
		cvCopy(subImage1, tmpSubimage);
		cvCopy(subImage2, subImage1);
		cvCopy(tmpSubimage, subImage2);
		permutCount--;
	}
	MakeNewTemporaryVersion(tempVersion, newRandomSubImages, totalExpectedSubImages, randomaizeSteps);
	cvReleaseMat(&tmpSubimage);
	cvReleaseMatHeader(&subImageHeader);
	cvReleaseMatHeader(&subImage1);
	cvReleaseMatHeader(&subImage2);
	return 0;
}

/*
	ExtractCondidateRegion

	Purpose: Extracts final genuin region which is suitable for final Q Estimation

	Author: Sadegh Soleimani

	Inputs:							
		srcImage : the source image
		
	outputs:
		CandidateRegion : the final detected genuin region as candidate region
*/
int CandidateRegionSelection::ExtractCondidateRegion(CvMat* srcImage, vector<SubImageInfo>& CandidateRegion)
{
	vector<SubImageInfo> arrUnRefineSubimages;
	int errCode = SelectSeedRegion(srcImage, CandidateRegion, arrUnRefineSubimages);
	if(errCode < 0)return errCode;
	errCode = GrowingAndRefinement(CandidateRegion, arrUnRefineSubimages);
	return errCode;
}
