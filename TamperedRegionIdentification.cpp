#include "StdAfx.h"
#include "CandidateRegionSelection.h"
#include "QEstimator.h"
#include "TamperedRegionIdentification.h"


TamperedRegionIdentification::TamperedRegionIdentification(void)
{
}


TamperedRegionIdentification::~TamperedRegionIdentification(void)
{
}

/*
	ExtractCRinfo

	Purpose: Extracts final genuin region which is suitable for final Q Estimation

	Author: Sadegh Soleimani

	Inputs:								
		srcImage : the source image
		
	outputs:
		CandidateRegion : the final detected genuin region as candidate region	 
*/
int TamperedRegionIdentification::ExtractCRinfo(CvMat* srcImage, vector<SubImageInfo>& CandidateRegion)
{
	const int SubimageWidth = 128;
	const int SubimageHeight = 128;

	QEstimator QE;
	CandidateRegionSelection CRS;
	////for test
	//CvMat* QT = cvCreateMat(8, 8, CV_32F);
	//QE.ExtractQTable(srcImage, QT);
	int errCode = CRS.ExtractCondidateRegion(srcImage, CandidateRegion);
	if(errCode< 0)return errCode;
	int condidateSubImageCount = CandidateRegion.size();
	CvMat* tempVersion = cvCreateMat(SubimageHeight , condidateSubImageCount * SubimageWidth, CV_32F);
	CvMat* subImage = cvCreateMatHeader(SubimageHeight, SubimageWidth, CV_32F);
	for(int m = 0; m < condidateSubImageCount ; m++)//creates final genuin region by arranging of detected subimage for final Q Estimation
	{
		cvGetSubRect(tempVersion, subImage, cvRect(m * SubimageWidth, 0, SubimageWidth, SubimageHeight));
		cvCopy(CandidateRegion[m].image, subImage);
	}
	QE.ExtractQTable(tempVersion,CandidateRegion[0].QTable);//the final Q Estimation of candidate region

	return errCode;
}
/*
	CalculateMSE

	Purpose: calculates the absolute error mentioned at formula (18)

	Author: Sadegh Soleimani

	Inputs:							
		srcSubImage : the source subimage
		QTable : the estimated Q Table of input subimage
	outputs:
		returns the absolute error mentioned at formula (18)
*/
double TamperedRegionIdentification::CalculateMSE(CvMat* srcSubImage, CvMat* QTable)
{
	QEstimator QE;
	CvMat* dstDCT = cvCreateMat(srcSubImage->rows, srcSubImage->cols, CV_32F);
	cvDCT(srcSubImage,dstDCT,CV_DXT_FORWARD);
	int xdirBlockCount =  8;
	int ydirBlockCount =  8;
	int xQ = 0;
	int xR = 0;
	double sumOfQes = 0;
	int counter = 0;
	for(int k = 0 ; k < 8 ; k++)
	{
		for(int l = 0 ; l < 8 ; l++)
		{
			//if(!k && !l)continue;
			if(QTable->data.fl[k * QTable->cols + l] > 0)
			{
				xQ = int(ceil(abs(dstDCT->data.fl[k * 8 + l]) ) / QTable->data.fl[k * 8 + l]);
				xR = int(xQ * QTable->data.fl[k * 8 + l]);
				sumOfQes += abs(ceil(abs(dstDCT->data.fl[k * 8 + l]) )  - xR);	
				counter++;
			}
		}
	}
	cvReleaseMat(&dstDCT);
	return sumOfQes ;
}
double FindMAEThreshold(int hist[], int maxMAE)
{
	double MAEThreshold = 0.42;
	double DistanceTh = 50;
	int maxBin = 0;
	for(int i = maxMAE ; i > 0 ; i--)
	{
		if(hist[i] > maxBin)
		{
			maxBin = hist[i];
		}
	}
	DistanceTh = max(maxBin / 10, 20);
	for(int i = maxMAE ; i > 1 ; i--)
	{
		if(hist[i - 1] - hist[i] > DistanceTh)
		{
			MAEThreshold = double(i) / maxMAE;
			MAEThreshold += double(i-1) / maxMAE;
			break;
		}
	}
	return MAEThreshold / 2.0;
}
/*
	TemperDetection

	Purpose: determines that each block is tampered or not, according to inconsistency of Q table and calculated MAE of the block

	Author: Sadegh Soleimani

	Inputs:							
		srcImage : the source image which likely contains the forgery region
		
	outputs:
			 
*/
int TamperedRegionIdentification::TemperDetection(CvMat* srcImage1)
{
	int nHeight = srcImage1->rows, nWidth = srcImage1->cols;
	CvMat* srcImageGray = cvCreateMat(nHeight, nWidth, CV_8UC1);
	CvMat* srcImage2 = cvCreateMat(nHeight, nWidth, CV_32FC3);
	CvMat* srcImage = cvCreateMat(nHeight, nWidth, CV_32F);
	CvMat* srcImageB = cvCreateMat(nHeight, nWidth, CV_32F);
	CvMat* srcImageG = cvCreateMat(nHeight, nWidth, CV_32F);
	CvMat* srcImageR = cvCreateMat(nHeight, nWidth, CV_32F);
	cvCvtColor(srcImage1, srcImageGray, CV_BGR2GRAY);
	cvScale(srcImageGray, srcImage);
	cvScale(srcImage1, srcImage2);
	cvSplit(srcImage2, srcImageB, srcImageG, srcImageR, 0);
	vector<SubImageInfo> CandidateRegion;
	int errCode = ExtractCRinfo(srcImage, CandidateRegion);
	CvMat* srcROI = cvCreateMatHeader(8, 8, CV_32F);
	int xdirBlockCount = srcImage->cols / 8;
	int ydirBlockCount = srcImage->rows / 8;
	double minMAE = 10000000;
	double maxMAE = 0;
	double MAE,MAEr,MAEg,MAEb;
	double MAEDistance = 0;
	double MAEThreshold = 0.08;//threshold for forgery detection according to figure 8
//	if(errCode < 0)MAEThreshold = 0.65;//if source image be bmp(not compressed)
//	else
//		MAEThreshold += errCode / 1000.0; 
//MAEThreshold = 0.21;
	CvMat* blockChecker = cvCreateMat(ydirBlockCount, xdirBlockCount, CV_32F);
	CvMat* destImg = cvCreateMat(srcImage->rows, srcImage->cols, CV_8U);
	cvScale(srcImage, destImg);
	if(CandidateRegion.size() == 16 || CandidateRegion.size() == 0 || errCode < 0 )
	{
			MessageBoxA(0,"Not Found Forgery",0,0);
			return 0;
	}
	int tamperedCounter = 0;
	int hist[4096]={};
	for(int counter = 0 ; counter < 1; counter++)
	{
		CvMat * src = 0;
		switch(counter)
		{
		case 0:src = srcImage; break;
		case 1:src = srcImageR; break;
		case 2:src = srcImageG; break;
		case 3:src = srcImageB; break;
		}
		minMAE = 10000000;
		maxMAE = 0;
		for(int i = 0 ; i < ydirBlockCount; i++)//calculate MAE according to formula (18)
		{
			for(int j = 0 ; j < xdirBlockCount; j++)
			{
				cvGetSubRect(src, srcROI, cvRect(j * 8, i * 8, 8, 8));
				MAE = CalculateMSE(srcROI, CandidateRegion[0].QTable);
				blockChecker->data.fl[i * blockChecker->cols + j] = MAE;
				hist[int(MAE)]++;
				if(MAE > maxMAE)
					maxMAE = MAE;
				if(MAE < minMAE)
					minMAE = MAE;
			}
		}
		MAEDistance = maxMAE - minMAE;
		MAEThreshold = FindMAEThreshold(hist, maxMAE);
		if(MAEDistance < 30)
		{			
			
			break;
		}
		for(int i = 0 ; i < ydirBlockCount; i++)//Normalizing of the MAEs and forgery detection for every 8*8 block
		{
			for(int j = 0 ; j < xdirBlockCount; j++)
			{
				MAE = (blockChecker->data.fl[i * blockChecker->cols + j] - minMAE ) / MAEDistance;
				if(MAE < MAEThreshold)
				{
					cvGetSubRect(destImg, srcROI, cvRect(j * 8, i * 8, 8, 8));
					cvZero(srcROI);
				}
				else					
					tamperedCounter++;
			}
		}
	}
	if(tamperedCounter < 20 || MAEDistance < 30)
		MessageBoxA(0,"Not Found Forgery",0,0);
	else
		cvShowImage("forgery detection",destImg);
	if(cvWaitKey()==13)	cvDestroyAllWindows();
	cvReleaseMatHeader(&srcROI);
	cvReleaseMat(&blockChecker);
	cvReleaseMat(&destImg);
	cvReleaseMat(&srcImage);
	cvReleaseMat(&srcImage2);
	cvReleaseMat(&srcImageR);
	cvReleaseMat(&srcImageG);
	cvReleaseMat(&srcImageB);

	return 0;
}
	 