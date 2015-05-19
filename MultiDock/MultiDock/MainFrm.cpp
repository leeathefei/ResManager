
#include "stdafx.h"
#include "MultiDock.h"
#include "MainFrm.h"
#include "DlgPaneConfig.h"
#include "DlgConfig.h"
#include "ModulePane.h"
#include "WndConfigDlg.h"


#include "..\Common\XmlConfig.h"
#include "..\Common\ModuleDefs.h"
#include "..\Common\CSVFileReader.h"
#include "..\Common\FileHelper.h"
#include "..\Common\ResourceHandle.h"
#include "..\Common\WndManager.h"
#include "..\Common\XmlDataProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
CString CMainFrame::m_strModuleMenuItems[2] = {_T("&动态链接库模块"), _T("&动态库的工具栏")};

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_COMMAND(ID_SET_PANE1_CMD1, &CMainFrame::OnRibbonPane1Cmd1)
	ON_COMMAND(ID_SET_PANE1_CMD2, &CMainFrame::OnRibbonPane1Cmd2)
	ON_COMMAND(ID_SET_PANE1_CMD3, &CMainFrame::OnRibbonPane1Cmd3)
	ON_COMMAND(ID_SET_PANE1_CMD4, &CMainFrame::OnRibbonPane1Cmd4)

	ON_COMMAND(ID_SET_PANE2_CMD1, &CMainFrame::OnRibbonPane2Cmd1)
	ON_COMMAND(ID_SET_PANE2_CMD2, &CMainFrame::OnRibbonPane2Cmd2)
	ON_COMMAND(ID_SET_PANE2_CMD3, &CMainFrame::OnRibbonPane2Cmd3)
	ON_COMMAND(ID_SET_PANE2_CMD4, &CMainFrame::OnRibbonPane2Cmd4)
	
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETMENU, OnResetMenu)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_PRESS_CLOSE_BUTTON,OnClosePane)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGING_ACTIVE_TAB, OnChangingActiveTab)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_OFF_2007_BLUE, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND_RANGE(BASE_MODULES_MENU_ID, BASE_MODULES_MENU_ID+MAX_NUM_MODULES, OnModuleMenuItem)
	ON_COMMAND_RANGE(BASE_UTILITIES_MENU_ID, BASE_UTILITIES_MENU_ID+MAX_NUM_UTILITES, OnUtilityMenuItem)
	ON_UPDATE_COMMAND_UI_RANGE(BASE_UTILITIES_MENU_ID, BASE_UTILITIES_MENU_ID+MAX_NUM_UTILITES, OnUpdateToolbarMenu)

	ON_MESSAGE(WM_USER+100, OnGetMainMenu)
	ON_MESSAGE(WM_USER+101, OnGetMainMenuBar)
	ON_MESSAGE(WM_USER+1, OnRegisterModulePane)
	ON_MESSAGE(WM_USER+2, OnToolBarControl)
	ON_MESSAGE(WM_USER+3, OnPaneControl)
	ON_MESSAGE(WM_USER+4, OnUnregisterModulePane)
	ON_MESSAGE(WM_USER+12, OnActivateTabbedView)
	ON_MESSAGE(WM_USER+13, OnGetWndInActivePane)
	ON_MESSAGE(WM_INIT_MODULES, OnInitModulePanes)
	ON_MESSAGE(WM_RECORD_OPEN_TABBED_VIEWS, OnRecordOpenTabbedViews)
	ON_MESSAGE(WM_OPEN_VIEWER, OnOpenViewer)

	ON_COMMAND(ID_SHOW_WND_CONFIG_DLG, &CMainFrame::OnShowWndConfigDlg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,         
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
	m_pDlgConfigWnd = NULL;
	m_pViewLastCreated = NULL;
    m_pActiveView = NULL;
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
	if (NULL != m_pDlgConfigWnd)
	{
		m_pDlgConfigWnd->DestroyWindow();
		delete m_pDlgConfigWnd;
	}
}

void CMainFrame::OnClose()
{
	ResetWorkspaceNode();
	EnumTabbedView();
	EnumDockablePane();

	CMDIFrameWndEx::OnClose();
}


void CMainFrame::PostNcDestroy()
{
	SafeCloseModules();//MAY CAUSE EXIT FAILED !

	CMDIFrameWndEx::PostNcDestroy();
}

BOOL CMainFrame::SafeCloseModules()
{
	typedef BOOL LPCANCLOSEFUNC(CString &);
	CString strMessage;
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if( mc.hLib && mc.m_bInitialized )
		{
			LPCANCLOSEFUNC *pFunc = (LPCANCLOSEFUNC*)GetProcAddress(mc.hLib, "CanClose");
			if ( pFunc && !pFunc(strMessage) )
			{
				CString strError;
				strError.Format(_T("%s CanClose return FALSE"), mc.m_strTitle);
				AfxMessageBox(strError);
				return FALSE;
			}
		}
	}


	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if( mc.hLib && mc.m_bInitialized )
		{
			LPDLLFUNC *pFunc = (LPDLLFUNC*)GetProcAddress(mc.hLib, "Term");
			if (pFunc)
			{
				try 
				{
					pFunc(0);
				}
				catch(...) 
				{
					CString strError;
					strError.Format(_T("Terminate %s throw an exception"), mc.m_strTitle);
					AfxMessageBox(strError);
				}
			}
		}
	}

	//FreeModules();

	return TRUE;
}

void CMainFrame::FreeModules()
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if( mc.hLib )
		{
			CIntaffCommonResourceHandle resHandle( mc.m_strDll, __FILEW__, __LINE__);
			FreeLibrary(mc.hLib);
		}
	}
}

LRESULT CMainFrame::OnGetMainMenu(WPARAM wp, LPARAM )
{
	HMENU hMenu = m_wndMenuBar.GetHMenu();
	TRACE1("Main HMenu:%08x\n", (DWORD)hMenu);
	CMenu* pMenu = CMenu::FromHandle(hMenu);
	return  (LRESULT)pMenu;
}

LRESULT CMainFrame::OnGetMainMenuBar(WPARAM wp, LPARAM )
{
	return (LRESULT)&m_wndMenuBar;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//Lee:initialize config reader.
	CString strModulesXml = CFileHelper::GetModuleDir()+_T("\\Config\\dllResources.xml");
	CXmlConfig::Instance(strModulesXml);

	CString strMainframName = _T("主框架窗口");
	CWndManager::Instance()->AddCreatedWnd(this, _T("CMainFrame"), strMainframName, _T("Main"));

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.SetRecentlyUsedMenus(FALSE);
	m_wndMenuBar.SetShowAllCommands(TRUE);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, 
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	//create rabin，这行代码必须在在Menu创建完成才可以。
	//m_wndRibbonBar.Create(this);
	//m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	//// create docking windows
	////if (!CreateFileView())
	//m_vecUserDlgs.clear();
	//int nBasePaneCount = AppXml()->GetAttributeInt(_T("BaseDlgNodeCount"), 0);
	//if(nBasePaneCount > 0)
	//{
	//	for(int index=1; index<=nBasePaneCount; index++)
	//	{
	//		stBasePane oneItem;
	//		oneItem.pBaseDlg = new CBaseDlg;
	//		
	//		CString strNode;
	//		strNode.Format(_T("BaseDlgNode\\Index_%d"), index);
	//		oneItem.eType = (EDLGTYPE)(AppXml()->GetAttributeInt(strNode, 0));

	//		strNode.Empty();
	//		strNode.Format(_T("BaseDlgNode\\Title_%d"), index);
	//		std::wstring strTitle= AppXml()->GetAttributeText(strNode, _T(""));
	//		oneItem.strPaneTitle = strTitle.c_str();

	//		strNode.Empty();
	//		strNode.Format(_T("BaseDlgNode\\bShow_%d"), index);
	//		oneItem.bShow = AppXml()->GetAttributeInt(strNode) == 0 ? FALSE : TRUE;
	//		
	//		m_vecUserDlgs.push_back(oneItem);
	//	}
	//}
	//else if(nBasePaneCount == 0)//没配置文件，默认创建一个。
	//{
	//	stBasePane oneItem;
	//	oneItem.pBaseDlg = new CBaseDlg;
	//	oneItem.strPaneTitle = _T("测试面板一");
	//	oneItem.eType = enmDlgType_Test1;
	//	m_vecUserDlgs.push_back(oneItem);

	//	stBasePane oneItem2;
	//	oneItem2.pBaseDlg = new CBaseDlg;
	//	oneItem2.strPaneTitle = _T("测试面板二");
	//	oneItem2.eType = enmDlgType_Test2;
	//	m_vecUserDlgs.push_back(oneItem2);
	//
	//	stBasePane oneItem3;
	//	oneItem3.pBaseDlg = new CBaseDlg;
	//	oneItem3.strPaneTitle = _T("测试面板三");
	//	oneItem3.eType = enmDlgType_Test3;
	//	m_vecUserDlgs.push_back(oneItem3);

	//	stBasePane oneItem4;
	//	oneItem4.pBaseDlg = new CBaseDlg;
	//	oneItem4.strPaneTitle = _T("测试面板四");
	//	oneItem4.eType = enmDlgType_Test4;
	//	m_vecUserDlgs.push_back(oneItem4);
	//}
	//

// 	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION,250))
// 	{
// 		TRACE0("Failed to create docking windows\n");
// 		return -1;
// 	}

	CreateFileView();


	//m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndFileView);

	//m_wndNavigationBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndNavigationBar);

	//EnableDocking(CBRS_ALIGN_LEFT);
	//EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	//Create config dialogs
	m_pDlgConfigWnd = new CWndConfigDlg;
	if (NULL != m_pDlgConfigWnd)
	{
		m_pDlgConfigWnd->Create(CWndConfigDlg::IDD, this);
	}

	PostMessage(WM_INIT_MODULES);//OnInitModulePanes




	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp("Short Cut");
	
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID,
		WS_CHILD 
		| WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI|CBRS_FLOATING))
	{
		return FALSE;
	}

	//lee：拿到当前bar下面的basetabctrl。
	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;
	static UINT uDlgID = 1200;

	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	int nDlgIndex = 1;
	BOOL bSetActive = FALSE;
	//for(VecBasePanes::iterator it = m_vecUserDlgs.begin();
	//	it != m_vecUserDlgs.end(); ++it)
	//{
	//	CBaseDlg* pBaseDlg = it->pBaseDlg;
	//	if(NULL != pBaseDlg)
	//	{
	//		CString strNode;
	//		strNode.Format(_T("BaseDlgNode\\Index_%d"), nDlgIndex);
	//		AppXml()->SetAttributeInt(strNode, (UINT)it->eType);
	//		AppXml()->FlushData();

	//		strNode.Empty();
	//		strNode.Format(_T("BaseDlgNode\\Title_%d"), nDlgIndex);
	//		AppXml()->SetAttribute(strNode, it->strPaneTitle);
	//		AppXml()->FlushData();

	//		strNode.Empty();
	//		strNode.Format(_T("BaseDlgNode\\bShow_%d"), nDlgIndex);
	//		AppXml()->SetAttributeInt(strNode, (UINT)it->bShow);
	//		AppXml()->FlushData();

	//		
	//		pBaseDlg->Create(rectDummy, &bar,  it->eType, uDlgID++);
	//		pOutlookBar->AddControl(pBaseDlg, it->strPaneTitle, ++uiPageID, TRUE, dwStyle);
	//		pOutlookBar->ShowTab(nDlgIndex-1, it->bShow,TRUE);//!:tab index start with 0;
	//		if (it->bShow && !bSetActive)//hide tab会使active tab index为空，导致显示的时候没有激活tab。需要设置一次激活tab。
	//		{
	//			pOutlookBar->SetActiveTab(nDlgIndex-1);
	//			bSetActive = TRUE;
	//		}
	//	}
	//	nDlgIndex++;
	//}
	//AppXml()->SetAttributeInt(_T("BaseDlgNodeCount"), m_vecUserDlgs.size());
	//AppXml()->FlushData();


	bNameValid = strTemp.LoadString(IDS_CALENDAR);
	ASSERT(bNameValid);
	
	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}
