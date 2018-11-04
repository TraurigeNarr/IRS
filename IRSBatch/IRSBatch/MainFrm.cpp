
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "IRSBatch.h"

#include "MainFrm.h"

#include "IMode.h"
#include "ModeInformation.h"
#include "MouseController.h"
#include "Renderer.h"
#include "RendererFrame.h"
#include "SettingsDialog.h"
#include "TabController.h"

#include <ctime>        // std::time

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND_RANGE(ID_MODE_SIMPLEMODE, ID_MODE_RECOGNITIONMODE, &CMainFrame::OnModeSwitch)
	ON_COMMAND_RANGE(ID_SETTINGS_MODEMOUSE, ID_SETTINGS_MOUSEBRUSH, &CMainFrame::OnMouseSwitch)
	ON_COMMAND(ID_SETTINGS_SETTINGS, &CMainFrame::OnSettings)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
  : mp_tab_controller(nullptr)
  , mp_render_frame(nullptr)
{
	std::srand ( unsigned ( std::time(0) ) );
}

CMainFrame::~CMainFrame()
{
  if (nullptr != mp_tab_controller)
    delete mp_tab_controller;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
  
  _InitializeControls();
	return 0;
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
  {
  // if the window is initialized then use the computed values
  if (GetSafeHwnd() != NULL)
    {
    lpMMI->ptMinTrackSize.x = 485;
    lpMMI->ptMinTrackSize.y = 280;
    }
  // else let the default handler take in
  else
    CFrameWndEx::OnGetMinMaxInfo(lpMMI);
  }

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
  if (nullptr == mp_tab_controller)
    return;
  
  CRect window_rc;
  CRect rc_tab_ctrl;  
  GetClientRect(&window_rc);

  mp_tab_controller->GetWindowRect(&rc_tab_ctrl);
  ScreenToClient(&rc_tab_ctrl);
  CRect rc = window_rc;
  //calculate render window size
  rc.left = 5;
  rc.top = 5;
  rc.right = rc.left + window_rc.Width()*0.75;
  rc.bottom -= 5;
  //move render window
  mp_render_frame->MoveWindow(rc);
  //move tab controller
  rc_tab_ctrl.left = rc.right + 10;
  rc_tab_ctrl.right = rc_tab_ctrl.left + window_rc.Width()*0.25 - 20;
  rc_tab_ctrl.bottom = rc.bottom;
  mp_tab_controller->MoveWindow(rc_tab_ctrl);
  CFrameWndEx::OnSize(nType, cx, cy);
}

void CMainFrame::_InitializeControls()
{
  CString class_name = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
    ::LoadCursor(NULL, IDC_ARROW), (HBRUSH) ::GetStockObject(WHITE_BRUSH),
    ::LoadIcon(NULL, IDI_APPLICATION));
  mp_render_frame = new CRendererFrame;
  if (!mp_render_frame->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, IDD_RENDERER_VIEW))
    TRACE0("Failed to create render frame\n");
  mp_render_frame->ShowWindow(SW_SHOW);
  mp_render_frame->MoveWindow(CRect(5,25,600,600));
  mp_render_frame->UpdateWindow();

  CString strOutputWnd;
  mp_tab_controller = new CTabController;
  DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
  class_name = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
    ::LoadCursor(NULL, IDC_ARROW), (HBRUSH) ::GetStockObject(WHITE_BRUSH),
    ::LoadIcon(NULL, IDI_APPLICATION));
  if (!mp_tab_controller->Create(class_name, dwStyle, CRect(610, 5, 890, 600), this, IDC_TAB_CONTROLLER))
    TRACE0("Failed to create Output window\n");

  DockPane(mp_tab_controller, AFX_IDW_DOCKBAR_RIGHT);
  mp_tab_controller->Initialize();
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CMainFrame::OnModeSwitch(UINT i_id)
{
	ASSERT(mp_tab_controller);
	if (!mp_tab_controller->IsVisible())
		mp_tab_controller->ShowPane(TRUE, FALSE, TRUE);

	switch (i_id)
	{
	case ID_MODE_SIMPLEMODE:
		mp_tab_controller->SetMode(MT_SIMPLE);
		break;
	case ID_MODE_CLUSTERING:
		mp_tab_controller->SetMode(MT_CLUSTERING);
		break;
	case ID_MODE_RECOGNITIONMODE:
		mp_tab_controller->SetMode(MT_RECOGNITION);
		break;
	default:
		ASSERT("WE SHOULD NOT BE HERE");
		break;
	}
}

void CMainFrame::OnMouseSwitch(UINT i_id)
{
	CMenu* p_menu = GetMenu();

	for (int i = ID_SETTINGS_MODEMOUSE; i <= ID_SETTINGS_MOUSEBRUSH; ++i)
	{
		p_menu->CheckMenuItem(i, MF_UNCHECKED);
	}

	switch (i_id)
	{
	case ID_SETTINGS_MODEMOUSE:
		MouseControllerInstance->SetMouse(mp_tab_controller->GetCurrentMode()->GetInformation().GetMouseType());
		p_menu->CheckMenuItem(i_id, MF_CHECKED);
		break;
	case ID_SETTINGS_SELECTIONMOUSE:
		MouseControllerInstance->SetMouse(MOUSE_SIMPLE);
		p_menu->CheckMenuItem(i_id, MF_CHECKED);
		break;
	case ID_SETTINGS_MOUSEBRUSH:
		MouseControllerInstance->SetMouse(MOUSE_BRUSH);
		p_menu->CheckMenuItem(i_id, MF_CHECKED);
		break;
	}
}

void CMainFrame::OnSettings()
  {
  CSettingsDialog settings_dlg(mp_tab_controller->GetCurrentMode(), this);

  if (IDOK == settings_dlg.DoModal())
    mp_render_frame->Invalidate();
  }

CRendererFrame* CMainFrame::GetRendererFrame()
{
  return mp_render_frame;
}

CTabController* CMainFrame::GetController()
{
  return mp_tab_controller;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

