#include "stdafx.h"

#include "ClusteringMode.h"

#include "ClusterRenderer.h"
#include "Database.h"
#include "Grid.h"
#include "MainFrm.h"
#include "MouseController.h"
#include "NotificationManager.h"
#include "Renderer.h"
#include "Vertex.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/ClusteringK_Means.h>
#include <Algorithms/Reporter.h>

#include <functional>


//////////////////////////////////////////////////////////////////////////

ClusteringMode::ClusteringMode(CWnd* pParent /*= NULL*/)
  : IMode(MT_CLUSTERING)
  , m_algo_page(this, pParent)
  , m_points_page(this, pParent)
  , m_results_page(std::make_shared<Algorithms::Reporters::Reporter>(), this, pParent)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
}

ClusteringMode::~ClusteringMode()
{
  NotificationManager::GetInstance()->UnregisterHandler(this);
  AlgorithmsInstance->SetNameGenerator(nullptr);
}

int ClusteringMode::InitializeMode(CMFCTabCtrl* ip_tab_ctrl)
{
  ASSERT(ip_tab_ctrl != nullptr);
  CRect rectDummy;
  CString strTabName;
  BOOL bNameValid = FALSE;

  ip_tab_ctrl->GetTabsRect(rectDummy);
  ip_tab_ctrl->RemoveAllTabs();
  // Create output panes:
  const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
  //points page
  m_points_page.DestroyWindow();
  if (!m_points_page.Create(PointsPage::IDD, ip_tab_ctrl) || !m_points_page.ModifyStyleEx(0,dwStyle))
    return FALSE;

  bNameValid = strTabName.LoadString(IDS_POINTS_PAGE);
  ASSERT(bNameValid);
  ip_tab_ctrl->AddTab(&m_points_page, strTabName, (UINT)0);
  //algorithms page
  m_algo_page.DestroyWindow();
  if (!m_algo_page.Create(ClusteringAlgoPage::IDD, ip_tab_ctrl) || !m_algo_page.ModifyStyleEx(0,dwStyle))
    return FALSE;
  bNameValid = strTabName.LoadString(IDS_CLUSTERING_ALGO_PAGE);
  ASSERT(bNameValid);
  ip_tab_ctrl->AddTab(&m_algo_page, strTabName, (UINT)1);
  //results page
  m_results_page.DestroyWindow();
  if (!m_results_page.Create(Results_Page::IDD, ip_tab_ctrl) || !m_results_page.ModifyStyleEx(0,dwStyle))
    return FALSE;
  bNameValid = strTabName.LoadString(IDS_CLUSTERING_RESULT_PAGE);
  ASSERT(bNameValid);
  ip_tab_ctrl->AddTab(&m_results_page, strTabName, (UINT)2);
  
  //clear points (we can already have clusters)
  IRS::DatabaseInstance->Clear();
  //start listen to notifications
  StartListen();
  GetInformation().SetCurrentMouse(MOUSE_POINTS);
  MouseControllerInstance->SetMouse(MOUSE_POINTS);
  m_algo_page.SetReporter(m_results_page.GetReporter());
  ModeInformation& mode_info = GetInformation();
  AlgorithmsInstance->SetNameGenerator(std::bind<std::wstring>(&ModeInformation::GetNextClusterName, std::ref(mode_info)));

  //set grid scale
  Grid* p_grid = IRS::DatabaseInstance->GetGrid();
  p_grid->SetMaxMin_X_Axis(GetInformation().GetGridXScaleMin(), GetInformation().GetGridXScaleMax());
  p_grid->SetDivisionsNumber(GetInformation().GetGridDivisionsNumber());
  return TRUE;
}

void ClusteringMode::Deactivate()
{
  //stop listen to notifications
  StopListen();
  AlgorithmsInstance->SetNameGenerator(nullptr);
}

CString ClusteringMode::GetModeName() const
{
  CString strTabName;

  // Attach list windows to tab:
  BOOL bNameValid = strTabName.LoadString(IDS_CLUSTERING_MODE);
  ASSERT(bNameValid);

  return strTabName;
}

void ClusteringMode::UpdateModeState()
{
  m_results_page.UpdateResults();
}

void ClusteringMode::HandleNotification(INotificationData* ip_data)
{
  if (ItemChanged* p_notification = dynamic_cast<ItemChanged*>(ip_data))
  {
    
  }
  else if (dynamic_cast<DatabaseCleared*>(ip_data) != nullptr)
    m_points_page.DeleteAllItems();
  else if (dynamic_cast<ClusteringSucceeded*>(ip_data) != nullptr)
    m_points_page.FullUpdate();
    
}