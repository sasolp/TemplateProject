// TemplateProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateProject.h"
#include "TemplateProjectDlg.h"
#include "DlgProxy.h"
#include "shlwapi.h"
#include "mybitmap.h"
#include "QEstimator.h"

#include "CandidateRegionSelection.h"
#include "TamperedRegionIdentification.h"
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
	
		IplImage *firstSrc=cvLoadImage(strPath,CV_LOAD_IMAGE_COLOR );
	
    int nWidth = firstSrc->width;int nHeight = firstSrc->height;uchar *pBitsData = (uchar*)firstSrc->imageData;int nWidthStep=firstSrc->widthStep;int nBytesPerPixel = firstSrc->nChannels  ;
	CvMat* QTable = cvCreateMat(8,8, CV_32F);
	CvMat* srcImage = cvCreateMat(nHeight, nWidth, CV_8UC3);
	//IplImage* srcGray = cvCreateImage(cvGetSize(),8,1);
	cvScale(firstSrc, srcImage);
	//cvFlip(srcImage,srcImage,-1);
//cvFlip(firstSrc,firstSrc,1);cvFlip(firstSrc,firstSrc,0);cvFlip(firstSrc,firstSrc,-1);
	
	cvShowImage("initial image", firstSrc);cvWaitKey();
	/*CvMat* dstImage = cvCloneMat(srcImage);*/
	QEstimator QE;
	//QE.ExtractQTable(srcImage, QTable);
	TamperedRegionIdentification TRI;
	vector<SubImageInfo> CandidateRegion;
	/*QE.ExtractDCTCoefficients(srcImage, dstImage);
	cvScale(dstImage,dstImage,.2);
	QE.ExtractRDCTCoefficients(dstImage, srcImage);*/
	//cvShowImage("initial image", firstSrc);cvWaitKey();
	TRI.TemperDetection(srcImage);
	cvScale(srcImage, firstSrc);
	//cvShowImage("Detected Forgery", firstSrc);cvWaitKey();
    int pntsCount = 0 ;
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

    cvReleaseImage(&firstSrc);
   
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

