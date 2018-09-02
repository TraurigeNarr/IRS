#pragma once

#include "resource.h"

class IMode;
namespace Algorithms
{
  namespace Reporters
  {
    class Reporter;
  }
}

class Results_Page : public CDialog
{
private:
  IMode* mp_owner;
  std::shared_ptr<Algorithms::Reporters::Reporter> mh_reporter;

public:
  Results_Page(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter, IMode* ip_owner, CWnd* pParent = NULL);
  virtual ~Results_Page();

  enum {IDD = IDD_RESULTS_TAB};

  std::shared_ptr<Algorithms::Reporters::Reporter> GetReporter() const;

public:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void SaveReportInFile();

  virtual BOOL OnInitDialog() override;

public:
  void UpdateResults();

protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
};