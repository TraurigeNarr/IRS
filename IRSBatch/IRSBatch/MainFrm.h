
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "RendererFrame.h"
#include "TabController.h"

class CMainFrame : public CFrameWndEx
{
private:
  CRendererFrame* mp_render_frame;
  CTabController* mp_tab_controller;

private:
  void _InitializeControls();

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

  CRendererFrame* GetRendererFrame();
  CTabController* GetController();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnModeSwitch(UINT i_id);
  afx_msg void OnMouseSwitch(UINT i_id);
  afx_msg void OnSettings();
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	DECLARE_MESSAGE_MAP()

};