BOOL CMainFrame::CreateFileView()
{
	BOOL bNameValid;

	//// Create file view
	//CString strFileView;
	//bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, 
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT| CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create File View window\n");
	//	return FALSE; 
	//}

	//SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	/*HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
		MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON), 0);

	m_wndFileView.SetIcon(hFileViewIcon, FALSE);*/
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif


void CMainFrame::OnRibbonPane1Cmd1()
{
	////lee：拿到当前bar下面的basetabctrl。
	//CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)m_wndNavigationBar.GetUnderlyingWindow();

	//if (pOutlookBar == NULL)
	//{
	//	ASSERT(FALSE);
	//	return /*FALSE*/;
	//}
	//
	////pOutlookBar->OnShowOptions();

	//COutlookOptionsDlg dlg(*pOutlookBar);
	//if (dlg.DoModal() == IDOK)
	//{
	//	pOutlookBar->RecalcLayout();
	//}

}
void CMainFrame::OnRibbonPane1Cmd2()
{
	CDlgPaneConfig dlg;
	dlg.DoModal();

	//AfxMessageBox(_T("调出你自己的用户界面1！"));
}
void CMainFrame::OnRibbonPane1Cmd3()
{
	AfxMessageBox(_T("调出你自己的用户界面2！"));
}
void CMainFrame::OnRibbonPane1Cmd4()
{
	AfxMessageBox(_T("调出你自己的用户界面3！"));
}
void CMainFrame::OnRibbonPane2Cmd1()
{
	AfxMessageBox(_T("调出你自己的用户界面4！"));
}
void CMainFrame::OnRibbonPane2Cmd2()
{
	AfxMessageBox(_T("调出你自己的用户界面5！"));
}
void CMainFrame::OnRibbonPane2Cmd3()
{
	AfxMessageBox(_T("调出你自己的用户界面6！"));
}
void CMainFrame::OnRibbonPane2Cmd4()
{
	AfxMessageBox(_T("调出你自己的用户界面7！"));
}
//系统自带
void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}
//系统自带
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}
//系统自带
LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		//m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);

	CString strTemp;
	CModulePane* pModulePane=NULL;
//	POSITION pos = m_HoriPaneMap.GetStartPosition();
// 	if( pos )
// 	{
// 		m_HoriPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
// 
// 		HWND hWndTab;
// 		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
// 		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
// 		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
// 		{
// 			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
// 			pTabCtrl->EnableAutoColor();
// 			pTabCtrl->RecalcLayout();
// 		}
// 	}
// 
// 	pos = m_VertPaneMap.GetStartPosition();
// 	if( pModulePane==NULL && pos )
// 	{
// 		m_VertPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
// 
// 		HWND hWndTab;
// 		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
// 		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
// 		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
// 		{
// 			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
// 			pTabCtrl->EnableAutoColor();
// 			pTabCtrl->RecalcLayout();
// 		}
// 	}

	POSITION pos = m_LeftPaneMap.GetStartPosition();
	if( pModulePane==NULL && pos )
	{
		m_LeftPaneMap.GetNextAssoc(pos, strTemp, pModulePane);

		HWND hWndTab;
		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
		{
			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
			pTabCtrl->EnableAutoColor();
			pTabCtrl->RecalcLayout();
		}
	}

	pos = m_RightPaneMap.GetStartPosition();
	if( pModulePane==NULL && pos )
	{
		m_RightPaneMap.GetNextAssoc(pos, strTemp, pModulePane);

		HWND hWndTab;
		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
		{
			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
			pTabCtrl->EnableAutoColor();
			pTabCtrl->RecalcLayout();
		}
	}

	pos = m_TopPaneMap.GetStartPosition();
	if( pModulePane==NULL && pos )
	{
		m_TopPaneMap.GetNextAssoc(pos, strTemp, pModulePane);

		HWND hWndTab;
		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
		{
			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
			pTabCtrl->EnableAutoColor();
			pTabCtrl->RecalcLayout();
		}
	}

	pos = m_BottomPaneMap.GetStartPosition();
	if( pModulePane==NULL && pos )
	{
		m_BottomPaneMap.GetNextAssoc(pos, strTemp, pModulePane);

		HWND hWndTab;
		CMFCBaseTabCtrl* pBaseTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
		{
			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
			pTabCtrl->EnableAutoColor();
			pTabCtrl->RecalcLayout();
		}
	}
}
LRESULT CMainFrame::OnResetMenu(WPARAM,LPARAM)
{
	return 0;
}

void CMainFrame::OnModuleMenuItem(UINT nID)
{
	//(_T("MenuID:%d"),nID-BASE_MODULES_MENU_ID);
	LockWindowUpdate();
	OpenModule(nID, false);
	UnlockWindowUpdate();
}

void CMainFrame::OnUtilityMenuItem( UINT nID )
{
	//(_T("MenuID:%d"),nID-BASE_UTILITIES_MENU_ID);
	//OpenUtility(nID);
	LockWindowUpdate();
	OpenToolbar(nID);
	UnlockWindowUpdate();
}
void CMainFrame::OnUpdateToolbarMenu(CCmdUI* pCmdUI)
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(pCmdUI->m_nID == mc.m_nMenuID+BASE_UTILITIES_MENU_ID)
		{
			pCmdUI->Enable(mc.m_bInitialized);
		}
	}
}

BOOL CMainFrame::OpenUtilityByName(CString strUtilName)
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_bUtility && mc.m_strTitle.CompareNoCase(strUtilName)==0)
		{
			UINT nID = mc.m_nMenuID + BASE_UTILITIES_MENU_ID;
			return OpenUtility(nID);
		}
	}

	return FALSE;
}

