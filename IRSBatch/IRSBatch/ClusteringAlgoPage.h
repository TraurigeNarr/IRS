#pragma once

#include "resource.h"

#include "PropertyGrid.h"

class IMode;
class Results_Page;

namespace Algorithms
{
  struct Clustering_Parameters;
  namespace Reporters
  {
    class Reporter;
  }

}

class ClusteringAlgoPage : public CDialogEx
{
private:
  CPropertyGrid m_property_grid;

  CString m_algo_name;
  std::shared_ptr<Algorithms::Reporters::Reporter> mh_reporter;
  
  HITEM m_algo_combobox;
  HITEM m_first_clusters_type; 
  HITEM m_radius_to_append;
  HITEM m_center_shifting_after_iteration;
  HITEM m_separating_type;
  HITEM m_separating_value;
  HITEM m_draw_first_point_as_center;
  HITEM m_number_of_clusters;
  HITEM m_iteration_number;

  IMode* mp_owner;

private:
  bool _SetParameters (Algorithms::Clustering_Parameters* op_parameters);

public:
  ClusteringAlgoPage(IMode* ip_owner, CWnd* pParent = NULL);
  virtual ~ClusteringAlgoPage();

  void SetReporter(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter);

  enum {IDD = IDD_CLUSTERING_ALGO_PAGE};

public:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnAnalyze();
  virtual BOOL OnInitDialog() override;
protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
};