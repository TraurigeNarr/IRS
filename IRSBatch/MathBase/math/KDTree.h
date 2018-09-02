#pragma once

#include <vector>
#include <memory>
#include <set>
#include "Box3D.h"

class Vector3D;

typedef std::vector<Vector3D> TPointList;

struct DividerPredicate
  {
          DividerPredicate(int i_axe_index, double i_plane_val);

  //returns true, if the point is on right side or equal value, false if on left
  bool    operator () (const Vector3D& i_point) const;
  private:
  double  m_value;
  int     m_axe_index;
  };

struct KDNode
  {
  public:
                                      KDNode(TPointList::iterator i_begin, TPointList::iterator i_end, KDNode* i_parent = NULL);

  bool                                IsLeaf() const;
  void                                FindNearestNeighbourNode(const Vector3D& i_point, std::set<const KDNode*>& io_visited, TPointList::iterator& io_nearest_neighbour) const;
  //private:
  TPointList::iterator                FindBestCandidateRecursive(const Vector3D& i_point, const KDNode*& i_node) const;
  TPointList::iterator                m_begin;
  TPointList::iterator                m_end;
  std::unique_ptr<DividerPredicate>   m_predicate;
  Box3D                               m_box;
  std::unique_ptr<KDNode>             m_left_node;
  std::unique_ptr<KDNode>             m_right_node;
  KDNode*                             m_parent;

  static int points;
  };

class KDTree
  {
  public:
                            KDTree(const TPointList& i_point_list);

  Vector3D                  FindNearestNeighbour(const Vector3D& i_point) const;
  Vector3D                  FindNearestNeighbourRecursevly(const Vector3D& i_point) const;
  std::vector<Vector3D>     SelectPointsInBox(const Box3D& i_box) const;
  void                      PrintAllPoints(TPointList& o_list, KDNode* node = nullptr) const;
  void                      PrintAllPoints(std::vector<Box3D*>& o_list, KDNode* node = NULL) const;
  const KDNode&             GetRootNode() const;

  private:
  void                      FindNearestNeighbourImpl(const Vector3D& i_point, const KDNode& i_node, double& io_distance, Vector3D& io_point) const;
  
  void                      SelectPointsInBoxImpl(const Box3D& i_box, const KDNode& i_node, std::vector<Vector3D>& io_points_in_box) const;
  std::unique_ptr<KDNode>   m_root_node;
  TPointList                m_points;
  };