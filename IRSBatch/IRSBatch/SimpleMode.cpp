#include "stdafx.h"

#include "SimpleMode.h"

#include "ClassHandlerRenderer.h"
#include "Database.h"
#include "DivisionFunctionsRenderer.h"
#include "EditPoint.h"
#include "EditPointDialog.h"
#include "Grid.h"
#include "ISelectable.h"
#include "MainFrm.h"
#include "MouseController.h"
#include "NotificationManager.h"
#include "Renderer.h"
#include "UI_Utilities.h"
#include "Vertex.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/SimpleSpaceDivider.h>
#include <Algorithms/Reporter.h>

#include <functional>

//////////////////////////////////////////////////////////////////////////

SimpleMode::SimpleMode(CWnd* pParent /*= NULL*/)
  : CDialog(SimpleMode::IDD, pParent)
  , IMode(MT_SIMPLE)
  , m_selected_mode(-1)
  , mp_prev_selected(nullptr)
  , m_results_page(std::make_shared<Algorithms::Reporters::Reporter>(), this, pParent)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
  GetInformation().SetCorePointNameTemplate(L"");
}

SimpleMode::~SimpleMode()
{
   NotificationManager::GetInstance()->UnregisterHandler(this);
   AlgorithmsInstance->SetNameGenerator(nullptr);
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(SimpleMode, CDialog)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(IDC_ANALYZE, OnAnalyze)
  ON_COMMAND(IDC_CLEAR, OnClear)
  ON_COMMAND(IDC_CORE_POINT_MODE, OnModeChanged)
  ON_COMMAND(IDC_POINTS_MODE, OnModeChanged)
  ON_COMMAND(IDC_EDIT_POINTS, OnEditPoints)
  ON_NOTIFY(TVN_SELCHANGED , IDC_ITEMS_TREE, OnSelectionChanged)
  ON_NOTIFY(NM_DBLCLK, IDC_ITEMS_TREE, OnPointDblClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void SimpleMode::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Radio(pDX, IDC_CORE_POINT_MODE, m_selected_mode);
  DDX_Control(pDX, IDC_ITEMS_TREE, m_items_list);
}

int SimpleMode::InitializeMode(CMFCTabCtrl* ip_tab_ctrl)
{
  ASSERT(ip_tab_ctrl != nullptr);
  CRect rectDummy;
  CString strTabName;
  BOOL bNameValid = FALSE;
  
  ip_tab_ctrl->GetTabsRect(rectDummy);

  ip_tab_ctrl->GetTabsRect(rectDummy);
  ip_tab_ctrl->RemoveAllTabs();

  // Create output panes:
  const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
  DestroyWindow();
  
  if (!Create(SimpleMode::IDD, ip_tab_ctrl) || !ModifyStyleEx(0,dwStyle))
    return FALSE;
  bNameValid = strTabName.LoadString(IDS_SIMPLE_MODE);
  ASSERT(bNameValid);
  ip_tab_ctrl->AddTab(this, strTabName, (UINT)0);  

  //results page
  if (!m_results_page.Create(Results_Page::IDD, ip_tab_ctrl) || !m_results_page.ModifyStyleEx(0,dwStyle))
    return FALSE;
  bNameValid = strTabName.LoadString(IDS_CLUSTERING_RESULT_PAGE);
  ASSERT(bNameValid);
  ip_tab_ctrl->AddTab(&m_results_page, strTabName, (UINT)1);

  m_selected_mode = 0;
  UpdateData(FALSE);
  OnModeChanged();
  OnClear();
  //start listen to notifications
  StartListen();
  //division renderer
  DivisionFunctionsRenderer* p_division_func_renderer = new DivisionFunctionsRenderer;
  p_division_func_renderer->DrawClassDivisionFunctions(GetInformation().DrawDivisionFunctions());
  p_division_func_renderer->DrawClassDecisionFunctions(GetInformation().DrawDecisionFunctions());
  IRS::DatabaseInstance->RegisterItem(p_division_func_renderer);
  m_results_page.GetReporter()->Flush();
  ModeInformation& mode_info = GetInformation();
  AlgorithmsInstance->SetNameGenerator(std::bind<std::wstring>(&ModeInformation::GetNextClusterName, std::ref(mode_info)));

  //set grid scale
  Grid* p_grid = IRS::DatabaseInstance->GetGrid();
  p_grid->SetMaxMin_X_Axis(GetInformation().GetGridXScaleMin(), GetInformation().GetGridXScaleMax());
  p_grid->SetDivisionsNumber(GetInformation().GetGridDivisionsNumber());
  return TRUE;
}

void SimpleMode::Deactivate()
{
  //stop listen to notifications
  StopListen();
  AlgorithmsInstance->SetNameGenerator(nullptr);
}

CString SimpleMode::GetModeName() const
{
  CString strTabName;

  // Attach list windows to tab:
  BOOL bNameValid = strTabName.LoadString(IDS_SIMPLE_MODE);
  ASSERT(bNameValid);

  return strTabName;
}

int SimpleMode::OnCreate(LPCREATESTRUCT lpsc)
{
  if (CDialog::OnCreate(lpsc) == -1)
    return -1;
  return 0;
}

void SimpleMode::OnSize(UINT nType, int cx, int cy)
  {
  CDialog::OnSize(nType, cx, cy);
  CWnd* item = nullptr;

  item = GetDlgItem(IDC_ITEMS_TREE);
  if (item != nullptr)
    {
    CRect rc;
    item->GetWindowRect(&rc);
    ScreenToClient(&rc);

    rc.right = rc.left + cx - 15;
    rc.bottom = rc.top + 0.85*cy;

    item->MoveWindow(rc);
    std::vector<DivisionFunctionsRenderer*> div_funcs = IRS::DatabaseInstance->GetItems<DivisionFunctionsRenderer>();

    for (size_t i = 0; i < div_funcs.size(); ++i)
      {
      div_funcs[i]->UpdateCash();
      div_funcs[i]->DrawClassDivisionFunctions(GetInformation().DrawDivisionFunctions());
      div_funcs[i]->DrawClassDecisionFunctions(GetInformation().DrawDecisionFunctions());
      }
    }  
  }

////////////////////////////////////////////////////////////////////////// 

void SimpleMode::OnClear()
{
   IRS::DatabaseInstance->Clear();
   IRS::DatabaseInstance->RegisterItem(new DivisionFunctionsRenderer);
   m_results_page.GetReporter()->Flush();
}

void SimpleMode::OnAnalyze()
{
  ASSERT (AlgorithmsInstance->GetAnalyzerNames(GetType()).size() > 0);
  m_results_page.GetReporter()->Flush();
  AlgorithmsInstance->SetCurrentAnalyzer(AlgorithmsInstance->GetAnalyzerNames(GetType())[0], GetType());

  MathBase::TClasses classes = IRS::DatabaseInstance->GetItems<ClassHandler>();
  
  //clear previous data
  MathBase::TClasses::const_iterator clusters_end = classes.end();
  MathBase::TClasses::iterator it_cluster = classes.begin();

  for ( ; it_cluster != clusters_end; ++it_cluster)
    (*it_cluster)->ClearLinkedPoints();

  Algorithms::SimpleSpaceDividerParameters params;
  params.m_points = IRS::DatabaseInstance->GetItems<Vector3D>();
  params.m_clusters = classes;

  for (size_t i = 0; i < params.m_points.size(); ++i)
    dynamic_cast<Vertex*>(params.m_points[i])->SetIndex(i+1);

  AlgorithmsInstance->Analyze(params, m_results_page.GetReporter());
  m_results_page.UpdateResults();

  UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
  IRS::DatabaseInstance->Update();
}

void SimpleMode::OnModeChanged()
{
  UpdateData(TRUE);

  if (m_selected_mode == 0)
  {
    MouseControllerInstance->SetMouse(MOUSE_CORE_POINT);
    GetInformation().SetCurrentMouse(MOUSE_CORE_POINT);
  }
  else
  {
    MouseControllerInstance->SetMouse(MOUSE_POINTS);
    GetInformation().SetCurrentMouse(MOUSE_POINTS);
  }
}

void SimpleMode::OnEditPoints()
{
  CEditPointsDialog dlg(true, this, this);

  if (IDOK != dlg.DoModal())
    return;

  std::vector<DivisionFunctionsRenderer*> div_funcs = IRS::DatabaseInstance->GetItems<DivisionFunctionsRenderer>();

  for (size_t i = 0; i < div_funcs.size(); ++i)
    div_funcs[i]->UpdateCash();
  IRS::DatabaseInstance->Update();
  //not efficient but...
  UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
}

void SimpleMode::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
  HTREEITEM treeitem = m_items_list.GetSelectedItem();
  IRenderable* item = (IRenderable*)m_items_list.GetItemData(treeitem);

  if (item == nullptr)
    return;

  //we don`t need to know what item now will be selecting
  StopListen();

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
  StartListen();
}

void SimpleMode::HandleNotification(INotificationData* ip_data)
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

void SimpleMode::OnPointDblClick(NMHDR* ip_nmhdr, LRESULT* op_result)
{
  HTREEITEM treeitem = m_items_list.GetSelectedItem();
  IRenderable* item = (IRenderable*)m_items_list.GetItemData(treeitem);

  if (nullptr == dynamic_cast<Vertex*>(item))
    return;

  CEditPoint edit_point;
  edit_point.SetPosition(item->GetPosition());

  if (edit_point.DoModal() == IDOK)
  {
    StopListen();
    Vector3D vec = edit_point.GetPosition();
    item->SetPosition(vec[0], vec[1], vec[2]);
    HTREEITEM parent = m_items_list.GetParentItem(treeitem);
    if (parent != nullptr)
    {
      ClassHandlerRenderer* p_parent = dynamic_cast<ClassHandlerRenderer*>((IRenderable*)m_items_list.GetItemData(parent));
      if (p_parent)
        p_parent->Invalidate();
    }
    IRS::DatabaseInstance->Update();
    //not efficient but...
    UI_Utilities::FullUpdateTreeCtrl(&m_items_list);
    StartListen();
  }
}