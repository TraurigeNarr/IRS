#include "stdafx.h"

#include "EditPointDialog.h"

#include "CPointItem.h"
#include "ClassHandlerRenderer.h"
#include "CustomItem.h"
#include "Database.h"
#include "IMode.h"
#include "ModeInformation.h"
#include "Vertex.h"

#include <MathBase/ClassHandler.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////////
const std::wstring CLASS_SECTION_NAME = L"Class ";

//////////////////////////////////////////////////////////////////////////

CEditPointsDialog::CEditPointsDialog(bool i_classification_mode, IMode* ip_owner, CWnd* pParent /* = NULL */)
  : CDialogEx(CEditPointsDialog::IDD, pParent)
  , m_classification_mode(i_classification_mode)
  , mp_owner(ip_owner)
  , m_vertices_section(-1)
{

}

CEditPointsDialog::~CEditPointsDialog()
{
  m_vertices.clear();
  for (size_t i = 0; i < m_pool.size(); ++i)
  {
    delete m_pool[i];
  }

  m_pool.clear();
}

BEGIN_MESSAGE_MAP(CEditPointsDialog, CDialogEx)
  ON_MESSAGE(WM_PG_ITEMCHANGED, OnItemChanged)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_BN_CLICKED(IDC_COLLAPSE_ALL, OnBnClickedCollapseAll)
  ON_BN_CLICKED(IDC_EXPAND_ALL, OnBnClickedExpandAll)
  ON_BN_CLICKED(IDC_ADD_CLASS, OnBnClickedAddClass)
  ON_BN_CLICKED(IDC_ADD_ETHALON, OnBnClickedAddEthalon)
  ON_BN_CLICKED(IDC_ADD_POINT, OnBnClickedAddPoint)
  ON_BN_CLICKED(IDC_DELETE, OnBnClickedRemoveItem)
END_MESSAGE_MAP()


void CEditPointsDialog::_UpdateInformation()
{
  m_vertices.clear();
  m_property_grid.ResetContents();
  for (size_t i = 0; i < m_pool.size(); ++i)
  {
    delete m_pool[i];
  }

  m_pool.clear();

  std::vector<ClassHandlerRenderer*> classes = IRS::DatabaseInstance->GetItems<ClassHandlerRenderer>();
  std::vector<ClassHandlerRenderer*>::iterator it_class = classes.begin();
  for ( ; it_class != classes.end(); ++it_class)
    _AddClass(*it_class);

  std::vector<Vertex*> vertices = IRS::DatabaseInstance->GetItems<Vertex>();
  std::vector<Vertex*>::iterator it_vertex = vertices.begin();
  std::vector<Vertex*>::iterator vertex_end = vertices.end();
  m_vertices_section = m_property_grid.AddSection(L"Vertices");
  for ( ; it_vertex != vertex_end; ++it_vertex)
    _AddVertex(*it_vertex, m_vertices_section);

  m_property_grid.ExpandAll(true);
}

void CEditPointsDialog::_AddVertex(Vertex* ip_vertex, HSECTION i_section, ClassHandlerRenderer* ip_owner)
{
  m_pool.push_back(new CPointItem(*ip_vertex));
  std::wstring name = L"Vertex ";
  wchar_t buf[8];
  _itow(ip_vertex->GetIndex(), buf, sizeof(buf));
  name += buf;
  HITEM point_item = m_property_grid.AddCustomItem(i_section, name, m_pool.back());
  m_vertices.push_back(ItemInfo(point_item, ip_vertex, i_section, ip_owner));
}

void CEditPointsDialog::_AddClass(ClassHandlerRenderer* ip_class)
{
  HSECTION hs = m_property_grid.AddSection(CLASS_SECTION_NAME + ip_class->GetName());

  const MathBase::TPointCloud& points = ip_class->GetCorePoints();
  MathBase::TPointCloud::const_iterator it_core = points.begin();
  MathBase::TPointCloud::const_iterator core_end = points.end();
  int i = 0;
  for ( ; it_core != core_end; ++it_core)
    _AddVertex(static_cast<Vertex*>(*it_core), hs, ip_class);
}

BOOL CEditPointsDialog::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  _UpdateInformation();

  if (!m_classification_mode)
  {
    GetDlgItem(IDC_ADD_ETHALON)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_ADD_CLASS)->ShowWindow(SW_HIDE);
  }
  
  return TRUE;
}

void CEditPointsDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROPERTY_GRID, m_property_grid);
}

void CEditPointsDialog::OnBnClickedOk()
{
  CDialogEx::OnOK();
}

void CEditPointsDialog::OnBnClickedCancel()
{
  CDialogEx::OnCancel();
}

LRESULT CEditPointsDialog::OnItemChanged(WPARAM wParam, LPARAM lParam)
{
  //find item in classes
  CEditPointsDialog::TVertices::iterator edited_vertex = m_vertices.end();
  edited_vertex = std::find_if(m_vertices.begin(), m_vertices.end(), [wParam](ItemInfo& i_vertex_item)->bool
  {
    return i_vertex_item.mp_grid_item == wParam;
  });

  CPointItem* temp = nullptr;
  m_property_grid.GetItemValue<CPointItem>(wParam, temp);
  if (nullptr != temp)
  {
    *static_cast<Vector3D*>((*edited_vertex).mp_real_item) = temp->GetPosition();
    if (edited_vertex->mp_owner != nullptr)
      edited_vertex->mp_owner->Invalidate();
  }

  return 0;
}

