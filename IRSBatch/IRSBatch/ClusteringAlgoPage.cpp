#include "stdafx.h"

#include "ClusteringAlgoPage.h"

#include "ClusterRenderer.h"
#include "Database.h"
#include "IMode.h"
#include "ModeInformation.h"
#include "MouseController.h"
#include "NotificationManager.h"
#include "Vertex.h"
#include "ResultsPage.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/ClusteringK_Means.h>
#include <Algorithms/Reporter.h>

//////////////////////////////////////////////////////////////////////////

ClusteringAlgoPage::ClusteringAlgoPage(IMode* ip_owner, CWnd* pParent /*= NULL*/)
  : CDialogEx(ClusteringAlgoPage::IDD, pParent)
  , mp_owner(ip_owner)
  , mh_reporter(nullptr)
{

}

ClusteringAlgoPage::~ClusteringAlgoPage()
{
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(ClusteringAlgoPage, CDialogEx)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(IDC_ANALYZE, OnAnalyze)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void ClusteringAlgoPage::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ALGORITHM_PROPERTIES, m_property_grid);
}

BOOL ClusteringAlgoPage::OnInitDialog()
  {
  if (TRUE != CDialogEx::OnInitDialog())
    return FALSE;

  m_property_grid.ResetContents();

  CString res_string;
  res_string.LoadString(IDC_ALGO_COMBOBOX);
  HSECTION hs = m_property_grid.AddSection(res_string.GetString());
  //algorithms  
  res_string.LoadString(IDS_ALGORITHM_TYPE);
  m_algo_combobox = m_property_grid.AddComboItem(hs,res_string.GetString(), 
                                                  AlgorithmsInstance->GetAnalyzerNames(mp_owner->GetType()), 
                                                  0);
  
  //type of selecting first point
  std::vector<std::wstring> first_cluster_selecting_type;
  res_string.LoadString(IDS_FROM_BEGIN);
  first_cluster_selecting_type.push_back(res_string.GetString());
  res_string.LoadString(IDS_FROM_END);
  first_cluster_selecting_type.push_back(res_string.GetString());
  res_string.LoadString(IDS_RANDOM);
  first_cluster_selecting_type.push_back(res_string.GetString());
  res_string.LoadString(IDS_FIRST_CLUSTER_SELECTING_TYPE);
  m_first_clusters_type = m_property_grid.AddComboItem(hs, res_string.GetString(), first_cluster_selecting_type, 0);

  res_string.LoadString(IDS_CENTER_SHIFTING_AFTER_OPERATION);
  m_center_shifting_after_iteration = m_property_grid.AddDoubleItem(hs, res_string.GetString(), 0.1, L"%g");

  res_string.LoadString(IDS_RADIUS_TO_APPEND);
  m_radius_to_append = m_property_grid.AddDoubleItem(hs, res_string.GetString(), 10.00, L"%g");  

  std::vector<std::wstring> separating_type;
  res_string.LoadString(IDS_AVERAGE_DISTANCE);
  separating_type.push_back(res_string.GetString());
  res_string.LoadString(IDS_DISTANCE_BETWEEN_TWO_LAST);
  separating_type.push_back(res_string.GetString());
  res_string.LoadString(IDS_SEPARATING_TYPE);
  m_separating_type = m_property_grid.AddComboItem(hs, res_string.GetString(), separating_type, 0);

  res_string.LoadString(IDS_SEPARATING_VALUE);
  m_separating_value = m_property_grid.AddDoubleItem(hs, res_string.GetString(), 0.5, L"%g%%");

  res_string.LoadString(IDS_ITERATION_NUMBER);
  m_iteration_number = m_property_grid.AddIntegerItem(hs, res_string.GetString(), 10);

  res_string.LoadString(IDS_CLUSTERS_NUMBER);
  m_number_of_clusters = m_property_grid.AddIntegerItem(hs, res_string.GetString(), 2);

  res_string.LoadString(IDS_DRAW_FIRST_POINT_AS_CENTER);
  m_draw_first_point_as_center = m_property_grid.AddBoolItem(hs, res_string.GetString(), false);

  return TRUE;
}

