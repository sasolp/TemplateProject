#include "stdafx.h"
#include "CharacterSegment.h"
void FitWindow_CH(Mat src, Mat &dst)
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
void RogueChar(Mat &src, int strokeSize, int position)
{
	int len = 0;
	int sum = 0;
	switch(position)
	{
	case POS_MIDDLE:
		
		for(int j = 0; j < src.cols; j++)
		{
			int transition = 0;
			sum = 0;
			for (int i = 0 ; i < src.rows ; i++)
			{
				if(src.data[i * src.cols + j])
				{
					transition++;
					while(i < src.rows && src.data[i * src.cols + j])
					{
						i++;
						sum++;
					}
				}
				
			}
			if(transition <= 1 && abs(sum - strokeSize) < 2)
			{
				len++;
			}
			else if(sum > 0)
				break;
		}
		//imshow("POS_MIDDLE", src * 255);cvWaitKey();		
		if(len > strokeSize)
			src(Rect(0, 0, len - strokeSize, src.rows - 1)) = 0;
		//imshow("POS_MIDDLE", src * 255);cvWaitKey();

		len = 0;
		sum = 0;
		for(int j = src.cols - 1; j >= 0; j--)
		{
			int transition = 0;
			sum = 0;
			for (int i = 0 ; i < src.rows ; i++)
			{
				if(src.data[i * src.cols + j])
				{
					transition++;
					while(i < src.rows && src.data[i * src.cols + j])
					{
						i++;
						sum++;
					}
				}
				
			}
			if(transition <= 1 && abs(sum - strokeSize) < 2)
			{
				len++;
			}
			else  if(sum > 0)
				break;
		}
		//imshow("POS_MIDDLE", src * 255);cvWaitKey();		
		if(len > strokeSize)
			src(Rect(src.cols - len + strokeSize, 0, len - strokeSize, src.rows - 1)) = 0;
		//cvNamedWindow("POS_MIDDLE", CV_WINDOW_KEEPRATIO);
		//imshow("POS_MIDDLE", src * 255);cvWaitKey();
		break;

	case POS_START:
		
		len = 0;
		sum = 0;
		for(int j = 0; j < src.cols; j++)
		{
			int transition = 0;
			sum = 0;
			for (int i = 0 ; i < src.rows ; i++)
			{
				if(src.data[i * src.cols + j])
				{
					transition++;
					while(i < src.rows && src.data[i * src.cols + j])
					{
						i++;
						sum++;
					}
				}
				
			}
			if(transition <= 1 && abs(sum - strokeSize) < 2)
			{
				len++;
			}
			else if(sum > 0)
				break;
		}
		//imshow("POS_START", src * 255);cvWaitKey();		
		if(len > strokeSize)
			src(Rect(0, 0, abs(len - strokeSize), src.rows - 1)) = 0;
		//cvNamedWindow("POS_START", CV_WINDOW_KEEPRATIO);
		//imshow("POS_START", src * 255);cvWaitKey();
		break;

	case POS_END:
		 len = 0;
		for(int j = src.cols - 1; j >= 0; j--)
		{
			int transition = 0;
			sum = 0;
			for (int i = 0 ; i < src.rows ; i++)
			{
				if(src.data[i * src.cols + j])
				{
					transition++;
					while(i < src.rows && src.data[i * src.cols + j])
					{
						i++;
						sum++;
					}
				}
				
			}
			if(transition <= 1 && abs(sum - strokeSize) < 2)
			{
				len++;
			}
			else if(sum > 0)
				break;
		}
		//imshow("POS_END", src * 255);cvWaitKey();
		if(len > strokeSize)
			src(Rect(src.cols - len + strokeSize, 0, len - strokeSize, src.rows - 1)) = 0;
		//cvNamedWindow("POS_END", CV_WINDOW_KEEPRATIO);
		//imshow("POS_END", src * 255);cvWaitKey();
		break;

	}
}
int FindPosition(int start, int end, vector<int> &slices)
{
	const int MAX_DISTANCE = 2;
	int position = POS_ALONE;
	bool bStartIsAtSides = false;
	bool bEndIsAtSides = false;
	for(int i = 0 ; i < slices.size(); i++)
	{
		if(abs(start - slices[i]) <= MAX_DISTANCE)
		{
			bStartIsAtSides = true;
		}
		if(abs(end - slices[i]) <= MAX_DISTANCE)
		{
			bEndIsAtSides = true;			
		}
	}
	if(bStartIsAtSides && bEndIsAtSides)
	{
		position = POS_MIDDLE;
	}
	else if(bStartIsAtSides && !bEndIsAtSides)
	{
		position = POS_START;
	}
	else if(!bStartIsAtSides && bEndIsAtSides)
	{
		position = POS_END;
	}

	return position;
}
void FindCharsInWords(Mat &src, vector<Word> &arrImgWords, double sw)
{
	const int MIN_VERTICAL_PROJECTION = 0;
	byte* pData1 = (byte*)src.data;
	double sum = 0;   
	int start = 0, end = 0;
	int prevPos = 0;
	//cvNamedWindow("POS_ALONE", CV_WINDOW_KEEPRATIO);
	//cvNamedWindow("POS_ALONE2", CV_WINDOW_KEEPRATIO);
	const int MAXIMUM_LEVEL_COUNT = 4;
	int visitedColors[MAXIMUM_LEVEL_COUNT];
	for(int index = 0; index < arrImgWords.size(); index++)
	{
		
		Rect boundRect = arrImgWords[index].boundRect;
		
		start = boundRect.x + 1;
		end = boundRect.x + boundRect.width + 1;
		prevPos = start;
		bool bCharFound = false;
		bool bNewSearch = true;
		
		for (int j = start ; j < end ; j++)
		{
			sum = 0;
			int nextColumn = 0;
			if(bNewSearch)
			{
				memset(visitedColors, -1, sizeof(int) * MAXIMUM_LEVEL_COUNT);				
				for (int j = prevPos ; j < end && j < prevPos + sw * 2 ; j++)
				{
					nextColumn = 0;
					for (int i = 0; i < src.rows; i++)
					{
						if(pData1[i * src.cols + j])
						{							
							for(int k = 0 ; pData1[i * src.cols + j] != visitedColors[k] && k < MAXIMUM_LEVEL_COUNT ; k++)
							{
								if(visitedColors[k] < 0)
								{
									visitedColors[k] = pData1[i * src.cols + j];
									break;
								}
							}
						}

					}
				}
				bNewSearch = false;
			}
			bool bFound = false;
			for (int i = 0; i < src.rows; i++)
			{
				uchar val = pData1[i * src.cols + j];
				if(val == visitedColors[0] || val == visitedColors[1] || val == visitedColors[2] || val == visitedColors[3] )
				{
					bFound = true;
				}
			}
			if(!bFound && (j - prevPos) > sw)
			{
				bCharFound = true;
				bNewSearch = true;
				
				Character detectedChar;
				detectedChar.position = FindPosition(prevPos, j, arrImgWords[index].slices);
				detectedChar.imgChar = src(Rect(prevPos, 0, j - prevPos, src.rows)).clone();
				if(detectedChar.position != POS_ALONE)
					RogueChar(detectedChar.imgChar , sw, detectedChar.position);
				//else
					//imshow("POS_ALONE", detectedChar.imgChar * 255);cvWaitKey();
				
				arrImgWords[index].arrChars.push_back(detectedChar);
				bFound = false;
				while(j < end && !bFound)
				{
					j++;
					for (int i = 0; i < src.rows; i++)
					{
						if(pData1[i * src.cols + j])
						{
							j--;
							bFound = true;
							break;
						}
					}
				}
				prevPos = j;
			}

		}
		if(!bCharFound)
		{
			Character detectedChar;
			detectedChar.position = FindPosition(start, end, arrImgWords[index].slices);
			detectedChar.imgChar = src(boundRect).clone();
			arrImgWords[index].arrChars.push_back(detectedChar);
			//imshow("POS_ALONE2", detectedChar.imgChar * 255);cvWaitKey();
		}
		else if(abs(end - prevPos) >= 3)
		{
			Character detectedChar;
			detectedChar.position = FindPosition(prevPos, end, arrImgWords[index].slices);
			detectedChar.imgChar = src(Rect(prevPos, 0,end - prevPos, src.rows)).clone();
			arrImgWords[index].arrChars.push_back(detectedChar);
		}
	}
	
}
void LineSegment(Mat &imgInputLine, vector<Word> &arrImgWords)
{
	const int MIN_STROKE_SIZE = 7;
	Mat BW;
	Mat fit_BW;
	threshold(imgInputLine, BW, 255 * 0.9, 1, CV_THRESH_BINARY | CV_THRESH_BINARY_INV);
	double *s2 = sum(BW).val ;
	FitWindow_CH(BW, fit_BW);
	//double *s = sum(fit_BW).val ;
	int rows = fit_BW.rows, cols = fit_BW.cols;
	imshow("fit_BW_M",fit_BW * 255);cvWaitKey();
////////////////////////////////////////////////size ghalam tasvire avalieh
	double sw=StrokeWidthMeasurement4DTime(fit_BW);
	
    if(sw > MIN_STROKE_SIZE )
	{
		Mat thined ;
		thining(fit_BW, thined, 1); 
		fit_BW.~Mat();
		fit_BW = thined;
	}
	
	imshow("fit_BW_M",fit_BW * 255);cvWaitKey();
////////////////////////////////////////////////size ghalam tasvire thin shode dar sorat koloft
////////////////////////////////////////////////bodan
	Mat fit_BW_M;
	
	copyMakeBorder(fit_BW, fit_BW, 1, 1, 0, 0, BORDER_CONSTANT, Scalar(0));
	copyMakeBorder(fit_BW, fit_BW_M, 0, 0, 0, 2, BORDER_CONSTANT, Scalar(0));
	//imshow("fit_BW",fit_BW_M * 255);cvWaitKey();
    sw = StrokeWidthMeasurement4DTime(fit_BW_M);        
//////////////////////////////////////////////// mohasebeye size ghalam jadid
	vector<int> arrStrokeSizes;
    for( int i = 0; i < fit_BW_M.cols; i++)
	{
		int first = 0; 
		int last = 0;
		for(int j = 0; j < fit_BW_M.rows; j++)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i])
			{
				first = j;
				break;
			}
		}
		for(int j = fit_BW_M.rows - 1; j >= 0; j--)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i])
			{
				last = j;
				break;
			}
		}
        int ind = last - first;
        if (ind < sw * 1.5 && ind > sw/2)
			arrStrokeSizes.push_back(ind);
	}       
        
