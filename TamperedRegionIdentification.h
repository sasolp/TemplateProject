#pragma once

class TamperedRegionIdentification
{
public:
	TamperedRegionIdentification(void);
	~TamperedRegionIdentification(void);
	int ExtractCRinfo(CvMat* srcImage, vector<SubImageInfo>& CandidateRegion);
	int TemperDetection(CvMat* srcImage);
	
	double CalculateMSE(CvMat* srcSubImage, CvMat* QTable);
};

