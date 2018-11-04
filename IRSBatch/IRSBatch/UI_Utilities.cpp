#include "stdafx.h"

#include "UI_Utilities.h"

#include "resource.h"

#include "ClassHandlerRenderer.h"
#include "ClusterRenderer.h"
#include "Database.h"
#include "NotificationManager.h"
#include "TreeCtrlEx.h"
#include "Vertex.h"
#include "VertexSet.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
namespace UI_Utilities
{
  HTREEITEM AddVertex(CTreeCtrlEx* ip_control, Vertex* ip_vertex, HTREEITEM hRoot /*= nullptr*/)
  {
    CString name;
    name.Format(IDS_VERTEX_NAME, ip_vertex->GetIndex(), ip_vertex->GetX(), ip_vertex->GetY());
    HTREEITEM hPoint = nullptr;
    if (hRoot)
      hPoint = ip_control->InsertItem(name.GetBuffer(), hRoot);
    else
      hPoint = ip_control->InsertItem(name.GetBuffer());

    ip_control->SetItemData(hPoint , (DWORD)ip_vertex);
    ip_control->SetItemColor(hPoint , RGB(0,0,150));
    return hPoint;
  }

  HTREEITEM AddVertexSet(CTreeCtrlEx* ip_control, VertexSet* ip_vertex, HTREEITEM hRoot /*= nullptr*/)
  {
	  CString name;
	  name.Format(IDS_VERTEXSET_NAME, ip_vertex->GetIndex());
	  HTREEITEM hPoint = nullptr;
	  if (hRoot)
		  hPoint = ip_control->InsertItem(name.GetBuffer(), hRoot);
	  else
		  hPoint = ip_control->InsertItem(name.GetBuffer());

	  ip_control->SetItemData(hPoint, (DWORD)ip_vertex);
	  ip_control->SetItemColor(hPoint, RGB(0, 0, 150));
	  return hPoint;
  }

  HTREEITEM AddCluster(CTreeCtrlEx* ip_control, ClusterRenderer* ip_cluster, MathBase::TPointCloud* o_points_affected /*= nullptr*/)
  {
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = NULL;
    tvInsert.item.mask = TVIF_TEXT;

    CString name;
    Vector3D cluster_center = ip_cluster->GetCenter();
    name.Format(IDS_CLUSTER_NAME, ip_cluster->GetName().c_str(), cluster_center.GetX(), cluster_center.GetY());
    tvInsert.item.pszText = name.GetBuffer();

    HTREEITEM hClass = ip_control->InsertItem(&tvInsert);
    ip_control->SetItemColor(hClass, ip_cluster->GetColor());
    ip_control->SetItemData(hClass, (DWORD)ip_cluster);

    //linked points
    const MathBase::TPointCloud& linked_points = ip_cluster->GetLinkedPoints();
    for (size_t i = 0; i < linked_points.size(); ++i)
    {
      ASSERT (dynamic_cast<Vertex*>(linked_points[i]));
      name.Format(IDS_VERTEX_NAME, dynamic_cast<Vertex*>(linked_points[i])->GetIndex(), linked_points[i]->GetX(), linked_points[i]->GetY());

      HTREEITEM hLinkedPoint = AddVertex(ip_control, dynamic_cast<Vertex*>(linked_points[i]), hClass);
      if (i == 0)
        ip_control->SetItemColor(hLinkedPoint , ip_cluster->GetColor());
      else
        ip_control->SetItemColor(hLinkedPoint , RGB(0,150,0));

      if (nullptr != o_points_affected)
        o_points_affected->push_back(linked_points[i]);
    }

    ip_control->Expand(hClass, TVE_EXPAND);
    return hClass;
  }

