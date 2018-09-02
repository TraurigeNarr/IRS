#pragma once

#include "resource.h"

#include "TreeCtrlEx.h"

class IMode;
class IRenderable;
class ISelectable;
class INotificationData;

class PointsPage : public CDialogEx
{
private:
  class PointsPageListener;
  friend class PointsPageListener;

private:
  void HandleNotification(INotificationData* ip_data);

private:
  IMode* mp_owner;
  CTreeCtrlEx m_items_list;
  ISelectable* mp_prev_selected;
  std::unique_ptr<PointsPageListener> mp_listener_impl;

public:
  PointsPage(IMode* ip_owner, CWnd* pParent = NULL);
  virtual ~PointsPage();

  enum {IDD = IDD_POINTS_TAB};

  void AddItem(IRenderable* ip_item);
  void FullUpdate();
  void DeleteAllItems();

public:
  DECLARE_MESSAGE_MAP()

  afx_msg int OnCreate(LPCREATESTRUCT lpsc);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnClear();
  afx_msg void OnPointDblClick(NMHDR* ip_nmhdr, LRESULT* op_result);
  afx_msg void OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnEditPoints();

  virtual BOOL OnInitDialog() override;
protected:
  virtual void DoDataExchange(CDataExchange* pDX) override;
};