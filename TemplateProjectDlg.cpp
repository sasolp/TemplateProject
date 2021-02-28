// TemplateProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateProject.h"
#include "TemplateProjectDlg.h"
#include "DlgProxy.h"
#include "shlwapi.h"
#include "mybitmap.h"
#include "d:\VCProjects\ImgProc\ImgProc\CharacterSegment.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MessageInt(number)\
	char str[32];\
	sprintf(str,"%d",number);\
	MessageBoxA(0,str,0,0);
typedef struct _BHRect
	{
		int x;
		int y;
		int width;
		int height;
	}BHRect;


typedef int (__stdcall * EXTRACTFINGER)(int nWidth, int nHeight, int nBytesPerPixel, int nWidthStep, int nSize, uchar * pInBitsData, uchar * pOutBitsData, BHRect* rectBoundary);

EXTRACTFINGER ExtractFinger;

typedef INT (WINAPI* LOCATEFINGER2)(PBYTE, INT, INT, PRECT);

LOCATEFINGER2 LocateFinger2;
//global functions
CString Number2Str(int xInput)
{
	char str[10];
	_ltoa_s(xInput,str,10,10);
	CString sNumber(str);
	return sNumber;
}
void wstr2str(char *strPath,wchar_t *wstrPath)
{
	int len=lstrlen(wstrPath);
	
	for(int i=0;i<len;i++)
	{
		strPath[i]=(char)wstrPath[i];
	}
	strPath[len]=0;
    
}
char* wstr2str(wchar_t *wstrPath)
{
    
	int len=lstrlen(wstrPath);
	char *strPath=new char[len+1];
	for(int i=0;i<len;i++)
	{
		strPath[i]=(char)wstrPath[i];
	}
	strPath[len]=0;
    return strPath;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTemplateProjectDlg dialog




IMPLEMENT_DYNAMIC(CTemplateProjectDlg, CDialog);

CTemplateProjectDlg::CTemplateProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateProjectDlg::IDD, pParent)
    , m_srcPath(_T(""))
    , m_ProgText(_T(""))
    , m_LogName(_T(""))
    , m_bSubFolders(FALSE)
    , m_step(FALSE)
    , m_bShowImage(true)
    , m_Threshold(0)
	, m_Delta1(0)
	, m_Delta2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
    bIsBatch = false;
	ExtractFinger = 0 ;
	Library = 0;
  
	
	
	
	
}

CTemplateProjectDlg::~CTemplateProjectDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
    if( Library )
        FreeLibrary(Library);
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CTemplateProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC1, m_srcPath);
	DDX_Text(pDX, IDC_STATIC2, m_ProgText);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_EDIT1, m_LogName);
	DDX_Check(pDX, IDC_CHECK1, m_bSubFolders);
	
	DDX_Check(pDX, IDC_CHECK2, m_step);

}

BEGIN_MESSAGE_MAP(CTemplateProjectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_DROPFILES()
    ON_BN_CLICKED(IDC_BUTTON1, &CTemplateProjectDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CTemplateProjectDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CTemplateProjectDlg::OnBnClickedButton3)

END_MESSAGE_MAP()


// CTemplateProjectDlg message handlers

BOOL CTemplateProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTemplateProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTemplateProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTemplateProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CTemplateProjectDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CTemplateProjectDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CTemplateProjectDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CTemplateProjectDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}
#include "baseapi.h"
#include "basedir.h"
#include "strngs.h"
using namespace cv;
void ThinSubiteration1(Mat & pSrc, Mat & pDst) {
        int rows = pSrc.rows;
        int cols = pSrc.cols;
        pSrc.copyTo(pDst);
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if(pSrc.at<float>(i, j) == 1.0f) {
                                /// get 8 neighbors
                                /// calculate C(p)
                                int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                                int neighbor1 = (int) pSrc.at<float>( i-1, j);
                                int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                                int neighbor3 = (int) pSrc.at<float>( i, j+1);
                                int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                                int neighbor5 = (int) pSrc.at<float>( i+1, j);
                                int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                                int neighbor7 = (int) pSrc.at<float>( i, j-1);
                                int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                                                 int(~neighbor3 & ( neighbor4 | neighbor5)) +
                                                 int(~neighbor5 & ( neighbor6 | neighbor7)) +
                                                 int(~neighbor7 & ( neighbor0 | neighbor1));
                                if(C == 1) {
                                        /// calculate N
                                        int N1 = int(neighbor0 | neighbor1) +
                                                         int(neighbor2 | neighbor3) +
                                                         int(neighbor4 | neighbor5) +
                                                         int(neighbor6 | neighbor7);
                                        int N2 = int(neighbor1 | neighbor2) +
                                                         int(neighbor3 | neighbor4) +
                                                         int(neighbor5 | neighbor6) +
                                                         int(neighbor7 | neighbor0);
                                        int N = min(N1,N2);
                                        if ((N == 2) || (N == 3)) {
                                                /// calculate criteria 3
                                                int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
                                                if(c3 == 0) {
                                                        pDst.at<float>( i, j) = 0.0f;
                                                }
                                        }
                                }
                        }
                }
        }
}