////////////////////////////////////////////////size ghalam jadid
	sort(arrStrokeSizes.begin(), arrStrokeSizes.end());
	int strokesCount = arrStrokeSizes.size(); 
	if(strokesCount)
	{
		if(strokesCount % 2)
		{
			sw = arrStrokeSizes[strokesCount / 2] + 1;
		}
		else
		{
			sw = (arrStrokeSizes[strokesCount / 2] + arrStrokeSizes[(strokesCount - 1) / 2]) * 0.5 + 1;
		}
	}
////////////////////////////////////////////////bakhshhai ba size kamtar az sw be onvane kandid

    int par1 = cvRound((5*sw)/4);
    if (par1 == 0)
        par1 = 1;
    int par2 = cvRound(sw/2);
    if (par2 == 0)
        par2 = 1;
        
    int par1_0 = cvRound((3*sw)/4);
    if (par1_0 == 0)
        par1_0 = 1;
	Mat fit_BW2(fit_BW_M.rows,fit_BW_M.cols, CV_8UC1);
	fit_BW2 = 0;
	Mat fit_BW2_body(fit_BW_M.rows,fit_BW_M.cols, CV_8UC1);
	fit_BW2_body = 0; 
	for( int i = 0; i < fit_BW_M.cols; i++)
	{
		int first = -1; 
		int last = -1;
		for(int j = 0; j < fit_BW_M.rows; j++)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i])
			{
				first = j;
				break;
			}
		}
		for(int j = fit_BW_M.rows - 1; j >= 0; j--)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i])
			{
				last = j;
				break;
			}
		}
        int ind = last - first;
		if (first > 0 && ind <  1.5*sw )
			fit_BW2(Range(0, fit_BW2.rows),Range(i, i + 1)) = 1;
	}       
    //imwrite("c:\\200.bmp", fit_BW2 * 255);
    //imwrite("c:\\201.bmp", fit_BW_M * 255);
        
//////////////////////////////////////////////// bakhshhhaye faseledar az baseline kandid nistan
    int par = max(cvRound(2*sw), cvRound(rows/3));
	vector<int> array_R_label;
    vector<int> array_R2_label;
	for( int i = 3; i < fit_BW2.cols; i++)
	{
		
		if( sum(fit_BW2(Range(0,fit_BW2.rows), Range(i - 1, i ))).val[0] == 0 &&
			sum(fit_BW2(Range(0,fit_BW2.rows), Range(i, i + 1))).val[0] > 0 && 
			sum(fit_BW_M(Range(0,fit_BW2.rows), Range(i - 2, i - 1))).val[0] > 0)
		{
		for(int j = 0; j < fit_BW_M.rows; j++)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i] && j > par)
			{
				array_R_label.push_back(j);
				break;
			}
		}
		for(int j = fit_BW_M.rows - 1; j >= 0; j--)
		{
			if(fit_BW_M.data[j * fit_BW_M.cols + i] && j > par)
			{
				array_R2_label.push_back(j);
				break;
			}
		}
		}
	}   
    sort(array_R_label.begin(), array_R_label.end());
	int  R_labelCount= array_R_label.size(); 
	float base_segment = 0;
	if(R_labelCount % 2)
	{
		base_segment = array_R_label[R_labelCount / 2] ;
	}
	else
	{
		base_segment = (array_R_label[R_labelCount / 2] + array_R_label[(R_labelCount - 1) / 2]) * 0.5 ;
	}
        
