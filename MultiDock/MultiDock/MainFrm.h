
#pragma once
#include "FileView.h"
#include "BaseDlg.h"
#include <vector>
#include <string>
#include "CommDefine.h"
#include "Workspace.h"
#include "..\Common\ModuleDefs.h"


using namespace std;

struct stBasePane
{
	BOOL    bShow;
	CString strPaneTitle;
	EDLGTYPE eType;
	CBaseDlg* pBaseDlg;

	stBasePane()
	{
		bShow = TRUE;
		pBaseDlg = NULL;
	}
};
typedef vector<stBasePane> VecBasePanes;

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const 
	{ 
		BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); 
		if (!bNameValid) strName.Empty(); 
	}
};

//////////////////////////////////////////////////////////////////////////

#define  MAX_NUM_MODULES		   (300)
#define  MAX_NUM_UTILITES		   (300)
#define  MAX_NUM_VIEW_TYPES		   (300)

#define  BASE_MODULES_MENU_ID	   (34000)
#define  BASE_UTILITIES_MENU_ID	(34000 + (MAX_NUM_MODULES*2))
#define  BASE_VIEWS_MENU_ID		(BASE_MODULES_MENU_ID + MAX_NUM_MODULES)


#define WM_INIT_MODULES					WM_USER+400
#define WM_RECORD_OPEN_TABBED_VIEWS		WM_USER+504
#define WM_OPEN_VIEWER					WM_USER+505

class CModulePane;
class CWndConfigDlg;
typedef CMap<CString, LPCTSTR, CModulePane*, CModulePane*> CModulePaneMap;
typedef CList<CMFCToolBar*, CMFCToolBar*>  CToolBarList;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();


public:
	void LoadModuleMenuItems();
	void OpenStartupModules();
	void OpenLastUsedModules();
	BOOL StartupAsWorkspace();
	void RecalcLayoutEx();

	//test
	void CreateDockWnd(LPCTSTR lpDllName, LPCTSTR lpClassName, EPANE_ALIGNMENT etype);
	void LoadDllByName(CString& strDllname);
	
protected:
	void CacheMenus();
	void AddModuleMenuItem(const CString &strModuleName, int nID, BOOL bIsView /*=FALSE*/, BOOL bIsUtility /*=FALSE*/);
	BOOL OpenModuleByName(CString strModuleName, bool onStartup = true);
	int  GetMDITabCtrls(vector<CMFCTabCtrl*>& vecTabCtrls);
	BOOL OpenModule(UINT nID, bool onStartup = true);
	void EnumTabbedView();
	const CMenuCommand* GetMenuCommand(LPCTSTR lpszModuleName);
	int  FindMenuItem(CMenu *pMenu, const CString &str);
	BOOL FindModulePane(CString strWndname, CModulePane*& pModulePane, BOOL& bVertical);
	BOOL AttachPane(CModulePane* pPane, DWORD dwAlignment, BOOL bActivate=TRUE, CDockablePane** pTabbedBar=NULL);
	BOOL ControlPane(LPCTSTR lpPaneName, MODULE_WINDOW_DEF::CONTROL_CODE code);

	BOOL OpenUtility(UINT nID);
	BOOL OpenUtilityByName(CString strUtilName);
	void RemovePane(CDockablePane* pDockablePane);
	BOOL RemovePaneFromMap(CModulePane* pPane, BOOL& bVertical);
	BOOL CreateChildProcess(CString strAppPath, CString strCmdLine);
	BOOL LaunchProcess(PROCESS_INFORMATION& pi, LPCTSTR szModulePath, LPCTSTR szCmdline);
	BOOL SafeCloseModules();
	void FreeModules();

	CView* GetActiveView();
	void SplitTabWnds(CMFCTabCtrl* pTabCtrl, BOOL bVert, int nGroupsNum=1, int* pGroupsNum=NULL);
	void TileWindow(BOOL bVert);
	CMFCTabCtrl* MergeTabWnds();
	int GetMDITabCtrlsNum();
	void EnumDockablePane();
	void ResetWorkspaceNode();

public:
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, 
		DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, 
		CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual void PostNcDestroy();

	class CMyMDIClientAreaWnd : public CMDIClientAreaWnd
	{
	public:
		BOOL IsAlign() const {return m_groupAlignment==0? FALSE:TRUE;};
		BOOL IsVertAlign() const {return m_groupAlignment==1? TRUE:FALSE; }
		BOOL IsHorzAlign() const {return m_groupAlignment==2? TRUE:FALSE; }
		int  GetAlignment() const {return m_groupAlignment;}
	};

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: 
	static CString	  m_strModuleMenuItems[2];
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	//CMFCRibbonBar     m_wndRibbonBar;
	//CFileView         m_wndFileView;
	COutlookBar       m_wndNavigationBar;
	VecBasePanes	  m_vecUserDlgs; 

	CMenuCommandList  m_AllCommands;
	CView*            m_pViewLastCreated;
	 CView*            m_pActiveView;
	map<CView*, CString> m_mapViewRegister;
	CWorkspace        m_workSpace;
	CModulePaneMap    m_HoriPaneMap;
	CModulePaneMap    m_VertPaneMap;
	CToolBarList      m_ToolbarList;

	CWndConfigDlg*    m_pDlgConfigWnd;

protected:
	afx_msg LRESULT OnInitModulePanes(WPARAM W, LPARAM L);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRibbonPane1Cmd1();
	afx_msg void OnRibbonPane1Cmd2();
	afx_msg void OnRibbonPane1Cmd3();
	afx_msg void OnRibbonPane1Cmd4();
	afx_msg void OnRibbonPane2Cmd1();
	afx_msg void OnRibbonPane2Cmd2();
	afx_msg void OnRibbonPane2Cmd3();
	afx_msg void OnRibbonPane2Cmd4();
	//Menu handlers
	afx_msg void OnShowWndConfigDlg();

	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnClose();
	afx_msg LRESULT OnResetMenu(WPARAM,LPARAM);
	afx_msg LRESULT OnChangeActiveTab(WPARAM,LPARAM);
	afx_msg LRESULT OnChangingActiveTab(WPARAM,LPARAM);
	afx_msg LRESULT OnClosePane(WPARAM,LPARAM);
	afx_msg LRESULT OnActivateTabbedView(WPARAM, LPARAM);
	//afx_msg LRESULT OnGetTabTooltip(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMainMenu(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMainMenuBar(WPARAM, LPARAM);
	afx_msg LRESULT OnRegisterModulePane(WPARAM, LPARAM);
	afx_msg LRESULT OnUnregisterModulePane(WPARAM, LPARAM);
	afx_msg LRESULT OnPaneControl(WPARAM, LPARAM);
	afx_msg LRESULT OnToolBarControl(WPARAM, LPARAM);
	afx_msg LRESULT OnActivateFrame(WPARAM, LPARAM);
	afx_msg LRESULT OnRecordOpenTabbedViews(WPARAM, LPARAM);
	afx_msg LRESULT OnGetWndInActivePane(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenViewer(WPARAM,LPARAM);

	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnModuleMenuItem(UINT nID);
	afx_msg void OnUtilityMenuItem(UINT nID);
	//afx_msg void OnViewMenuItem(UINT nID);
	//afx_msg void OnUpdateViewMenuItem(CCmdUI *pCmdUI);
	
	
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth);
	BOOL CreateFileView();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTestCreateSampleviewer();
	afx_msg void OnTestCreate4childwndFrompanelproj();
};


