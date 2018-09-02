#include "stdafx.h"

#include "PointsPage.h"

#include "ClassHandlerRenderer.h"
#include "ClusterRenderer.h"
#include "Database.h"
#include "DivisionFunctionsRenderer.h"
#include "EditPoint.h"
#include "EditPointDialog.h"
#include "IRenderable.h"
#include "ISelectable.h"
#include "NotificationManager.h"
#include "Notifications.h"
#include "UI_Utilities.h"
#include "Vertex.h"

//////////////////////////////////////////////////////////////////////////

class PointsPage::PointsPageListener : public INotificationHandler
  {
  private:
    PointsPage* mp_owner;
  public:
    PointsPageListener(PointsPage* ip_owner)
      : mp_owner(ip_owner)
      {      
      NotificationManager::GetInstance()->RegisterHandler(this);
      }
    ~PointsPageListener()
      {
      NotificationManager::GetInstance()->UnregisterHandler(this);
      }

    virtual void HandleNotification(INotificationData* ip_data) override
      {
      mp_owner->HandleNotification(ip_data);
      }
  };

//////////////////////////////////////////////////////////////////////////

PointsPage::PointsPage(IMode* ip_owner, CWnd* pParent /* = NULL */)
  : CDialogEx(PointsPage::IDD, pParent)
  , mp_owner(ip_owner)
  , mp_prev_selected(nullptr)
  , mp_listener_impl(new PointsPageListener(this))
{

}

PointsPage::~PointsPage()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(PointsPage, CDialogEx)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(IDC_CLEAR, OnClear)
  ON_COMMAND(IDC_EDIT_POINTS, OnEditPoints)
  ON_NOTIFY(NM_DBLCLK, IDC_ITEMS_TREE, OnPointDblClick)
  ON_NOTIFY(TVN_SELCHANGED , IDC_ITEMS_TREE, OnSelectionChanged)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void PointsPage::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ITEMS_TREE, m_items_list);
}

int PointsPage::OnCreate(LPCREATESTRUCT lpsc)
{
  if (CDialogEx::OnCreate(lpsc) == -1)
    return -1;

  return 0;
}

BOOL PointsPage::OnInitDialog()
{
  if (CDialogEx::OnInitDialog() != TRUE)
    return FALSE;

  return TRUE;
}

void PointsPage::OnSize(UINT nType, int cx, int cy)
{
  CDialogEx::OnSize(nType, cx, cy);
  
  CWnd* item = nullptr;
  CRect rc;

  item = GetDlgItem(IDC_ITEMS_TREE);
  if (item != nullptr)
    {
    item->GetWindowRect(&rc);
    ScreenToClient(&rc);

    rc.right = rc.left + cx - 15;
    rc.bottom = rc.top + cy;

    item->MoveWindow(rc);
    }
}

void PointsPage::OnClear()
{
  IRS::DatabaseInstance->Clear();
}

void PointsPage::AddItem(IRenderable* ip_item)
{
  if (m_items_list.m_hWnd == nullptr)
    return;

  if (ClusterRenderer* p_cluster = dynamic_cast<ClusterRenderer*>(ip_item))
    UI_Utilities::AddCluster(&m_items_list, p_cluster);
  else if (Vertex* p_vertex = dynamic_cast<Vertex*>(ip_item))
    UI_Utilities::AddVertex(&m_items_list, p_vertex);
}

void PointsPage::FullUpdate()
{
  if (m_items_list.m_hWnd == nullptr)
    return;

  UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
}

void PointsPage::DeleteAllItems()
{
  if (m_items_list.m_hWnd == nullptr)
    return;

  m_items_list.DeleteAllItems();
}

void PointsPage::OnPointDblClick(NMHDR* ip_nmhdr, LRESULT* op_result)
{
  HTREEITEM treeitem = m_items_list.GetSelectedItem();
  IRenderable* item = (IRenderable*)m_items_list.GetItemData(treeitem);

  if (nullptr == dynamic_cast<Vertex*>(item))
    return;

  CEditPoint edit_point;
  edit_point.SetPosition(item->GetPosition());
  
  if (edit_point.DoModal() == IDOK)
  {
    mp_listener_impl->StopListen();
    Vector3D vec = edit_point.GetPosition();
    item->SetPosition(vec[0], vec[1], vec[2]);
    std::vector<Cluster*> clusters = IRS::DatabaseInstance->GetItems<Cluster>();
    //clear clusters
    for (size_t i = 0; i < clusters.size(); ++i)
    {
      ASSERT(dynamic_cast<ClusterRenderer*>(clusters[i]));
      IRS::DatabaseInstance->UnregisterItem(dynamic_cast<ClusterRenderer*>(clusters[i]));
    }
    IRS::DatabaseInstance->Update();
    //not efficient but...
    UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
    mp_listener_impl->StartListen();
  }
}

void PointsPage::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
  HTREEITEM treeitem = m_items_list.GetSelectedItem();
  IRenderable* item = (IRenderable*)m_items_list.GetItemData(treeitem);

  if (item == nullptr)
    return;

  //we don`t need to know what item now will be selecting
  mp_listener_impl->StopListen();

  if (!item->IsSelected())
  {
    if (mp_prev_selected != nullptr)
      mp_prev_selected->Deselect();
    item->Select();
    mp_prev_selected = item;
    NotificationManager::GetInstance()->SendNotification(new ItemSelectionChange(item, ItemSelectionChange::IS_SELECTED));
    *pResult = 0;
  }
  IRS::DatabaseInstance->Update();
  mp_listener_impl->StartListen();
}

void PointsPage::OnEditPoints()
{
  CEditPointsDialog dlg(false, mp_owner);

  if (IDOK != dlg.DoModal())
    return;

  std::vector<DivisionFunctionsRenderer*> div_funcs = IRS::DatabaseInstance->GetItems<DivisionFunctionsRenderer>();

  for (size_t i = 0; i < div_funcs.size(); ++i)
    div_funcs[i]->UpdateCash();
  IRS::DatabaseInstance->Update();
  //not efficient but...
  UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
}

void PointsPage::HandleNotification(INotificationData* ip_data)
{
  if (m_items_list.m_hWnd == 0)
    return;

  if (ItemChanged* p_notification = dynamic_cast<ItemChanged*>(ip_data))
  {
    if (p_notification->m_state == ItemChanged::IS_ADDED)
    {
      if (ClassHandlerRenderer* p_class = dynamic_cast<ClassHandlerRenderer*>(p_notification->mp_renderable))
        UI_Utilities::AddClassHandler(&m_items_list, p_class);
      else if (Vertex* p_vertex = dynamic_cast<Vertex*>(p_notification->mp_renderable))
        UI_Utilities::AddVertex(&m_items_list, p_vertex);
    }
    else if (p_notification->m_state == ItemChanged::IS_REMOVED)
      UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
  }
  else if (dynamic_cast<DatabaseCleared*>(ip_data) != nullptr)
  {
    m_items_list.DeleteAllItems();
  }
  else if (ItemSelectionChange* p_selection = dynamic_cast<ItemSelectionChange*>(ip_data))
  {
    ASSERT (p_selection->mp_renderable != nullptr);

    HTREEITEM hItem = UI_Utilities::FindItem(&m_items_list, (DWORD)p_selection->mp_renderable);
    if (hItem != nullptr)
    {
      if (mp_prev_selected != nullptr)
        mp_prev_selected->Deselect();
      mp_prev_selected = p_selection->mp_renderable;
      m_items_list.Select(hItem, TVGN_CARET);
    }
  }
}