BOOL CMainFrame::OpenToolbar(UINT nID)
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_nMenuID == nID-BASE_UTILITIES_MENU_ID )
		{
			if( !mc.hLib )
			{
				CString strPath = CFileHelper::GetModuleDir();
				mc.hLib = LoadLibrary(strPath+_T("\\")+mc.m_strDll);
			}

			if( !mc.hLib )
			{
				CString strError;
				strError.Format(_T("Load %s failed. ErrorCode:%u"), mc.m_strDll, GetLastError());
				AfxMessageBox(strError);
				return FALSE;
			}

			if( !mc.m_bInitialized)
			{
				LPDLLFUNC *pInit = (LPDLLFUNC*)GetProcAddress(mc.hLib, "Init");
				if(pInit)
					pInit(0);
				mc.m_bInitialized = true;
			}

			//load toolbar.
			if (!mc.m_bToolbarLoaded)
			{
				LPDLLFUNC* pAddToolbar = (LPDLLFUNC*)GetProcAddress(mc.hLib, "AddToolbar");
				if (pAddToolbar)
				{
					pAddToolbar(0);
					mc.m_bToolbarLoaded = true;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CMainFrame::OpenUtility( UINT nID )
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_bUtility && mc.m_nMenuID == nID-BASE_UTILITIES_MENU_ID )
		{
			CString strExeName = mc.m_strDll;
			CString strExePath = CFileHelper::GetModuleDir()+_T("\\")+strExeName;
			if( CreateChildProcess(strExePath, _T("")) )
			{
				CString strError;
				strError.Format(_T("Open Utility '%s' failed"), strExeName);
				AfxMessageBox(strError);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	return FALSE;
}
BOOL CMainFrame::CreateChildProcess(CString strAppPath, CString strCmdLine)
{
	strCmdLine.TrimLeft();
	strCmdLine.TrimRight();
	if(!strCmdLine.IsEmpty())
	{
		strCmdLine = _T(" /c \"") + strCmdLine + _T("\"");
	}

	PROCESS_INFORMATION pi;
	if (LaunchProcess(pi, strAppPath.GetBuffer(), strCmdLine.GetBuffer()))
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CMainFrame::LaunchProcess(PROCESS_INFORMATION& pi, LPCTSTR szModulePath, LPCTSTR szCmdline )
{
	ZeroMemory( &pi, sizeof(pi) );

	STARTUPINFO si;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	CString str = _T(" ");
	str += szCmdline;

	if (!CreateProcess(szModulePath, (LPTSTR)str.GetBuffer(0), NULL,NULL,FALSE,0,NULL,NULL, &si, &pi))
	{
		DWORD nError = GetLastError();
		CString strErr;
		strErr.Format(_T("Create Process %s Failed! ErrorCode = %d"), szModulePath, nError);
		return FALSE;
	}

	return TRUE;
}


void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}
//////////////////////////////////////////////////////////////////////////

void CMainFrame::CacheMenus()
{
	CCSVFile modules;
	if( !modules.Load() )
	{
		AfxMessageBox(_T("Load ...\\Config\\InternalModules.csv file failed!"));
		return;
	}


	int nModuleMenuId  = 0;
	int nUtilityMenuId = 0;
	for(int i=0; i<modules.GetCount(); ++i)
	{
		try
		{
			CMenuCommand mc;
			mc.m_strDll = modules.ReadString(i, _T("DLLName"));
			mc.m_strTitle =  modules.ReadString(i, _T("Title"));
			mc.m_nMenuID = i;
			mc.m_bAddToMenu = modules.ReadBool(i,_T("IsAddToMenu"));
			mc.m_bUtility = modules.ReadBool(i,_T("IsUtility"));
			mc.m_bHasView = modules.ReadBool(i,_T("HasView"));
			mc.m_bSingleView = modules.ReadBool(i,_T("IsSingleView"));
			mc.m_bLoadAtStartup = modules.ReadBool(i,_T("IsLoadAtStartup"));
			if(mc.m_bUtility)
				mc.m_nMenuID = nUtilityMenuId++;
			else
				mc.m_nMenuID = nModuleMenuId++;

			m_AllCommands.AddTail(mc);
		}
		catch(...)
		{
			continue;
		}
	}

	if(m_AllCommands.GetCount()==0)
	{
		AfxMessageBox(_T("Exec\\DB\\InternalModules.csv may be empty or corrupted!"));
	}

}

void CMainFrame::LoadModuleMenuItems()
{
	CacheMenus();

	POSITION	 pos;
	for (pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if ( mc.m_bAddToMenu )
		{
			AddModuleMenuItem(mc.m_strTitle, mc.m_nMenuID, FALSE, mc.m_bUtility);
			
			CString strLoadToolbar;
			strLoadToolbar.Format(_T("加载%s Toolbar"), mc.m_strTitle.GetString());
			AddModuleMenuItem(strLoadToolbar, mc.m_nMenuID, FALSE, TRUE);
		}

		if(!mc.hLib)
		{
			mc.hLib = LoadLibrary(mc.m_strDll);
		}

		if(mc.hLib && !mc.m_bUtility )
		{
			typedef BOOL (*LPFN_GETICONRESOUCEID)(UINT&);
			LPFN_GETICONRESOUCEID pFunc = (LPFN_GETICONRESOUCEID)GetProcAddress(mc.hLib, "GetIconResourceID");
			if(pFunc)
			{
				UINT nResID;
				if(pFunc(nResID))
				{
					CIntaffCommonResourceHandle resHandler(mc.m_strDll, __FILEW__, __LINE__);
					HICON hIcon = (HICON) LoadImage( AfxGetResourceHandle(), MAKEINTRESOURCE( nResID ), IMAGE_ICON, 16, 16, 0 );
					
					mc.m_hIcon = hIcon;
				}
			}
		}

	}

	m_wndMenuBar.RecalcLayout();
}

void CMainFrame::AddModuleMenuItem(const CString &strModuleName, int nID, BOOL bIsView /*=FALSE*/, BOOL bIsUtility /*=FALSE*/)
{
	CMenu menu;
	menu.Attach(m_wndMenuBar.GetHMenu());

	BOOL bAppend=FALSE;
	int nIndex = FindMenuItem(&menu, m_strModuleMenuItems[bIsUtility?1:0]);
	CMenu *pSubMenu = menu.GetSubMenu(nIndex);
	if(bIsView)
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_VIEWS_MENU_ID+nID, strModuleName);
	else if(bIsUtility)
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_UTILITIES_MENU_ID+nID, strModuleName);
	else
		bAppend = pSubMenu->AppendMenu(MF_ENABLED|MF_STRING, BASE_MODULES_MENU_ID+nID, strModuleName);


	// Remove dummy
	if( pSubMenu->GetMenuItemCount() )
	{
		CString str;
		pSubMenu->GetMenuString(0, str, MF_BYPOSITION);
		if( str.CompareNoCase(_T("Dummy"))==0 )
		{
			pSubMenu->RemoveMenu(0, MF_BYPOSITION);
		}
	}

	m_wndMenuBar.CreateFromMenu(menu.Detach(), TRUE, TRUE);
}

int CMainFrame::FindMenuItem(CMenu *pMenu, const CString &str)
{
	int count = pMenu->GetMenuItemCount();
	CString temp;
	for (int i=0; i<count; i++)
	{
		if( pMenu->GetMenuString(i, temp, MF_BYPOSITION) && str == temp)
			return i;
	}

	return -1;
}

LRESULT CMainFrame::OnInitModulePanes(WPARAM W, LPARAM L)
{
	LockWindowUpdate();

	// Open Viewers
	OpenLastUsedModules();

	CreateDockpanesAndChilds();

	//OpenAlwaysShowPane();

	// Activate Pane
	//ActiveOnePaneByName(_T("Module1"), MODULE_WINDOW_DEF::PANE_ACTIVATE);

	UnlockWindowUpdate();

	return 0;

}

void CMainFrame::OpenLastUsedModules()
{
	LockWindowUpdate();

	StartupAsWorkspace();

	UnlockWindowUpdate();
}
BOOL CMainFrame::StartupAsWorkspace()
{
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//pXml->LockToRead();
	CString strNode;
	strNode.Format(_T("Workspace\\TabbedView\\Alignment"));
	int nAlign = pXml->GetAttributeInt(strNode, 0);
	BOOL bVert = (nAlign==1);

	BOOL bNewMDITabbedGroup=FALSE;
	strNode.Format(_T("Workspace\\TabbedView\\GroupCount"));
	int nGroupCount = pXml->GetAttributeInt(strNode, 0);
	if (nGroupCount>0)
	{
		for (int nGroupIndex = 0; nGroupIndex<nGroupCount; nGroupIndex++)
		{
			CString strNode;
			strNode.Format(_T("Workspace\\TabbedView\\Group%d\\TabCount"), nGroupIndex);
			int nTabCount = pXml->GetAttributeInt(strNode, 0);
			if (nTabCount>0)
			{
				for (int nTabIndex = 0; nTabIndex<nTabCount; nTabIndex++)
				{
					strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\DllName"), nGroupIndex, nTabIndex);
					wstring wsDllName = pXml->GetAttributeText(strNode, _T(""));
				
					if (!wsDllName.empty())
					{
						//pXml->UnLock();//释放xmllock，允许其他模块写入
						OpenModuleByName(wsDllName.c_str());//打开dll模块。

						//默认的都是一个组里面。
						if( bNewMDITabbedGroup && nAlign>0 )
						{
							//新建一个tab组，在里面放置tab列。
							MDITabNewGroup(bVert);
							bNewMDITabbedGroup = FALSE;
						}
						//pXml->LockToRead();
						if(m_pViewLastCreated)
						{
							//add rect info
							CRect rcView;
							CString strNode;
							strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\left"), nGroupIndex,nTabIndex);
							rcView.left = AppXml()->GetAttributeInt(strNode, 0);

							strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\right"), nGroupIndex,nTabIndex);
							rcView.right = AppXml()->GetAttributeInt(strNode, 0);

							strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\top"), nGroupIndex,nTabIndex);
							rcView.top = AppXml()->GetAttributeInt(strNode, 0);

							strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\bottom"), nGroupIndex,nTabIndex);
							rcView.bottom = AppXml()->GetAttributeInt(strNode, 0);

							m_mapGroupTabRects.insert(make_pair(nGroupIndex, rcView));
				
							//m_pViewLastCreated->MoveWindow(&rcView);
							//AdjustClientArea();

							//m_pViewLastCreated->SendMessage(WM_SET_WKSDATA, eTabbedView, (LPARAM)pTabNode);
						}
					}
				}
			}
		
			bNewMDITabbedGroup = TRUE;
		}
	}

	//DockablePane
	typedef xml_node<TCHAR> NodeT;
	typedef NodeT* NodePtrT;
	NodeT*  pNodePane = pXml->FindChild(_T("Workspace\\DockablePane"));
	if(pNodePane)
	{
		NodeT* pNodeModule = pNodePane->first_node();
		while(pNodeModule)
		{
			//Open Module first
			CString strModule = (LPCTSTR)pNodeModule->name();
			if (!strModule.IsEmpty())
			{
				BOOL bRet = TRUE;
				if(theApp.GetNumOfView(strModule)==0)
				{
					//pXml->UnLock();
					bRet = OpenModuleByName(strModule);
					//pXml->LockToRead();
				}

				if(!bRet)
				{
					CString strError;
					strError.Format(_T("Failed to Open module %s"), strModule);
					AfxMessageBox(strError);
					break;
				}


				// Get InitPaneByWorkspace function from dll
				LPFN_DLLINITPANEBYWORKSPACE pFunc = NULL;
				for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
				{
					CMenuCommand &mc = m_AllCommands.GetNext(pos);
					if(!mc.m_bUtility && mc.m_strTitle.CompareNoCase(strModule)==0)
					{
						pFunc = (LPFN_DLLINITPANEBYWORKSPACE)GetProcAddress(mc.hLib, "InitPaneByWorkspace");
						break;
					}
				}
				if(!pFunc)
				{
					CString strError;
					strError.Format(_T("Failed to find InitPaneByWorkspace function in %s.dll"), strModule);
					break;
				}


				// call InitPaneByWorkspace function ......
				NodeT* pNodeDashboards = pNodeModule->first_node();
				if(pNodeDashboards)
				{
					CString strEr;
					strEr.Format(_T("Calling %s InitPaneByWorkspace function..."), strModule);
					bRet = FALSE;

					try
					{
						//pXml->UnLock();
						bRet = pFunc(pNodeDashboards);
						//pXml->LockToRead();
					}
					catch (CException* e)
					{
						CString strError;
						e->GetErrorMessage(strError.GetBuffer(256), 256);
						strError.ReleaseBuffer();
						strEr.Format(_T("Exception from InitPaneByWorkspace, error: %s"), strError.GetString());
						AfxMessageBox(strEr);
						break;
					}
					catch(...)
					{
						strEr.Format(_T("Unknown exception from InitPaneByWorkspace"));
						AfxMessageBox(strEr);
						break;
					}
				}
			}
			
			pNodeModule = pNodeModule->next_sibling();
		}

	}


	//CXmlConfig::Instance()->UnLock();


	RecalcLayoutEx();

	return TRUE;
}

void CMainFrame::RecalcLayoutEx()
{
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);
	if( nGroupNum > 1 )
	{
		const CMyMDIClientAreaWnd* pClientArea = (const CMyMDIClientAreaWnd*) &m_wndClientArea;    

		CRect rc;
		pClientArea->GetClientRect(&rc);
		for(int i=0; i<nGroupNum; ++i)
		{
			CMFCTabCtrl* pTabCtrl = vecTabCtrls.at(i);
			CRect rect;
			map<UINT, CRect>::iterator itFind = m_mapGroupTabRects.find(i);
			if (itFind != m_mapGroupTabRects.end())
			{
				rect = itFind->second;
				pTabCtrl->MoveWindow(&rect);
			}
		}
	}

	__super::RecalcLayout();
}

void CMainFrame::CreateDockpanesAndChilds()
{
	map<UINT, ListOneDllChildWnds>& mapDll2ChildWnds = CXmlDataProc::Instance()->m_mapDllIndex2Childlist;
	for (map<UINT, ListOneDllChildWnds>::iterator it = mapDll2ChildWnds.begin(); it != mapDll2ChildWnds.end(); ++it)
	{
		ListOneDllChildWnds& listChildWnds = it->second;
		for (ListOneDllChildWnds::iterator itChild = listChildWnds.begin(); itChild != listChildWnds.end(); ++itChild)
		{
			stChildWndObj& oneChild = *itChild;
			CWnd* pParent = CWndManager::Instance()->CreateChildWndEx(oneChild.strParentClass, oneChild.strChildClass, 
				oneChild.rcChild, oneChild.rcParent,_T(""), oneChild.strDllname);

			if (NULL != pParent && NULL != pParent->GetSafeHwnd())
			{
				pParent->SendMessage(WM_SIZE);
			}
		}
	}

	//udpate xml
	CWndManager::Instance()->RefreshChildGroup();

	//create float panes.
	list<stFloatWnd>& listFloatWnds = CXmlDataProc::Instance()->m_listFloatWnds;
	for (list<stFloatWnd>::iterator itFloat = listFloatWnds.begin(); itFloat != listFloatWnds.end(); ++itFloat)
	{
		stFloatWnd& oneFloat = *itFloat;
		CWnd* pFloat = CWndManager::Instance()->CreateFloatWnd((CWnd*)AfxGetMainWnd(), oneFloat.strClass, oneFloat.strWndName, oneFloat.strDllname);
		CModulePane* pModulePane = NULL;
		CString strhist;
		strhist.Format(_T("0x%08x"), pFloat);
		if (m_FloatPaneMap.Lookup(strhist, pModulePane))
		{
			pModulePane->MoveWindow(oneFloat.rcWnd);
			pModulePane->RecalcLayout();
		}
	}

	//create dock panes.
	list<stDockWnd>& listDockWnds = CXmlDataProc::Instance()->m_listDockWnds;
	for(list<stDockWnd>::iterator itDock = listDockWnds.begin(); itDock != listDockWnds.end(); ++itDock)
	{
		stDockWnd& oneDock = *itDock;
		CWnd* pDockDlg = CWndManager::Instance()->CreateDockWnd((CWnd*)AfxGetMainWnd(), oneDock.strClass, 
																oneDock.eDockType, CString(_T("")), oneDock.strDllname);
		if (pDockDlg != NULL)
		{
			CModulePane* pDockpane = NULL;
			CString strhist;
			strhist.Format(_T("0x%08x"), pDockDlg);
			if (oneDock.eDockType == ALIGN_LEFT ||oneDock.eDockType == ALIGN_LEFT_GROUP)
			{
				if(m_LeftPaneMap.Lookup(strhist, pDockpane))
				{
					pDockpane->MoveWindow(oneDock.rcWnd);
					pDockpane->RecalcLayout();
				}
			}
			else if (oneDock.eDockType == ALIGN_RIGHT || oneDock.eDockType == ALIGN_RIGHT_GROUP)
			{
				if (m_RightPaneMap.Lookup(strhist, pDockpane))
				{
					pDockpane->MoveWindow(oneDock.rcWnd);
					pDockpane->RecalcLayout();
				}
			}
			else if (oneDock.eDockType == ALIGN_TOP || oneDock.eDockType == ALIGN_TOP_GROUP)
			{
				if (m_TopPaneMap.Lookup(strhist, pDockpane))
				{
					pDockpane->MoveWindow(oneDock.rcWnd);
					pDockpane->RecalcLayout();
				}
			}
			else if (oneDock.eDockType == ALIGN_BOTTON || oneDock.eDockType == ALIGN_BOTTOM_GROUP)
			{
				if (m_BottomPaneMap.Lookup(strhist, pDockpane))
				{
					pDockpane->MoveWindow(oneDock.rcWnd);
					pDockpane->RecalcLayout();
				}
			}
		}
	}
}

