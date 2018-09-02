#pragma once

#include "resource.h"

class IMode;

class ClusteringResults_Page : public CDialog
{
private:
  IMode* mp_owner;

public:
  ClusteringResults_Page(IMode* ip_owner, CWnd* pParent = NULL);
  virtual ~ClusteringResults_Page();

  enum {IDD = IDD_CLUSTERING_RESULTS_TAB};

public:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg void OnSize(UINT nType, int cx, int cy);

  virtual BOOL OnInitDialog() override;
protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
};