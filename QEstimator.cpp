#include "StdAfx.h"
#include "QEstimator.h"


QEstimator::QEstimator(void)
{
}


QEstimator::~QEstimator(void)
{
}

/*
	ExtractDCTCoefficients

	Purpose: extracts blockwise dct coefficients of source image 

	Author: Sadegh Soleimani

	Inputs:							
		src : the input source image
		
	outputs:
		dst : destination image contains of dct coefficients
*/
int QEstimator::ExtractDCTCoefficients(CvMat* src, CvMat* dst)
{
	CvMat* srcROI = cvCreateMatHeader(8, 8, CV_32F);
	CvMat* dstROI = cvCreateMatHeader(8, 8, CV_32F);
	int xdirBlockCount = src->cols / 8;
	int ydirBlockCount = src->rows / 8;
	for(int i = 0 ; i < ydirBlockCount; i++)//extract DCT Coefficients for xdirBlockCount*ydirBlockCount blocks
	{
		for(int j = 0 ; j < xdirBlockCount; j++)
		{
			cvGetSubRect(src, srcROI, cvRect(j * 8, i * 8, 8, 8));
			cvGetSubRect(dst, dstROI, cvRect(j * 8, i * 8, 8, 8));	
			cvDCT(srcROI,dstROI,CV_DXT_FORWARD);
			
		}
	}
	cvReleaseMatHeader(&srcROI);
	cvReleaseMatHeader(&dstROI);
	return 0;
}
int QEstimator::ExtractRDCTCoefficients(CvMat* src, CvMat* dst)
{
	CvMat* srcROI = cvCreateMatHeader(8, 8, CV_32F);
	CvMat* dstROI = cvCreateMatHeader(8, 8, CV_32F);;
	int xdirBlockCount = src->cols / 8;
	int ydirBlockCount = src->rows / 8;
	for(int i = 0 ; i < ydirBlockCount; i++)//extract DCT Coefficients for xdirBlockCount*ydirBlockCount blocks
	{
		for(int j = 0 ; j < xdirBlockCount; j++)
		{
			cvGetSubRect(src, srcROI, cvRect(j * 8, i * 8, 8, 8));
			cvGetSubRect(dst, dstROI, cvRect(j * 8, i * 8, 8, 8));
			
			cvDCT(srcROI,dstROI,CV_DXT_INVERSE);
		}
	}
	cvReleaseMatHeader(&srcROI);
	cvReleaseMatHeader(&dstROI);
	return 0;
}
/*
	ExtractACSeqOfK_L

	Purpose: extracts Sequence of (k,l)th AC Coefficient of all DCT blocks

	Author: Sadegh Soleimani

	Inputs:							
		src : the DCT source image contains blocks of DCT
		k : ydirIndex of (k,l)th AC Coefficient
		l : xdirIndex of (k,l)th AC Coefficient
	outputs:
		ACSeq : the output Sequence of (k,l)th AC Coefficient of all DCT blocks
*/

int QEstimator::ExtractACSeqOfK_L(CvMat* src, int k, int l, CvMat* ACSeq)
{
	
	int xdirBlockCount = src->cols / 8;
	int ydirBlockCount = src->rows / 8;
	int ACCounter = 0;
	for(int i = 0 ; i < ydirBlockCount ; i++)//finds (k,l)th AC Coefficient of all DCT blocks and appends it to the ACSeq Matix
	{
		for(int j = 0 ; j < xdirBlockCount; j++)
		{
			ACSeq->data.fl[ACCounter++] = src->data.fl[(i * 8 + k) * src->cols + (j * 8 + l)];
		}
	}
	return 0;
}

/*
	ExtractHistOfK_L

	Purpose: extracts hist of ACSeq according to formula at section II-5

	Author: Sadegh Soleimani

	Inputs:							
		src : the Sequence of (k,l)th AC Coefficient of all DCT blocks
		
	outputs:
		hist : histogram of the (k, l)th AC coefﬁcient 
*/
int QEstimator::ExtractHistOfK_L(CvMat* src, CvMat* hist)
{
	double minAC = 0, maxAC = 0;
	cvMinMaxLoc(src, &minAC, &maxAC); 
	for(int i = 0 ; i < src->cols; i++)
	{
		//hist->data.fl[int(ceil(src->data.fl[i] - minAC))]++;
		hist->data.fl[int(src->data.fl[i] - minAC)]++;
	}
	return 0;
}