BOOL CMainFrame::OpenModuleByName(CString strModuleName, bool onStartup /*=true*/)
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_strTitle.CompareNoCase(strModuleName)==0)
		{
			UINT nID = mc.m_nMenuID + BASE_MODULES_MENU_ID;
			return OpenModule(nID, onStartup);
		}
	}

	return FALSE;
}

BOOL CMainFrame::OpenModule( UINT nID, bool onStartup /*=true*/ )
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if (!mc.m_bUtility && mc.m_nMenuID == nID-BASE_MODULES_MENU_ID )
		{
			if( !mc.hLib )
			{
				CString strPath = CFileHelper::GetModuleDir();
				mc.hLib = LoadLibrary(strPath+_T("\\")+mc.m_strDll);
			}

			if( !mc.hLib )
			{
				CString strError;

				strError.Format(_T("Load %s failed. ErrorCode:%u"), mc.m_strDll, GetLastError());
				return FALSE;
			}

			if( !mc.m_bInitialized)
			{
				LPDLLFUNC *pInit = (LPDLLFUNC*)GetProcAddress(mc.hLib, "Init");
				if(pInit)
					pInit(0);//lee:注册并生成文档模板（docTemplate+Toolbar完成加载）
				mc.m_bInitialized = true;
			}

			if( mc.m_bHasView && !mc.m_strTitle.IsEmpty())
			{
				BeginWaitCursor();
				CIntaffCommonResourceHandle resHandle(mc.m_strDll, __FILEW__, __LINE__);
				//lee：如果包含view，此时打开(View+doc)
				m_pViewLastCreated = theApp.OpenView(mc.m_strTitle, mc.m_bSingleView);
				EndWaitCursor();
				m_mapViewRegister[m_pViewLastCreated] = mc.m_strTitle;
			}


			LPDLLFUNC* pFunc = (LPDLLFUNC*)GetProcAddress(mc.hLib, "LoadModulePane");
			if(pFunc)
				pFunc(0);//lee:加载panels

			//不是通过读取csv的workspace来加载的，是用户点击菜单加载的。
			if (!onStartup)	this->EnumTabbedView();

			return TRUE;
		}

	}

	return FALSE;
}

void CMainFrame::EnumTabbedView()
{
	CString strTemp;
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
		return;

	//xml_document<TCHAR>* pDoc = pXml->GetDocument();

	//MakeNode <Workspace\\TabbedView>
	/*pXml->RemoveNode(_T("Workspace\\TabbedView"));
	pXml->SetAttributeBool(_T("Workspace\\TabbedView"), false);
	xml_node<TCHAR>* pWksNode = pXml->FindChild(_T("Workspace\\TabbedView"));*/


	//MakeNode<Alignment>
	const CMyMDIClientAreaWnd* pClientArea = (const CMyMDIClientAreaWnd*) &m_wndClientArea;
	int iAlign = pClientArea->IsAlign()? (pClientArea->IsVertAlign()?1:2):0;
	strTemp.Format(_T("%d"), iAlign);
	//TCHAR* pchName = pDoc->allocate_string(_T("Alignment"));
	//TCHAR* pchValue= pDoc->allocate_string(strTemp.GetString());
	//xml_node<TCHAR>* pNodeAlign = pWksNode->set_node(pchName, pchValue);
	CString strNode;
	strNode.Format(_T("Workspace\\TabbedView\\Alignment"));
	AppXml()->SetAttributeInt(strNode, iAlign);
	pXml->FlushData();


	//MakeNode<TabbedGroup>
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);
	//write by lee
	strNode.Format(_T("Workspace\\TabbedView\\GroupCount"));
	pXml->SetAttributeInt(strNode, nGroupNum);
	pXml->FlushData();


	for(int i=0; i<nGroupNum; ++i)
	{
		vector<CWorkspaceData>& vecWksData = m_workSpace.m_mapWks[i];
		CMFCTabCtrl* pTabCtrl = vecTabCtrls[i];
		const int nTabsNum = pTabCtrl->GetTabsNum();
	
		//add by lee.
		CString strNode;
		strNode.Format(_T("Workspace\\TabbedView\\Group%d\\TabCount"), i);
		pXml->SetAttributeInt(strNode, nTabsNum);
		pXml->FlushData();


		for(int n=0; n<nTabsNum; ++n)
		{
			CWnd* pWnd = pTabCtrl->GetTabWnd(n);
			if(!pWnd->GetSafeHwnd())
				continue;

			CFrameWnd* pFrame = dynamic_cast<CFrameWnd*>(pWnd);
			if(!pFrame||!(pFrame->GetActiveView()))
				continue;

			CView* pView = pFrame->GetActiveView();
			map<CView*, CString>::const_iterator it = m_mapViewRegister.find(pView);
			if(it!=m_mapViewRegister.end())
			{
				CString strViewName = it->second;
				const CMenuCommand* pMenuCmd = GetMenuCommand(strViewName);
				if(pMenuCmd && !pMenuCmd->m_bLoadAtStartup)
				{
					continue;
				}

				CString strTab;
				strTab.Format(_T("Workspace\\TabbedView\\Group%d\\Tab%d"), i, n);
				pXml->SetAttributeBool(strTab.GetString(), false);

				//modify by lee
				CString strNode;
				strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\DllName"), i,n);
				pXml->SetAttribute(strNode, strViewName.GetString());


				//pXml->LockToWrite();
				xml_node<TCHAR>* pTabNode = pXml->FindChild(strTab);
				xml_attribute<TCHAR>* pAttr = pTabNode->document()->allocate_attribute(_T("Name"),strViewName.GetString());
				pTabNode->append_attribute(pAttr);

				try
				{
					pView->SendMessage(WM_GET_WKSDATA, 0, (LPARAM)pTabNode);
				}
				catch (...)
				{
					pXml->RemoveNode(strTab.GetString());
				}

				//pXml->UnLock();

				//add rect info
				CRect rcView;
				pView->GetWindowRect(&rcView);
				//CString strNode;
				strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\left"), i,n);
				AppXml()->SetAttributeInt(strNode, rcView.left);
				AppXml()->FlushData();
				
				strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\right"), i,n);
				AppXml()->SetAttributeInt(strNode, rcView.right);
				AppXml()->FlushData();

				strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\top"), i,n);
				AppXml()->SetAttributeInt(strNode, rcView.top);
				AppXml()->FlushData();

				strNode.Format(_T("Workspace\\TabbedView\\Group%d\\Tab_%d\\bottom"), i,n);
				AppXml()->SetAttributeInt(strNode, rcView.bottom);
				AppXml()->FlushData();
			}
		}
	}

	pXml->FlushData();
}


int CMainFrame::GetMDITabCtrls(vector<CMFCTabCtrl*>& vecTabCtrls)
{
	const CObList& obList = GetMDITabGroups();
	POSITION pos = obList.GetHeadPosition();
	while(pos)
	{
		const CObject* pOb = obList.GetNext(pos);
		CMFCTabCtrl* pTab = DYNAMIC_DOWNCAST(CMFCTabCtrl, pOb);
		if(pTab)
		{
			vecTabCtrls.push_back(pTab);
		}
	}

	return vecTabCtrls.size();
}

const CMenuCommand* CMainFrame::GetMenuCommand( LPCTSTR lpszModuleName )
{
	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		const CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.m_strTitle.CompareNoCase(lpszModuleName)==0)
			return &mc;
	}

	return NULL;
}


