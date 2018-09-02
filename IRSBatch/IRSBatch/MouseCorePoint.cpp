#include "stdafx.h"

#include "MouseCorePoint.h"

#include "ClassHandlerRenderer.h"
#include "Database.h"
#include "Grid.h"
#include "IMode.h"
#include "MainFrm.h"
#include "ModeInformation.h"
#include "Renderer.h"
#include "RendererFrame.h"
#include "TabController.h"
#include "Vertex.h"
#include "NotificationManager.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////////

MouseCorePoint::MouseCorePoint()
  : IMouse(MOUSE_CORE_POINT)
{
  m_cursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(ID_CURSOR_CORE_POINT));
}

MouseCorePoint::~MouseCorePoint()
{

}

void MouseCorePoint::OnActivate()
{
  
}

void MouseCorePoint::OnDeactivate()
{

}

void MouseCorePoint::MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam)
{
  CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

  if(nullptr == render_frame || !m_active)
    return;

  int xPos = GET_X_LPARAM(lParam); 
  int yPos = GET_Y_LPARAM(lParam);

  CMainFrame* pMainWnd = (CMainFrame *)AfxGetMainWnd();
  ModeInformation& mode_info = pMainWnd->GetController()->GetCurrentMode()->GetInformation();

  Vertex* v = new Vertex (IRS::DatabaseInstance->GetGrid()->GetPoint(xPos, yPos), mode_info.GetNextPointIndex());
  
  ClassHandlerRenderer* cl = new ClassHandlerRenderer(mode_info.GetNextClusterName());
  cl->AddCorePoint(v);

  int color = mode_info.GetNextColorRGBA();
  cl->SetColor(color);
  v->SetColor(color);

  IRS::DatabaseInstance->RegisterItem(cl);
  NotificationManager::GetInstance()->SendNotification(new ClassAdded(cl));
  render_frame->Invalidate();

}