/*
	ExtractPSIofK_L

	Purpose: extracts the energy density spectrum (EDS) sequence of DFT of the histogram of the (k, l)th AC coefﬁcient according to formula at section II-6

	Author: Sadegh Soleimani

	Inputs:							
		src : the histogram of the (k, l)th AC coefﬁcient
		
	outputs:
		dst : EDS sequence of DFT of the histogram of the (k, l)th AC coefﬁcient according to formula at section II-6
*/
int QEstimator::ExtractPSIofK_L(CvMat* src, CvMat* dst)
{
	int optimumSize = cvGetOptimalDFTSize(src->cols);
	CvMat* dftOfSrc = cvCreateMat(src->rows, optimumSize * 2, CV_32FC2);
	CvMat* src2 = cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	CvMat* tmp = cvCreateMatHeader(src->rows, src->cols, CV_32F);
	cvZero(src2);
	cvGetSubRect(src2, tmp, cvRect(0, 0, src->cols,1));
	cvCopy(src, tmp);
	cvDFT(src2, dftOfSrc, CV_DXT_FORWARD);
	CvMat* IM =  cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	CvMat* RE =  cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	cvSplit(dftOfSrc, RE, IM,0,0);
	cvCartToPolar(IM, RE, src2);
	cvPow(src2, src2, 2.0);
	cvGetSubRect(src2,tmp, cvRect(0, 0, src->cols,1));
	cvCopy(tmp, dst);

	cvReleaseMat(&dftOfSrc);
	cvReleaseMat(&IM);
	cvReleaseMat(&RE);
	cvReleaseMat(&src2);
	cvReleaseMatHeader(&tmp);
	return 0;
}

/*
	ExtractPHIofK_L(body of this function is same of 'ExtractPSIofK_L' function)

	Purpose: extracts the Magnitude sequence of DFT of PSI Sequence according to formula at section II-7

	Author: Sadegh Soleimani

	Inputs:							
		src : the PSI Sequence (EDSs) mentioned at section II-6
		
	outputs:
		dst : the Magnitude sequence of DFT of PSI Sequence
			 
*/
int QEstimator::ExtractPHIofK_L(CvMat* src, CvMat* dst)
{
	int optimumSize = cvGetOptimalDFTSize(src->cols);
	CvMat* dftOfSrc = cvCreateMat(src->rows, optimumSize * 2, CV_32FC2);
	CvMat* src2 = cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	CvMat* tmp = cvCreateMatHeader(src->rows, src->cols, CV_32F);
	cvZero(src2);
	cvGetSubRect(src2, tmp, cvRect(0, 0, src->cols,1));
	cvCopy(src, tmp);
	cvDFT(src2, dftOfSrc, CV_DXT_FORWARD);
	CvMat* IM =  cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	CvMat* RE =  cvCreateMat(src->rows, optimumSize * 2, CV_32F);
	cvSplit(dftOfSrc, RE, IM,0,0);
	cvCartToPolar(IM, RE, src2);
	
	cvGetSubRect(RE,tmp, cvRect(0, 0, src->cols,1));
	cvCopy(tmp, dst);
	cvNormalize(dst, dst, 0 ,100, CV_MINMAX);

	
	cvReleaseMat(&dftOfSrc);
	cvReleaseMat(&IM);
	cvReleaseMat(&RE);
	cvReleaseMat(&src2);
	cvReleaseMatHeader(&tmp);
	return 0;
}

/*
	FindExtremums~(FindPeaks)

	Purpose: finds peaks count in EDS Sequence and finds first peak's index and finally 
			 calculates shape factor according to formula at sectionIII-A

	Author: Sadegh Soleimani

	Inputs:							
		src : EDS sequence of DFT of the histogram of the (k, l)th AC coefﬁcient according to formula at section II-6
		
	outputs:
		firstEx : first peak's index
		ExCount : count of peak in Sequence 
		shapeFactor : shape factor according to formula at sectionIII-A
*/
int QEstimator::FindExtremums(CvMat* src, int& firstEx, int& ExCount, int& shapeFactor)
{
	int horizon = 0;
	shapeFactor = 1000000;
	double maxEDS = 0, minEDS = 0;
	cvMinMaxLoc(src, &minEDS, &maxEDS);
	//find f2(shape factor) according to section III-A(formula 6)
	while(shapeFactor == 1000000 && horizon < maxEDS)
	{
		for(int i = 0 ; i < src->cols ; i++)
		{
			if(src->data.i[i] == horizon)
			{
				if(i < shapeFactor)
				{
					shapeFactor = i;
				}
			}
		}
		horizon++;
	}
	//finding the peaks in sequence and counts number of peaks
	firstEx = -1;
	ExCount = 0;
	double peakStep = 30;
	while(firstEx < 0 && peakStep >= 20)
	{
		for(int i = 2 ; i < src->cols - 1 ; i++)
		{
			if(((src->data.i[i] - src->data.i[i - 1]) >= peakStep || (src->data.i[i] - src->data.i[i - 2]) >= peakStep ) && ((src->data.i[i] - src->data.i[i + 1]) >= peakStep || (src->data.i[i] - src->data.i[i + 2]) >= peakStep ) )
			{
				ExCount++;
				if(firstEx < 0)
				{
					firstEx = i;
				}
			}
		}
		peakStep -= 10;
	}
	return 0;
}

