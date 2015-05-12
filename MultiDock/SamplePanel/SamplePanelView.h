// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// SamplePanelView.h : interface of the CSamplePanelView class
//

#pragma once

class CSamplePanelDoc;
class CSamplePanelView : public CView
{
	DECLARE_DYNCREATE(CSamplePanelView)

protected: // create from serialization only
	CSamplePanelView();
	virtual ~CSamplePanelView();
	

// Attributes
public:
	CSamplePanelDoc* GetDocument() const;


public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	

public:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnResponseMenuItem1();
	afx_msg void OnResponseMenuItem2();
	afx_msg void OnToolButton1();
	afx_msg void OnToolButton2();
	afx_msg void OnToolButton3();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SamplePanelView.cpp
inline CSamplePanelDoc* CSamplePanelView::GetDocument() const
   { return reinterpret_cast<CSamplePanelDoc*>(m_pDocument); }
#endif

