#pragma once


// CSampleViewerFrame

class CSampleViewerFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CSampleViewerFrame)

public:
	CSampleViewerFrame();
	virtual ~CSampleViewerFrame();
	virtual void PostNcDestroy();

protected:
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
   afx_msg void OnClose();
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CMFCStatusBar m_wndStatusBar;
};


