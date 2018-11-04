#pragma once

#include "resource.h"

#include "PropertyGrid.h"

class ICustomItem;
class IMode;

class CSettingsDialog : public CDialogEx
  {
  private:
    CPropertyGrid m_property_grid;
    
    HITEM m_bool_draw_decision_functions;
    HITEM m_bool_draw_division_functions;

    HITEM m_grid_scale;
    HITEM m_grid_divisions_number;
	HITEM m_point_radius;
    ICustomItem* mp_grid_scale_custom_item;

    HITEM m_log_on_each_iteration;
	HITEM m_show_point_numbers;

    IMode* mp_owner;

  private:
    void _Initialize();
    void _SetInformation();

  public:
    CSettingsDialog(IMode* ip_owner, CWnd* pParent = NULL);
    virtual ~CSettingsDialog();

    enum {IDD = IDD_SETTINGS_DIALOG };

    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;

  public:
    DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedResetAll();

    afx_msg LRESULT OnItemChanged(WPARAM, LPARAM);
  };