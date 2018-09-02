#pragma once

#include "ModeTypes.h"
#include "resource.h"

class IMode;

class CTabController : public CPane
{
private:
  CMFCTabCtrl	m_wndTabs;
  IMode* mp_current_mode;
  std::vector<IMode*> m_modes;

private:
  int _InitializeModes();
  IMode* _GetMode(ModeType i_mode);

protected:
  DECLARE_DYNCREATE(CTabController)

public:
  CTabController();
  ~CTabController();

  void SetMode(ModeType i_mode);
  IMode* GetCurrentMode();
protected:
  DECLARE_MESSAGE_MAP()

public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg int OnCreate(LPCREATESTRUCT lpsc);

  void Initialize();
};