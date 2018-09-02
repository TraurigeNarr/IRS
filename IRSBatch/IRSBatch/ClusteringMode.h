#pragma once

#include "IMode.h"

#include "ClusteringAlgoPage.h"
#include "ResultsPage.h"
#include "PointsPage.h"

class ClusteringMode 
  : public IMode
{
public:
  enum ClusteringTabs
  {
    CT_POINTS,
    CT_ALGO,
    CT_RESULTS
  };
private:
  CString m_algo_name;

  int    m_first_clusters_type; 
  double m_distance;
  size_t m_number_of_clusters;
  size_t m_iteration_number;

  ClusteringAlgoPage  m_algo_page;
  PointsPage          m_points_page;
  Results_Page        m_results_page;

private:
  void _SetParameters (Algorithms::Clustering_Parameters* op_parameters);

public:
  ClusteringMode(CWnd* ip_parent = NULL);
  virtual ~ClusteringMode();

  void EnableTab(ClusteringTabs);
  void DisableTab();

//overrides
public:
  virtual int InitializeMode(CMFCTabCtrl* ip_tab_ctrl) override;
  virtual void Deactivate() override;
  virtual CString GetModeName() const override;
  virtual void UpdateModeState() override;
  virtual void HandleNotification(INotificationData* ip_data) override;
};