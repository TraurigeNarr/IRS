#include "stdafx.h"

#include "MousePoints.h"

#include "Database.h"
#include "Grid.h"
#include "IMode.h"
#include "MainFrm.h"
#include "Renderer.h"
#include "RendererFrame.h"
#include "TabController.h"
#include "Vertex.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////////

MousePoints::MousePoints()
  : IMouse(MOUSE_POINTS)
{
  m_cursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(ID_CURSOR_POINTS));
}

MousePoints::~MousePoints()
{

}

void MousePoints::OnActivate()
{

}

void MousePoints::OnDeactivate()
{

}

void MousePoints::MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam)
{
  CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

  if(nullptr == render_frame || !m_active)
    return;

  int xPos = GET_X_LPARAM(lParam); 
  int yPos = GET_Y_LPARAM(lParam);

  CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
  ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();

  Vertex* v = new Vertex (IRS::DatabaseInstance->GetGrid()->GetPoint(xPos, yPos), mode_info.GetNextPointIndex());

  IRS::DatabaseInstance->RegisterItem(v);
  render_frame->Invalidate();
}