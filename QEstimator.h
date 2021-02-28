#pragma once
class QEstimator
{
public:
	QEstimator(void);
	~QEstimator(void);
	int ExtractDCTCoefficients(CvMat* src, CvMat* dst);
	int ExtractRDCTCoefficients(CvMat* src, CvMat* dst);
	int ExtractHistOfK_L(CvMat* src, CvMat* hist);
	int ExtractPSIofK_L(CvMat* src, CvMat* dst);
	int ExtractPHIofK_L(CvMat* src, CvMat* dst);
	int FindExtremums(CvMat* src, int& firstEx, int& ExCount, int& shapeFactor);
	int FindFirstPeakAndTurnPoints(CvMat* src, int& firstIndex, vector<int>& turnPoints, int* turnPointsRange);
	bool IsExistMultiples(CvMat* PSI,int i);
	bool IsExistMultiplesInPhi(CvMat* PSI,int i);
	int FindQForType2(CvMat* PHI, CvMat* PSI, CvMat* ACSeq, int firstIndexOfPSI);
	int FindQForType3(CvMat* PHI, CvMat* ACSeq, int firstIndexOfPSI);
	int ExtractQTable(CvMat* src, CvMat* QTable);
	int ExtractACSeqOfK_L(CvMat* src, int k, int l, CvMat* ACSeq);
};