//////////////////////////////////////////   hazfe linehaye zirin
	Mat body_fit, body_fit_ref;
	//imshow("fit_BW",fit_BW * 255);cvWaitKey();
    BWLabel(fit_BW, body_fit, 20*sw);
	//imshow("body_fit",body_fit * 255);cvWaitKey();
   

    if (rows - base_segment > 5*sw)
        body_fit(Range(rows-1.5*sw, rows ), Range(0, body_fit.cols)) = 0;
    Mat body_fit2;
	body_fit = body_fit & fit_BW;
    thining(body_fit, body_fit2);
	cv::copyMakeBorder(body_fit, body_fit_ref, 0, 0, 0, 2, BORDER_CONSTANT, Scalar(0));
	cv::copyMakeBorder(fit_BW, fit_BW, 0, 0, 0, 2, BORDER_CONSTANT, Scalar(0));
	int startOfTextLine = 2;
	vector<SpaceCoordinate> arrSpaceCoordinates;
	DetectSpaces_CH(fit_BW, arrSpaceCoordinates);
	SpaceCoordinate prevSpace;
	if(arrSpaceCoordinates.size())
	{
		prevSpace = arrSpaceCoordinates[0];
		if(prevSpace.start > startOfTextLine + 1)
		{
			Word newWord;
			newWord.boundRect.height = fit_BW.rows;
			newWord.boundRect.y = 0;
			newWord.index = 0;
			newWord.boundRect.x = 0;
			newWord.boundRect.width = prevSpace.start;
			arrImgWords.push_back(newWord);
			//imshow("word", fit_BW(newWord.boundRect) * 255);cvWaitKey();
		}

	}
	for(int i = 1; i < arrSpaceCoordinates.size(); i++)
	{
		SpaceCoordinate space = arrSpaceCoordinates[i];
		Word newWord;
		newWord.boundRect.height = fit_BW.rows;
		newWord.boundRect.y = 0;
		newWord.index = i;
		newWord.boundRect.x = prevSpace.end;
		newWord.boundRect.width = space.start - prevSpace.end;
		arrImgWords.push_back(newWord);
		//imshow("word", fit_BW(newWord.boundRect) * 255);cvWaitKey();
		prevSpace = space;

	}
	//imwrite("c:\\201.bmp", body_fit * 255);
	//imwrite("c:\\200.bmp", body_fit2 * 255);
	cv::copyMakeBorder(body_fit2, body_fit2, 0, 0, 2, 0, BORDER_CONSTANT, Scalar(0));

    Mat fit_BW_body = body_fit;
    Mat fit_BW_M_body;
	cv::copyMakeBorder(fit_BW_body, fit_BW_M_body, 0, 0, 2, 0, BORDER_CONSTANT, Scalar(0));
	Mat Proj_Fit(1, body_fit_ref.cols, CV_32FC1);
	for(int i = 0 ; i < body_fit_ref.cols; i++)
	{
		Proj_Fit.data[i] = 0;
		for(int j = 0; j < body_fit_ref.rows; j++)
		{
			Proj_Fit.data[i] += body_fit_ref.data[j * body_fit_ref.cols + i];
		}
	}   
	Mat Edge_body;
	float filterData[] = {-1, 1};
	Mat kernel(2, 1, CV_32F, filterData);
	filter2D(body_fit2, Edge_body, -1, kernel);
	for(int i = 0 ; i < Edge_body.rows; i++)
	{
		int step = i * Edge_body.cols;
		for(int j = 0; j < Edge_body.cols; j++)
		{
			if(Edge_body.data[step + j] < 0) 
				Edge_body.data[step + j] = 0;
		}
	}   
    int param = rows - base_segment;
    if (param > sw);
        param = sw;
	int Proj_Edge = base_segment;
	int max_value = 0;
	for(int i = base_segment ; i < Edge_body.rows && i <= (base_segment+param); i++)
	{
		int step = i * Edge_body.cols;
		int sum = 0;
		for(int j = 0; j < Edge_body.cols; j++)
		{
			sum += Edge_body.data[step + j];
		}
		if(sum > max_value)
		{
			Proj_Edge = i - base_segment ;
			max_value = sum;
		}
	} 
   Proj_Edge--;
  //imshow("Edge_body",Edge_body * 255);cvWaitKey();
  //imwrite("c:\\200.bmp", body_fit2 * 255);
   for(int i = 0 ; i < Edge_body.cols; i++)
	{
		int first = 0; 
		int last = 0;
		for(int j = 0; j < Edge_body.rows; j++)
		{
			if(Edge_body.data[j * Edge_body.cols + i])
			{
				first = j - 2;
				break;
			}
		}
		for(int j = Edge_body.rows - 1; j >= 0; j--)
		{
			if(Edge_body.data[j * Edge_body.cols + i])
			{
				last = j - 2;
				break;
			}
		}
		if( (first - (base_segment+Proj_Edge)<-1) || (first - (base_segment+(sw-1)/2) > par1/2.0) || first!=last )
            Edge_body(Range(0, Edge_body.rows), Range(i, i + 1)) = 0;
	}
  //imshow("Edge_body",Edge_body * 255);cvWaitKey();
   /*
    for i= sw:Col+2
            ind = find(fit_BW_M_body(:,i),1, 'last')-find(fit_BW_M_body(:,i),1, 'first');
            if (ind <=  1.5*sw ) & find(Edge_body(:,i),1, 'first')>1

                 fit_BW2_body(:,i)=1;
            end
            
        end
   */
    for(int i = 0 ; i < fit_BW_M_body.cols; i++)
	{
		int first = 0; 
		int last = 0;
		int edge_body_first  = 0;
		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i])
			{
				first = j;
				break;
			}
		}
		for(int j = fit_BW_M_body.rows - 1; j >= 0; j--)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i])
			{
				last = j;
				break;
			}
		}
		for(int j = 0; j < Edge_body.rows; j++)
		{
			if(Edge_body.data[j * Edge_body.cols + i])
			{
				edge_body_first = j;
				break;
			}
		}
		int ind = last - first;
		if(ind <=  1.5*sw && edge_body_first > 0)
            fit_BW2_body(Range(0, Edge_body.rows), Range(i, i + 1)) = 1;
	}
	
	//imshow("fit_BW2_body",fit_BW2_body * 255);cvWaitKey();
	//imshow("fit_BW_M_body", fit_BW_M_body* 255);cvWaitKey();
	//imwrite("c:\\102.bmp", fit_BW2_body * 255);
    int par4 = 1;
    if( cvRound(sw / 4) > 1)
        par4 = cvRound(sw/4);
		for( int i = 3; i < fit_BW2_body.cols; i++)
	{
		if( sum(fit_BW2_body(Range(0, fit_BW2_body.rows), Range(i - 1, i))).val[0] > 0 &&
			sum(fit_BW2_body(Range(0, fit_BW2_body.rows),Range(i+1, i + par4 + 1))).val[0] > 0 )
		{
			fit_BW2_body(Range(0, fit_BW2_body.rows), Range(i, i + 1)) = 1;
		}
		int first = -1; 
		int firstPrev = -1; 
		int firstNext = -1; 
		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i])
			{
				first = j;
				break;
			}
		}
		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i - 1])
			{
				firstPrev = j;
				break;
			}
		}

		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i + 1])
			{
				firstNext = j;
				break;
			}
		}

		if(firstPrev >= 0 && firstNext >= 0 && first >= 0 && sum(fit_BW2_body(Range(0, fit_BW2_body.rows), Range(i-1, i))).val[0] == 0 && 
			sum(fit_BW2_body(Range(0, fit_BW2_body.rows), Range(i, i + 1))).val[0] > 0 && 
			firstPrev > first && firstNext < first)
		{
			fit_BW2_body(Range(0,fit_BW2.rows),Range(i,i + 1)) = 0;
		}
	}  
	for( int i = 3; i < fit_BW2_body.cols; i++)
	{
		int first = -1; 
		int firstPrev = -1; 
		int firstNext = -1; 
		int firstNext2 = -1; 
		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i])
			{
				first = j;
				break;
			}
		}
		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i - 1])
			{
				firstPrev = j;
				break;
			}
		}

		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i + 1])
			{
				firstNext = j;
				break;
			}
		}

		for(int j = 0; j < fit_BW_M_body.rows; j++)
		{
			if(fit_BW_M_body.data[j * fit_BW_M_body.cols + i + 2])
			{
				firstNext2 = j;
				break;
			}
		}
        if (first >= 0 && ((first - base_segment > par1) || (base_segment - first > par2)))
		{
			fit_BW2_body(Range(0,fit_BW2.rows),Range(i, i + 1)) = 0;
		}
		else if( (first >= 0 && firstNext >= 0 && first - base_segment > par1_0 &&  firstNext - base_segment > par1) ||
				 (first >= 0 && firstNext2 >= 0 && first - base_segment > par1_0 && firstNext2 - base_segment > par1))
		{
			fit_BW2_body(Range(0,fit_BW2.rows),Range(i, i + 1)) = 0;
		}
	}  
     
  //imshow("fit_BW2_body", fit_BW2_body* 255);cvWaitKey();   
   
   // imwrite("c:\\200.bmp",body_fit2 *255 );
       
        
    Mat Label_body;
	vector<Points> labels;
	BWLabel(fit_BW2_body, Label_body, labels);
	
    body_fit_ref = body_fit_ref & fit_BW;
	Mat fit_BW_M2 = body_fit_ref;
	for(int i = 0; i < labels.size(); i++)
	{
		cv::Rect rect = boundingRect(labels[i]);
		bool bDeleted = false;
		for(int j = rect.x; j < rect.width; j++)
		{ 
			if( sum(fit_BW_M_body(Range(0,fit_BW_M_body.rows), Range(j - 1, j))).val[0] == 0 &&
				sum(fit_BW2_body(Range(0,fit_BW2_body.rows), Range(j, j + 1))).val[0] > 0)
			{
				drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
				bDeleted = true;
				break;
			}
		}
		if(!bDeleted)
		{
			line(fit_BW_M2, Point(rect.x + rect.width *.5, 0), Point(rect.x + rect.width *.5, Label_body.rows), Scalar(0), 1);
		}
	}
	
   // //imshow("fit_BW_M2", fit_BW_M2* 255);cvWaitKey();  
	//fit_BW_M2 = fit_BW_M2 & (Label_body == 0);

   // imshow("fit_BW_M2", fit_BW_M2* 255);cvWaitKey();  
    Mat fit_BW_M3(2 * fit_BW_M2.rows, fit_BW_M2.cols, fit_BW_M2.type(), Scalar(0)); 
    //fit_BW_M3(Range(cvRound(fit_BW_M2.rows/2), cvRound(fit_BW_M2.rows/2) + fit_BW_M2.rows), Range(0, fit_BW_M2.cols )) = fit_BW_M2(Range(0, fit_BW_M2.rows), Range(0, fit_BW_M2.cols));
	fit_BW_M2(Range(0, fit_BW_M2.rows), Range(0, fit_BW_M2.cols)).copyTo(fit_BW_M3(Range(cvRound(fit_BW_M2.rows/2), cvRound(fit_BW_M2.rows/2) + fit_BW_M2.rows), Range(0, fit_BW_M2.cols )));
	Mat lineKernel(cvRound(fit_BW_M2.rows/2), 1, CV_32FC1);
	lineKernel = 1;
	cv::morphologyEx(fit_BW_M3, fit_BW_M3, CV_MOP_CLOSE, lineKernel);
    
	Mat fit_BW_M4(fit_BW_M2.rows, fit_BW_M2.cols, fit_BW_M2.type());
    //fit_BW_M4(Range(0, fit_BW_M4.rows), Range(0, fit_BW_M4.cols)) = fit_BW_M3(Range(cvRound(rows/2), cvRound(rows/2) + rows), Range(0, fit_BW_M2.cols)); 
	fit_BW_M3(Range(cvRound(fit_BW_M4.rows/2), cvRound(fit_BW_M4.rows/2) + fit_BW_M4.rows), Range(0, fit_BW_M2.cols)).copyTo(fit_BW_M4(Range(0, fit_BW_M4.rows), Range(0, fit_BW_M4.cols)) );
   // imshow("fit_BW_M3", fit_BW_M3* 255);cvWaitKey();   
	//imshow("fit_body", fit_BW2_body* 255);cvWaitKey();   
    Mat Label2;   
    Mat body_fit_labels;
	vector<vector<Point>> labels2;
	vector<vector<Point>> tmpLabels;
	BWLabel(fit_BW_M4, Label2, labels2);
	BWLabel(body_fit2, body_fit_labels, tmpLabels);
    int par3 = min(cvRound(2.5*sw), cvRound(base_segment/3));