void ThinSubiteration2(Mat & pSrc, Mat & pDst) {
        int rows = pSrc.rows;
        int cols = pSrc.cols;
        pSrc.copyTo( pDst);
        for(int i = 0; i < rows; i++) {
                for(int j = 0; j < cols; j++) {
                        if (pSrc.at<float>( i, j) == 1.0f) {
                                /// get 8 neighbors
                                /// calculate C(p)
                            int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
                            int neighbor1 = (int) pSrc.at<float>( i-1, j);
                            int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
                            int neighbor3 = (int) pSrc.at<float>( i, j+1);
                            int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
                            int neighbor5 = (int) pSrc.at<float>( i+1, j);
                            int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
                            int neighbor7 = (int) pSrc.at<float>( i, j-1);
                                int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
                                        int(~neighbor3 & ( neighbor4 | neighbor5)) +
                                        int(~neighbor5 & ( neighbor6 | neighbor7)) +
                                        int(~neighbor7 & ( neighbor0 | neighbor1));
                                if(C == 1) {
                                        /// calculate N
                                        int N1 = int(neighbor0 | neighbor1) +
                                                int(neighbor2 | neighbor3) +
                                                int(neighbor4 | neighbor5) +
                                                int(neighbor6 | neighbor7);
                                        int N2 = int(neighbor1 | neighbor2) +
                                                int(neighbor3 | neighbor4) +
                                                int(neighbor5 | neighbor6) +
                                                int(neighbor7 | neighbor0);
                                        int N = min(N1,N2);
                                        if((N == 2) || (N == 3)) {
                                                int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
                                                if(E == 0) {
                                                        pDst.at<float>(i, j) = 0.0f;
                                                }
                                        }
                                }
                        }
                }
        }
}
//void CreateHoles(Mat & src)
//{
//	//Fill Holes
//
//	//flat branches
//	int heightOffset = 0;
//	int prevHeightOffset = 0;
//	for(int i = 1 ; i < src.rows - 1; i++)
//	{
//		heightOffset = i * src.cols;
//		prevHeightOffset = i - 1 * src.cols;
//		for(int j = 1 ; j < src.cols - 1; j++)
//		{
//			if(!src.data[heightOffset + j] && src.data[prevHeightOffset + j] && src.data[heightOffset + j - 1] && src.data[prevHeightOffset + j - 1] )
//			{
//				int count = 0;
//				while( j < src.cols - 1 && !src.data[heightOffset + ++j] && src.data[prevHeightOffset + j])
//					count++;
//				if(count >= 2 && src.data[heightOffset + j] && src.data[prevHeightOffset + j])
//				{
//					src.data[prevHeightOffset + j - 1] = 0;
//				}
//			}
//		}
//	}
//	imwrite("C:\\2.bmp",src);
//}
void CalculateGravity(vector<Point> contour, Point &center)
{
	float sumX = 0, sumY = 0;
	for(int i = 0 ; i < contour.size(); i++)
	{
		sumX += contour[i].x;
		sumY += contour[i].y;
	}
	center.x = sumX / contour.size();
	center.y = sumY / contour.size();
}
void PreThining(Mat & src, float strokeSize)
{
	//Fill Holes
	//Mat dst;
	//distanceTransform(src, dst, CV_DIST_L2, 3);
	//dst = dst > strokeSize;
	//dst.convertTo(dst, CV_8UC1);
	//vector<vector<Point>> contours;	
	//imshow("dist",dst);cvWaitKey();
	//imwrite("c:\\1.bmp",dst);
	//findContours(dst, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//dst = 0;
	//for(int i = 0; i < contours.size(); i++)
	//{
	//	Rect rect = boundingRect(contours[i]);
	//	if(rect.width >= strokeSize && rect.height >= strokeSize && contours[i].size() >= 2*strokeSize)
	//	{
	//		Point center;
	//		CalculateGravity(contours[i], center);
	//		int maxAxis = max(rect.width, rect.height);
	//		Rect extendedRect(center.x - maxAxis * 1.5, center.y - maxAxis * 1.5, maxAxis * 3 + 1, maxAxis * 3 + 1);
	//		imshow("1",src(extendedRect));cvWaitKey();
	//		drawContours(dst, contours, i, Scalar(255), CV_FILLED);
	//		imshow("2",dst(extendedRect));cvWaitKey();
	//		int xOccurrence = 0, yOccurrence = 0;
	//		int j = 0;//Rightward
	//		cv::LineIterator line(src, Point(extendedRect.x, extendedRect.y), Point(extendedRect.x + extendedRect.width, extendedRect.y));
	//		for(j = 0; j < line.count && !*line.ptr; line++,j++)
	//		{
	//			Point pos = line.pos();
	//			if(pos.x == center.x)xOccurrence++;
	//		}
	//		if(xOccurrence)
	//		{
	//			cv::LineIterator line(src, Point(extendedRect.x + extendedRect.width, extendedRect.y), Point(extendedRect.x + extendedRect.width , extendedRect.y + extendedRect.height));
	//			for(j = 0; j < line.count && !*line.ptr; line++,j++)
	//			{
	//				Point pos = line.pos();
	//				if(pos.y == center.y)yOccurrence++;
	//			}
	//			if(yOccurrence)
	//			{
	//				cv::LineIterator line(src, Point(extendedRect.x + extendedRect.width , extendedRect.y + extendedRect.height), Point(extendedRect.x , extendedRect.y + extendedRect.height));
	//				for(j = 0; j < line.count && !*line.ptr; line++,j++)
	//				{
	//					Point pos = line.pos();
	//					if(pos.x == center.x)xOccurrence++;
	//				}

	//			}
	//		}
	//		//Downward
	//		cv::LineIterator line2(src, Point(extendedRect.x, extendedRect.y), Point(extendedRect.x , extendedRect.y + extendedRect.height));
	//		for(j = 0; j < line2.count && !*line2.ptr; line2++,j++)
	//		{
	//			Point pos = line2.pos();
	//			if(pos.y == center.y)yOccurrence++;
	//		}
	//		if(yOccurrence)
	//		{
	//			cv::LineIterator line(src, Point(extendedRect.x, extendedRect.y + extendedRect.height), Point(extendedRect.x + extendedRect.width , extendedRect.y + extendedRect.height));
	//			for(j = 0; j < line.count && !*line.ptr; line++,j++)
	//			{
	//				Point pos = line.pos();
	//				if(pos.x == center.x)xOccurrence++;
	//			}
	//			if(xOccurrence)
	//			{
	//				cv::LineIterator line(src, Point(extendedRect.x + extendedRect.width , extendedRect.y + extendedRect.height), Point(extendedRect.x  + extendedRect.width, extendedRect.y));
	//				for(j = 0; j < line.count && !*line.ptr; line++,j++)
	//				{
	//					Point pos = line.pos();
	//					if(pos.y == center.y)yOccurrence++;
	//				}

	//			}
	//		}
	//		//if((xOccurrence >= 1 && yOccurrence >= 1) && (yOccurrence >= 2 || xOccurrence >= 2))
	//		{
	//			drawContours(src, contours, i, Scalar(0), CV_FILLED);
	//		}
	//	}
	//}
	//imshow("src",src);cvWaitKey();
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
	imwrite("C:\\1.bmp",src);
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
					while( start < src.cols - 1 && start <= end)
						if(src.data[nextHeightOffset + start++])
							count2++;
					if(count1 == count2)
						src.data[prevHeightOffset + (start + end) / 2] = 255;
				}
			}
		}
	}
	imwrite("C:\\2.bmp",src);
}
void thining(Mat & inputarray, Mat & outputarray, float strokeSize) {
	//imshow("",inputarray);cvWaitKey();
	//imwrite("C:\\1.bmp",inputarray);
		PreThining(inputarray, strokeSize);
		imshow("src1",inputarray);cvWaitKey();
	//imwrite("C:\\2.bmp",inputarray);
        bool bDone = false;
        int rows = inputarray.rows;
        int cols = inputarray.cols;
		inputarray.copyTo(outputarray);
        outputarray.convertTo(outputarray,CV_32FC1);
		//imshow("",outputarray);cvWaitKey();
        /*inputarray.convertTo(inputarray,CV_32FC1);

        inputarray.copyTo(outputarray);

        outputarray.convertTo(outputarray,CV_32FC1);*/
		imshow("src2",outputarray);cvWaitKey();

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
                        if (inputarray.at<uchar>(i, j) >= 20.0f) {
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
		outputarray.convertTo(outputarray,CV_8UC1);
		threshold(outputarray,outputarray,0,255,CV_THRESH_BINARY);
		imshow("thin",outputarray);cvWaitKey();
		imwrite("c:\\thin.bmp",outputarray);
		//dilate(outputarray, outputarray, Mat(), Point(1,1), 1);
}
void UpProfiling(Mat & src, Mat &dst)
{
	dst = Mat(src.rows, src.cols, src.type());
	dst = 0;
	for(int j = 0 ; j < dst.cols; j++)
	{
		for(int i = 0 ; i < dst.rows; i++)
		{
			if(src.data[i * src.cols + j])
			{
				dst.data[i * src.cols + j] = src.data[i * src.cols + j];
				while(i < dst.rows && src.data[i * src.cols + j])
					i++;
			}
		}
	}
}
void DownProfiling(Mat & src, Mat &dst)
{
	dst = Mat(src.rows, src.cols, src.type());
	dst = 0;
	for(int j = 0 ; j < dst.cols; j++)
	{
		for(int i = dst.rows - 1 ; i >= 0; i--)
		{
			if(src.data[i * src.cols + j])
			{
				dst.data[i * src.cols + j] = src.data[i * src.cols + j];
				while(i >= 0 && src.data[i * src.cols + j])
					i--;
			}
		}
	}
}
void GetStrokeLengths(Mat & src, int baseLine, int strokeSize, map<int, int> &mapOfLens)
{
	int len;
	int start = baseLine - strokeSize * 1.5;
	int end = baseLine + strokeSize * 1.5 ;
	for(int i = start ; i < end; i++)
	{
		len = 0;
		for(int j = 0 ; j < src.cols; j++)
		{
			if(src.data[i * src.cols + j] && !src.data[(i + 1) * src.cols + j] && !src.data[(i - 1) * src.cols + j])
			{
				len++;
			}
			else
			{
				if(len)
				{
					mapOfLens[len]++;
				}
				len = 0;
			}
		}
	}
}
void GetStrokeLengths(Mat & src, map<int, int> &mapOfLens)
{
	int len;
	int start = 0;
	int end = src.rows ;
	for(int i = start ; i < end; i++)
	{
		len = 0;
		for(int j = 0 ; j < src.cols; j++)
		{
			if(src.data[i * src.cols + j] && !src.data[(i + 1) * src.cols + j] && !src.data[(i - 1) * src.cols + j])
			{
				len++;
			}
			else
			{
				if(len)
				{
					mapOfLens[len]++;
				}
				len = 0;
			}
		}
	}
}
typedef vector<int> Line; 
int GetLinesIntersect(Line l1, Line l2)
{
	if(l2[0] > l1[1] || l2[1] < l1[0])
	{
		return 0;
	}
	if(l1[0] >= l2[0] && l2[1] <= l1[1])
	{
		return l2[1] - l1[0];
	}
	if(l1[1] >= l2[0] && l2[1] >= l1[1])
	{
		return l2[1] - l1[0];
	}
	return min(l1[1] - l1[0], l2[1] - l2[0]);
}
float VectorMedian(vector<int> vec)
{
	if(!vec.size())
		return -1;
	float retVal = 0;
	sort(vec.begin(), vec.end());
	int max = vec.size() - 1;
	if(vec.size() % 2)
	{
		retVal = vec[max / 2];
	}
	else
	{
		retVal = 0.5 * (vec[max / 2] + vec[max / 2 + 1]);
	}
	return retVal;
}
void AlefDetect(int baseline,Mat &Thin,int &Hi_alef,Mat &alef)
{
	
	int Rows = Thin.rows, Cols = Thin.cols;
    alef = Mat(Rows, Cols, Thin.type(), Scalar(0));
	int tlor_base= cvRound(baseline/10.0);
    int start = 0, end = 0;
	vector<vector<Point>> contours;
	vector<vector<Point>> remainedContours;
	Mat tmp = Thin.clone();
	findContours(tmp, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//Thin.copyTo(tmp);
	Line l00;l00.push_back(baseline + tlor_base); l00.push_back(Rows - 1);
	Line l01;l01.push_back(baseline - tlor_base); l01.push_back(baseline + tlor_base);
	int tlor2_baseline = 1000000;
	for(int i = 0; i < contours.size(); i++)
	{
		Rect rect = cv::boundingRect(contours[i]);
		Line l1; l1.push_back(rect.tl().y); l1.push_back(rect.br().y);
		if(GetLinesIntersect(l00, l1) > 0 || !GetLinesIntersect(l01, l1))
		{
			drawContours(Thin, contours, i, Scalar(0), 1, 8, noArray(), 0);
		}
		else
		{
			remainedContours.push_back(contours[i]);
			if(tlor2_baseline > rect.tl().y)
			{
				tlor2_baseline = rect.tl().y;
			}
		}
	}

    if (tlor2_baseline > cvRound(baseline/3.0))
        tlor2_baseline= tlor_base;    
    int Step = cvRound(baseline/15.0);
	float TH = baseline - (1.8 * tlor_base) - tlor2_baseline ;
    if (baseline > (Rows*.55))
	{
		for(int i = 0; i < remainedContours.size(); i++)
		{
			Rect rect = cv::boundingRect(remainedContours[i]);
			Mat alef2 = Mat(Rows, Cols, Thin.type(), Scalar(0));
			//alef2 = 0;
			drawContours(alef2, remainedContours, i, Scalar(1), 1, 8, noArray(), 0);
			start = rect.x + Step + 1; end = rect.x + rect.width - Step;
			for(int j = start; j < end; j++)
			{
				Mat range = alef2(Rect(j - Step, 0, Step * 2, Rows));
				Scalar s = sum(range);
				if(s[0] > TH)
				{
					drawContours(alef, remainedContours, i, Scalar(1), 1, 8, noArray(), 0);
					drawContours(Thin, remainedContours, i, Scalar(0), 1, 8, noArray(), 0);
					break;
				}
			}

		}
		
	}
	contours.clear();
	tmp.~Mat();
	tmp = alef.clone();
	findContours(tmp, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	vector<int> Start_alef;
	vector<int> Start_alef2;
	float MajorAxisLength = 0.0f;
	float MinorAxisLength = 0.0f;
	for(int i = 0; i < contours.size(); i++)
	{
		Rect rect = cv::boundingRect(contours[i]);
		RotatedRect box = fitEllipse(contours[i]);
		MajorAxisLength = max(box.size.height, box.size.width);
		MinorAxisLength = min(box.size.height, box.size.width);
		if(MajorAxisLength / MinorAxisLength > 15)
		{
			Start_alef2.push_back(rect.height);
		}
		else
		{
			Start_alef.push_back(rect.height);
		}
	}
	if (Start_alef2.size())
	{
		Hi_alef = baseline - VectorMedian(Start_alef2) + 0.5;
	}
	else
	{
		Hi_alef = baseline - VectorMedian(Start_alef) + 0.5;
	}
}
void FitWindow_CH(Mat src, Mat &dst);
void ErodeTines(Mat src, Mat &dst)
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
typedef struct PolyLine
{
	Point start;
	Point end;
};
void FindMaxConcavity(Mat src, float strokeSize, int &crCount, float &dist, float &area, float &angle, float ecc)
{
	Mat thin;
	//imwrite("c:\\t.bmp",src * 255);
	//imshow("fit_thin",src * 255);cvWaitKey();
	//src = ~src;
	int arrDirections[16] = {};//from -90 til +90
	if(strokeSize <= 7)
	{
		resize(src, src, Size(src.cols * 3, src.rows * 3));
		strokeSize *= 3; 
	}
	//cv::morphologyEx(src, src, CV_MOP_OPEN, Mat(),Point(-1,-1),strokeSize / 3);
	copyMakeBorder(src, src, 1, 1, 1, 1, BORDER_CONSTANT, Scalar::all(0));
	
	imshow("src",src * 255);cvWaitKey();
	thining(src, thin);
	//imshow("fit_thin",thin * 255);cvWaitKey();
	Mat fit_thin;
	FitWindow_CH(thin, fit_thin);
	copyMakeBorder(fit_thin, fit_thin, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0));
	vector<Point> points;
	Point moment(0,0) ;
	vector<Point> crossPoints;
	vector<Point> endPoints;
	float sum = 0;
	for(int j = fit_thin.cols - 1 ; j >= 0; j--)
	{
		for(int i = fit_thin.rows - 1; i >= 0; i--)
		{
			if(fit_thin.data[ i * fit_thin.cols + j])
			{
				points.push_back(Point(j, i));
				sum = fit_thin.data[ (i - 1) * fit_thin.cols + j + 0] + fit_thin.data[ (i - 1) * fit_thin.cols + j + 1] +
					  fit_thin.data[ (i - 1) * fit_thin.cols + j - 1] + fit_thin.data[ (i + 1) * fit_thin.cols + j + 0] +
					  fit_thin.data[ (i + 1) * fit_thin.cols + j + 1] + fit_thin.data[ (i + 1) * fit_thin.cols + j - 1] +
					  fit_thin.data[ i * fit_thin.cols + j + 1] + fit_thin.data[ i * fit_thin.cols + j - 1];
				if(sum >= 3)
				{
					crossPoints.push_back(Point(j, i));
				}
				if(sum == 1)
				{
					endPoints.push_back(Point(j, i));
				}
			}
		}
	}
	for(int i = 0 ; i < crossPoints.size(); i++)
	{
		Point pt1 = crossPoints[i];
		for(int j = i - 1 ; j >=  0 ; j--)
		{
			Point pt2 = crossPoints[j];
			if(abs(pt1.x - pt2.x) <= 2 && abs(pt1.y - pt2.y) <= 2)
			{
				crossPoints.erase(crossPoints.begin() + i);i--;
					break;
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
	fit_thin.copyTo(thin_copy);
	thin_copy *= 255;/**/
	int maxIndex = -1;
	int maxSize = 0;
	vector<Point>polyPoints;
	vector<Point>finalKeyPoints;
	vector<vector<Point>>polyRings;
	vector<PolyLine> arrLines;
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
		int newStrokeSize = strokeSize / 2;//min(float(strokeSize), 5.0f);
		polyPoints = thin_points[maxIndex];
		//refine end points
		Rect boundOfMaxContour = boundingRect(polyPoints);
		for(int i = 0 ; i < endPoints.size(); i++)
		{
			Point pt = endPoints[i];
			if(!boundOfMaxContour.contains(pt))
			{
				endPoints.erase(endPoints.begin() + i);i--;
			}
		}
		int ringCounter = 0;
		const int MIN_RING_COUNT = 3;
		int counter = 3;
		int lastIndexVisited = 0;
		bool bNormilized = false;
		bool bIsCrossPoint = false;
		for(int i = 0 ; i < polyPoints.size(); i++)
		{
			Point pt1 = polyPoints[i];
			bIsCrossPoint = false;
			for(int m = 0 ; m < crossPoints.size(); m++)
			{
				Point pt2 = crossPoints[m];
				if(abs(pt1.x - pt2.x) <= 1 && abs(pt1.y - pt2.y) <= 1)
				{
					bIsCrossPoint = true;
				}
			}
			if(bIsCrossPoint && bNormilized)
			{
				vector<Point>polyRing;
				for(int m = lastIndexVisited ; m <= i; m++)
				{
					polyRing.push_back(polyPoints[m]);
				}
				lastIndexVisited = i;
				polyRings.push_back(polyRing);
			}
			else
			{
				for(int j = i - 1 ; j >= lastIndexVisited ; j--)
				{
					Point pt2 = polyPoints[j];
					if(abs(pt1.x - pt2.x) <= 0 && abs(pt1.y - pt2.y) <= 0)
					{
						ringCounter++;
						while(counter)
						{
							Point pt1 = polyPoints[i + counter];
							Point pt2 = polyPoints[j - counter];
							if(abs(pt1.x - pt2.x) <= 1 && abs(pt1.y - pt2.y) <= 1)
								ringCounter++;
							counter--;
						}
						if(ringCounter >= MIN_RING_COUNT)
						{	
							if(!bNormilized)
							{
								//Normalization
								for(int m = 0 ; m < i; m++)
								{
									polyPoints.push_back(polyPoints[0]);
									polyPoints.erase(polyPoints.begin());
								}
								bNormilized = true;
								i = 0;
							}
							else
							{
								vector<Point>polyRing;
								for(int m = lastIndexVisited ; m <= i; m++)
								{
									polyRing.push_back(polyPoints[m]);
								}
								lastIndexVisited = i;
								polyRings.push_back(polyRing);
							}
						}
						break;
					}
				}
			}
		}
		vector<Point>polyRing;
		for(int m = lastIndexVisited ; m < polyPoints.size(); m++)
		{
			polyRing.push_back(polyPoints[m]);
		}
		polyRings.push_back(polyRing);
		thin_copy = 0;
		for(int i = 0; i < polyRings.size();i++)
		{
			if(polyRings[i].size() < 5)
			{
				continue;
			}
			Mat curve(polyRings[i]);
			vector<Point> approxPoly;
			cv::approxPolyDP(curve, approxPoly, newStrokeSize, 0);
			for(int j = 0 ; j < approxPoly.size() - 1; j++)
			{
				for(int m = 0 ; m < crossPoints.size(); m++)
				{
					if(
						((crossPoints[m].x >= approxPoly[j].x  && crossPoints[m].x <= approxPoly[j + 1].x) || (crossPoints[m].x >= approxPoly[j + 1].x  && crossPoints[m].x <= approxPoly[j].x ) ) &&
						((crossPoints[m].y >= approxPoly[j].y  && crossPoints[m].y <= approxPoly[j + 1].y) || (crossPoints[m].y >= approxPoly[j + 1].y  && crossPoints[m].y <= approxPoly[j].y ) )
						)
					{
						approxPoly.insert(approxPoly.begin() + j + 1, crossPoints[m]);
						j++;
						break;
					}
				}
			}
			polylines(thin_copy, approxPoly, 0, Scalar(255));
			const int MIN_END_POINT_DIST = 2;
			for(int j = 0 ; j < approxPoly.size() - 1; j++)//extract lines
			{
				PolyLine line;
				line.start = approxPoly[j];
				line.end = approxPoly[j + 1];
				if(line.start.x > line.end.x)
				{
					swap(line.start, line.end);
				}
				else if(line.start.x == line.end.x && line.start.y > line.end.x)
				{
					swap(line.start, line.end);
				}
				bool bFound = false;
				for(int m = 0; m < arrLines.size(); m++)
				{
					PolyLine line2 = arrLines[m];
					float r1 = sqrt((float)abs(line.start.x - line2.start.x) * abs(line.start.x - line2.start.x) + abs(line.start.y - line2.start.y) * abs(line.start.y - line2.start.y));
					float r2 = sqrt((float)abs(line.end.x - line2.end.x) * abs(line.end.x - line2.end.x) + abs(line.end.y - line2.end.y) * abs(line.end.y - line2.end.y));
					if(r1 <= MIN_END_POINT_DIST && r2 <= MIN_END_POINT_DIST)
					{
						bFound = true;
						break;
					}
					r1 = sqrt((float)abs(line.start.x - line2.end.x) * abs(line.start.x - line2.end.x) + abs(line.start.y - line2.end.y) * abs(line.start.y - line2.end.y));
					r2 = sqrt((float)abs(line.end.x - line2.start.x) * abs(line.end.x - line2.start.x) + abs(line.end.y - line2.start.y) * abs(line.end.y - line2.start.y));
					if(r1 <= MIN_END_POINT_DIST && r2 <= MIN_END_POINT_DIST)
					{
						bFound = true;
						break;
					}
				}
				if(!bFound)
				{
					arrLines.push_back(line);
				}
			}
			for(int j = 0 ; j < approxPoly.size(); j++)
			{
				finalKeyPoints.push_back(approxPoly[j]);
				cv::rectangle(thin_copy, Rect(approxPoly[j].x - 2, approxPoly[j].y - 2,4,4), Scalar(128 ), -1);
				//polyDst.data[ approxPoly[i].y * polyDst.cols + approxPoly[i].x] = 128;
			}
			imshow("polyDst",thin_copy);cvWaitKey();
		}
		for(int i = 0 ; i < finalKeyPoints.size(); i++)
		{
			Point pt1 = finalKeyPoints[i];
			for(int j = i - 1 ; j >= 0; j--)
			{
				Point pt2 = finalKeyPoints[j];
				if(abs(pt1.x - pt2.x) <= 2 && abs(pt1.y - pt2.y) <= 2)
				{
					finalKeyPoints.erase(finalKeyPoints.begin() + i);i--;
					break;
				}
			}
		}
		//filling directions
		for(int i = 0 ; i < arrLines.size(); i++)
		{
			double theta = (180 * atan2(-1 * ((double)arrLines[i].end.y - arrLines[i].start.y), (double)arrLines[i].end.x - arrLines[i].start.x)) / CV_PI;
			theta += 90;
			arrDirections[cvRound(theta / 12)]++;
			//arrDirections[0] -90 til -78
			//arrDirections[1] -78 til -66
			//arrDirections[2] -66 til -54
			//arrDirections[3] -54 til -42
			//arrDirections[4] -42 til -30
			//arrDirections[5] -30 til -18
			//arrDirections[6] -18 til -06 
			//arrDirections[7] -06 til +06 
			//arrDirections[0] +06 til +18
			//arrDirections[1] +18 til +30
			//arrDirections[2] +30 til +42
			//arrDirections[3] +42 til +54
			//arrDirections[4] +54 til +66
			//arrDirections[5] +66 til +78
			//arrDirections[6] +78 til +90 
			//arrDirections[7] +90 til +102 

		}
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
	}
	hulls.push_back(hullPoints);
	Mat hull = Mat(fit_thin.rows, fit_thin.cols, fit_thin.type(), Scalar(0));
	drawContours(hull, hulls, 0, Scalar(1), -1);
	//concavityMat = Mat(hull - fit_thin).clone();
	dilate(fit_thin, fit_thin, Mat());
	double hullArea = cv::sum(hull)[0];
	double forgroundArea = cv::sum(fit_thin)[0] ;
	Mat newHull = hull - fit_thin;
	//imshow("newHull1",newHull);cvWaitKey();
	ErodeTines(newHull, newHull);
	
	return;
	imshow("newHull2",newHull);cvWaitKey();
	vector<vector<Point>> contours;
	findContours(newHull, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	
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
	if(contours.size())
	{
		RotatedRect box = fitEllipse(contours[maxInx]);
		RotatedRect box2 = minAreaRect(contours[maxInx]);
		newHull = 0;
		cv::ellipse(newHull, box, Scalar(128));
		
		imshow("newHullWithEllipse",newHull);cvWaitKey();
		angle = box.angle;
		for(int i = 0 ; i < contours[maxInx].size(); i++)
		{
			moment.x += contours[maxInx][i].x;
			moment.y += contours[maxInx][i].y;
		}
		moment.x = cvRound(float(moment.x) / contours[maxInx].size());
		moment.y = cvRound(float(moment.y) / contours[maxInx].size());
		dist = sqrt(float(moment.x * moment.x + moment.y * moment.y)) / strokeSize; 
		area = maxArea / strokeSize;
		float MajorAxisLength = max(box.size.height, box.size.width);
		float MinorAxisLength = min(box.size.height, box.size.width);
		ecc = 2*sqrt(pow(MajorAxisLength/2,2) - pow(MinorAxisLength/2, 2)) / MajorAxisLength;
	}
}
double GetStrokeSize(Mat src)
{
	Mat dst;	
	copyMakeBorder(src, src, 1, 1, 1, 1, BORDER_CONSTANT, Scalar(0));
	distanceTransform(src, dst, CV_DIST_L2, 3);
	double min_val = 0, max_val = 0;
	minMaxLoc(dst, &min_val, &max_val);
	Mat mean , sdv ;
	cv::meanStdDev(dst, mean, sdv);
	return (max_val - ((double*)sdv.data)[0] )* 1.5;
}
void CTemplateProjectDlg::ProcessImage()
{
    UpdateData();
	
	if(m_srcPath == CString(""))
	{
		MessageBoxA (0,"Please,Select a Path For Reference Image ",0,0);
		return;
	}
	if( !PathFileExists(m_srcPath))
	{
		MessageBoxA(0,"Please,Select a Correct Path For Reference  Image ",0,0);
		return;
	}
    // TODO: Add your control notification handler code here
    char strPath[MAX_PATH]="";
    char outPath[MAX_PATH]="";
    char outFileName[MAX_PATH]="";
    #ifdef _UNICODE
        wstr2str(strPath,m_srcPath.GetBuffer());
    #else
        strcpy_s(strPath,MAX_PATH,m_srcPath.GetBuffer());    
    #endif
		Mat src = imread(strPath, CV_LOAD_IMAGE_GRAYSCALE); 
		threshold(src, src, 128, 255, CV_THRESH_BINARY_INV);
		int crCount = 0;
		float dist = 0.0f, area = 0.0f, angle = 0.0f, ecc = 0.0f;
		FindMaxConcavity(src, GetStrokeSize(src), crCount, dist, area, angle, ecc);
		cvWaitKey(); cv::destroyAllWindows();
		return;

		//resize(src, src, Size(src.cols *2, src.rows * 2), 0.0, 0.0, 3);
		Mat m = src(Range(0, src.rows), Range(src.cols - 1, src.cols));
		Scalar s = sum(m);

		imshow("src", src);cvWaitKey();
		vector<Word> arrImgWords;
		LineSegment(src, arrImgWords);
		return;
		/*int x = ex.(0);
		int y = ex.row(0);
		ex = ex.t();
		x = ex.col(0);
		y = ex.row(1);*/
		Mat Thin;
		thining(src, Thin, 0);
		Mat thin = Thin(Rect(0, 7, Thin.cols, 38));
		Mat alef;
		int alefHeight = 0;
		int baseLine = 27;
		AlefDetect(baseLine, thin, alefHeight, alef);
imwrite("C:\\alef.bmp",alef * 255);imshow("",alef * 255);cvWaitKey();

		Mat upProfile;
		Mat downProfile;
		UpProfiling(src, upProfile);
		imwrite("C:\\upProfile.bmp",upProfile);imshow("",upProfile);cvWaitKey();
		DownProfiling(src, downProfile);
		imwrite("C:\\downProfile.bmp",downProfile);imshow("",downProfile);cvWaitKey();
		map<int, int> mapOfUpProfileLens;
		GetStrokeLengths(upProfile, mapOfUpProfileLens);
		map<int, int> mapOfDownProfileLens;
		GetStrokeLengths(downProfile, mapOfDownProfileLens);
		GetStrokeLengths(downProfile, mapOfUpProfileLens);


		//thining(src, dst, 3.5);
		cv::destroyAllWindows();
		return;
		MSER mser;
		vector<KeyPoint> contours;	
		mser.detect(src, contours);
		StarDetector star;
		star.detect(src, contours);
			for(int i = 0 ;i < contours.size();i++)
				drawContours(src, contours, i, Scalar(i * 10), CV_FILLED);
		
		return;
		{
			CvMat* src= cvLoadImageM(strPath,CV_LOAD_IMAGE_GRAYSCALE );
			//cvThreshold(src, src, 128, 255, CV_THRESH_OTSU);
			tesseract::TessBaseAPI api;
			STRING tessdata_dir("C:\\Program Files (x86)\\Tesseract-OCR\\tessdata");
			STRING text_out("");
			int rc = api.Init(tessdata_dir.string(), "ara", tesseract::OcrEngineMode::OEM_DEFAULT);
			for(int i = 0 ; i < 1 ; i++)
			{
				//CvMat* dst = cvCreateMat(src->rows * (1 + (i * 0.1)), src->cols * (1 + (i * 0.1)), CV_8U);
				//CvMat* dst = cvCreateMat(src->rows * (0.2 + (i * 0.2)), src->cols * (0.2 + (i * 0.2)), CV_8U);
				//CvMat* dst = cvCreateMat(src->rows * 0.6, src->cols * 0.6, CV_8U);
				CvMat* dst = cvCreateMat(src->rows * 1, src->cols * 1, CV_8U);
				//cvResize(src, dst, 3);
				cvCopyImage(src, dst);
				//cvThreshold(dst, dst, 128, 255, CV_THRESH_OTSU);
				char strOut[256];
				sprintf(strOut, "c:\\AOCR_Pic_%d.bmp",i + 1);
				cvSaveImage(strOut, dst);
				api.ProcessPages(strOut, NULL, 0, &text_out);


				char *strTemp = strrchr(strPath,'\\');
				strcpy(outFileName,strTemp+1);
				strTemp[1]=0;
				strcpy(outPath,strPath);
				strcat(outPath,"ProcessedImages\\");
				if(!PathIsDirectoryA(outPath) )
				{        
					CreateDirectoryA(outPath,0);
				}
				strcat(outPath,outFileName);
				sprintf(strOut, "c:\\AOCR_Result_%d.txt",i + 1);
				FILE* fout = fopen(strOut, "wb");
				if (fout == NULL) {
					exit(1);
				}
				fwrite(text_out.string(), 1, text_out.length(), fout);
				fclose(fout);
				cvReleaseMat(&dst);
			}
		}
}
void CTemplateProjectDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default
    wchar_t strPath[MAX_PATH];
    DragQueryFile(hDropInfo,0,strPath,MAX_PATH);
    CPoint point;
    CRect rect;
    DragQueryPoint(hDropInfo,&point);
    GetWindowRect(&rect);
    ScreenToClient(&rect);
    UpdateData();
    if(point.x >= rect.left && point.y<=rect.right && point.y>=rect.top&&point.y <=rect.bottom )
    {
        m_srcPath=CString(strPath);
        UpdateData(0);
        if(PathIsDirectory( m_srcPath))
        {
         
            BSTR bstrPath=SysAllocStringLen(L"",MAX_PATH*4);
            lstrcpy(bstrPath,m_srcPath.GetBuffer());
            
            if(m_step)
            {
                ProcessBatchStepByStep(bstrPath);
            }
            else
            {
                ProcessBatch(bstrPath);
            }
            SysFreeString(bstrPath);
        }
        else
            ProcessImage();
    }

    CDialog::OnDropFiles(hDropInfo);
}

void CTemplateProjectDlg::OnBnClickedButton1()
{
    bIsBatch = false;
    UpdateData();    
	ProcessImage();
}
void FindFilesHierarchically(const wchar_t *strPath,vector<wstring>&filePaths,vector<wstring> strExtensions,int level,int levelTH)
{
    
    WIN32_FIND_DATA ffd={0};
    if(level >= levelTH )return;
    level++;
    wstring strDirPath(strPath);
    HANDLE hFile= FindFirstFile((strDirPath+wstring(L"\\*.*")).c_str(),&ffd);
    
    do
    {
        if(lstrcmpi(ffd.cFileName,L".")==0 || lstrcmpi(ffd.cFileName,L"..")==0)continue;
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY   )
        {            
            FindFilesHierarchically((strDirPath+wstring(L"\\")+wstring(ffd.cFileName)).c_str(),filePaths, strExtensions, level, levelTH);
            continue;
        }
        wstring strPathOfFile= strDirPath+wstring(L"\\")+wstring(ffd.cFileName);
        for(int i = 0 ;i < (int)(strExtensions.size()) ;i++)
        {
            if(strPathOfFile.find(strExtensions[i]) != -1)
            {
                filePaths.push_back(strPathOfFile);        
                break;
            }
        }
    }while(FindNextFile(hFile,&ffd)!=0);

}
void FindFiles(const wchar_t *strPath,vector<wstring>&filePaths,vector<wstring>  strExtensions,int levelTH)
{
    int level = 0 ;

    if(levelTH < 0 )
        levelTH = 1000000;
    FindFilesHierarchically(strPath,filePaths, strExtensions, level, levelTH);
}
void CTemplateProjectDlg::OnBnClickedButton2()
{
    UpdateData();
	int WrongCounts=0;
	int x=RegisterHotKey(m_hWnd,100, MOD_WIN ,'S');
	x=RegisterHotKey(m_hWnd,101,MOD_WIN ,'P');
	x=RegisterHotKey(m_hWnd,102, MOD_WIN ,'I');

	char path1[MAX_PATH ];

	BROWSEINFO bf={0};
	bf.hwndOwner=m_hWnd;
	bf.ulFlags= BIF_RETURNONLYFSDIRS  |BIF_BROWSEFORCOMPUTER |BIF_STATUSTEXT ; 
	bf.lpszTitle =L"Please Select Reference Dataset";
	LPITEMIDLIST pidl     = NULL;
	wchar_t strPath[MAX_PATH ];
	wchar_t strDirPath[MAX_PATH ];
	bool bSelected=false;
	if((pidl=SHBrowseForFolder(&bf))!= NULL)
	{
		SHGetPathFromIDList(pidl,strPath );
		wstr2str(path1,strPath);
		bSelected=true;
		CoTaskMemFree(pidl);
	}
	
    vector<wstring>filePaths;
    vector<wstring>arrExtensions;
    arrExtensions.push_back(L"bmp");
	arrExtensions.push_back(L"BMP");
	lstrcpy(strDirPath,strPath);
    if(m_bSubFolders)
        FindFiles(strDirPath,filePaths,arrExtensions,-1);
    else
        FindFiles(strDirPath,filePaths,arrExtensions,1);
	lstrcat(strDirPath,L"\\");

	int numberOfFiles=filePaths.size();
	double nProcessedCounter=0;
	if(bSelected)
		do
		{
			
			MSG msg;
			
			PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
			switch(msg.message)
			{
			case WM_HOTKEY: 
				switch(msg.wParam)
				{
				case 100:{
					wchar_t *strMSG=L"Are you Ensure to Stop?";
					if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
					{

						UnregisterHotKey(m_hWnd,100);
						UnregisterHotKey(m_hWnd,101);
						UnregisterHotKey(m_hWnd,102);
						return;
					}
					break;}
				case 101:{

					wchar_t *strMSG=L"Are you Ensure to Pause?";
					if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
					{
						MSG msg;
						bool bIsOk=false;
						while(WaitMessage())
						{
							
							PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
							switch(msg.message)
							{
							case WM_HOTKEY:
								switch(msg.wParam)
								{
								case 101:bIsOk=true;break;
								}
							}
							if(bIsOk)break;
						}
					}
					break;}
				case 102:{
					MessageBox(
						Number2Str((nProcessedCounter*100)/(numberOfFiles))+
						CString("% From Total Image are Compared")+CString("\nCount Of Wrong Detection is :")+Number2Str(WrongCounts)
						);
					break;}
				}
			}



			m_srcPath=CString(( filePaths[nProcessedCounter].c_str()));
			m_dstPath=CString(strDirPath);
			if(m_srcPath.Find(L"Segmented")>=0)continue;
			if( !PathFileExists(m_srcPath))
			{
				MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
				return;
			}
			m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
			m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
			UpdateData(0);
			RedrawWindow();
			CString log(L"");

			try
			{
				ProcessImage();
			}
			catch(cv::Exception  &ex)
			{
				const char* err_msg = ex.what();
				MessageBox(log+CString(err_msg));
			}
            
			nProcessedCounter++;
		}while(nProcessedCounter < numberOfFiles);
		int xx=GetLastError();

		UnregisterHotKey(m_hWnd,100);
		UnregisterHotKey(m_hWnd,101);
		UnregisterHotKey(m_hWnd,102);
}
void CTemplateProjectDlg::ProcessBatchStepByStep(wchar_t strPath[])
{
    UpdateData();
    bIsBatch = true; 
    int WrongCounts=0;
	int x=RegisterHotKey(m_hWnd,103, MOD_CONTROL, 'N');
	x=RegisterHotKey(m_hWnd,104,MOD_CONTROL ,'P');
    x=RegisterHotKey(m_hWnd,105,MOD_CONTROL ,'S');

    wchar_t strDirPath[MAX_PATH ];
    bool bSelected=true;
    vector<wstring>filePaths;
    vector<wstring>arrExtensions;
    arrExtensions.push_back(L"bmp");
	arrExtensions.push_back(L"BMP");
    lstrcpy(strDirPath,strPath);
    if(m_bSubFolders)
        FindFiles(strDirPath,filePaths,arrExtensions,-1);
    else
        FindFiles(strDirPath,filePaths,arrExtensions,1);
    lstrcat(strDirPath,L"\\");

    int numberOfFiles=filePaths.size();
    double nProcessedCounter=0;
    int nFileCounter = -1 ;
    if(bSelected)
        while(true)
        {
            
            MSG msg;
            
			PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
            switch(msg.message)
            {
            case WM_HOTKEY: 
                switch(msg.wParam)
                {
                case 105:{
                    wchar_t *strMSG=L"Are you Ensure to Stop?";
                    if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
                    {

                        UnregisterHotKey(m_hWnd,103);
                        UnregisterHotKey(m_hWnd,104);
                        UnregisterHotKey(m_hWnd,105);
                        return;
                    }
                    break;}
                case 103:{
                    if(nFileCounter >= numberOfFiles )
                    {
                        MessageBoxA(0,"End Of Files ",0,0);continue;
                    }

                    nProcessedCounter = nFileCounter +1;
                    m_srcPath=CString(filePaths[++nFileCounter].c_str());
                    m_dstPath=CString(strDirPath);
                    if(m_srcPath.Find(L"Segmented")>=0)continue;
                    if( !PathFileExists(m_srcPath))
                    {
                        MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
                        return;
                    }
                    m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
                    m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
                    UpdateData(0);
                    RedrawWindow();
                    CString log(L"");

                    try
                    {
                        ProcessImage();
                    }
                    catch(cv::Exception  &ex)
                    {
                        const char* err_msg = ex.what();
                        MessageBox(log+CString(err_msg));
                    }

                    break;}
                case 104:{
                    if(nFileCounter <= 0 )
                    {
                        MessageBoxA(0,"Start Of Files ",0,0);continue;
                    }
                    nProcessedCounter = nFileCounter -1;
                    m_srcPath=CString(filePaths[--nFileCounter].c_str());
                    m_dstPath=CString(strDirPath);
                    if(m_srcPath.Find(L"Segmented")>=0)continue;
                    if( !PathFileExists(m_srcPath))
                    {
                        MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
                        return;
                    }
                    m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
                    m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
                    UpdateData(0);
                    RedrawWindow();
                    CString log(L"");

                    try
                    {
                        ProcessImage();
                    }
                    catch(cv::Exception  &ex)
                    {
                        const char* err_msg = ex.what();
                        MessageBox(log+CString(err_msg));
                    }
                    break;}
                }
            }

        }
        int xx=GetLastError();

        UnregisterHotKey(m_hWnd,103);
        UnregisterHotKey(m_hWnd,104);
        UnregisterHotKey(m_hWnd,105);




}

void CTemplateProjectDlg::ProcessBatch(wchar_t strPath[])
{
    int WrongCounts=0;
    bIsBatch = true;
	int x=RegisterHotKey(m_hWnd,100, MOD_WIN ,'S');
	x=RegisterHotKey(m_hWnd,101,MOD_WIN ,'P');
	x=RegisterHotKey(m_hWnd,102, MOD_WIN ,'I');
	
	wchar_t strDirPath[MAX_PATH ];
	bool bSelected=true;
	 vector<wstring>filePaths;
    vector<wstring>arrExtensions;
    arrExtensions.push_back(L"bmp");
	arrExtensions.push_back(L"BMP");
	lstrcpy(strDirPath,strPath);
    if(m_bSubFolders)
        FindFiles(strDirPath,filePaths,arrExtensions,-1);
    else
        FindFiles(strDirPath,filePaths,arrExtensions,1);
	lstrcat(strDirPath,L"\\");

	int numberOfFiles=filePaths.size();
	double nProcessedCounter=0;
	if(bSelected)
		do
		{
			
			MSG msg;
			
			PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
			switch(msg.message)
			{
			case WM_HOTKEY: 
				switch(msg.wParam)
				{
				case 100:{
					wchar_t *strMSG=L"Are you Ensure to Stop?";
					if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
					{
						m_Progress.SetPos(100);
						UnregisterHotKey(m_hWnd,100);
						UnregisterHotKey(m_hWnd,101);
						UnregisterHotKey(m_hWnd,102);
						return;
					}
					break;}
				case 101:{

					wchar_t *strMSG=L"Are you Ensure to Pause?";
					if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
					{
						MSG msg;
						bool bIsOk=false;
						while(WaitMessage())
						{
							
							PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
							switch(msg.message)
							{
							case WM_HOTKEY:
								switch(msg.wParam)
								{
								case 101:bIsOk=true;break;
								}
							}
							if(bIsOk)break;
						}
					}
					break;}
				case 102:{
					MessageBox(
						Number2Str((nProcessedCounter*100)/(numberOfFiles))+
						CString("% From Total Image are Compared")+CString("\nCount Of Wrong Detection is :")+Number2Str(WrongCounts)
						);
					break;}
				}
			}



			m_srcPath=CString(filePaths[nProcessedCounter].c_str());
			m_dstPath=CString(strDirPath);
			if(m_srcPath.Find(L"Segmented")>=0)continue;
			if( !PathFileExists(m_srcPath))
			{
				MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
				return;
			}
			nProcessedCounter++;
			m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
			m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
			UpdateData(0);
			RedrawWindow();
			CString log(L"");

			try
			{
				ProcessImage();
			}
			catch(cv::Exception  &ex)
			{
				const char* err_msg = ex.what();
				//MessageBox(log+CString(err_msg));
			}
            
		}while(nProcessedCounter < numberOfFiles);
		int xx=GetLastError();
		
		UnregisterHotKey(m_hWnd,100);
		UnregisterHotKey(m_hWnd,101);
		UnregisterHotKey(m_hWnd,102);
}

void CTemplateProjectDlg::OnBnClickedButton3()
{
   UpdateData();
	int WrongCounts=0;
	int x=RegisterHotKey(m_hWnd,103, MOD_CONTROL, 'N');
	x=RegisterHotKey(m_hWnd,104,MOD_CONTROL ,'P');
    x=RegisterHotKey(m_hWnd,105,MOD_CONTROL ,'S');

	char path1[MAX_PATH ];

	BROWSEINFO bf={0};
	bf.hwndOwner=m_hWnd;
	bf.ulFlags= BIF_RETURNONLYFSDIRS  |BIF_BROWSEFORCOMPUTER |BIF_STATUSTEXT;
	bf.lpszTitle =L"Please Select Reference Dataset";
	LPITEMIDLIST pidl     = NULL;
	wchar_t strPath[MAX_PATH ];
	wchar_t strDirPath[MAX_PATH ];
	bool bSelected=false;
	if((pidl=SHBrowseForFolder(&bf))!= NULL)
	{
		SHGetPathFromIDList(pidl,strPath );
		wstr2str(path1,strPath);
		bSelected=true;
		CoTaskMemFree(pidl);
	}
	vector<wstring>filePaths;
    vector<wstring>arrExtensions;
    arrExtensions.push_back(L"bmp");
	arrExtensions.push_back(L"BMP");
	lstrcpy(strDirPath,strPath);
    if(m_bSubFolders)
        FindFiles(strDirPath,filePaths,arrExtensions,-1);
    else
        FindFiles(strDirPath,filePaths,arrExtensions,1);
	lstrcat(strDirPath,L"\\");
	int numberOfFiles=filePaths.size();
	double nProcessedCounter=0;

      int nFileCounter = -1 ;
      if(bSelected)
        while(true)
        {
            nProcessedCounter++;
            MSG msg;
           
			PeekMessage(&msg, m_hWnd , WM_HOTKEY, WM_HOTKEY,0);
            switch(msg.message)
            {
            case WM_HOTKEY: 
                switch(msg.wParam)
                {
                case 105:{
                    wchar_t *strMSG=L"Are you Ensure to Stop?";
                    if(MessageBox((LPCTSTR)strMSG,0,MB_OKCANCEL)!=IDCANCEL)
                    {

                        UnregisterHotKey(m_hWnd,103);
                        UnregisterHotKey(m_hWnd,104);
                        UnregisterHotKey(m_hWnd,105);
                        return;
                    }
                    break;}
                case 103:{
                    if(nFileCounter >= numberOfFiles )
                    {
                         MessageBoxA(0,"End Of Files ",0,0);continue;
                    }
                    
                    nProcessedCounter = nFileCounter +1;
                    m_srcPath=CString(filePaths[++nFileCounter].c_str());
                    m_dstPath=CString(strDirPath);
                    if(m_srcPath.Find(L"Segmented")>=0)continue;
                    if( !PathFileExists(m_srcPath))
                    {
                        MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
                        return;
                    }
                    m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
                    m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
                    UpdateData(0);
                    RedrawWindow();
                    CString log(L"");

                    try
                    {
                        ProcessImage();
                    }
                    catch(cv::Exception  &ex)
                    {
                        const char* err_msg = ex.what();
                        MessageBox(log+CString(err_msg));
                    }
                    
                    break;}
                case 104:{
                    if(nFileCounter <= 0 )
                    {
                         MessageBoxA(0,"Start Of Files ",0,0);continue;
                    }
                    nProcessedCounter = nFileCounter -1;
                    m_srcPath=CString(filePaths[--nFileCounter].c_str());
                    m_dstPath=CString(strDirPath);
                    if(m_srcPath.Find(L"Segmented")>=0)continue;
                    if( !PathFileExists(m_srcPath))
                    {
                        MessageBoxA(0,"Please,Select a Correct Path For Reference  Images ",0,0);
                        return;
                    }
                    m_ProgText= Number2Str((nProcessedCounter*100)/(numberOfFiles))+CString("% is Processed");
                    m_Progress.SetPos((nProcessedCounter*100)/(numberOfFiles));
                    UpdateData(0);
                    RedrawWindow();
                    CString log(L"");

                    try
                    {
                        ProcessImage();
                    }
                    catch(cv::Exception  &ex)
                    {
                        const char* err_msg = ex.what();
                        MessageBox(log+CString(err_msg));
                    }
                    break;}
                }
            }

        }
		int xx=GetLastError();
		
		UnregisterHotKey(m_hWnd,103);
		UnregisterHotKey(m_hWnd,104);
		UnregisterHotKey(m_hWnd,105);
}