  HTREEITEM AddClassHandler(CTreeCtrlEx* ip_control, ClassHandlerRenderer* ip_class_handler, MathBase::TPointCloud* o_points_affected /*= nullptr*/)
  {
    TVINSERTSTRUCT tvInsert;
    tvInsert.hParent = NULL;
    tvInsert.hInsertAfter = NULL;
    tvInsert.item.mask = TVIF_TEXT;

    CString name;
    Vector3D class_center = ip_class_handler->GetCenter();
    name.Format(IDS_CLASS_NAME, ip_class_handler->GetName().c_str(), class_center.GetX(), class_center.GetY());
    tvInsert.item.pszText = name.GetBuffer();

    HTREEITEM hClass = ip_control->InsertItem(&tvInsert);
    ip_control->SetItemColor(hClass, ip_class_handler->GetColor());
    ip_control->SetItemData(hClass, (DWORD)ip_class_handler);

    //center points
    const MathBase::TPointCloud& center_points = ip_class_handler->GetCorePoints();
    for (size_t i = 0; i < center_points.size(); ++i)
    {
      name.Format(IDS_VERTEX_NAME, i, center_points[i]->GetX(), center_points[i]->GetY());

      HTREEITEM hCorePoint = AddVertex(ip_control, dynamic_cast<Vertex*>(center_points[i]), hClass);
      ip_control->SetItemColor(hCorePoint, ip_class_handler->GetColor());
    }

    //linked points
    const MathBase::TPointCloud& linked_points = ip_class_handler->GetLinkedPoints();
    for (size_t i = 0; i < linked_points.size(); ++i)
    {
      ASSERT (dynamic_cast<Vertex*>(linked_points[i]));
      name.Format(IDS_VERTEX_NAME, dynamic_cast<Vertex*>(linked_points[i])->GetIndex(), linked_points[i]->GetX(), linked_points[i]->GetY());

      HTREEITEM hLinkedPoint = AddVertex(ip_control, dynamic_cast<Vertex*>(linked_points[i]), hClass);
      ip_control->SetItemColor(hLinkedPoint , RGB(0,150,0));

      if (nullptr != o_points_affected)
        o_points_affected->push_back(linked_points[i]);
    }

    ip_control->Expand(hClass, TVE_EXPAND);
    return hClass;
  }

  void FullUpdateTreeCtrl(CTreeCtrlEx* ip_control, bool i_expand_all /*= false*/)
  {
    using namespace MathBase;

    ip_control->DeleteAllItems();

    std::vector<Vertex*> vertices = IRS::DatabaseInstance->GetItems<Vertex>();
    TPointCloud points_affected;

    //update classes
    std::vector<ClassHandlerRenderer*> classes = IRS::DatabaseInstance->GetItems<ClassHandlerRenderer>();
    for (size_t i = 0; i < classes.size(); ++i)
    {
      AddClassHandler(ip_control, classes[i], &points_affected);

      //remove from vertices points that we have already added from class handler
      for (size_t j = 0; j < points_affected.size(); ++j)
      {
        std::vector<Vertex*>::iterator it = std::find(vertices.begin(), vertices.end(), points_affected[j]);
        if (it != vertices.end())
          vertices.erase(it);
      }
    }

    //update clusters
    std::vector<ClusterRenderer*> clusters = IRS::DatabaseInstance->GetItems<ClusterRenderer>();
    for (size_t i = 0; i < clusters.size(); ++i)
    {
      AddCluster(ip_control, clusters[i], &points_affected);

      //remove from vertices points that we have already added from class handler
      for (size_t j = 0; j < points_affected.size(); ++j)
      {
        std::vector<Vertex*>::iterator it = std::find(vertices.begin(), vertices.end(), points_affected[j]);
        if (it != vertices.end())
          vertices.erase(it);
      }
    }

    //update vertices that were not been affected by classes
    for (size_t i = 0; i < vertices.size(); ++i)
      AddVertex(ip_control, vertices[i]);
  }

  HTREEITEM FindItem(CTreeCtrlEx* ip_tree, DWORD ip_data)
  {
    HTREEITEM root = ip_tree->GetRootItem();
    while(root != nullptr)
    {
      HTREEITEM hFound = FindItem(ip_tree, ip_data, root);
      if (hFound)
        return hFound; 

      root = ip_tree->GetNextSiblingItem(root);
    }

    return nullptr;
  }

  HTREEITEM FindItem(CTreeCtrlEx* ip_tree, DWORD ip_data, HTREEITEM hRoot)
  {
    // check whether the current item is the searched one
    DWORD data = ip_tree->GetItemData(hRoot);
    if (ip_data == ip_tree->GetItemData(hRoot))
      return hRoot; 

    // get a handle to the first child item
    HTREEITEM hSub = ip_tree->GetChildItem(hRoot);
    // iterate as long a new item is found
    while (hSub)
    {
      // check the children of the current item
      HTREEITEM hFound = FindItem(ip_tree, ip_data, hSub);
      if (hFound)
        return hFound; 

      // get the next sibling of the current item
      hSub = ip_tree->GetNextSiblingItem(hSub);
    } 

    // return NULL if nothing was found
    return NULL;
  }

} // UI_Utilities