//	imshow("fit_BW_M4",fit_BW_M4 * 255);cvWaitKey();  
	std::sort( labels.begin(), labels.end(), sortContoursX);
//	imwrite("c:\\200.bmp", body_fit_labels);
   	for(int i = 0; i < labels.size(); i++)
	{
		cv::Rect rect = boundingRect(labels[i]);
		int Count_par = 0;
		for(int k = 2; k < par3; k++)
		{
			if(!Proj_Fit.data[rect.x - k] || !Proj_Fit.data[rect.x + rect.width + k])
			{
				Count_par=1;
				break;
			}
		}
		if(Count_par == 1)
		{
			cv::Rect rect2;bool prevIsExist = false;int nFoundedIndex = -1;bool nextIsExist = false;

			for(int index = 0; index < labels2.size(); index++)
			{
				rect2 = boundingRect(labels2[index]);
				int middle = rect.x + rect.width *.5;
				if(rect2.x + rect2.width > middle - 2 && rect2.x + rect2.width < middle + 2)// - 5
				{
					nFoundedIndex = index;
					prevIsExist = true;
					int base;
					Rect prevStrokeRect = FindPrevStrokeRect(body_fit_labels, rect.x + rect.width *.5, base);
					if(prevStrokeRect.width < 0)
					{
						drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
						break;
					}
					if(prevStrokeRect.y + prevStrokeRect.height < base + 2 && prevStrokeRect.height < 5*sw && abs(prevStrokeRect.width - rect2.width) < sw)
					{
						drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
						break;
					}
				}
				if(middle > rect2.x - 2 && middle < rect2.x + 2)// - 5
				{
					nFoundedIndex = index;
					nextIsExist = true;
					int base;
					Rect nextStrokeRect = FindNextStrokeRect(body_fit_labels, rect.x + rect.width *.5, base);
					if(nextStrokeRect.width < 0)
					{
						drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
						break;
					}
					if(nextStrokeRect.y + nextStrokeRect.height < base + 2 && nextStrokeRect.height < 3*sw && nextStrokeRect.width < sw)
					{
						drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
						break;
					}
				}
				if(nextIsExist && prevIsExist)
					break;
			}
			/*if((prevIsExist || nextIsExist) && (rect2.width < 3*sw && rect2.height < 3*sw && rect2.y > base_segment - 3*sw && rect2.y < base_segment + 3*sw))
			{
				drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
			}
			else */
			/*if(nextIsExist)
			{
				int base;
				Rect nextStrokeRect = FindNextStrokeRect(body_fit_labels, rect.x + rect.width *.5, base);
				if(nextStrokeRect.y + nextStrokeRect.height < base + 2 && nextStrokeRect.height < 3*sw && nextStrokeRect.width < 5)
					drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
			}
			if(prevIsExist)
			{
				int base;
				Rect prevStrokeRect = FindPrevStrokeRect(body_fit_labels, rect.x + rect.width *.5, base);
				if(prevStrokeRect.y + prevStrokeRect.height < base + 2 && prevStrokeRect.height < 5*sw && abs(prevStrokeRect.width - rect2.width) < 5)
					drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
			}*/
			/*if(prevIsExist && ((rect2.height < 6*sw & rect2.y> base_segment - 2*par3) || rect2.height < 3*sw))
			{
				drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
			}
			if(nextIsExist && ((rect2.height < 6*sw & rect2.y> base_segment - 2*par3) || rect2.height < 3*sw))
			{
				drawContours(Label_body, labels, i, Scalar(0), CV_FILLED);
			}*/
		}
	}
	//imshow("Label_body", Label_body* 255);cvWaitKey();
	Mat input_copy = fit_BW.clone();
	//dilate(input_copy, input_copy, Mat());
	Mat finalCrossRegions;
	labels.clear();
	BWLabel(Label_body, finalCrossRegions, labels);
    for(int i = 0; i < labels.size(); i++)
	{
		cv::Rect rect = boundingRect(labels[i]);
		rect.x -= 2;   
		int sliceOffset = rect.x + rect.width *.5;
		for(int j = 0 ; j < arrImgWords.size(); j++)
		{
			Rect wordRect = arrImgWords[j].boundRect;
			if(sliceOffset >= wordRect.x && sliceOffset <= wordRect.x + wordRect.width)
			{
				arrImgWords[j].slices.push_back(sliceOffset);
				break;
			}
		}
		line(input_copy, Point(sliceOffset, 0), Point(sliceOffset, Label_body.rows), Scalar(0), 1);
	}
	cvDestroyAllWindows();
	Mat final_Labeled;
	labels.clear();
	BWLabel(input_copy, final_Labeled,  sw );
	input_copy = input_copy * 255;
	imshow("final_Labeled", final_Labeled* 255);cvWaitKey();
	final_Labeled = final_Labeled & input_copy;
	imshow("final_Labeled", final_Labeled* 255);cvWaitKey();
	FindCharsInWords(final_Labeled, arrImgWords, sw);
	imgInputLine.~Mat();
	imgInputLine = final_Labeled.clone();
	/////////////////
	////   SAD	/////
	/////////////////

	/////////////////
	////   SAD	/////
	/////////////////

	//FindContours(input_copy, labels, CV_RETR_EXTERNAL);
	//std::sort( labels.begin(), labels.end(), sortContoursX);

	//imshow("input_copy",input_copy * 255);cvWaitKey();
	//double area = 0;
	//for(int i = 0; i < labels.size(); i++)
	//{
	//	cv::Rect rect = boundingRect(labels[i]);
	//	area = sum(fit_BW(rect)).val[0];
	//	if(area > sw * 5)
	//	{
	//		Mat charImg = fit_BW(rect).clone();
	//		RogueChar(charImg, sw, true);
	//		arrImgChars.push_back(charImg);
	//		imshow("input_copy",arrImgChars[arrImgChars.size() - 1] * 255);cvWaitKey();
	//	}
	//
	//}
}
Rect FindPrevStrokeRect(Mat &label, int xStart, int &base)
{
	int minX = 100000, maxX = 0, minY = 100000, maxY = 0;
	bool bFounded = true;
	int firstValue = -1;  
	for(int j = 0; j < label.rows; j++)
	{
		if(label.data[j * label.cols + xStart])
		{
			firstValue = label.data[j * label.cols + xStart];
			base = j;
			break;
		}
		if(label.data[j * label.cols + xStart + 1])
		{
			firstValue = label.data[j * label.cols + xStart + 1];
			base = j;
		}
		if( label.data[j * label.cols + xStart + 2])
		{
			firstValue = label.data[j * label.cols + xStart + 2];
			base = j;
			break;
		}
	}
	
	while(bFounded && xStart >= 0)
	{
		xStart--;
		bFounded = false;
		for(int i = 0 ; i < label.rows; i++)
		{
			if(label.data[i * label.cols + xStart] == firstValue)
			{
				bFounded = true;
				if(i > maxY)
				{
					maxY = i;
				}
				if(i < minY)
				{
					minY = i;
				}
			}
		}
		if(bFounded)
		{
			if(xStart > maxX)
			{
				maxX = xStart;
			}
			if(xStart < minX)
			{
				minX = xStart;
			}
		}
	}
	return Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}