int ClusteringAlgoPage::OnCreate(LPCREATESTRUCT lpsc)
{
  if (CDialogEx::OnCreate(lpsc) == -1)
    return -1;

  return 0;
}

void ClusteringAlgoPage::OnSize(UINT nType, int cx, int cy)
{
  CDialogEx::OnSize(nType, cx, cy);
}

void ClusteringAlgoPage::OnAnalyze()
{
  Algorithms::Clustering_Parameters params;
  std::vector<Cluster*> clusters = IRS::DatabaseInstance->GetItems<Cluster>();
  //clear clusters
  for (size_t i = 0; i < clusters.size(); ++i)
  {
    ASSERT(dynamic_cast<ClusterRenderer*>(clusters[i]));
    IRS::DatabaseInstance->UnregisterItem(dynamic_cast<ClusterRenderer*>(clusters[i]));
  }
  clusters.clear();
  if (!_SetParameters(&params))
    return;

  params.m_points = IRS::DatabaseInstance->GetItems<Vector3D>();
  for (size_t i = 0; i < params.m_points.size(); ++i)
    dynamic_cast<Vertex*>(params.m_points[i])->SetIndex(i+1);

  params.mp_clusters = &clusters;
  mh_reporter->Flush();
  AlgorithmsInstance->Analyze(params, mh_reporter);
  mp_owner->UpdateModeState();
  
  ModeInformation& mode_info = mp_owner->GetInformation();
  bool draw_first_point_as_center = true;
  m_property_grid.GetItemValue(m_draw_first_point_as_center, draw_first_point_as_center);
  for (size_t i = 0; i < clusters.size(); ++i)
  {
    ClusterRenderer* p_cluster = new ClusterRenderer(*clusters[i]);
    if (p_cluster->GetName() == L"")
      p_cluster->SetName(mode_info.GetNextClusterName());
    p_cluster->SetColor(mode_info.GetNextColorRGBA());
    IRS::DatabaseInstance->RegisterItem(p_cluster);
    p_cluster->DrawCenterPoint(!draw_first_point_as_center);
  }

  IRS::DatabaseInstance->Update();

  NotificationManager::GetInstance()->SendNotification(new ClusteringSucceeded());
}

bool ClusteringAlgoPage::_SetParameters(Algorithms::Clustering_Parameters* op_parameters)
{
  UpdateData(TRUE);
  std::wstring algo_name;
  m_property_grid.GetItemValue(m_algo_combobox, algo_name);
  VERIFY(AlgorithmsInstance->SetCurrentAnalyzer(algo_name, mp_owner->GetType()));

  m_property_grid.GetItemValue(m_first_clusters_type,              (int&)op_parameters->m_select_first_algorithm);
  m_property_grid.GetItemValue(m_number_of_clusters,               (int&)op_parameters->m_number_of_clusters);
  m_property_grid.GetItemValue(m_radius_to_append,                 op_parameters->m_radius_to_append);
  m_property_grid.GetItemValue(m_iteration_number,                 (int&)op_parameters->m_iterations);
  m_property_grid.GetItemValue(m_center_shifting_after_iteration,  op_parameters->m_center_shift_allowed);
  m_property_grid.GetItemValue(m_separating_type,                  (int&)op_parameters->m_maxmin_distance_criteria);
  m_property_grid.GetItemValue(m_separating_value,                 op_parameters->m_maxmin_separating_value);

  op_parameters->m_report_on_each_iteration = mp_owner->GetInformation().LogOnEachIteration();
  //should be some checks for values
  /*if (m_number_of_clusters < 1)
  {
    AfxMessageBox(IDS_ERROR_CLUSTER_NUMBER);
    return false;
  }

  if (m_radius_to_append <= 0)
  {
    AfxMessageBox(L"Border distance should be greater than 0.");
    return false;
  }
  if (m_iteration_number < 1)
  {
    AfxMessageBox(IDS_ERROR_ITERATIONS);
    return false;
  }*/

  return true;
}

void ClusteringAlgoPage::SetReporter(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter)
{
  mh_reporter = ih_reporter;
}