LRESULT CMainFrame::OnRegisterModulePane( WPARAM wp, LPARAM )
{
	static int counter = 0;

	MODULE_WINDOW_DEF* pDef = (MODULE_WINDOW_DEF*)wp;
	ASSERT(pDef->pWnd!=NULL);


	//////////////////////////////////////////////////////////////////////////
	// Create Pane
	EPANE_ALIGNMENT Align = pDef->nEnabledAlign;
	UINT nDockID;//= (Align==ALIGN_HORIZONTAL)? AFX_IDW_DOCKBAR_BOTTOM:(Align==ALIGN_VERTICAL?AFX_IDW_DOCKBAR_RIGHT:AFX_IDW_DOCKBAR_FLOAT);
	if (Align == ALIGN_LEFT || Align == ALIGN_LEFT_GROUP)
	{
		nDockID = AFX_IDW_DOCKBAR_LEFT;
	}
	else if (Align == ALIGN_RIGHT || Align == ALIGN_RIGHT_GROUP)
	{
		nDockID = AFX_IDW_DOCKBAR_RIGHT;
	}
	else if (Align == ALIGN_TOP || Align == ALIGN_TOP_GROUP)
	{
		nDockID = AFX_IDW_DOCKBAR_TOP;
	}
	else if (Align == ALIGN_BOTTON || Align == ALIGN_BOTTOM_GROUP)
	{
		nDockID = AFX_IDW_DOCKBAR_BOTTOM;
	}
	else if (Align == ALIGN_FLOAT)
	{
		nDockID = AFX_IDW_DOCKBAR_FLOAT;
	}
// 	else if (Align == ALIGN_HORIZONTAL)
// 	{
// 		nDockID = AFX_IDW_DOCKBAR_BOTTOM;
// 	}
// 	else if (Align == ALIGN_VERTICAL)
// 	{
// 		nDockID = AFX_IDW_DOCKBAR_RIGHT;
// 	}

	BOOL bVertical=FALSE;
	CModulePane* pModulePane;
	//lee:ignore
	//if( FindModulePane(pDef->strWindowName, pModulePane, bVertical) )
	//{
	//	CBaseTabbedPane* pTabbedBar = pModulePane->GetParentTabbedPane();

	//	if(pModulePane->IsAutoHideMode())
	//	{
	//		//pModulePane->SetAutoHideMode(FALSE, pModulePane->GetCurrentAlignment());
	//		//pModulePane->SetAutoHideMode(TRUE, pModulePane->GetCurrentAlignment());
	//	}
	//	else
	//	{
	//		pModulePane->ShowPane(SW_SHOW, FALSE, TRUE);
	//	}

	//	return 0;
	//}

	//为pane分配ID，并创建Pane。
	int nNewID = BASE_MODULES_MENU_ID+MAX_NUM_VIEW_TYPES+counter++;
	pModulePane = new CModulePane;
	UINT style = WS_CHILD | CBRS_RIGHT |CBRS_FLOAT_MULTI|CBRS_HIDE_INPLACE;
	DWORD tabbedStyle;
	if(pDef->nEnabledAlign==ALIGN_LEFT || pDef->nEnabledAlign == ALIGN_RIGHT || 
		pDef->nEnabledAlign == ALIGN_LEFT_GROUP||pDef->nEnabledAlign == ALIGN_RIGHT_GROUP)	
	{
		tabbedStyle = AFX_CBRS_OUTLOOK_TABS;
	}
	else
	{
		tabbedStyle = AFX_CBRS_REGULAR_TABS;
	}

	if (!pModulePane->Create(pDef->strWindowName, this, CRect(0, 0, 280, 900), TRUE,
		nNewID, style, tabbedStyle))
	{
		ASSERT(FALSE);
		delete pModulePane;
		return 1; 
	}


	//////////////////////////////////////////////////////////////////////////
	// Init Pane
	pModulePane->InitPane(pDef);

	//////////////////////////////////////////////////////////////////////////
	// Docking Pane
	pModulePane->EnableDocking(CBRS_ALIGN_ANY/*pDef->nEnabledAlign*/);

	//////////////////////////////////////////////////////////////////////////
	// Attach to existing Pane
	CDockablePane* pTabbedBar = NULL;
	if (pDef->nEnabledAlign == ALIGN_LEFT_GROUP||
		pDef->nEnabledAlign == ALIGN_RIGHT_GROUP||
		pDef->nEnabledAlign == ALIGN_TOP_GROUP||
		pDef->nEnabledAlign == ALIGN_BOTTOM_GROUP)
	{
		//当前是否存在Pane，如果有就往响应的pane组上靠。
		if( !AttachPane(pModulePane, pDef->nEnabledAlign, TRUE, &pTabbedBar) )
		{
			//没有，就单独停靠。
			DockPane((CBasePane*)pModulePane, nDockID);  
			pModulePane->ShowPane(TRUE,FALSE,TRUE);
		}
	}
	else
	{
		DockPane((CBasePane*)pModulePane, nDockID);  
		pModulePane->ShowPane(TRUE,FALSE,TRUE);
	}

	//////////////////////////////////////////////////////////////////////////
	// Modify tab Style
	CBaseTabbedPane* pBaseTabbedPane = dynamic_cast<CBaseTabbedPane*>(pTabbedBar);
	CMFCOutlookBar* pOutlookTabbedBar = dynamic_cast<CMFCOutlookBar*>(pTabbedBar);
	if( pBaseTabbedPane && !pOutlookTabbedBar )
	{
		CMFCBaseTabCtrl* pBaseTabCtrl = pBaseTabbedPane->GetUnderlyingWindow();
		CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
		if(pTabCtrl && !pTabCtrl->IsOneNoteStyle())
		{
			pTabCtrl->ModifyTabStyle(CMFCTabCtrl::STYLE_3D_ONENOTE);
			pTabCtrl->EnableAutoColor();
			pTabCtrl->RecalcLayout();
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//AddToMap
	CString strPane;
	strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
	if (pDef->nEnabledAlign == ALIGN_LEFT || pDef->nEnabledAlign == ALIGN_LEFT_GROUP)
	{
		m_LeftPaneMap.SetAt(/*pDef->strWindowName*/strPane, pModulePane);
	}

	if (pDef->nEnabledAlign == ALIGN_RIGHT || pDef->nEnabledAlign == ALIGN_RIGHT_GROUP)
	{
		m_RightPaneMap.SetAt(/*pDef->strWindowName*/strPane, pModulePane);
	}

	if (pDef->nEnabledAlign == ALIGN_TOP || pDef->nEnabledAlign == ALIGN_TOP_GROUP)
	{
		m_TopPaneMap.SetAt(/*pDef->strWindowName*/strPane, pModulePane);
	}

	if (pDef->nEnabledAlign == ALIGN_BOTTON || pDef->nEnabledAlign == ALIGN_BOTTOM_GROUP)
	{
		m_BottomPaneMap.SetAt(/*pDef->strWindowName*/strPane, pModulePane);
	}
	if (pDef->nEnabledAlign == ALIGN_FLOAT)
	{
		m_FloatPaneMap.SetAt(strPane, pModulePane);
	}

// 	if(pDef->nEnabledAlign==ALIGN_LEFT || pDef->nEnabledAlign == ALIGN_RIGHT||pDef->nEnabledAlign == ALIGN_VERTICAL )
// 	{
// 		m_VertPaneMap.SetAt(pDef->strWindowName, pModulePane);
// 	}
// 	if(pDef->nEnabledAlign== ALIGN_TOP||pDef->nEnabledAlign == ALIGN_BOTTON||pDef->nEnabledAlign == ALIGN_HORIZONTAL )
// 	{
// 		m_HoriPaneMap.SetAt(pDef->strWindowName, pModulePane);
// 	}

	if (Align == ALIGN_FLOAT)
	{
		pModulePane->FloatPane(CRect(100,100,200,800));
	}
// 	else
// 	{
// 		CRect rc(400,0,600,800);//移动dock的窗口，貌似，只关心width + Height
// 		pModulePane->MoveWindow(rc);
// 	}

	RecalcLayout();

	RefreshPanesXmlNodes(Align);
	return 0;
}

void CMainFrame::RefreshPanesXmlNodes(EPANE_ALIGNMENT eAlign)
{
	switch(eAlign)
	{
	case ALIGN_LEFT:
	case ALIGN_LEFT_GROUP:
		{
			RefreshDockLeftNodeXml();
		}
		break;

	case ALIGN_RIGHT:
	case ALIGN_RIGHT_GROUP:
		{
			RefreshDockRightNodeXml();
		}
		break;
	case ALIGN_TOP:
	case ALIGN_TOP_GROUP:
		{
			RefreshDockTopNodeXml();
		}
		break;
	case ALIGN_BOTTON:
	case ALIGN_BOTTOM_GROUP:
		{
			RefreshDockBottomNodeXml();
		}
		break;
	case ALIGN_FLOAT:
		{
			RefreshFloatNodeXml();
		}
		break;
	default:
		break;
	}
}

void CMainFrame::RefreshDockLeftNodeXml()
{
	//[dllname,count]
	map<CString, UINT> mapDll2PaneCount;
	MapCreatedWnd mapCreatedWnds;
	if(CWndManager::Instance()->GetCreatedWnd(mapCreatedWnds))
	{
		POSITION pos = m_LeftPaneMap.GetStartPosition();
		while (pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			m_LeftPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if(NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator it2 = mapCreatedWnds.find(strPane);
				if (it2 != mapCreatedWnds.end())
				{
					map<CString, UINT>::iterator it3 = mapDll2PaneCount.find(it2->second.strDllname);
					if (it3 != mapDll2PaneCount.end())
					{
						UINT& nValue = it3->second;
						nValue++;
					}
					else
					{
						mapDll2PaneCount.insert(make_pair(it2->second.strDllname, 1));
					}
				}
			}
		}
	}

	//copy.
	CModulePaneMap tempPaneMap;// = m_LeftPaneMap;
	POSITION pos = m_LeftPaneMap.GetStartPosition();
	while(pos)
	{
		CString strname;
		CModulePane* pPane = NULL;
		m_LeftPaneMap.GetNextAssoc(pos, strname, pPane);
		tempPaneMap.SetAt(strname, pPane);
	}

	for(map<CString, UINT>::iterator iterIdx = mapDll2PaneCount.begin(); 
		iterIdx != mapDll2PaneCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nPaneCount = iterIdx->second;
		int nDllIndex = CXmlDataProc::Instance()->GetDllIndex(strDllname);
		CString strNode;
		strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\WndCount"), nDllIndex);
		AppXml()->SetAttributeInt(strNode, nPaneCount);
		AppXml()->FlushData();

		//写入所有的工程名是strDllname的，写入的都擦出掉
		POSITION pos = tempPaneMap.GetStartPosition();
		int nWndIndex=0;
		while(pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			tempPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if (NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CRect rcPane;
				pModulePane->GetWindowRect(&rcPane);

				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator itFind = mapCreatedWnds.find(strPane);
				if (itFind != mapCreatedWnds.end())
				{
					stCreateWndItem& oneCreated = itFind->second;
					if (strDllname.CompareNoCase(oneCreated.strDllname) != 0)
					{
						continue;
					}

					//write pane class name;
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\Wnd_%d\\Name"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, oneCreated.strClassName);

					//write pane rect
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\Wnd_%d\\SIZE\\left"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.left);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\Wnd_%d\\SIZE\\right"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.right);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\Wnd_%d\\SIZE\\top"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.top);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\LEFT\\Wnd_%d\\SIZE\\bottom"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.bottom);

					AppXml()->FlushData();


					tempPaneMap.RemoveKey(strWndname);
					nWndIndex++;
					if (nWndIndex >= nPaneCount)
					{
						//当前数据strDllname的pane已经写完了，写下一个dll name的xml文件。
						break;
					}
				}
			}
		}
	}
}
void CMainFrame::RefreshDockRightNodeXml()
{
	//[dllname,count]
	map<CString, UINT> mapDll2PaneCount;
	MapCreatedWnd mapCreatedWnds;
	if(CWndManager::Instance()->GetCreatedWnd(mapCreatedWnds))
	{
		POSITION pos = m_RightPaneMap.GetStartPosition();
		while (pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			m_RightPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if(NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator it2 = mapCreatedWnds.find(strPane);
				if (it2 != mapCreatedWnds.end())
				{
					map<CString, UINT>::iterator it3 = mapDll2PaneCount.find(it2->second.strDllname);
					if (it3 != mapDll2PaneCount.end())
					{
						UINT& nValue = it3->second;
						nValue++;
					}
					else
					{
						mapDll2PaneCount.insert(make_pair(it2->second.strDllname, 1));
					}
				}
			}
		}
	}

	//copy.
	CModulePaneMap tempPaneMap;
	POSITION pos = m_RightPaneMap.GetStartPosition();
	while(pos)
	{
		CString strname;
		CModulePane* pPane = NULL;
		m_RightPaneMap.GetNextAssoc(pos, strname, pPane);
		tempPaneMap.SetAt(strname, pPane);
	}

	for(map<CString, UINT>::iterator iterIdx = mapDll2PaneCount.begin(); 
		iterIdx != mapDll2PaneCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nPaneCount = iterIdx->second;
		int nDllIndex = CXmlDataProc::Instance()->GetDllIndex(strDllname);
		CString strNode;
		strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\WndCount"), nDllIndex);
		AppXml()->SetAttributeInt(strNode, nPaneCount);
		AppXml()->FlushData();

		//写入所有的工程名是strDllname的，写入的都擦出掉
		POSITION pos = tempPaneMap.GetStartPosition();
		int nWndIndex=0;
		while(pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			tempPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if (NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CRect rcPane;
				pModulePane->GetWindowRect(&rcPane);

				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator itFind = mapCreatedWnds.find(strPane);
				if (itFind != mapCreatedWnds.end())
				{
					stCreateWndItem& oneCreated = itFind->second;
					if (strDllname.CompareNoCase(oneCreated.strDllname) != 0)
					{
						continue;
					}

					//write pane class name;
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\Wnd_%d\\Name"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, oneCreated.strClassName);

					//write pane rect
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\Wnd_%d\\SIZE\\left"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.left);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\Wnd_%d\\SIZE\\right"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.right);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\Wnd_%d\\SIZE\\top"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.top);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\RIGHT\\Wnd_%d\\SIZE\\bottom"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.bottom);

					AppXml()->FlushData();


					tempPaneMap.RemoveKey(strWndname);
					nWndIndex++;
					if (nWndIndex >= nPaneCount)
					{
						break;
					}
				}
			}
		}
	}
}
void CMainFrame::RefreshDockTopNodeXml()
{
	//[dllname,count]
	map<CString, UINT> mapDll2PaneCount;
	MapCreatedWnd mapCreatedWnds;
	if(CWndManager::Instance()->GetCreatedWnd(mapCreatedWnds))
	{
		POSITION pos = m_TopPaneMap.GetStartPosition();
		while (pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			m_TopPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if(NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator it2 = mapCreatedWnds.find(strPane);
				if (it2 != mapCreatedWnds.end())
				{
					map<CString, UINT>::iterator it3 = mapDll2PaneCount.find(it2->second.strDllname);
					if (it3 != mapDll2PaneCount.end())
					{
						UINT& nValue = it3->second;
						nValue++;
					}
					else
					{
						mapDll2PaneCount.insert(make_pair(it2->second.strDllname, 1));
					}
				}
			}
		}
	}

	//copy.
	CModulePaneMap tempPaneMap;
	POSITION pos = m_TopPaneMap.GetStartPosition();
	while(pos)
	{
		CString strname;
		CModulePane* pPane = NULL;
		m_TopPaneMap.GetNextAssoc(pos, strname, pPane);
		tempPaneMap.SetAt(strname, pPane);
	}

	for(map<CString, UINT>::iterator iterIdx = mapDll2PaneCount.begin(); 
		iterIdx != mapDll2PaneCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nPaneCount = iterIdx->second;
		int nDllIndex = CXmlDataProc::Instance()->GetDllIndex(strDllname);
		CString strNode;
		strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\WndCount"), nDllIndex);
		AppXml()->SetAttributeInt(strNode, nPaneCount);
		AppXml()->FlushData();

		//写入所有的工程名是strDllname的，写入的都擦出掉
		POSITION pos = tempPaneMap.GetStartPosition();
		int nWndIndex=0;
		while(pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			tempPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if (NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CRect rcPane;
				pModulePane->GetWindowRect(&rcPane);

				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator itFind = mapCreatedWnds.find(strPane);
				if (itFind != mapCreatedWnds.end())
				{
					stCreateWndItem& oneCreated = itFind->second;
					if (strDllname.CompareNoCase(oneCreated.strDllname) != 0)
					{
						continue;
					}

					//write pane class name;
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\Wnd_%d\\Name"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, oneCreated.strClassName);

					//write pane rect
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\Wnd_%d\\SIZE\\left"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.left);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\Wnd_%d\\SIZE\\right"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.right);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\Wnd_%d\\SIZE\\top"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.top);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\TOP\\Wnd_%d\\SIZE\\bottom"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.bottom);

					AppXml()->FlushData();


					tempPaneMap.RemoveKey(strWndname);
					nWndIndex++;
					if (nWndIndex >= nPaneCount)
					{
						break;
					}
				}
			}
		}
	}
}
void CMainFrame::RefreshDockBottomNodeXml()
{
	//[dllname,count]
	map<CString, UINT> mapDll2PaneCount;
	MapCreatedWnd mapCreatedWnds;
	if(CWndManager::Instance()->GetCreatedWnd(mapCreatedWnds))
	{
		POSITION pos = m_BottomPaneMap.GetStartPosition();
		while (pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			m_BottomPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if(NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator it2 = mapCreatedWnds.find(strPane);
				if (it2 != mapCreatedWnds.end())
				{
					map<CString, UINT>::iterator it3 = mapDll2PaneCount.find(it2->second.strDllname);
					if (it3 != mapDll2PaneCount.end())
					{
						UINT& nValue = it3->second;
						nValue++;
					}
					else
					{
						mapDll2PaneCount.insert(make_pair(it2->second.strDllname, 1));
					}
				}
			}
		}
	}

	//copy.
	CModulePaneMap tempPaneMap;
	POSITION pos = m_BottomPaneMap.GetStartPosition();
	while(pos)
	{
		CString strname;
		CModulePane* pPane = NULL;
		m_BottomPaneMap.GetNextAssoc(pos, strname, pPane);
		tempPaneMap.SetAt(strname, pPane);
	}

	for(map<CString, UINT>::iterator iterIdx = mapDll2PaneCount.begin(); 
		iterIdx != mapDll2PaneCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nPaneCount = iterIdx->second;
		int nDllIndex = CXmlDataProc::Instance()->GetDllIndex(strDllname);
		CString strNode;
		strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\WndCount"), nDllIndex);
		AppXml()->SetAttributeInt(strNode, nPaneCount);
		AppXml()->FlushData();

		//写入所有的工程名是strDllname的，写入的都擦出掉
		POSITION pos = tempPaneMap.GetStartPosition();
		int nWndIndex=0;
		while(pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			tempPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if (NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CRect rcPane;
				pModulePane->GetWindowRect(&rcPane);

				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator itFind = mapCreatedWnds.find(strPane);
				if (itFind != mapCreatedWnds.end())
				{
					stCreateWndItem& oneCreated = itFind->second;
					if (strDllname.CompareNoCase(oneCreated.strDllname) != 0)
					{
						continue;
					}

					//write pane class name;
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\Wnd_%d\\Name"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, oneCreated.strClassName);

					//write pane rect
					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\Wnd_%d\\SIZE\\left"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.left);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\Wnd_%d\\SIZE\\right"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.right);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\Wnd_%d\\SIZE\\top"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.top);

					strNode.Format(_T("Dll_%d\\DOCK_GROUP\\BOTTOM\\Wnd_%d\\SIZE\\bottom"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.bottom);

					AppXml()->FlushData();


					tempPaneMap.RemoveKey(strWndname);
					nWndIndex++;
					if (nWndIndex >= nPaneCount)
					{
						break;
					}
				}
			}
		}
	}
}