/*
	FindFirstPeakAndTurnPoints

	Purpose: finds first peak in PHI sequence and also finds turn points around of it

	Author: Sadegh Soleimani

	Inputs:							
		src : the Magnitude sequence of DFT of PSI Sequence according to formula at section II-7		
	outputs:
		firstIndex : the first detected peak index
		turnPoints : an array of turn points around of first peak
*/
int QEstimator::FindFirstPeakAndTurnPoints(CvMat* src, int& firstIndex, vector<int>& turnPoints, int* turnPointsRange)
{
	firstIndex = -1;
	int forwardCounter = 0, backwardCounter = 0 ;
	int peakStep = 20;
	int lastUnValidated = -1;
	while(firstIndex < 0 && peakStep >= 5)
	{
		for(int i = 2 ; i < src->cols - 2 ; i++)
		{

			if(((src->data.fl[i] - src->data.fl[i - 1]) >= peakStep || (src->data.fl[i] - src->data.fl[i - 2]) >= peakStep ) && ((src->data.fl[i] - src->data.fl[i + 1]) >= peakStep || (src->data.fl[i] - src->data.fl[i + 2]) >= peakStep ) )
			{
				lastUnValidated = i;
				if(!IsExistMultiplesInPhi(src, i))continue;
				firstIndex = i;
				backwardCounter = i;
				forwardCounter = i;
				while(backwardCounter > 1 && src->data.fl[backwardCounter] > src->data.fl[backwardCounter - 1])backwardCounter--;
				while(forwardCounter < src->cols - 1 && src->data.fl[forwardCounter] >= src->data.fl[forwardCounter + 1])forwardCounter++;
				turnPointsRange[0] = backwardCounter;
				turnPointsRange[1] = forwardCounter;
				for(int j = backwardCounter ; j < forwardCounter ; j++)
				{
					if(src->data.fl[j + 1] - src->data.fl[j - 1] != 0)
					{
						turnPoints.push_back(j);
					}
				}
				break;
			}
		}
		if(lastUnValidated > 0)
			firstIndex = lastUnValidated;
		peakStep--;
	}
	return 0;
}

