
#include "stdafx.h" 
#include "SVMClassfier.h" 
#include "CharacterSegment.h"
void SVMClassifier::ErodeTines(Mat src, Mat &dst)
{
	Mat tmp1 = src.clone();
	Mat tmp2 = src.clone();
	/*int xOccurrence, yOccurrence;
	int yOffsets1[] = {-1, -1, -1, +0, +1, +1, +1, +0};
	int xOffsets1[] = {-1, +0, +1, +1, +1, +0, -1, -1};
	int yOffsets2[] = {-1, +0,  +1, +1, +1, +0, -1, -1};
	int xOffsets2[] = {-1, -1,  -1, +0, +1, +1, +1, +0};*/
	bool bFound = false;
	int counter = 0;
	do
	{
		bFound = false;
		for(int i = 1; i < tmp1.rows - 1; i++)
		{
			for(int j = 1 ; j < tmp1.cols - 1; j++)
			{
				if(tmp1.data[i * tmp1.cols + j])
				{
					if(!tmp1.data[i * tmp1.cols + j - 1] && !tmp1.data[i * tmp1.cols + j + 1])
					{
						tmp2.data[i * tmp1.cols + j] = 0;
						bFound = true;
					}
					else if(!tmp1.data[(i + 1) * tmp1.cols + j] && !tmp1.data[(i - 1) * tmp1.cols + j])
					{
						tmp2.data[i * tmp1.cols + j] = 0;
						bFound = true;
					}
				}
			}
		}
		counter++;
		tmp1 = tmp2.clone();
	}while(bFound);
	if(dst.data)
	{
		dst.~Mat();
	}
	dst = tmp1.clone();
}
void SVMClassifier::FitWindow(Mat src, Mat &dst)
{
	int minX = 0, maxX = 0, minY = 0, maxY = 0;
	bool bFound = false;
	for(int i = 0 ; i < src.rows && !bFound; i++)
	{
		for(int j = 0 ; j < src.cols; j++)
		{
			if(src.data[ i * src.cols + j])
			{
				minY = i;
				bFound = true;
				break;
			}
		}
	}
	bFound = false;
	for(int i = src.rows - 1; i >= 0 && !bFound; i--)
	{
		for(int j = 0 ; j < src.cols; j++)
		{
			if(src.data[ i * src.cols + j])
			{
				maxY = i;
				bFound = true;
				break;
			}
		}
	}
	bFound = false;
	for(int j = 0 ; j < src.cols && !bFound; j++)
	{
		for(int i = 0 ; i < src.rows; i++)
		{
			if(src.data[ i * src.cols + j])
			{
				minX = j;
				bFound = true;
				break;
			}
		}
	}
	bFound = false;
	for(int j = src.cols - 1 ; j >= 0 && !bFound; j--)
	{
		for(int i = src.rows - 1; i >= 0; i--)
		{
			if(src.data[ i * src.cols + j])
			{
				maxX = j;
				bFound = true;
				break;
			}
		}
	}
	dst = src(Rect(minX, minY, maxX - minX + 1, maxY - minY + 1)).clone();
}
void SVMClassifier::FindMaxConcavity(Mat src, Mat &concavityMat, float strokeSize, int &crCount, float &dist, float &area, float &roudness, float &solidity/*, int &endPointCount*/) 
{
	Mat thin;
	//imwrite("c:\\t.bmp",src * 255);
	//imshow("fit_thin",src * 255);cvWaitKey();
	//src = ~src;
	if(strokeSize <= 5)
	{
		resize(src, src, Size(src.cols * 3, src.rows * 3));
		strokeSize *= 3; 
	}
	//cv::morphologyEx(src, src, CV_MOP_OPEN, Mat(),Point(-1,-1),strokeSize / 3);
	copyMakeBorder(src, src, 1, 1, 1, 1, BORDER_CONSTANT, Scalar::all(0));
	thining(src, thin);
	//imshow("fit_thin",thin * 255);cvWaitKey();
	Mat fit_thin;
	FitWindow(thin, fit_thin);
	copyMakeBorder(fit_thin, fit_thin, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0));
	vector<Point> points;
	Point moment(0,0) ;
	for(int j = fit_thin.cols - 1 ; j >= 0; j--)
	{
		for(int i = fit_thin.rows - 1; i >= 0; i--)
		{
			if(fit_thin.data[ i * fit_thin.cols + j])
			{
				points.push_back(Point(j, i));
			}
		}
	}
	vector<Point> hullPoints;
	vector<vector<Point>> hulls;
	convexHull(points, hullPoints);
	vector<vector<Point>> thin_points;
	imshow("fit_thin",fit_thin * 255);cvWaitKey();
	Mat thin_copy = fit_thin.clone();
	findContours(thin_copy, thin_points, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	/*fit_thin.copyTo(thin_copy);
	thin_copy *= 255;*/
	int maxIndex = -1;
	int maxSize = 0;
	vector<Point>polyPoints;
	for(int i = 0 ; i < thin_points.size(); i++)
	{
		float area0 ;
		Rect rect = boundingRect(thin_points[i]);
		if(thin_points[i].size() > 6)
		{
			area0 = contourArea(thin_points[i]);
			area0 = max(double(area0), arcLength(thin_points[i], true));
		}
		else
			area0 = rect.width * rect.height;
		if(area0 > maxSize)
		{
			maxIndex = i;
			maxSize = area0;
		}
	}
	if(thin_points.size())
	{
		int newStrokeSize = min(float(strokeSize), 5.0f);
		polyPoints = thin_points[maxIndex];
		/*for(int i = 0 ; i < polyPoints.size(); i++)
		{
			Point pt1 = polyPoints[i];
			for(int j = i - 1 ; j >= 0; j--)
			{
				Point pt2 = polyPoints[j];
				if(abs(pt1.x - pt2.x) <= newStrokeSize && abs(pt1.y - pt2.y) <= newStrokeSize)
				{
					polyPoints.erase(polyPoints.begin() + i);i--;
					break;
				}
			}
		}
		for(int i = 0 ; i < polyPoints.size(); i++)
		{
			Point pt1 = polyPoints[i];
			for(int j = i - 1 ; j >= 0; j--)
			{
				Point pt2 = polyPoints[j];
				if(pt1 == pt2)
				{
					polyPoints.erase(polyPoints.begin() + i);i--;
					break;
				}
			}
		}*/
		//polyPoints.erase(polyPoints.end() - 1);
		//polyPoints.push_back(polyPoints[0]);
		Mat curve(polyPoints);
		vector<Point> approxPoly;
		cv::approxPolyDP(curve, approxPoly, newStrokeSize, 1);
		/*for(int i = 0 ; i < approxPoly.size(); i++)
		{
			Point pt1 = approxPoly[i];
			for(int j = i - 1 ; j >= 0; j--)
			{
				Point pt2 = approxPoly[j];
				if(abs(pt1.x - pt2.x) <= newStrokeSize && abs(pt1.y - pt2.y) <= newStrokeSize)
				{
					approxPoly.erase(approxPoly.begin() + i);i--;
					break;
				}
			}
		}*/
		thin_copy = 0;
		cv::polylines(thin_copy, approxPoly, 0, Scalar(255));
		for(int i = 0 ; i < approxPoly.size(); i++)
		{
			cv::rectangle(thin_copy, Rect(approxPoly[i].x - 1, approxPoly[i].y - 1,2,2), Scalar(128), -1);
			//polyDst.data[ approxPoly[i].y * polyDst.cols + approxPoly[i].x] = 128;
		}
		imshow("polyDst",thin_copy);cvWaitKey();
	}
	hulls.push_back(hullPoints);
	Mat hull = Mat(fit_thin.rows, fit_thin.cols, fit_thin.type(), Scalar(0));
	drawContours(hull, hulls, 0, Scalar(1), -1);
	concavityMat = Mat(hull - fit_thin).clone();
	dilate(fit_thin, fit_thin, Mat());
	double hullArea = cv::sum(hull)[0];
	double forgroundArea = cv::sum(fit_thin)[0] ;
	Mat newHull = hull - fit_thin;
	//imshow("newHull1",newHull);cvWaitKey();
	ErodeTines(newHull, newHull);
	//imshow("newHull2",newHull);cvWaitKey();
	vector<vector<Point>> contours;
	if(0)
	{
	 imshow("fit_thin",fit_thin * 255);cvWaitKey();
	 imshow("hull",hull * 255);cvWaitKey();
	 imshow("",newHull * 255);cvWaitKey();
	}
	//concavityMat = newHull.clone();
	//concavityMat = concavityMat * 255;
	findContours(newHull, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	if(!contours.size())
	{
		copyMakeBorder(fit_thin, fit_thin, 1, 1, 1, 1, BORDER_CONSTANT, Scalar::all(0));
		findContours(fit_thin, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	}
	vector<Point> maxConcavity;
	float maxArea = 0;
	int maxInx = -1;
	for(int i = 0; i < contours.size(); i++)
	{
		float area0 ;
		Rect rect = boundingRect(contours[i]);
		if(contours[i].size() > 6)
		{
			area0 = contourArea(contours[i]);
			area0 = max(double(area0), arcLength(contours[i], true));
		}
		else
			area0 = rect.width * rect.height;
		if(area0 > maxArea)
		{
			maxArea = area0;
			maxInx = i;
		}
		if(area0 > strokeSize * strokeSize)
		{
			crCount++;
		}
	}

	solidity = forgroundArea / hullArea ;
	RotatedRect box = fitEllipse(points);
	float MajorAxisLength = max(1.0f, max(box.size.height, box.size.width));
	float MinorAxisLength = max(1.0f, min(box.size.height, box.size.width));
	roudness = MinorAxisLength / MajorAxisLength;
	if(contours.size())
	{
		RotatedRect box ;
		Rect rect = boundingRect(contours[maxInx]);
		if(contours[maxInx].size() > 6)
		{
			box = fitEllipse(contours[maxInx]);
			//RotatedRect box2 = minAreaRect(contours[maxInx]);
		}
		else
		{
			box.angle = (rect.height <= rect.width)? 0: 90;
			box.size.height = max(rect.height, rect.width);
			box.size.width = min(rect.height, rect.width);
		}
		//newHull = 0;
		//cv::ellipse(newHull, box, Scalar(128));
		
		//imshow("newHullWithEllipse",newHull);cvWaitKey();
		//angle = box.angle;
		for(int i = 0 ; i < contours[maxInx].size(); i++)
		{
			moment.x += contours[maxInx][i].x;
			moment.y += contours[maxInx][i].y;
		}
		moment.x = cvRound(float(moment.x) / contours[maxInx].size());
		moment.y = cvRound(float(moment.y) / contours[maxInx].size());
		dist = sqrt(float(moment.x * moment.x + moment.y * moment.y)) / strokeSize; 
		area = maxArea / strokeSize;
		//ecc = 2*sqrt(pow(MajorAxisLength/2,2) - pow(MinorAxisLength/2, 2)) / MajorAxisLength;
	}
}
double SVMClassifier::GetStrokeSize(Mat src)
{
	Mat dst;	
	distanceTransform(src, dst, CV_DIST_L2, 3);
	double min_val = 0, max_val = 0;
	minMaxLoc(dst, &min_val, &max_val);
	Mat mean , sdv ;
	cv::meanStdDev(dst, mean, sdv);
	return (max_val - ((double*)sdv.data)[0] )* 1.5;
}
void SVMClassifier::Feature_Char(Mat charImg,Mat &FeatureVector)
{	
	//-------------------------------------------
	//imshow("charImg", charImg);cvWaitKey();
	Mat sw;
	//strokeSize = GetStrokeSize(charImg );
	//double strokeSize = StrokeWidthMeasurement4DTime(charImg, sw);
	Rect boundRect;
	Mat re_BW;
	FitWindow(charImg, re_BW);
	copyMakeBorder(re_BW, re_BW, 1 , 1 + (re_BW.rows % 2), 1 , 1 + (re_BW.cols % 2), BORDER_CONSTANT, Scalar::all(0));
	imshow("charImg", re_BW);cvWaitKey();
	double strokeSize = GetStrokeSize(re_BW );
	int width=re_BW.rows;
	int height=re_BW.cols;
	int size_normal=width/8;
	FeatureVector=Mat(1,25,CV_32FC1,Scalar::all(0));
	//re_BW.convertTo(re_BW,CV_32FC1);

	int size_window = re_BW.cols;
	{
		int i = 0;
		int i_F=0;
		i_F=(i);

		//=================DCT======================
		Mat DC ;
		re_BW.convertTo(re_BW, CV_32FC1);
		//imwrite("c:\\1.bmp",re_BW);
		dct(re_BW,DC);
		re_BW.convertTo(re_BW, CV_8UC1);
		float DCVector[9];
		int y_dct_Indexes[] = { 0, 1, 2, 1, 0, 1, 2, 2};
		int x_dct_Indexes[] = { 1, 0, 0, 1, 2, 2, 1, 2};
		float dc = DC.at<float>(0, 0);
		for(int i = 0 ; i < 8 ; i++)
		{
			DCVector[i]=  ((float*)DC.data)[y_dct_Indexes[i] * DC.cols + x_dct_Indexes[i]];
		}
		for(int k=0;k<8;k++)
		{
			FeatureVector.at<float>(i_F,k) = DCVector[k] / dc;
		}

		DC.~Mat();
		//================ regionprops =================
		
		int crCount = 0;
		float dist = 0.0f, area = 0.0f, roudness = 0.0f, solidity = 0.0f;
		Mat concavityMat;
		FindMaxConcavity(re_BW, concavityMat, strokeSize, crCount, dist, area, roudness, solidity);
		{
			FeatureVector.at<float>(i_F,8) = crCount * 10;   ////////////
			FeatureVector.at<float>(i_F,9) = dist;  
			FeatureVector.at<float>(i_F,10)= area;   
			FeatureVector.at<float>(i_F,11)= solidity;  
			FeatureVector.at<float>(i_F,12)= roudness;   
		}
		//imwrite("c:\\concavityMat.bmp", concavityMat * 255);imshow("", concavityMat * 255);cvWaitKey();
		////////////Concavity/////////////
		Mat DC_Concavity ;
		copyMakeBorder(concavityMat, concavityMat, 0 , (concavityMat.rows % 2), 0 , (concavityMat.cols % 2), BORDER_CONSTANT, Scalar::all(0));
		concavityMat.convertTo(concavityMat, CV_32FC1);
		dct(concavityMat,DC_Concavity);
		float dc2 = DC_Concavity.at<float>(0, 0);
		for(int i = 0 ; i < 8 ; i++)
		{
			DCVector[i]=  ((float*)DC_Concavity.data)[y_dct_Indexes[i] * DC_Concavity.cols + x_dct_Indexes[i]];
		}
		for(int i = 13; i < 21 ; i++)
		{
			FeatureVector.at<float>(i_F,i) = DCVector[i - 13] / dc2;
		}

		DC_Concavity.~Mat();
		concavityMat.~Mat();
		////////////////////////////////
		//================ calculate hu============================	
		Moments moments = cv::moments(re_BW, true);
		double hu[7];
		HuMoments(moments, hu);

		for(int k = 21; k < 23; k++)
		{
			FeatureVector.at<float>(i_F,k)=(float)(hu[k-21]);
		}		
		FeatureVector.at<float>(i_F,23)=(float)(dc / strokeSize);
		FeatureVector.at<float>(i_F,24)=(float)(dc2 / strokeSize);
		//============================================================

	}

	re_BW.~Mat();

}
int SVMClassifier::CreateSVMFeatureFile(char * outFilePath, int classId, Mat features)
{

	FILE * fp = fopen(outFilePath,"a");
	if(fp == NULL)
	{
		fp = fopen(outFilePath,"w");
		if(fp == NULL)
		{
			return -1;
		}
	}
	int featuresCount = features.cols;

	fprintf(fp, "%d\t", classId);
	for(int i = 0 ; i < featuresCount; i++)
	{
		fprintf(fp, "%i:%e\t", i + 1, features.at<float>(0, i));
	}
	fprintf(fp, "\n");
	fclose(fp);
	return 0;
}
int SVMClassifier::FindSymbolIndex(string strInput)
{
	int index = -1;
	string strSymbol = "";
	for(int i = 0 ; i < strInput.size(); i++)
	{
		if(strInput[i] == '-')
			break;
		strSymbol += strInput[i];
	}
	for(int i = 0 ; i < SymbolCount; i++)
	{
		if(!stricmp(strSymbol.c_str(), arrModelSymbols2[i].c_str()))
		{
			return i + 1;
		}
	}
	return -1;
}
void SVMClassifier::LoadSvmModels(char*models_path[])
{
	models.model_letter.path = models_path[0];
	models.model_letter_.path = models_path[1];
	models.model__letter.path = models_path[2];
	models.model__letter_.path = models_path[3];

	models.model_letter.model = svm_load_model  (models.model_letter.path.c_str()	);
	models.model_letter_.model = svm_load_model (models.model_letter_.path.c_str()  );
	models.model__letter.model = svm_load_model (models.model__letter.path.c_str()  );
	models.model__letter_.model = svm_load_model(models.model__letter_.path.c_str() );

	models.model_letter.number_of_classes = models.model_letter.model->nr_class;
	models.model_letter_.number_of_classes = models.model_letter_.model->nr_class;
	models.model__letter.number_of_classes = models.model__letter.model->nr_class;
	models.model__letter_.number_of_classes = models.model__letter_.model->nr_class;

	models.model_letter.feature_vector_size = 25;
	models.model_letter_.feature_vector_size = 25;
	models.model__letter.feature_vector_size = 25;
	models.model__letter_.feature_vector_size = 25;

	models.model_letter.feature_min = new double[models.model_letter.feature_vector_size];
	models.model_letter.feature_max = new double[models.model_letter.feature_vector_size];

	models.model_letter_.feature_min = new double[models.model_letter_.feature_vector_size];
	models.model_letter_.feature_max = new double[models.model_letter_.feature_vector_size];

	models.model__letter.feature_min = new double[models.model__letter.feature_vector_size];
	models.model__letter.feature_max = new double[models.model__letter.feature_vector_size];

	models.model__letter_.feature_min = new double[models.model__letter_.feature_vector_size];
	models.model__letter_.feature_max = new double[models.model__letter_.feature_vector_size];

	MODEL_Read_Range_File(models.model_letter, (models.model_letter.path + string("_Range")).c_str());
	MODEL_Read_Range_File(models.model_letter_, (models.model_letter_.path + string("_Range")).c_str());
	MODEL_Read_Range_File(models.model__letter, (models.model__letter.path + string("_Range")).c_str());
	MODEL_Read_Range_File(models.model__letter_, (models.model__letter_.path + string("_Range")).c_str());

}
double SVMClassifier::MODEL_output(SVM_MODEL &model, int index, double value ) //try to change value in svm.cpp to float to fasten the process
{
    /* skip single-valued attribute */
    double Max = model.feature_max[index];
    double Min = model.feature_min[index];
    if(Max == Min)
        return value;

    if(value == Min)
        value = model.lower;
    else if(value == Max)
        value = model.upper;
    else
    {	
		double den = (Max-Min);
		double num = (model.upper - model.lower)*(value-Min);
		value = model.lower + num/den	;
    }
    return value;
}
void SVMClassifier::MODEL_Read_Range_File(SVM_MODEL &model, const char *restore_filename)
{

	if(restore_filename)
	{
		FILE *fp_restore = fopen(restore_filename, "r");
		/* fp_restore rewinded in finding max_index */
		int idx, c,i;
		double fmin, fmax;
		int next_index = 1;
		c = fgetc(fp_restore);
		ungetc(c, fp_restore);

		if (fgetc(fp_restore) == 'x')
		{
			fscanf(fp_restore, "%lf %lf\n", &model.lower, &model.upper);
			while(fscanf(fp_restore,"%d %lf %lf\n",&idx,&fmin,&fmax)==3)
			{
				for(i = next_index;i<idx;i++)
					if(model.feature_min[i-1] != model.feature_max[i-1])
						fprintf(stderr,	"WARNING: feature index %d appeared in file %s was not seen in the scaling factor file %s.\n",i, "", restore_filename);

				model.feature_min[idx-1] = fmin;
				model.feature_max[idx-1] = fmax;

				next_index = idx + 1;
			}

			for(i = next_index ; i <= model.max_index; i++)
				if(model.feature_min[i-1] != model.feature_max[i-1])
					fprintf(stderr,	"WARNING: feature index %d appeared in file %s was not seen in the scaling factor file %s.\n",i, "", restore_filename);
		}
		fclose(fp_restore);
	}

}

MODEL_OCR_RESPONSE SVMClassifier::MODEL_OCR(Mat Feature, SVM_MODEL &model)
{
    MODEL_OCR_RESPONSE OCR_R;
    svm_node *svm_nodes = new svm_node[model.feature_vector_size + 1];

	for (int featureIndex = 0 ; featureIndex < Feature.cols ; featureIndex++)
    {

        svm_nodes[featureIndex].index = featureIndex + 1;
        svm_nodes[featureIndex].value = MODEL_output(model, featureIndex,(double)Feature.at<float>(0,featureIndex));
    }

    svm_nodes[model.feature_vector_size ].index = -1;
    svm_nodes[model.feature_vector_size ].value = 0;
    double Testing_Time = cv::getTickCount();
	OCR_R.probability = new double[model.number_of_classes];
    double response = svm_predict_probability(model.model, svm_nodes, OCR_R.probability);
    OCR_R.response = response ;
    return OCR_R;
}
extern "C" __declspec(dllexport) int __stdcall FeatureSVMTrain(char* inPath, char* outPath)
{
	SVMClassifier SvmClassifier;
	Mat input = imread(inPath, CV_LOAD_IMAGE_GRAYSCALE);
	char* tmpPath = (char*)malloc(strlen(inPath) + 1);
	strcpy_s(tmpPath, strlen(inPath) + 1, inPath);
	char * name = strrchr(tmpPath, '\\');
	char * ext = strrchr(tmpPath, '.');
	name++; 
	*ext = 0;
	int classIndex = SvmClassifier.FindSymbolIndex(name);
	if(classIndex < 0)
	{
		delete tmpPath;
		return 1;
	}
	threshold(input, input, 128, 255, CV_THRESH_BINARY_INV);
	//================================
	Mat features;
	SvmClassifier.Feature_Char(input, features);
	//================================
	if(outPath!=0)
		SvmClassifier.CreateSVMFeatureFile(outPath, classIndex, features);
	features.~Mat();
	delete tmpPath;
	return 0;
}
extern "C" __declspec(dllexport) int __stdcall LoadSVMModels(void**svm_classifier)
{
	*svm_classifier = new SVMClassifier();
	
	char* models_path[] = {"D:\\svm-lib\\windows\\model02\\model_letter","D:\\svm-lib\\windows\\model02\\model_letter_","D:\\svm-lib\\windows\\model02\\model__letter","D:\\svm-lib\\windows\\model02\\model__letter_"};
	((SVMClassifier*)*svm_classifier)->LoadSvmModels(models_path);
	 CreateHashArray();
	return 0;
}
extern "C" __declspec(dllexport) int __stdcall GetOCR(void**svm_classifier, char* inPath, wchar_t** strOcrText)
{
	SVMClassifier *classifier = (SVMClassifier *)*svm_classifier;
	Mat src = imread(inPath, CV_LOAD_IMAGE_GRAYSCALE);
	if(!src.data)
	{
		return 1;
	}
	
	vector<Word> arrWords;
	LineSegment(src, arrWords);
	for(int i = arrWords.size() - 1; i >= 0 ; i--)
	{
		Word newWord = arrWords[i];
		cvNamedWindow("Detected Word", CV_WINDOW_KEEPRATIO);
		imshow("Detected Word", src(newWord.boundRect) * 255);cvWaitKey();
		for(int j = newWord.arrChars.size() - 1; j >= 0; j--)
		{
			Character newChar = newWord.arrChars[j];
			Mat features;
			classifier->Feature_Char(newChar.imgChar * 255, features);
			
			MODEL_OCR_RESPONSE ocr_response;
			switch(newChar.position)
			{
			case POS_ALONE://4
				//imshow("POS_ALONE", newChar.imgChar * 255);cvWaitKey();
				ocr_response = classifier->MODEL_OCR(features, classifier->models.model_letter);
				break;
			case POS_END://3
				//imshow("POS_END", newChar.imgChar * 255);cvWaitKey();
				ocr_response = classifier->MODEL_OCR(features, classifier->models.model_letter_);
				break;
			case POS_START://1
				//imshow("POS_START", newChar.imgChar * 255);cvWaitKey();
				ocr_response = classifier->MODEL_OCR(features, classifier->models.model__letter);
				break;
			case POS_MIDDLE://2
				//imshow("POS_MIDDLE", newChar.imgChar * 255);cvWaitKey();
				ocr_response = classifier->MODEL_OCR(features, classifier->models.model__letter_);
				break;
			}
			newChar.ch = arrModelSymbols2[ocr_response.response - 1];
			newWord.text.push_back(newChar.ch);// += FindFarsiChar(ocr_response.response - 1 , arrModelSymbols2[ocr_response.response - 1]);
			free(ocr_response.probability);
		}
		newWord.ocrText = ConvertToFarsi(newWord.text);
		MessageBoxW(0,newWord.ocrText.c_str(),0,0);
		arrWords[i] = newWord;
	}
}