void CMainFrame::RefreshFloatNodeXml()
{
	//[dllname,count]
	map<CString, UINT> mapDll2PaneCount;
	MapCreatedWnd mapCreatedWnds;
	if(CWndManager::Instance()->GetCreatedWnd(mapCreatedWnds))
	{
		POSITION pos = m_FloatPaneMap.GetStartPosition();
		while (pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			m_FloatPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if(NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator it2 = mapCreatedWnds.find(strPane);
				if (it2 != mapCreatedWnds.end())
				{
					map<CString, UINT>::iterator it3 = mapDll2PaneCount.find(it2->second.strDllname);
					if (it3 != mapDll2PaneCount.end())
					{
						UINT& nValue = it3->second;
						nValue++;
					}
					else
					{
						mapDll2PaneCount.insert(make_pair(it2->second.strDllname, 1));
					}
				}
			}
		}
	}

	//copy.
	CModulePaneMap tempPaneMap;
	POSITION pos = m_FloatPaneMap.GetStartPosition();
	while(pos)
	{
		CString strname;
		CModulePane* pPane = NULL;
		m_FloatPaneMap.GetNextAssoc(pos, strname, pPane);
		tempPaneMap.SetAt(strname, pPane);
	}

	for(map<CString, UINT>::iterator iterIdx = mapDll2PaneCount.begin(); 
		iterIdx != mapDll2PaneCount.end(); ++iterIdx)
	{
		CString strDllname = iterIdx->first;
		UINT nPaneCount = iterIdx->second;
		int nDllIndex = CXmlDataProc::Instance()->GetDllIndex(strDllname);
		CString strNode;
		strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\WndCount"), nDllIndex);
		AppXml()->SetAttributeInt(strNode, nPaneCount);
		AppXml()->FlushData();
		

		//写入所有的工程名是strDllname的，写入的都擦出掉
		POSITION pos = tempPaneMap.GetStartPosition();
		int nWndIndex=0;
		while(pos)
		{
			CString strWndname;
			CModulePane* pModulePane = NULL;
			tempPaneMap.GetNextAssoc(pos, strWndname, pModulePane);
			if (NULL != pModulePane && NULL != pModulePane->GetSafeHwnd())
			{
				CRect rcPane;
				pModulePane->GetWindowRect(&rcPane);

				CString strPane;
				strPane.Format(_T("0x%08x"), pModulePane->m_pWnd);
				MapCreatedWnd::iterator itFind = mapCreatedWnds.find(strPane);
				if (itFind != mapCreatedWnds.end())
				{
					stCreateWndItem& oneCreated = itFind->second;
					if (strDllname.CompareNoCase(oneCreated.strDllname) != 0)
					{
						continue;
					}

					//write pane class name;
					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\Name"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, oneCreated.strClassName);

					//Write pane name
					CString strPanename;
					pModulePane->GetPaneName(strPanename);
					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\WndName"), nDllIndex, nWndIndex);
					AppXml()->SetAttribute(strNode, strPanename);

					//write pane rect
					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\SIZE\\left"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.left);

					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\SIZE\\right"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.right);

					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\SIZE\\top"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.top);

					strNode.Format(_T("Dll_%d\\FLOAT_GROUP\\Wnd_%d\\SIZE\\bottom"),nDllIndex, nWndIndex);
					AppXml()->SetAttributeInt(strNode, rcPane.bottom);

					AppXml()->FlushData();


					tempPaneMap.RemoveKey(strWndname);
					nWndIndex++;
					if (nWndIndex >= nPaneCount)
					{
						break;
					}
				}
			}
		}
	}
}

CModulePane* pPrevPane=NULL;
LRESULT CMainFrame::OnUnregisterModulePane( WPARAM, LPARAM lp)
{
	LPCTSTR lpszWndName = (LPCTSTR)lp;

	// Get Pane from map
	BOOL bVertical;
	CModulePane* pModulePane=NULL;
	if( FindModulePane(lpszWndName, pModulePane, bVertical) )
	{
		RemovePaneFromMap(pModulePane, bVertical);
	}
	else
	{
		return -1;
	}


	if(pModulePane->IsAutoHideMode())
	{
		pModulePane->GetAutoHideToolBar()->RemoveAutoHideWindow(pModulePane);
	}

	if(pModulePane&&pModulePane->GetSafeHwnd())
	{
		pModulePane->UndockPane();
		pModulePane->SendMessage(WM_CLOSE,0,0);
	}

	return 0;   
}


BOOL CMainFrame::RemovePaneFromMap( CModulePane* pPane, BOOL& bVertical )
{
	ASSERT(pPane);
	CString strWnd;
	strWnd.Format(_T("0x%08x"), pPane->m_pWnd);

	if( m_LeftPaneMap.Lookup(/*pPane->m_strWndName*/strWnd, pPane) )
	{
		m_LeftPaneMap.RemoveKey(strWnd);
		bVertical = TRUE;
		return TRUE;
	}
	if( m_RightPaneMap.Lookup(/*pPane->m_strWndName*/strWnd, pPane) )
	{
		m_RightPaneMap.RemoveKey(strWnd);
		bVertical = TRUE;
		return TRUE;
	}
	if( m_TopPaneMap.Lookup(/*pPane->m_strWndName*/strWnd, pPane) )
	{
		m_TopPaneMap.RemoveKey(strWnd);
		bVertical = FALSE;
		return TRUE;
	}
	if( m_BottomPaneMap.Lookup(/*pPane->m_strWndName*/strWnd, pPane) )
	{
		m_BottomPaneMap.RemoveKey(strWnd);
		bVertical = FALSE;
		return TRUE;
	}
	if (m_FloatPaneMap.Lookup(strWnd, pPane))
	{
		m_BottomPaneMap.RemoveKey(strWnd);
		bVertical = FALSE;
		return TRUE;
	}


	/*if( m_VertPaneMap.Lookup(pPane->m_strWndName, pPane) )
	{
	m_VertPaneMap.RemoveKey(pPane->m_strWndName);
	bVertical = TRUE;
	return TRUE;
	}

	if( m_HoriPaneMap.Lookup(pPane->m_strWndName, pPane) )
	{
	m_HoriPaneMap.RemoveKey(pPane->m_strWndName);
	bVertical = FALSE;
	return TRUE;
	}*/

	return FALSE;
}

void CMainFrame::RemovePane( CDockablePane* pDockablePane )
{
	if( pDockablePane==NULL )
	{
		ASSERT(FALSE);
		return;
	}   

	CModulePane* pModulePane = dynamic_cast<CModulePane*>(pDockablePane);
	if(pModulePane)
	{
		BOOL bVertical;
		pModulePane->ShowPane(FALSE,FALSE,FALSE);
		pModulePane->UndockPane();
		RemovePaneFromMap(pModulePane, bVertical);
		RemovePaneFromDockManager((CBasePane*)pDockablePane, FALSE,TRUE,TRUE,NULL);
		pModulePane->SendMessage(WM_CLOSE);
	}
	else
	{
		RemovePaneFromDockManager((CBasePane*)pDockablePane, TRUE,TRUE,TRUE,NULL);//remove from manager
		pDockablePane->PostMessage(WM_CLOSE);
	}

	//UnregCmdMsg(pane);//unregister pane from command routing list

	AdjustDockingLayout();//recalc docking layout
	return;
}

