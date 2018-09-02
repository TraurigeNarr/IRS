#pragma once

#include "IMode.h"
#include "resource.h"
#include "ResultsPage.h"

#include "TreeCtrlEx.h"

#include <memory>

class ISelectable;
class DivisionFunctionsRenderer;

class SimpleMode 
  : public CDialog
  , public IMode
{
private:
  int m_selected_mode;
  CTreeCtrlEx m_items_list;
  ISelectable* mp_prev_selected;
  Results_Page m_results_page;

public:
  SimpleMode(CWnd* ip_parent = NULL);
  virtual ~SimpleMode();
  
  enum {IDD = IDD_SIMPLE_DIALOG};

  //overrides
public:
  virtual int InitializeMode(CMFCTabCtrl* ip_tab_ctrl) override;
  virtual void Deactivate() override;
  virtual CString GetModeName() const override;
  virtual void HandleNotification(INotificationData* ip_data) override;

public:
  DECLARE_MESSAGE_MAP()
  
  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnAnalyze();
  afx_msg void OnClear();
  afx_msg void OnModeChanged();
  afx_msg void OnEditPoints();
  afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnPointDblClick(NMHDR* ip_nmhdr, LRESULT* op_result);
protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
};