#pragma once

#include "resource.h"

#include "PropertyGrid.h"

class ICustomItem;
class Vertex;
class ClassHandlerRenderer;
class IMode;

class CEditPointsDialog : public CDialogEx
{

  struct ItemInfo
  {
    HITEM mp_grid_item;
    Vertex* mp_real_item;
    ClassHandlerRenderer* mp_owner;
    UINT m_section;

    ItemInfo(){}

    ItemInfo(HITEM ip_grid_item, Vertex* const ip_real_item, UINT i_section, ClassHandlerRenderer* ip_owner = nullptr)
      : mp_grid_item(ip_grid_item), mp_real_item(ip_real_item), m_section(i_section), mp_owner(ip_owner)
    {}

    virtual ~ItemInfo(){}
  };

  typedef std::vector<ItemInfo>       TVertices;
private:
  CPropertyGrid m_property_grid;
  bool m_classification_mode;
  IMode* mp_owner;
  TVertices m_vertices;
  HSECTION m_vertices_section;

  std::vector<ICustomItem*> m_pool;

private:
  void _UpdateInformation();
  void _AddVertex(Vertex* ip_vertex, HSECTION i_section, ClassHandlerRenderer* ip_owner = nullptr);
  void _AddClass(ClassHandlerRenderer* ip_class);

public:
  CEditPointsDialog(bool i_classification_mode, IMode* ip_owner, CWnd* pParent = NULL);
  virtual ~CEditPointsDialog();

  enum {IDD = IDD_EDIT_POINTS_DIALOG };

  

  virtual BOOL OnInitDialog() override;
  virtual void DoDataExchange(CDataExchange* pDX) override;
public:
  DECLARE_MESSAGE_MAP()

  afx_msg LRESULT OnItemChanged(WPARAM, LPARAM);

  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedCollapseAll();
  afx_msg void OnBnClickedExpandAll();
  afx_msg void OnBnClickedAddClass();
  afx_msg void OnBnClickedAddEthalon();
  afx_msg void OnBnClickedAddPoint();
  afx_msg void OnBnClickedRemoveItem();
};