BOOL CMainFrame::FindModulePane(CString strWndname, CModulePane*& pModulePane, BOOL& bVertical )
{
	CString strTemp;

	pModulePane=NULL;
	POSITION pos = m_LeftPaneMap.GetStartPosition();
	while( pos )
	{
		m_LeftPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
		if(strTemp.Compare(strWndname)==0)
		{
			bVertical = TRUE;
			return TRUE;
		}
	}
	while( pos )
	{
		m_RightPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
		if(strTemp.Compare(strWndname)==0)
		{
			bVertical = TRUE;
			return TRUE;
		}
	}
	while( pos )
	{
		m_TopPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
		if(strTemp.Compare(strWndname)==0)
		{
			bVertical = FALSE;
			return TRUE;
		}
	}
	while( pos )
	{
		m_BottomPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
		if(strTemp.Compare(strWndname)==0)
		{
			bVertical = FALSE;
			return TRUE;
		}
	}


// 	pos = m_HoriPaneMap.GetStartPosition();
// 	while( pos )
// 	{
// 		m_HoriPaneMap.GetNextAssoc(pos, strTemp, pModulePane);
// 		if(strTemp.Compare(strWndname)==0)
// 		{
// 			bVertical = FALSE;
// 			return TRUE;
// 		}
// 	}

	pModulePane = NULL;
	return FALSE;
}

