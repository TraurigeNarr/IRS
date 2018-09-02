#include "stdafx.h"

#include "SimpleMouse.h"

#include "Database.h"
#include "Grid.h"
#include "MainFrm.h"
#include "NotificationManager.h"
#include "Notifications.h"
#include "Renderer.h"
#include "RendererFrame.h"
#include "TabController.h"
#include "Utilities.h"

#include "resource.h"

//////////////////////////////////////////////////////////////////////////

SelectionMouse::SelectionMouse()
  : IMouse(MOUSE_SIMPLE)
{
  m_cursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(ID_CURSOR_SIMPLE));
}

SelectionMouse::~SelectionMouse()
{

}

void SelectionMouse::OnActivate()
{

}

void SelectionMouse::OnDeactivate()
{

}

void SelectionMouse::MouseClicked(CView* ip_view, WPARAM wParam, LPARAM lParam)
{
  CRendererFrame* render_frame = dynamic_cast<CRendererFrame*>(ip_view);

  if (render_frame == nullptr || !m_active)
    return;

  int xPos = GET_X_LPARAM(lParam); 
  int yPos = GET_Y_LPARAM(lParam);

  Vector3D point = IRS::DatabaseInstance->GetGrid()->GetPoint(xPos, yPos);
  IRenderable* item = IRS::GetNearestItem(point);

  if (item == nullptr)
    return;

  if (!item->IsSelected())
  {
    item->Select();
    NotificationManager::GetInstance()->SendNotification(new ItemSelectionChange(item, ItemSelectionChange::IS_SELECTED));
  }
  else
  {
    item->Deselect();
    NotificationManager::GetInstance()->SendNotification(new ItemSelectionChange(item, ItemSelectionChange::IS_DESELECTED));
  }
}