Rect FindNextStrokeRect(Mat &label, int xStart, int &base)
{
	int minX = 100000, maxX = 0, minY = 100000, maxY = 0;
	bool bFounded = true;
	int firstValue = -1;  
	for(int j = 0; j < label.rows; j++)
	{
		if(label.data[j * label.cols + xStart])
		{
			firstValue = label.data[j * label.cols + xStart];
			base = j;
			break;
		}
		if(label.data[j * label.cols + xStart - 1])
		{
			firstValue = label.data[j * label.cols + xStart - 1];
			base = j;
		}
		if( label.data[j * label.cols + xStart - 2])
		{
			firstValue = label.data[j * label.cols + xStart - 2];
			base = j;
			break;
		}
	}
	
	while(bFounded && xStart < label.cols)
	{
		xStart++;
		bFounded = false;
		for(int i = 0 ; i < label.rows; i++)
		{
			if(label.data[i * label.cols + xStart] == firstValue)
			{
				bFounded = true;
				if(i > maxY)
				{
					maxY = i;
				}
				if(i < minY)
				{
					minY = i;
				}
			}
		}
		if(bFounded)
		{
			if(xStart > maxX)
			{
				maxX = xStart;
			}
			if(xStart < minX)
			{
				minX = xStart;
			}
		}
	}
	return Rect(minX, minY, maxX - minX + 1, maxY - minY + 1);
}
inline bool sortContoursX(const Points& i, const Points& j)
{

		cv::Rect rect_i = boundingRect(i); 
		cv::Rect rect_j = boundingRect(j); 
	if(rect_i.x < rect_j.x )
		return true;
	else
		return false;
}
void BWLabel(Mat src, Mat &dst, vector<vector<Point>> &labels )
{
	dst = src.clone();
	FindContours(src, labels, CV_RETR_LIST);
	dst = 0;
	int labelCounter = 1;
	for(int i = 0; i < labels.size(); i++)
	{		
		drawContours(dst, labels, i, Scalar(labelCounter++), CV_FILLED);
	}     
}
 const CvPoint icvCodeDeltas[8] =
    { {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1} };
void FindContours(Mat src, vector<vector<Point>> &_contours, int mode)
{
	//src = imread("C:\\100.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	//IplImage newSrc = (IplImage)src;
	//IplImage* tmp = cvCloneImage(&newSrc);
	//CvMemStorage * sm = cvCreateMemStorage();
	//CvChain *contours;
	//CvChain *nextContour;
	//double area = 0;	
	////cvFindContours(tmp, sm, (CvSeq**)&contours, 88, mode, CV_CHAIN_APPROX_SIMPLE);
	//cvFindContours(tmp, sm, (CvSeq**)&contours, sizeof(CvChain), mode, CV_CHAIN_CODE);
	//
	//cvZero(tmp);
	//nextContour = contours;
	//while(nextContour) 
	//{ 
	//	//nextContour->
	//	CvChainPtReader  reader;
	//	cvStartReadSeq( (CvSeq *) nextContour, (CvSeqReader *) &reader, 0 );

	//	reader.pt = nextContour->origin;
	//	for(int i = 0; i < 8; i++ )
	//	{
	//		reader.deltas[i][0] = (schar) icvCodeDeltas[i].x;
	//		reader.deltas[i][1] = (schar) icvCodeDeltas[i].y;
	//	}


 // // Chain code:
 // //        3 2 1
 // //        4   0
 // //        5 6 7
	//	//cvStartReadChainPoints(nextContour, &reader);
	//	int counter = 0;
	//	do
	//	{
	//		counter++;
	//		CvPoint pt = cvReadChainPoint(&reader);
	//	}while(counter < nextContour->total / 2);
	//	nextContour = (CvChain*)nextContour->h_next;
	//}
	//cvReleaseImage(&tmp);
	//cvReleaseMemStorage(&sm);
	IplImage newSrc = (IplImage)src;
	IplImage* tmp = cvCloneImage(&newSrc);
	CvMemStorage * sm = cvCreateMemStorage();
	CvContour *contours;
	CvContour *nextContour;
	double area = 0;	
	cvFindContours(tmp, sm, (CvSeq**)&contours, 88, mode, CV_CHAIN_APPROX_SIMPLE);
	cvZero(tmp);
	nextContour = contours;
	while(nextContour) 
	{ 
		vector<Point> tmpContour(nextContour->total );
		for(int i = 0 ; i < nextContour->total;i++)
		{
		
			CvPoint *pnt = (CvPoint *)cvGetSeqElem((CvSeq*)nextContour, i);
			tmpContour[i] = *pnt ;
		}
		_contours.push_back(tmpContour);
		nextContour = (CvContour*)nextContour->h_next;
	}
	cvReleaseImage(&tmp);
	cvReleaseMemStorage(&sm);
}

