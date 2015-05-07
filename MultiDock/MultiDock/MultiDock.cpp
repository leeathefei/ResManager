
// MultiDock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MultiDock.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MultiDockDoc.h"
#include "MultiDockView.h"
#include "..\Common\MTDocTemplate.h"
#include "..\Common\FileHelper.h"
#include "..\Common\XmlConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiDockApp

BEGIN_MESSAGE_MAP(CMultiDockApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMultiDockApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CMultiDockApp construction

CMultiDockApp::CMultiDockApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MultiDock.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMultiDockApp object

CMultiDockApp theApp;


// CMultiDockApp initialization

BOOL CMultiDockApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_TestDockMultiDoTYPE,
		RUNTIME_CLASS(CMultiDockDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMultiDockView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	pMainFrame->LoadModuleMenuItems();

	return TRUE;
}

// int CMultiDockApp::ExitInstance()
// {
// 	//TODO: handle additional resources you may have added
// 	//AfxOleTerm(FALSE);
// 
// 	CMTDockVisualManager::SafeCleanupThemes();
// 
// 	return __super::ExitInstance();
// }


// main running routine until thread exits
//int CMultiDockApp::Run()
//{
//	if (m_pMainWnd == NULL && AfxOleGetUserCtrl())
//	{
//		TRACE(traceAppMsg, 0, "Warning: m_pMainWnd is NULL in CWinApp::Run - quitting application.\n");
//		AfxPostQuitMessage(0);
//	}
//
//	ASSERT_VALID(this);
//	_AFX_THREAD_STATE* pState = AfxGetThreadState();
//
//	// for tracking the idle time state
//	BOOL bIdle = TRUE;
//	LONG lIdleCount = 0;
//
//	// acquire and dispatch messages until a WM_QUIT message is received.
//	for (;;)
//	{
//		// phase1: check to see if we can do idle work
//		while (bIdle &&
//			!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
//		{
//			// call OnIdle while in bIdle state
//			if (!OnIdle(lIdleCount++))
//				bIdle = FALSE; // assume "no idle" state
//		}
//
//		// phase2: pump messages while available
//		do
//		{
//			// pump message, but quit on WM_QUIT
//			BOOL bRet = FALSE;
//			bRet =  PumpMessage();
//			if(!bRet)
//				return ExitInstance();
//
//			// reset "no idle" state after pumping "normal" message
//			//if (IsIdleMessage(&m_msgCur))
//			if (IsIdleMessage(&(pState->m_msgCur)))
//			{
//				bIdle = TRUE;
//				lIdleCount = 0;
//			}
//
//		} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));
//	}
//}
//
//BOOL CMultiDockApp::PumpMessage()
//{
//	//#ifdef _DEBUG
//	//   return CWinAppEx::PumpMessage();
//	//#else
//	return MTDOCK_AfxInternalPumpMessage();
//	//#endif
//}
//
//BOOL AFXAPI MTDOCK_AfxInternalPumpMessage()
//{
//	_AFX_THREAD_STATE *pState = AfxGetThreadState();
//
//	if (!::GetMessage(&(pState->m_msgCur), NULL, NULL, NULL))
//	{
//#ifdef _DEBUG
//		TRACE(traceAppMsg, 1, "CWinThread::PumpMessage - Received WM_QUIT.\n");
//		pState->m_nDisablePumpCount++; // application must die
//#endif
//		// Note: prevents calling message loop things in 'ExitInstance'
//		// will never be decremented
//		return FALSE;
//	}
//
//#ifdef _DEBUG
//	if (pState->m_nDisablePumpCount != 0)
//	{
//		TRACE(traceAppMsg, 0, "Error: CWinThread::PumpMessage called when not permitted.\n");
//		ASSERT(FALSE);
//	}
//#endif
//
//#ifdef _DEBUG
//	//_AfxTraceMsg(_T("PumpMessage"), &(pState->m_msgCur));
//#endif
//
//	// process this message
//	CWnd* pMainWnd = AfxGetMainWnd();
//	if(pMainWnd==NULL || !::IsWindow(pMainWnd->m_hWnd))
//	{
//		const MSG& rMsg = pState->m_msgCur;
//		return TRUE;
//	}
//
//	if (pState->m_msgCur.message != WM_KICKIDLE && !AfxPreTranslateMessage(&(pState->m_msgCur)))
//	{
//		::TranslateMessage(&(pState->m_msgCur));
//		::DispatchMessage(&(pState->m_msgCur));
//	}
//	return TRUE;
//}


// CMultiDockApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMultiDockApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMultiDockApp customization load/save methods

void CMultiDockApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CMultiDockApp::LoadCustomState()
{
}

void CMultiDockApp::SaveCustomState()
{
}

// CMultiDockApp message handlers



CView* CMultiDockApp::OpenView(const CString &strViewName, BOOL bSingle /*=FALSE*/)
{
	CDocument *pDoc = NULL;

	for ( POSITION pos = GetFirstDocTemplatePosition(); pos != NULL; )
	{
		CDocTemplate *pTemp = GetNextDocTemplate(pos);

		if ( strcmp(pTemp->GetRuntimeClass()->m_lpszClassName, "CMTDocTemplate") != 0 )
			continue;

		CMTDocTemplate *pTemplate = (CMTDocTemplate *)pTemp; 

		if ( pTemplate->m_TemplateName.CompareNoCase(strViewName) == 0 )
		{
			if ( bSingle )
			{
				POSITION pos = pTemplate->GetFirstDocPosition();
				if ( pos )
				{
					CDocument *pDoc = pTemplate->GetNextDoc(pos);
					pos = pDoc->GetFirstViewPosition();
					CView *pView = pDoc->GetNextView(pos);
					pView->GetParentFrame()->ActivateFrame();
					return pView;
				}
			}

			pDoc = pTemplate->OpenDocumentFile(NULL, NULL, TRUE);
			POSITION pos = pDoc->GetFirstViewPosition();
			CView* pView = pDoc->GetNextView(pos);
			CMDIChildWnd* pFrame = (CMDIChildWnd*)pView->GetParentFrame();

			AfxGetMainWnd()->RedrawWindow();
			return pView;
		}
	}

	return NULL;
}



int CMultiDockApp::GetNumOfView( const CString& strViewName )
{
	int nNumOfView = 0;
	CDocument *pDoc = NULL;

	for ( POSITION pos = GetFirstDocTemplatePosition(); pos != NULL; )
	{
		CDocTemplate *pTemp = GetNextDocTemplate(pos);

		if ( strcmp(pTemp->GetRuntimeClass()->m_lpszClassName, "CMTDocTemplate") != 0 )
			continue;

		CMTDocTemplate *pTemplate = (CMTDocTemplate *)pTemp; 

		if ( pTemplate->m_TemplateName.CompareNoCase(strViewName) == 0 )
		{
			POSITION pos = pTemplate->GetFirstDocPosition();
			if ( pos )
			{
				CDocument *pDoc = pTemplate->GetNextDoc(pos);
				pos = pDoc->GetFirstViewPosition();
				CView *pView = pDoc->GetNextView(pos);
				nNumOfView++;
			}

			return nNumOfView;
		}
	}

	return nNumOfView;
}