BOOL CMainFrame::AttachPane(CModulePane* pPane, DWORD dwAlignment, BOOL bActivate, CDockablePane** pTabbedBar)
{
	CString strWindowName;
	CModulePane* pPrevPane= NULL;

	if (dwAlignment==ALIGN_LEFT || dwAlignment == ALIGN_LEFT_GROUP)
	{
		POSITION pos = m_LeftPaneMap.GetStartPosition();
		while(pos)
		{
			m_LeftPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
		}
	}
	if (dwAlignment==ALIGN_RIGHT || dwAlignment == ALIGN_RIGHT_GROUP)
	{
		POSITION pos = m_RightPaneMap.GetStartPosition();
		while(pos)
		{
			m_RightPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
		}
	}
	if (dwAlignment==ALIGN_TOP || dwAlignment == ALIGN_TOP_GROUP)
	{
		POSITION pos = m_TopPaneMap.GetStartPosition();
		while(pos)
		{
			m_TopPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
		}
	}
	if (dwAlignment==ALIGN_BOTTON || dwAlignment == ALIGN_BOTTOM_GROUP)
	{
		POSITION pos = m_BottomPaneMap.GetStartPosition();
		while(pos)
		{
			m_BottomPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
		}
	}

	//if( dwAlignment==ALIGN_LEFT || dwAlignment == ALIGN_RIGHT ||dwAlignment == ALIGN_VERTICAL/**/ )
	//{
	//	POSITION pos = m_VertPaneMap.GetStartPosition();
	//	while( pos )
	//	{
	//		m_VertPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
	//	}
	//}

	//if(pPrevPane==NULL && (dwAlignment==ALIGN_TOP || dwAlignment == ALIGN_BOTTON) ||dwAlignment == ALIGN_HORIZONTAL)
	//{
	//	POSITION pos = m_HoriPaneMap.GetStartPosition();
	//	while( pos )
	//	{
	//		m_HoriPaneMap.GetNextAssoc(pos, strWindowName, pPrevPane);
	//	}
	//}

	//当前是否有Pane跟当前待靠的pane是否是一个停靠类型，有就停靠到一组。
	if( pPrevPane && pPrevPane->GetSafeHwnd() )
	{
		BOOL bIsAutoHide = pPrevPane->IsAutoHideMode();
		if(bIsAutoHide)
		{
			CPaneDivider* pDefaultSlider = pPrevPane->GetDefaultPaneDivider();
			pPrevPane->SetAutoHideMode(FALSE, pDefaultSlider->GetCurrentAlignment());
		}

		pPane->AttachToTabWnd(pPrevPane, DM_SHOW, bActivate, pTabbedBar);

		if(bIsAutoHide && pTabbedBar && *pTabbedBar)
		{
			(*pTabbedBar)->SetAutoHideMode(TRUE, (*pTabbedBar)->GetCurrentAlignment());
		}

		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::OnClosePane(WPARAM, LPARAM lp)
{
	CDockablePane* pDockablePane = (CDockablePane*)lp;
	CMFCOutlookBar* pOutlookTabbedBar = dynamic_cast<CMFCOutlookBar*>(pDockablePane);
	CMFCBaseTabCtrl* pTabCtrl=NULL;
	if( pOutlookTabbedBar )
	{
		pTabCtrl = pOutlookTabbedBar->GetUnderlyingWindow();
	}
	else
	{ 
		HWND hWndTab;
		pTabCtrl = pDockablePane->GetParentTabWnd(hWndTab);
	}

	// Get DockablePane
	if(pTabCtrl && pTabCtrl->GetTabsNum()>0 )
	{
		int iTab = pTabCtrl->GetActiveTab();
		pDockablePane = dynamic_cast<CDockablePane*>(pTabCtrl->GetTabWnd(iTab));
	}



	CBaseTabbedPane* pTabbedBar = dynamic_cast<CBaseTabbedPane*>(pDockablePane);
	if(pTabbedBar)
	{
		//pTabbedBar->SetAutoHideMode(TRUE, pTabbedBar->GetCurrentAlignment());
		pTabbedBar->ShowPane(FALSE,FALSE,FALSE);
		return TRUE;
	}

	CModulePane* pModulePane = dynamic_cast<CModulePane*>(pDockablePane);
	if(pModulePane&&pModulePane->m_bAutoDelete)
	{
		if(!pDockablePane->IsAutoHideMode())
		{
			CBaseTabbedPane* pTabbedPane = pDockablePane->GetParentTabbedPane();
			if( pTabbedPane )
				pTabbedPane->SetAutoHideMode(TRUE, pTabbedPane->GetCurrentAlignment());
			else
				pDockablePane->SetAutoHideMode(TRUE, pDockablePane->GetCurrentAlignment());
		}

		GetDockingManager()->RecalcLayout();
		CString strWinText;
		pDockablePane->GetWindowText(strWinText);
		return TRUE;
	}


	// Remove from TabCtrl
	if(pTabCtrl && pTabCtrl->GetTabsNum()>0 )
	{
		int iTab = pTabCtrl->GetActiveTab();
		pDockablePane = dynamic_cast<CDockablePane*>(pTabCtrl->GetTabWnd(iTab));
		ASSERT(pDockablePane);
		pTabCtrl->RemoveTab(iTab);
	}


	// Remove From AutoHideToolBar
	pDockablePane->ShowPane(FALSE, FALSE, FALSE);
	if(pDockablePane->IsAutoHideMode())
		pDockablePane->GetAutoHideToolBar()->RemoveAutoHideWindow(pDockablePane);


	// Remove From DockingManager
	pDockablePane->UndockPane();


	// Remove From Map
	BOOL bVertical;
	RemovePaneFromMap(dynamic_cast<CModulePane*>(pDockablePane), bVertical);


	// Close Pane
	pDockablePane->PostMessage(WM_CLOSE);


	AdjustDockingLayout();
	return TRUE;
}

BOOL bChanging=FALSE;
LRESULT CMainFrame::OnChangingActiveTab( WPARAM wParam, LPARAM lParam)
{

#ifdef _DEBUG
	int iActiveTab = (int)wParam;
	CMFCBaseTabCtrl* pBaseTabCtrl = (CMFCBaseTabCtrl*)(lParam);
	CMFCOutlookBarTabCtrl* pOutlookTabCtrl = dynamic_cast<CMFCOutlookBarTabCtrl*>(pBaseTabCtrl);
	if(pOutlookTabCtrl)
	{
		int x = 0;
	}
	else
	{
		CWnd* pWnd = pBaseTabCtrl->GetTabWnd(iActiveTab);
		CMDIChildWndEx* pMDIChild = dynamic_cast<CMDIChildWndEx*>(pWnd);
		if(pMDIChild)
		{
			CString strWinText;
			pMDIChild->GetWindowText(strWinText);
		}
	}

	CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
#endif

	if(!bChanging)
	{
		LockWindowUpdate();
		bChanging=TRUE;
	}
	return 0;
}


LRESULT CMainFrame::OnChangeActiveTab( WPARAM wParam, LPARAM lParam)
{
	int iActiveTab = (int)wParam;
	CMFCBaseTabCtrl* pBaseTabCtrl = (CMFCBaseTabCtrl*)(lParam);
	CMFCTabCtrl* pTabCtrl = dynamic_cast<CMFCTabCtrl*>(pBaseTabCtrl);
	CMFCOutlookBarTabCtrl* pOutlookTabCtrl = dynamic_cast<CMFCOutlookBarTabCtrl*>(pBaseTabCtrl);

	if (pTabCtrl)
	{
		CWnd* pWnd = pTabCtrl->GetTabWnd(iActiveTab);
		CMDIChildWndEx* pMDIChild= dynamic_cast<CMDIChildWndEx*>(pWnd);
		if(pMDIChild)
		{
			CString strWinText;
			pMDIChild->GetWindowText(strWinText);
			m_pActiveView = pMDIChild->GetActiveView();
		}
		else
		{
			m_pActiveView = NULL;
		}
	}

	if(pOutlookTabCtrl)
	{
		CWnd* pWnd = pOutlookTabCtrl->GetTabWnd(iActiveTab);
		CDockablePane* pDockpane = dynamic_cast<CDockablePane*>(pWnd);
		CModulePane::SetActivePane(pDockpane);
	}
	else
	{
		CWnd* pWnd = pBaseTabCtrl->GetTabWnd(iActiveTab);
		CDockablePane* pDockpane = dynamic_cast<CDockablePane*>(pWnd);
		if(pDockpane)
			CModulePane::SetActivePane(pDockpane);
	}


	if(bChanging)
	{
		UnlockWindowUpdate();
		bChanging=FALSE;
	}

	return 0;
}


LRESULT CMainFrame::OnActivateTabbedView( WPARAM wp, LPARAM lp)
{ 
	for (POSITION pos = m_wndClientArea.GetMDITabGroups().GetHeadPosition(); pos != 0;)
	{
		CMFCTabCtrl* pNextWnd = DYNAMIC_DOWNCAST(CMFCTabCtrl, m_wndClientArea.GetMDITabGroups().GetNext(pos));
		int nTabsNum = pNextWnd->GetTabsNum();
		for(int iTab=0; iTab<nTabsNum; ++iTab)
		{
			CWnd* pWnd = pNextWnd->GetTabWnd(iTab);
			CMDIChildWnd* pChildFrame = dynamic_cast<CMDIChildWnd*>(pWnd);
			if(pChildFrame && pChildFrame==(CMDIChildWnd*)lp)
			{
				pNextWnd->SetActiveTab(iTab);
				CString strTitle;
				pChildFrame->GetWindowText(strTitle);
				return 0;
			}
		}
	}

	return -1;
}

LRESULT CMainFrame::OnPaneControl( WPARAM wp, LPARAM lp)
{
	MODULE_WINDOW_DEF::CONTROL_CODE code = (MODULE_WINDOW_DEF::CONTROL_CODE)wp;

	LPCTSTR lpszWindowName = (LPCTSTR)lp;

	if( ControlPane(lpszWindowName, code) )
		return 0;
	return -1;
}

BOOL CMainFrame::ControlPane(LPCTSTR lpPaneName, MODULE_WINDOW_DEF::CONTROL_CODE code)
{
	BOOL bVertical;
	CModulePane* pModulePane;
	if(! FindModulePane(lpPaneName, pModulePane, bVertical) )
	{
		return FALSE;
	}

	switch(code)
	{
	case MODULE_WINDOW_DEF::PANE_ACTIVATE:
		{
			if(pModulePane->IsAutoHideMode())
			{
				CPaneDivider* pDefaultSlider = pModulePane->GetDefaultPaneDivider();
				pModulePane->SetAutoHideMode(FALSE, pDefaultSlider->GetCurrentAlignment());
			}


			HWND hWndTab;
			CMFCBaseTabCtrl* pTabCtrl = pModulePane->GetParentTabWnd(hWndTab);
			if(pTabCtrl)
			{
				int nTabs = pTabCtrl->GetTabsNum();
				for(int i=0; i<nTabs; ++i)
				{
					if( pTabCtrl->GetTabWnd(i) == pModulePane )
					{
						pTabCtrl->SetActiveTab(i);
					}
				}
			}
			return TRUE;
		}
		break;

	case MODULE_WINDOW_DEF::PANE_SHOW:
		{
			pModulePane->ShowPane(TRUE,FALSE,TRUE);
			return TRUE;
		}
		break;

	case MODULE_WINDOW_DEF::PANE_HIDE:
		{
			if(!pModulePane->IsAutoHideMode() )
				pModulePane->ToggleAutoHide();
			return TRUE;
		}
		break;

	default:
		break;
	}

	return FALSE;
}

LRESULT CMainFrame::OnToolBarControl(WPARAM wp, LPARAM lp)
{
	TOOLBAR_DEF::CONTROL_CODE code = (TOOLBAR_DEF::CONTROL_CODE)lp;
	CMFCToolBar* pToolbar = (CMFCToolBar*)wp;

	switch(code)
	{
	case TOOLBAR_DEF::TOOLBAR_ADD:
		{
			pToolbar->SetOwner(this);
			pToolbar->SetParent(this);
			pToolbar->EnableDocking(CBRS_ALIGN_TOP);
			DockPane(pToolbar, AFX_IDW_DOCKBAR_TOP);
			DockPaneLeftOf(pToolbar, &m_wndToolBar);
			DockPaneLeftOf(&m_wndToolBar, pToolbar);
			m_ToolbarList.AddTail(pToolbar);
			return 0;
		}
		break;

	case TOOLBAR_DEF::TOOLBAR_REMOVE:
		{
			POSITION pos = m_ToolbarList.Find(pToolbar);
			if(pos)
			{
				pToolbar->UndockPane();
				m_ToolbarList.RemoveAt(pos);
				return 0;
			}
		}
		break;

	case TOOLBAR_DEF::TOOLBAR_SHOW:
		{
			POSITION pos = m_ToolbarList.Find(pToolbar);
			if(pos)
			{
				if( !pToolbar->IsVisible())
					ShowPane(pToolbar, TRUE, TRUE, TRUE);
				DockPaneLeftOf(pToolbar, &m_wndToolBar);
				DockPaneLeftOf(&m_wndToolBar, pToolbar);
				return 0;
			}
		}
		break;

	case TOOLBAR_DEF::TOOLBAR_HIDE:
		{
			POSITION pos = m_ToolbarList.Find(pToolbar);
			if(pos)
			{
				if( pToolbar->IsVisible())
					ShowPane(pToolbar, FALSE, TRUE, TRUE);
				return 0;
			}
		}
		break;

	default:
		break;

	}

	return -1;
}

LRESULT CMainFrame::OnGetWndInActivePane( WPARAM, LPARAM )
{
	CWnd* pWnd = CModulePane::GetWndInActivePane();
	return (LRESULT)pWnd;
}

LRESULT CMainFrame::OnOpenViewer(WPARAM, LPARAM lp)
{
	/*CMsgOpenViewer* pMsg = (CMsgOpenViewer*)lp;

	if( OpenModuleByName(_T("LogViewer"), false) )
	{
	m_pCurrentUserLogViewFrame = m_pViewLastCreated->GetParentFrame();
	}
	else
	{
	CString strError;
	strError.Format(_T("Open/Activate Log Viewer failed"));
	AfxMessageBox(strError);
	}

	pMsg->DecRef();*/
	return 0;
}

void CMainFrame::OnViewCaptionBar()
{
	//m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	//RecalcLayout(FALSE);
}
void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	//pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}
LRESULT CMainFrame::OnRecordOpenTabbedViews(WPARAM, LPARAM)
{
	this->EnumTabbedView();
	return 0;
}

CView* CMainFrame::GetActiveView()
{
	CMFCTabCtrl* pTabCtrl = m_wndClientArea.FindActiveTabWnd();
	if(pTabCtrl)
	{
		int nActive = pTabCtrl->GetActiveTab();
		CWnd * pWnd = pTabCtrl->GetTabWndNoWrapper( nActive );
		CMDIChildWndEx * pChild = dynamic_cast<CMDIChildWndEx*>(pWnd);
		if(pChild)
		{
			CView* pView = pChild->GetActiveView();
			return pView;
		}
	}

	return NULL;
}

void CMainFrame::SplitTabWnds( CMFCTabCtrl* pTabCtrl, BOOL bVert, int nGroupsNum/*=1*/, int* pGroupsNum/*=NULL*/ )
{
	const int nTabNum = pTabCtrl->GetTabsNum();

	if(pGroupsNum==NULL)
	{
		pGroupsNum = new int[2];
		nGroupsNum = 2;
		pGroupsNum[1] = nTabNum/2;
		pGroupsNum[0] = nTabNum - pGroupsNum[1];
	}

	int iTab = 0;
	map<int, vector<CWnd*>> mapTabWnds;
	for(int i=0; i<nGroupsNum; ++i)
	{
		vector<CWnd*>& vecWnds = mapTabWnds[i];
		for(int n=0; n<pGroupsNum[i] && iTab<nTabNum; ++n)
		{
			vecWnds.push_back(pTabCtrl->GetTabWnd(iTab));
			++iTab;
		}
	}


	if( nTabNum>1 )
	{
		int iMapSize = (int)mapTabWnds.size();
		for(int iGroup=iMapSize-1; iGroup>0; --iGroup)
		{
			vector<CWnd*>& vecWnds = mapTabWnds[iGroup];
			if(vecWnds.size()==0)
				continue;

			int iTab = -1;
			for(size_t i=0; i<vecWnds.size(); ++i)
			{
				CWnd* pTab = vecWnds[i];
				iTab = pTabCtrl->GetTabFromHwnd(pTab->GetSafeHwnd());
				if(iTab!=-1)
				{
					vecWnds[i] = NULL;
					break;
				}
			}

			if(iTab==-1)
				continue;

			pTabCtrl->SetActiveTab(iTab);
			MDITabNewGroup(bVert);
			vector<CMFCTabCtrl*> vecTabCtrls;
			int nTempGroupNum = GetMDITabCtrls(vecTabCtrls);
			CMFCTabCtrl* pTabFrom = vecTabCtrls.at(0);
			CMFCTabCtrl* pTabTo = vecTabCtrls.at(1);

			for(size_t i=0; i<vecWnds.size(); ++i)
			{
				CWnd* pTab = vecWnds[i];
				if(pTab==NULL)
					continue;

				int iTab = pTabFrom->GetTabFromHwnd(pTab->GetSafeHwnd());
				if(iTab==-1)
					continue;

				m_wndClientArea.MoveWindowToTabGroup(pTabFrom, pTabTo, iTab);
			}
		}
	}
}


CMFCTabCtrl* CMainFrame::MergeTabWnds()
{
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);

	if(nGroupNum==0) return NULL;
	if(nGroupNum==1) return vecTabCtrls.at(0);


	CMFCTabCtrl* pFirstTab = vecTabCtrls.at(0);
	CRect rc;
	m_wndClientArea.GetClientRect(&rc);
	pFirstTab->MoveWindow(&rc);


	for(int i=1; i<nGroupNum; ++i)
	{
		CMFCTabCtrl* pTabRemove = vecTabCtrls.at(i);
		while(pTabRemove->GetTabsNum())
		{
			m_wndClientArea.MoveWindowToTabGroup(pTabRemove, pFirstTab, 0);
		}
		m_wndClientArea.RemoveTabGroup(pTabRemove, FALSE);
	}

	return pFirstTab;
}

void CMainFrame::TileWindow(BOOL bVert)
{
	vector<CMFCTabCtrl*> vecTabCtrls;
	const int nGroupNum = GetMDITabCtrls(vecTabCtrls);

	if( nGroupNum==1 )
	{
		SplitTabWnds(vecTabCtrls.at(0), bVert);
	}

	if( nGroupNum>1 )
	{
		vector<int> vecTabsNum;
		for(int i=0; i<nGroupNum; ++i)
		{
			CMFCTabCtrl* pTabCtrl = vecTabCtrls.at(i);
			vecTabsNum.push_back((int)pTabCtrl->GetTabsNum());
		}

		CMFCTabCtrl* pFirstTab = MergeTabWnds();
		SplitTabWnds(pFirstTab, bVert, (int)vecTabsNum.size(), &vecTabsNum[0]);
	}
}

void CMainFrame::ResetWorkspaceNode()
{
	CString strTemp;
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
		return;

	xml_document<TCHAR>* pDoc = pXml->GetDocument();

	//MakeNode <Workspace>
	pXml->RemoveNode(_T("Workspace"));
	pXml->SetAttributeBool(_T("Workspace"), false);
}

int CMainFrame::GetMDITabCtrlsNum()
{
	int nNum = 0;
	const CObList& obList = GetMDITabGroups();
	POSITION pos = obList.GetHeadPosition();
	while(pos)
	{
		const CObject* pOb = obList.GetNext(pos);
		CMFCTabCtrl* pTab = DYNAMIC_DOWNCAST(CMFCTabCtrl, pOb);
		if(pTab)
		{
			++nNum;
		}
	}

	return nNum;
}

void CMainFrame::EnumDockablePane()
{
	CString strTemp;
	CXmlConfig* pXml = CXmlConfig::Instance();
	if(pXml==NULL)
		return;

	xml_document<TCHAR>* pDoc = pXml->GetDocument();
	pXml->SetAttributeBool(_T("Workspace\\DockablePane"), false);
	xml_node<TCHAR>* pNodeDockPane = pXml->FindChild(_T("Workspace\\DockablePane"));
	if(!pNodeDockPane)
		return;

	// Get exported function GetPanesWorkspace

	for (POSITION pos  = m_AllCommands.GetHeadPosition(); pos !=NULL; )
	{
		LPFN_DLLGETPANESWORKSPACE pFunc = NULL;
		CMenuCommand &mc = m_AllCommands.GetNext(pos);
		if(mc.hLib && mc.m_bInitialized && !mc.m_bUtility )
		{
			pFunc = (LPFN_DLLINITPANEBYWORKSPACE)GetProcAddress(mc.hLib, "GetPanesWorkspace");
		}

		if(pFunc)
		{
			//pXml->LockToWrite();
			TCHAR* pchName = pDoc->allocate_string(mc.m_strTitle);
			pNodeDockPane->set_node(pchName);
			xml_node<TCHAR>* pNodeModule = pNodeDockPane->first_node(mc.m_strTitle);
			ASSERT(pNodeModule);
			BOOL bRet=FALSE;
			if(pNodeModule)
			{
				bRet = pFunc(pNodeModule);
				if(!bRet)
				{
					pNodeDockPane->remove_node(pNodeModule);
				}
			}
			else
			{
				CString strError;
				strError.Format(_T("%s GetPanesWorkspace return FALSE"), mc.m_strTitle);
				AfxMessageBox(strError);
			}

			//pXml->UnLock();
		}
	} // for each Module

	pXml->FlushData();
	return;
}

void CMainFrame::OnShowWndConfigDlg()
{
	if (NULL != m_pDlgConfigWnd && NULL != m_pDlgConfigWnd->GetSafeHwnd())
	{
		m_pDlgConfigWnd->ShowWindow(SW_SHOW);
	}
}



void CMainFrame::LoadDllByName(CString& strDllname)
{
	int index = strDllname.Find(_T(".dll"));
	CString strModule = strDllname.Left(index);
	
	OpenModuleByName(strModule);
	
	//MDITabNewGroup();
}
		