bool sortAngles_CH(const double angle1, const double angle2)
{
	return angle1 < angle2;
}
int ExtractFontHight_CH(Mat& src, int &spaceSize)
{
	const int MIN_PROJECTION = 10;
	int fontHeight = -1;
	byte* pData1 = (byte*)src.data;
	double maxSum = 0;
	double sum = 0;                
	int minimumY = -1;
	int maximumY = -1;
	for (int i = 0; i < src.rows; i++)
	{
		sum = 0;
		for (int j = 0; j < src.cols; j++)
		{
			sum += pData1[i * src.cols + j];
		}
		if (sum > maxSum)
		{
			maxSum = sum;
			if(sum > MIN_PROJECTION && minimumY < 0)
				minimumY = i;
		}
		if(sum > MIN_PROJECTION)
			maximumY = i;
	}
	int minY = 0, maxY = 0;
	if (maxSum < MIN_PROJECTION || minY < 0 || maxY < 0)
	{		
		return fontHeight;
	}
	fontHeight = maximumY - minimumY;

	
	const int MIN_VERTICAL_PROJECTION = 1;
	int j = 0;
	int spaceWidth = 0;
	vector<double> arrSpaces;
	for (j = 0; j < src.cols ; j++)
	{
		sum = 0;
		for (int i = 0; i < src.rows; i++)
		{
			sum += pData1[i * src.cols + j];
		}
		if(sum < MIN_VERTICAL_PROJECTION)
			spaceWidth++;
		else
		{
			if(spaceWidth > 2)
				arrSpaces.push_back(spaceWidth);
			spaceWidth = 0;
		}
	}
	if(spaceWidth > 2)
		arrSpaces.push_back(spaceWidth);
	double meanSpaceSize= 0; 
	sort(arrSpaces, sortAngles_CH);
	int spaceCount = 0;
	double tmpSpaceSize = fontHeight / 8;
	for(int i = 1 ; i < arrSpaces.size();i++)
	{
		if(arrSpaces[i] == arrSpaces[i - 1] )
		{
			arrSpaces.erase(arrSpaces.begin() + i);
			i--;
		}
	}
	if(arrSpaces.size() == 1)
	{
		spaceSize = arrSpaces[0];
		return fontHeight;
	}
	else if(arrSpaces.size() == 0)
	{
		spaceSize = 12;
		return fontHeight;
	}
	if(fontHeight < 24)
	{
		spaceSize = 12;
		return fontHeight;
	}
	for(int i = 0 ; i < arrSpaces.size() ;i++)
	{
		if(arrSpaces[i] > 3 && arrSpaces[i] < tmpSpaceSize && arrSpaces[i + 1] >= tmpSpaceSize)
		{
			spaceCount++;
			meanSpaceSize = (arrSpaces[i] + arrSpaces[i + 1]) * 0.5;
			break;
		}
		
	}
	/*if(spaceCount)
	{
		meanSpaceSize /= spaceCount;
	}
	else
	{
		meanSpaceSize = tmpSpaceSize;
	}
	spaceSize = meanSpaceSize;
	spaceCount = 0;meanSpaceSize= 0; 
	for(int i = arrSpaces.size()* 0.3 ; i < arrSpaces.size() * 0.9;i++)
	{
		if(abs(spaceSize - arrSpaces[i]) < 2)
		{
			spaceCount++;
			meanSpaceSize += arrSpaces[i];
		}
		
	}*/
	if(spaceCount)
	{
		meanSpaceSize /= spaceCount;
	}
	else
	{
		meanSpaceSize = tmpSpaceSize;
	}
	
	spaceSize = meanSpaceSize  ;
	return fontHeight;
}
void FindNextSpace2_CH(Mat& src,int SpaceSize, int &startX, int &endX)
{
	const int MIN_PROJECTION = 10;
	const int MIN_VERTICAL_PROJECTION = 0;
	int fontHeight = -1;
	byte* pData1 = (byte*)src.data;
	double maxSum = 0;
	double sum = 0;                
	int minimumY = -1;
	int maximumY = -1;
	int spaceWidth = 0;
	int j = startX;
	int bFoundSpace = false;
	for (j = startX; j < src.cols; j++)
	{
		sum = 0;
		if(spaceWidth == 1)
		{
			startX = j - 1;
		}
		for (int i = 0; i < src.rows; i++)
		{
			sum += pData1[i * src.cols + j];
		}
		if(sum <= MIN_VERTICAL_PROJECTION)
			spaceWidth++;
		else
		{
			if (spaceWidth > SpaceSize*0.8)
			{
				j--;
				bFoundSpace = true;
				break;
			}
			spaceWidth = 0;
		}
	}
	endX = j;
}
void DetectSpaces_CH(Mat& inputImg, vector<SpaceCoordinate> &arrCoordinates)
{
	int segWidth = 0, startX = 0, endX = 0;
	int spaceSize = 12;
	RECT rect;
	rect.top = 0;
	rect.bottom = inputImg.rows - 1;
	int fontHeight = ExtractFontHight_CH(inputImg, spaceSize);
	
	for(int m = 0; endX < inputImg.cols ;m++)
	{
		FindNextSpace2_CH(inputImg, spaceSize, startX, endX);
		rect.left = startX;
		rect.right = endX;
		segWidth =  endX - startX;
		startX = endX;
		//if(segWidth <= spaceSize * 1.5 && segWidth >=  spaceSize * 0.8)
		{
			SpaceCoordinate space;
			space.start = rect.left;
			space.end = rect.right;
			space.len = space.end - space.start;
			space.index = arrCoordinates.size() ;
			arrCoordinates.push_back(space);
		}
	}
}
