void CEditPointsDialog::OnBnClickedCollapseAll()
{
  m_property_grid.ExpandAll(false);
}

void CEditPointsDialog::OnBnClickedExpandAll()
{
  m_property_grid.ExpandAll(true);
}

void CEditPointsDialog::OnBnClickedAddEthalon()
{
  HSECTION focused_section_id = m_property_grid.GetFocusedSection();
  CPropertyGrid::CSection* focused_section = m_property_grid.FindSection(focused_section_id);
  if (focused_section == nullptr || focused_section->m_title[0] != CLASS_SECTION_NAME[0])
    return;

  ASSERT (focused_section->m_items.size() != 0);

  HITEM first_core_point = focused_section->m_items[0].m_id;
  CEditPointsDialog::TVertices::iterator edited_vertex = m_vertices.end();
  edited_vertex = std::find_if(m_vertices.begin(), m_vertices.end(), [first_core_point](ItemInfo& i_vertex_item)->bool
  {
    return i_vertex_item.mp_grid_item == first_core_point;
  });
  ASSERT (edited_vertex != m_vertices.end() && edited_vertex->mp_owner != nullptr);

  //this check is unnecessary but I am scary to obuse this because I`m vary tired now;
  //need to be checked later...
  if (edited_vertex == m_vertices.end() && edited_vertex->mp_owner == nullptr)
    return;

  ModeInformation& mode_info = mp_owner->GetInformation();
  Vertex* v = new Vertex (Vector3D(), mode_info.GetNextPointIndex());
  edited_vertex->mp_owner->AddCorePoint(v);
  _AddVertex(v, focused_section_id, edited_vertex->mp_owner);
}

void CEditPointsDialog::OnBnClickedAddPoint()
{
  ModeInformation& mode_info = mp_owner->GetInformation();
  Vertex* v = new Vertex (Vector3D(), mode_info.GetNextPointIndex());
  
  _AddVertex(v, m_vertices_section);

  IRS::DatabaseInstance->RegisterItem(v);
}

void CEditPointsDialog::OnBnClickedAddClass()
{
  ModeInformation& mode_info = mp_owner->GetInformation();

  Vertex* v = new Vertex (Vector3D(), mode_info.GetNextPointIndex());
  ClassHandlerRenderer* cl = new ClassHandlerRenderer(mode_info.GetNextClusterName());

  cl->AddCorePoint(v);
  int color = mode_info.GetNextColorRGBA();
  cl->SetColor(color);  
  v->SetColor(color);

  _AddClass(cl);

  IRS::DatabaseInstance->RegisterItem(cl);
}

void CEditPointsDialog::OnBnClickedRemoveItem()
{
  HITEM selected_item = m_property_grid.GetFocusedItem();

  TVertices::iterator selected_vertex = std::find_if(m_vertices.begin(), m_vertices.end(), [selected_item](ItemInfo& item_info) -> bool
  {
    return item_info.mp_grid_item == selected_item;
  });

  if (selected_vertex != m_vertices.end())
  {
    //handle class
    if (m_classification_mode && selected_vertex->mp_owner != nullptr)
    {
      if (!selected_vertex->mp_owner->RemoveCorePoint(selected_vertex->mp_real_item))
      {
        AfxMessageBox(L"Can not delete last point.");
        return;
      }
    }
    else // if it is separate vertex
    {
      IRS::DatabaseInstance->UnregisterItem(selected_vertex->mp_real_item);
    }

    m_property_grid.RemoveItem(selected_item);
    m_property_grid.ExpandAll(true);
    m_vertices.erase(selected_vertex);
    return;
  }

  HSECTION focused_section_id = m_property_grid.GetFocusedSection();
  CPropertyGrid::CSection* focused_section = m_property_grid.FindSection(focused_section_id);
  if (focused_section != nullptr && focused_section->m_title[0] == CLASS_SECTION_NAME[0])
  {
    ASSERT (focused_section->m_items.size() != 0);

    HITEM first_core_point = focused_section->m_items[0].m_id;
    CEditPointsDialog::TVertices::iterator edited_vertex = m_vertices.end();
    edited_vertex = std::find_if(m_vertices.begin(), m_vertices.end(), [first_core_point](ItemInfo& i_vertex_item)->bool
    {
      return i_vertex_item.mp_grid_item == first_core_point;
    });

    ASSERT (edited_vertex != m_vertices.end() && edited_vertex->mp_owner != nullptr);

    //this check is unnecessary but I am scary to obuse this because I`m vary tired now;
    //need to be checked later...
    if (edited_vertex == m_vertices.end() || edited_vertex->mp_owner == nullptr)
      return;

    IRS::DatabaseInstance->UnregisterItem(edited_vertex->mp_owner);
    _UpdateInformation();
  }
}