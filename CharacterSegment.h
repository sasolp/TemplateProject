
using namespace cv;
typedef vector<Point> Points;
#define POS_START 1
#define POS_MIDDLE 2
#define POS_END 3
#define POS_ALONE 4
typedef struct _Character
{
	Mat imgChar;
	int position;
	string ch;
}Character;
typedef struct _Word
{
	vector<int> slices;
	int index;
	Rect boundRect;
	vector<string> text;
	std::wstring ocrText;
	vector<Character> arrChars;
}Word;
typedef struct _SpaceCoordinate
{
	int index;
	int start;
	int end;
	int len;
}SpaceCoordinate;
void LineSegment(Mat &imgInputLine, vector<Word> &arrImgWords);
extern double StrokeWidthMeasurement4DTime(Mat I, Mat &SW);
extern double StrokeWidthMeasurement4DTime(Mat I);
void thining(Mat & inputarray, Mat & outputarray);
void thining(Mat & inputarray, Mat & outputarray, int iterations) ;
void BWLabel(Mat src, Mat &dst, int minArea);
void BWLabel(Mat src, Mat &dst, vector<vector<Point>> &labels );
void FindContours(Mat src, vector<vector<Point>> &_contours, int mode);
inline bool sortContoursX(const Points& i, const Points& j);
Rect FindPrevStrokeRect( Mat &label, int xStart, int &base);
Rect FindNextStrokeRect(Mat &label, int xStart, int &base);

void FitWindow_CH(Mat src, Mat &dst);

bool sortAngles_CH(const double angle1, const double angle2);
int ExtractFontHight_CH(Mat&src, int &spaceSize);
void FindNextSpace2_CH(Mat& src,int SpaceSize, int &startX, int &endX);
void DetectSpaces_CH(Mat& inputImg, vector<SpaceCoordinate> &arrCoordinates);
double computeMean(Mat SW);