#pragma once

class CTreeCtrlEx;
class ClassHandlerRenderer;
class Vertex;
class ClusterRenderer;

namespace UI_Utilities
{

  HTREEITEM AddVertex(CTreeCtrlEx* ip_control, Vertex* ip_vertex, HTREEITEM hRoot = nullptr);
  HTREEITEM AddCluster(CTreeCtrlEx* ip_control, ClusterRenderer* ip_cluster, MathBase::TPointCloud* o_points_affected = nullptr);
  HTREEITEM AddClassHandler(CTreeCtrlEx* ip_control, ClassHandlerRenderer* ip_class_handler, MathBase::TPointCloud* o_points_affected = nullptr);
  void FullUpdateTreeCtrl(CTreeCtrlEx* ip_control, bool i_expand_all = false);
  HTREEITEM FindItem(CTreeCtrlEx* ip_tree, DWORD ip_data);
  // ip_tree - a reference to the tree control
  // ip_data - data that was set earlier to item
  HTREEITEM FindItem(CTreeCtrlEx* ip_tree, DWORD ip_data, HTREEITEM hRoot);

} // UI_Utilities