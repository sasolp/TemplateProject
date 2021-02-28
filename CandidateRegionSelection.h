#pragma once
	typedef struct SubImageInfo
	{
		int i;//x position of subimage
		int j;//y position of subimage
		double MSE;//mean square error
		CvMat* QTable;//Estimated Quantization table
		CvMat* image;
		int T;//number of 1 in Qtable
	}SubImageInfo;
class CandidateRegionSelection
{
public:
	CandidateRegionSelection(void);
	~CandidateRegionSelection(void);
	int SelectSeedRegion(CvMat* srcImage, vector<SubImageInfo>& arrSeedSubImages, vector<SubImageInfo>& arrUnRefineSubimages);
	double CalculateMSE(CvMat* srcSubImage, CvMat* QTable);
	int CalculateT(CvMat* QTable);
	double CalculateMAE(CvMat* QTable1, CvMat* QTable2);
public:
	int GrowingAndRefinement(vector<SubImageInfo>& seedRegion, vector<SubImageInfo>& arrUnRefineSubimages);
	int MakeNewTemporaryVersion(CvMat* tempVersion, vector<SubImageInfo>& newRandomSubImages, int totalExpectedSubImages, int randomaizeSteps);
	int ExtractCondidateRegion(CvMat* srcImage, vector<SubImageInfo>& CandidateRegion);
};