/*
	

	Purpose: 

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
bool QEstimator::IsExistMultiplesInPhi(CvMat* src,int index)
{
	bool retVal = false;
	int peakOnIntervalsCount = 0;
	int interestIndex = 0;
	for(int i = 2 ; i <= src->cols / index ; i++)
	{
		interestIndex = i * index;
		/*
		here, interestIndex is considered with telorance of 1 index at left and right of original index
		*/
		if(
			(src->data.fl[(interestIndex)] - src->data.fl[(interestIndex) - 1]> 0 || src->data.fl[(interestIndex)] - src->data.fl[(interestIndex) - 2]> 0)  &&
			(src->data.fl[(interestIndex)] - src->data.fl[(interestIndex) + 1]> 0 || src->data.fl[(interestIndex)] - src->data.fl[(interestIndex) + 2]> 0) 
		  )
		{
			peakOnIntervalsCount++;
		}
		else
			break;
	}
	if(peakOnIntervalsCount >= 1)
		retVal = true;
	return retVal;
}
/*
	

	Purpose: 

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
bool QEstimator::IsExistMultiples(CvMat* src,int index)
{
	bool retVal = false;
	int peakOnIntervalsCount = 0;
	int interestIndex = 0;
	for(int i = 0 ; i < src->cols / index ; i++)
	{
		interestIndex = i * index;
		/*
		here, interestIndex is considered with telorance of 1 index at left and right of original index
		*/
		if((src->data.i[(interestIndex)] - src->data.i[(interestIndex) - 1]) > 10 && (src->data.i[(interestIndex)] - src->data.i[(interestIndex) + 1]) > 10 )
		{
			peakOnIntervalsCount++;continue;
		}
		if((src->data.i[(interestIndex - 1)] - src->data.i[(interestIndex - 1) - 1]) > 10 && (src->data.i[(interestIndex - 1)] - src->data.i[(interestIndex - 1) + 1]) > 10 )
		{
			peakOnIntervalsCount++;continue;
		}
		if((src->data.i[(interestIndex + 1)] - src->data.i[(interestIndex + 1) - 1]) > 10 && (src->data.i[(interestIndex + 1)] - src->data.i[(interestIndex + 1) + 1]) > 10 )
		{
			peakOnIntervalsCount++;continue;
		}
	}
	if(peakOnIntervalsCount >= 2)
		retVal = true;
	return retVal;
}
/*
	

	Purpose: 

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
int QEstimator::FindQForType2(CvMat* PHI, CvMat* PSI, CvMat* ACSeq, int firstIndexOfPSI)
{
	int retQ = -1;
	int QofPSI = 0;
	int QofPHI = 0;
	double ThresholdOfPHI = 0;
	double minAC = 0, maxAC = 0;
	cvMinMaxLoc(ACSeq, &minAC, &maxAC);
	int firstIndexOfPHI = 0;
	int turnPointsRange[2] = {0, 0};
	vector<int> turnPoints;
	//E-II-1
	QofPSI = int(ceil(maxAC - minAC) / firstIndexOfPSI); //formula 7 in SectionIII-B
	//E-II-2
	FindFirstPeakAndTurnPoints(PHI, firstIndexOfPHI, turnPoints, turnPointsRange);
	QofPHI = firstIndexOfPHI;
	if(QofPHI < 0)return QofPHI;
	//E-II-3
	if (QofPHI == QofPSI)
	{
		retQ = QofPSI;
	}
	else//E-II-4
	{
		double maxRange = max(QofPSI, QofPHI);//formula 8 in SectionIII-B
		ThresholdOfPHI = 0.9 * max (PHI->data.fl[QofPSI], PHI->data.fl[QofPHI]);//formula 9 in SectionIII-B
		for(int i = 2 ; i <= maxRange ; i++)//finding the minimum index of peaks in rang [0 - maxRange] in PHI
		{
			if(PHI->data.fl[i] > ThresholdOfPHI)
			{
				//E-II-5
				if(IsExistMultiples(PSI, i))
				{
						retQ = i;
				}
				else
				{
					if(PHI->data.fl[QofPSI] > PHI->data.fl[QofPHI])
					{
						retQ = QofPSI;
					}
					else
					{
						retQ = QofPHI;
					}
				}
				break;
			}
		}
	}
	return retQ;
}

/*
	

	Purpose: 

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
int QEstimator::FindQForType3(CvMat* PHI, CvMat* ACSeq, int firstIndexOfPSI)
{
	int retQ = -1;
	int QofPSI = 0;
	int QofPHI = 0;
	double ThresholdOfPHI = 0;
	double minAC = 0, maxAC = 0;
	cvMinMaxLoc(ACSeq, &minAC, &maxAC);
	int firstIndexOfPHI = 0;
	int turnPointsRange[2] = {0, 0};
	vector<int> turnPoints;
	//E-II-01
	QofPSI = int(ceil(maxAC - minAC) / firstIndexOfPSI); //formula 7 in SectionIII-B
	//E-II-2
	FindFirstPeakAndTurnPoints(PHI, firstIndexOfPHI, turnPoints, turnPointsRange);
	QofPHI = firstIndexOfPHI;

	if(QofPHI < 0)return QofPHI;
	//E-III-2
	if (turnPointsRange[1] - turnPointsRange[0] == 0)//mean that count of detected turnPoints is 1
	{
		retQ = QofPHI;
	}
	else
	{
		if(QofPSI >= turnPointsRange[0] && QofPSI <= turnPointsRange[1])//E-III-2->1
		{
			int minDiff = 100000;
			for(int i = 0 ; i < turnPoints.size() ; i++)
			{
				if(abs(QofPSI - turnPoints[i]) < minDiff)
				{
					minDiff = abs(QofPSI - turnPoints[i]);
					retQ =  turnPoints[i];
				}
			}

		}
		else//E-III-2->2
		{
			retQ = QofPHI;
		}
	}
	return retQ;
}

/*
	

	Purpose: 

	Author: Sadegh Soleimani

	Inputs:							
		
		
	outputs:
			 
*/
int QEstimator::ExtractQTable(CvMat* src, CvMat* QTable)
{
	
	int xdirBlockCount = src->cols / 8;
	int ydirBlockCount = src->rows / 8;
	CvMat* dstDCT = cvCreateMat(src->rows, src->cols, CV_32F);
	ExtractDCTCoefficients(src, dstDCT);
	/*CvMat* srct = cvCreateMat(src->rows, src->cols, CV_8S);
	cvScale(src,srct);
	cvShowImage("initial image", srct);cvWaitKey();*/
	CvMat* ACSeq = cvCreateMat(1, xdirBlockCount * ydirBlockCount, CV_32F);
	double minAC = 0, maxAC = 0;
	int firstExIndex = 0, exCount = 0, shapeFactor = 0;
	int T1 = 0, T2 = 0, T3 = 0;
	for(int k = 0 ; k < 8 ; k++)
	{
		for(int l = 0 ; l < 8 ; l++)
		{
			cvZero(ACSeq);
			ExtractACSeqOfK_L(dstDCT, k, l, ACSeq);
			//cvScale(ACSeq,ACSeq,);
			cvMinMaxLoc(ACSeq, &minAC, &maxAC); 
			if(maxAC - minAC < 5)
			{				
				QTable->data.fl[k * 8 + l] = -1;
				continue;
			}

			CvMat* ACHist = cvCreateMat(1, maxAC - minAC + 1, CV_32F);	
			CvMat* PSI = cvCreateMat(1, maxAC - minAC + 1, CV_32F);	
			CvMat* PSI_INT = cvCreateMat(1, maxAC - minAC + 1, CV_32S);	
			CvMat* PHI = cvCreateMat(1, maxAC - minAC + 1, CV_32F);	
			cvZero(ACHist);		cvZero(PSI);		
			ExtractHistOfK_L(ACSeq, ACHist);
			ExtractPSIofK_L(ACHist, PSI);
			cvScale(PSI, PSI_INT);
				/*CvPoint m1,m2;
				cvMinMaxLoc(PSI, &minAC, &maxAC,&m1,&m2); */
			
			cvNormalize(PSI_INT, PSI_INT, 0, 1000, CV_MINMAX);
			cvNormalize(PSI, PSI, 0, 100, CV_MINMAX);
			ExtractPHIofK_L(PSI, PHI); 
			FindExtremums(PSI_INT, firstExIndex, exCount, shapeFactor);
			T2 = max(100.0,0.3 * (maxAC - minAC));
			T3 = 0.8 * (maxAC - minAC);
			if(exCount == T1 && shapeFactor < T2 && shapeFactor < T3)//Type1
			{
				QTable->data.fl[k * 8 + l] = 1;
			}
			else 
			{
				if(exCount > T1 && shapeFactor < T2 )//Type2
				{
					QTable->data.fl[k * 8 + l] = FindQForType2(PHI, PSI_INT, ACSeq, firstExIndex);
				}
				else
				{
					if(exCount > T1 && shapeFactor >= T2 )//Type3
					{
						QTable->data.fl[k * 8 + l] = FindQForType3(PHI, ACSeq, firstExIndex);
					}
					else//Type4
					{
						QTable->data.fl[k * 8 + l] = -2;
					}
				}
				if(QTable->data.fl[k * 8 + l] == -1 && exCount > 2 && abs(maxAC) < 90 && abs(minAC) < 90)
				{
					if(abs(minAC) < 1)
						QTable->data.fl[k * 8 + l] =floor(abs(maxAC) + 0.4); //int(abs(maxAC));
					else if(abs(maxAC) < 1)
						QTable->data.fl[k * 8 + l] =floor(abs(minAC) + 0.4); //int(abs(minAC));
					else
					{
						QTable->data.fl[k * 8 + l] =floor(min(abs(maxAC),abs(minAC)) + 0.4); //int(min(abs(maxAC),abs(minAC)));
					}
				}
			}

			cvReleaseMat(&ACHist);
			cvReleaseMat(&PSI);
			cvReleaseMat(&PHI);
			
		}
	}
	cvReleaseMat(&dstDCT);
	cvReleaseMat(&ACSeq);
	return 0;
}

