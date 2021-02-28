// DlgProxy.h: header file
//

#pragma once

class CTemplateProjectDlg;


// CTemplateProjectDlgAutoProxy command target

class CTemplateProjectDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CTemplateProjectDlgAutoProxy)

	CTemplateProjectDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CTemplateProjectDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CTemplateProjectDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CTemplateProjectDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