//void BWLabel(Mat src, Mat &dst, int minArea)
//{
//	dst = src.clone();
//	IplImage* tmp = &(IplImage(dst));
//	CvMemStorage * sm = cvCreateMemStorage();
//	CvContour *contours;
//	CvContour *nextContour;
//	double area = 0;	
//	cvFindContours(tmp, sm, (CvSeq**)&contours, 88, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
//	cvZero(tmp);
//	nextContour = contours;
//	int labelCounter = 1;
//	while(nextContour) 
//	{ 
//		area = sum(src(Rect(nextContour->rect.x, nextContour->rect.y, nextContour->rect.width, nextContour->rect.height))).val[0];
//		/*if(nextContour->total > 6)
//		{
//			area = cvContourArea(nextContour);
//		}
//		else 
//			area = nextContour->rect.width * nextContour->rect.height;*/
//		if(area > minArea)
//		{
//			cvDrawContours(tmp, (CvSeq*)nextContour ,cvScalar(1) , cvScalar(1), 0, -1);	
//			labelCounter++;
//		}
//		nextContour = (CvContour*)nextContour->h_next;
//	}
//	//dst = tmp;
//	//cvReleaseImage(&tmp);
//	cvReleaseMemStorage(&sm);
//}
//
double computeMean(Mat SW)
{
	double sum = 0;
	int counter = 0;
	for(int i = 0 ; i < SW.rows ; i++)
	{
		for(int j = 0 ; j < SW.cols ; j++)
		{
			if(SW.at<float>(i,j))
			{
				sum += SW.at<float>(i,j);
				counter++;
			}
		}
	}
	if(counter)
		return sum / counter;
	else
		return 0;
}
double StrokeWidthMeasurement4DTime(Mat I)
{
	int M = I.size[0];
	int N = I.size[1];
	Mat BW1 = I.clone();
	//Mat tmp;	
	//BW1 = ~I;
	for(int i = 0 ; i < BW1.rows ; i++)
	{
		for(int j = 0 ; j < BW1.cols ; j++)
		{
			BW1.at<unsigned char>(i,j) = !BW1.at<unsigned char>(i,j);
		}
	}
	//double *s = sum(BW1).val ;
	/*double sum = 0;
	for(int i = 0 ; i < BW1.rows ; i++)
	{
		for(int j = 0 ; j < BW1.cols ; j++)
		{
			sum +=BW1.at<unsigned char>(i,j);
		}
	}*/
	//threshold(I, tmp, 0, 255, CV_THRESH_BINARY);
	//imshow("bw1",tmp);cvWaitKey(0);
	BW1(Range(0,1), Range(0,N))=1;
	BW1(Range(0,M), Range(0,1))=1;
	BW1(Range(M - 1,M), Range(0,N))=1;
	BW1(Range(0,M), Range(N - 1,N))=1;
	
	Mat SW(M,N, CV_32F);
	SW = 0.0f;
	for (int i = 0 ; i < M;i++)
	{
		int j=0;
		int S=0;
		while(j<N-1)
		{
			if(BW1.at<unsigned char>(i,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for (int R=j-S; R <= j-1; R++)
					SW.at<float>(i,R)=S;            
				/*if(S>0)
				{
					ROI.at<unsigned char>(i,j-S)=1;
					ROI.at<unsigned char>(i,j-1)=1;
				} */           
				j=j+1;
				S=0;
			}
		}
	}

	for (int j=0 ; j < N;j++)
	{
		int i=19;
		int S=0;
		while(i<M-1)
		{
			if(BW1.at<unsigned char>(i,j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for(int R = i-S; R <= i-1; R++)
				{
					if(SW.at<float>(R,j)>S)
						SW.at<float>(R,j)=S;       
					/*if(S>0)
					{
						ROI.at<unsigned char>(i-S,j)=1;
						ROI.at<unsigned char>(i-1,j)=1;
					}*/
				}
				i=i+1;
				S=0;
			}
		}
	}
	/*
	*/
	for (int i = 0 ; i < M;i++)
	{
		int j=0;

		int S=0;
		while((i+j)<M-1 && j<N-1)
		{
			if(BW1.at<unsigned char>(i+j,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for( int R=j-S ; R <=j-1;R++)
				{
					if(SW.at<float>(i+R,R)>1.4*S)
						SW.at<float>(i+R,R)=1.4*S;
				}
				j=j+1;
				S=0;
			}
		}
	}
	for (int j=0 ; j < N;j++)
	{
		int i=0;
		int S=0;
		while((j+i)<N-1 && i<M-1)
		{
			if(BW1.at<unsigned char>(i,i+j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for (int R=i-S; R <= i-1; R++)
				{
					if(SW.at<float>(R,R+j)>1.4*S)
						SW.at<float>(R,R+j)=1.4*S;                            
				}
				i=i+1;
				S=0;
			}
		}
	}
	for (int i = 0 ; i < M;i++)
	{
		int j=0;
		int S=0;
		while((i-j)>=0 && j<N-1)
		{
			if(BW1.at<unsigned char>(i-j,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for (int R=j-S ; R <= j-1; R++)
				{
					if(SW.at<float>(i-R,R)>S)
						SW.at<float>(i-R,R)=S;
				}
				j=j+1;
				S=0;
			}
		}
	}
	for (int j=0 ; j < N;j++)
	{
		int i=0;
		int S=0;
		while((j+i)<N-1 && i<M-1)
		{
			if(BW1.at<unsigned char>(M - 1 -i,i+j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for (int R=i-S; R <= i-1; R++)
				{
					if(SW.at<float>(M-1-R,R+j)>S)
						SW.at<float>(M-1-R,R+j)=S;
				}
				i=i+1;
				S=0;
			}
		}
	}
	
	//return 0;
	//imshow("SW",SW);cvWaitKey();
	double mean = computeMean(SW);
	double th = 2.5 * mean;
	for(int i = 0 ; i < SW.rows ; i++)
	{
		for(int j = 0 ; j < SW.cols ; j++)
		{
			if(SW.at<float>(i,j) > th)
			{
				SW.at<float>(i,j) = 0;
			}
		}
	}

	mean = computeMean(SW);
	th = 0.3 * mean;
	for(int i = 0 ; i < SW.rows ; i++)
	{
		for(int j = 0 ; j < SW.cols ; j++)
		{
			if(SW.at<float>(i,j) < th)
			{
				SW.at<float>(i,j) = 0;
			}
		}
	}
	return computeMean(SW);
}
double StrokeWidthMeasurement4DTime(Mat I, Mat &SW)
{
	int M = I.size[0];
	int N = I.size[1];
	Mat BW1 = I.clone();
	//Mat tmp;	
	//BW1 = ~I;
	for(int i = 0 ; i < BW1.rows ; i++)
	{
		for(int j = 0 ; j < BW1.cols ; j++)
		{
			BW1.at<unsigned char>(i,j) = !BW1.at<unsigned char>(i,j);
		}
	}
	/*double sum = 0;
	for(int i = 0 ; i < BW1.rows ; i++)
	{
		for(int j = 0 ; j < BW1.cols ; j++)
		{
			sum +=BW1.at<unsigned char>(i,j);
		}
	}*/
	//threshold(I, tmp, 0, 255, CV_THRESH_BINARY);
	//imshow("bw1",tmp);cvWaitKey(0);
	BW1(Range(0,1), Range(0,N))=1;
	BW1(Range(0,M), Range(0,1))=1;
	BW1(Range(M - 1,M), Range(0,N))=1;
	BW1(Range(0,M), Range(N - 1,N))=1;
	SW = BW1.zeros(M,N, CV_32F);
	//Mat ROI = BW1.zeros(M,N, CV_8UC1);
	
	//threshold(BW1, tmp, 0, 255, CV_THRESH_BINARY);
	//imshow("bw1",tmp);cvWaitKey(0);
	//at<unsigned char>
	for (int i = 0 ; i < M;i++)
	{
		int j=0;
		int S=0;
		while(j<N-1)
		{
			if(BW1.at<unsigned char>(i,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for (int R=j-S; R <= j-1; R++)
					SW.at<float>(i,R)=S;            
				/*if(S>0)
				{
					ROI.at<unsigned char>(i,j-S)=1;
					ROI.at<unsigned char>(i,j-1)=1;
				} */           
				j=j+1;
				S=0;
			}
		}
	}
	for (int j=0 ; j < N;j++)
	{
		int i=19;
		int S=0;
		while(i<M-1)
		{
			if(BW1.at<unsigned char>(i,j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for(int R = i-S; R <= i-1; R++)
				{
					if(SW.at<float>(R,j)>S)
						SW.at<float>(R,j)=S;       
					/*if(S>0)
					{
						ROI.at<unsigned char>(i-S,j)=1;
						ROI.at<unsigned char>(i-1,j)=1;
					}*/
				}
				i=i+1;
				S=0;
			}
		}
	}
	for (int i = 0 ; i < M;i++)
	{
		int j=0;

		int S=0;
		while((i+j)<M-1 && j<N-1)
		{
			if(BW1.at<unsigned char>(i+j,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for( int R=j-S ; R <=j-1;R++)
				{
					if(SW.at<float>(i+R,R)>1.4*S)
						SW.at<float>(i+R,R)=1.4*S;
				}
				j=j+1;
				S=0;
			}
		}
	}
	for (int j=0 ; j < N;j++)
	{
		int i=0;
		int S=0;
		while((j+i)<N-1 && i<M-1)
		{
			if(BW1.at<unsigned char>(i,i+j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for (int R=i-S; R <= i-1; R++)
				{
					if(SW.at<float>(R,R+j)>1.4*S)
						SW.at<float>(R,R+j)=1.4*S;                            
				}
				i=i+1;
				S=0;
			}
		}
	}
	for (int i = 0 ; i < M;i++)
	{
		int j=0;
		int S=0;
		while((i-j)>=0 && j<N-1)
		{
			if(BW1.at<unsigned char>(i-j,j)==0)
			{
				S=S+1;
				j=j+1;
			}
			else
			{
				for (int R=j-S ; R <= j-1; R++)
				{
					if(SW.at<float>(i-R,R)>S)
						SW.at<float>(i-R,R)=S;
				}
				j=j+1;
				S=0;
			}
		}
	}
	for (int j=0 ; j < N;j++)
	{
		int i=0;
		int S=0;
		while((j+i)<N-1 && i<M-1)
		{
			if(BW1.at<unsigned char>(M - 1 -i,i+j)==0)
			{
				S=S+1;
				i=i+1;
			}
			else
			{
				for (int R=i-S; R <= i-1; R++)
				{
					if(SW.at<float>(M-1-R,R+j)>S)
						SW.at<float>(M-1-R,R+j)=S;
				}
				i=i+1;
				S=0;
			}
		}
	}
	//return 0;
	//imshow("SW",SW);cvWaitKey();
	double mean = computeMean(SW);
	double th = 2.5 * mean;
	for(int i = 0 ; i < SW.rows ; i++)
	{
		for(int j = 0 ; j < SW.cols ; j++)
		{
			if(SW.at<float>(i,j) > th)
			{
				SW.at<float>(i,j) = 0;
			}
		}
	}

	mean = computeMean(SW);
	th = 0.3 * mean;
	for(int i = 0 ; i < SW.rows ; i++)
	{
		for(int j = 0 ; j < SW.cols ; j++)
		{
			if(SW.at<float>(i,j) < th)
			{
				SW.at<float>(i,j) = 0;
			}
		}
	}
	return computeMean(SW);
}
void ThinSubiteration1(Mat & pSrc, Mat & pDst);


void ThinSubiteration2(Mat & pSrc, Mat & pDst);

void CalculateGravity(vector<Point> contour, Point &center);
void PreThining(Mat & src)
{
	//Fill Holes
	
	//flat branches
	int heightOffset = 0;
	int prevHeightOffset = 0;
	int nextHeightOffset = 0;
	int start = 0, end = 0;
	//////////////////////////////////////////////////////////////
	//												///			//
	//					///////					///////			//	
	//					///////		===\	    ///////			//
	//					///////		===/		///////			//
	//					///////					///////			//
	//////////////////////////////////////////////////////////////
	for(int i = 1 ; i < src.rows - 1; i++)
	{
		heightOffset = i * src.cols;
		prevHeightOffset = (i - 1) * src.cols;
		nextHeightOffset = (i + 1) * src.cols;
		for(int j = 1 ; j < src.cols - 1; j++)
		{
			if(src.data[heightOffset + j] && !src.data[prevHeightOffset + j] && !src.data[heightOffset + j - 1] && !src.data[prevHeightOffset + j - 1] )
			{
				int count1 = 1;
				int count2 = 0;
				start = j;
				while( j < src.cols - 1 && src.data[heightOffset + ++j] && !src.data[prevHeightOffset + j])
					count1++;
				end = j - 1;
				if(count1 >= 2 && !src.data[heightOffset + j] && !src.data[prevHeightOffset + j])
				{
					start --;
					while( start < src.cols - 1 && src.data[nextHeightOffset + start++])
						count2++;
					if(count1 == count2)
						src.data[prevHeightOffset + (start + end) / 2] = 255;
				}
			}
		}
	}
	//imshow("C:\\2.bmp",src * 255);cvWaitKey();
}

void thining(Mat & inputarray, Mat & outputarray) {
	
        bool bDone = false;
        int rows = inputarray.rows;
        int cols = inputarray.cols;
		PreThining(inputarray);
		inputarray.copyTo(outputarray);
        outputarray.convertTo(outputarray,CV_32FC1);
		//imshow(outputarray);
/*
        inputarray.copyTo(outputarray);

        outputarray.convertTo(outputarray,CV_32FC1);
		outputarray */
        /// pad source
        Mat p_enlarged_src = Mat(rows + 2, cols + 2, CV_32FC1);
        for(int i = 0; i < (rows+2); i++) {
            p_enlarged_src.at<float>(i, 0) = 0.0f;
            p_enlarged_src.at<float>( i, cols+1) = 0.0f;
        }
        for(int j = 0; j < (cols+2); j++) {
                p_enlarged_src.at<float>(0, j) = 0.0f;
                p_enlarged_src.at<float>(rows+1, j) = 0.0f;
        }
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if (inputarray.at<uchar>(i, j) > 0) {
                                p_enlarged_src.at<float>( i+1, j+1) = 1.0f;
                        }
                        else
                                p_enlarged_src.at<float>( i+1, j+1) = 0.0f;
                }
        }

        /// start to thin
        Mat p_thinMat1 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
        Mat p_thinMat2 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
        Mat p_cmp = Mat::zeros(rows + 2, cols + 2, CV_8UC1);

        while (bDone != true) {
                /// sub-iteration 1
                ThinSubiteration1(p_enlarged_src, p_thinMat1);
                /// sub-iteration 2
                ThinSubiteration2(p_thinMat1, p_thinMat2);
                /// compare
                compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
                /// check
                int num_non_zero = countNonZero(p_cmp);
                if(num_non_zero == (rows + 2) * (cols + 2)) {
                        bDone = true;
                }
                /// copy
                p_thinMat2.copyTo(p_enlarged_src);
        }
        // copy result
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        outputarray.at<float>( i, j) = p_enlarged_src.at<float>( i+1, j+1);
                }
        }
		outputarray.convertTo(outputarray, CV_8UC1);
		threshold(outputarray,outputarray,0,1,CV_THRESH_BINARY);
		//outputarray = ~outputarray;
}	
void thining(Mat & inputarray, Mat & outputarray, int iteration) {
	
        bool bDone = false;
        int rows = inputarray.rows;
        int cols = inputarray.cols;
		PreThining(inputarray);
		inputarray.copyTo(outputarray);
        outputarray.convertTo(outputarray,CV_32FC1);
		//imshow(outputarray);
/*
        inputarray.copyTo(outputarray);

        outputarray.convertTo(outputarray,CV_32FC1);
		outputarray */
        /// pad source
        Mat p_enlarged_src = Mat(rows + 2, cols + 2, CV_32FC1);
        for(int i = 0; i < (rows+2); i++) {
            p_enlarged_src.at<float>(i, 0) = 0.0f;
            p_enlarged_src.at<float>( i, cols+1) = 0.0f;
        }
        for(int j = 0; j < (cols+2); j++) {
                p_enlarged_src.at<float>(0, j) = 0.0f;
                p_enlarged_src.at<float>(rows+1, j) = 0.0f;
        }
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if (inputarray.at<uchar>(i, j) > 0) {
                                p_enlarged_src.at<float>( i+1, j+1) = 1.0f;
                        }
                        else
                                p_enlarged_src.at<float>( i+1, j+1) = 0.0f;
                }
        }

        /// start to thin
        Mat p_thinMat1 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
        Mat p_thinMat2 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
        Mat p_cmp = Mat::zeros(rows + 2, cols + 2, CV_8UC1);
		int iter = 1;
        while (bDone != true) {
                /// sub-iteration 1
                ThinSubiteration1(p_enlarged_src, p_thinMat1);
                /// sub-iteration 2
                ThinSubiteration2(p_thinMat1, p_thinMat2);
                /// compare
                compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
                /// check
                int num_non_zero = countNonZero(p_cmp);
				if(num_non_zero == (rows + 2) * (cols + 2) || iter == iteration) {
                        bDone = true;
                }
				iter++;
                /// copy
                p_thinMat2.copyTo(p_enlarged_src);
        }
        // copy result
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        outputarray.at<float>( i, j) = p_enlarged_src.at<float>( i+1, j+1);
                }
        }
		outputarray.convertTo(outputarray, CV_8UC1);
		threshold(outputarray,outputarray,0,1,CV_THRESH_BINARY);
		//outputarray = ~outputarray;
}	
void BWLabel(Mat src, Mat &dst, int minArea)
{
	IplImage* tmp = &(IplImage(src));
	CvMemStorage * sm = cvCreateMemStorage();
	CvContour *contours;
	CvContour *nextContour;
	double area = 0;	
	cvFindContours(tmp, sm, (CvSeq**)&contours, 88, CV_RETR_LIST);
	cvZero(tmp);
	nextContour = contours;
	while(nextContour) 
	{ 
		if(nextContour->total > 6)
		{
			area = cvContourArea(nextContour);
		}
		else 
			area = nextContour->rect.width * nextContour->rect.height;
		if(area > minArea)
		{
			cvDrawContours(tmp, (CvSeq*)nextContour ,cvScalar(1) , cvScalar(1), 0, -1);	
		}
		nextContour = (CvContour*)nextContour->h_next;
	}
	dst = tmp;
	//cvReleaseImage(&tmp);
	cvReleaseMemStorage(&sm);
}