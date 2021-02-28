// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateProject.h"
#include "DlgProxy.h"
#include "TemplateProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateProjectDlgAutoProxy

IMPLEMENT_DYNCREATE(CTemplateProjectDlgAutoProxy, CCmdTarget)

CTemplateProjectDlgAutoProxy::CTemplateProjectDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CTemplateProjectDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CTemplateProjectDlg)))
		{
			m_pDialog = reinterpret_cast<CTemplateProjectDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CTemplateProjectDlgAutoProxy::~CTemplateProjectDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CTemplateProjectDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CTemplateProjectDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTemplateProjectDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ITemplateProject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {70FEA115-0C3A-483A-91F4-6B0BFC71CEE8}
static const IID IID_ITemplateProject =
{ 0x70FEA115, 0xC3A, 0x483A, { 0x91, 0xF4, 0x6B, 0xB, 0xFC, 0x71, 0xCE, 0xE8 } };

BEGIN_INTERFACE_MAP(CTemplateProjectDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CTemplateProjectDlgAutoProxy, IID_ITemplateProject, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {655B0A83-AED4-4ECA-AEED-9E9027D5C1EF}
IMPLEMENT_OLECREATE2(CTemplateProjectDlgAutoProxy, "TemplateProject.Application", 0x655b0a83, 0xaed4, 0x4eca, 0xae, 0xed, 0x9e, 0x90, 0x27, 0xd5, 0xc1, 0xef)


// CTemplateProjectDlgAutoProxy message handlers
