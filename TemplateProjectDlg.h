// TemplateProjectDlg.h : header file
//

#pragma once
#include "afxcmn.h"

class CTemplateProjectDlgAutoProxy;


// CTemplateProjectDlg dialog
class CTemplateProjectDlg : public CDialog
{
	DECLARE_DYNAMIC(CTemplateProjectDlg);
	friend class CTemplateProjectDlgAutoProxy;
    
// Construction
public:
	CTemplateProjectDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CTemplateProjectDlg();

// Dialog Data
	enum { IDD = IDD_TEMPLATEPROJECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
public :
    void ProcessImage();
    void ProcessBatch(wchar_t strPath[MAX_PATH ]);
    void ProcessBatchStepByStep(wchar_t strPath[MAX_PATH ]);
protected:
	CTemplateProjectDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
    HINSTANCE Library;
	BOOL CanExit();
    bool bIsBatch ; 
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
    CString m_srcPath;
    CString m_dstPath;
    afx_msg void OnDropFiles(HDROP hDropInfo);
    CString m_ProgText;
    CProgressCtrl m_Progress;
    CString m_LogName;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    BOOL m_bSubFolders;
    BOOL m_step;
	
    BOOL m_bShowImage;
    long m_Threshold;
	int m_Delta1;
	int m_Delta2;
};
