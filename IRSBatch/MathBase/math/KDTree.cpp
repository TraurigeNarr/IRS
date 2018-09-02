#include "KDTree.h"

#include "Vector3D.h"

#include <algorithm>
#include <cassert>

namespace NodeImpl
  {
  Box3D   ConstructBox(TPointList::iterator i_begin, TPointList::iterator i_end);
  double  GetSplitValue(TPointList::iterator i_begin, TPointList::iterator i_end, size_t i_axis_index);
  size_t  ChooseAxisForDivision(const Box3D& i_box);
  };
using namespace NodeImpl;
//////////////////////////////////////////////////////////////////////////
//Divider Predicate
DividerPredicate::DividerPredicate(int i_axe_index, double i_plane_val)
  : m_value(i_plane_val), m_axe_index(i_axe_index)
{

}

bool DividerPredicate::operator() (const Vector3D& i_point) const
{
  return (i_point[m_axe_index] < m_value);
}

//////////////////////////////////////////////////////////////////////////
//KDNode
int KDNode::points = 0;
KDNode::KDNode(TPointList::iterator i_begin, TPointList::iterator i_end, KDNode* i_parent /* = NULL */)
   : m_predicate(nullptr), m_begin(i_begin), m_end(i_end), m_parent(i_parent)
{
  //select axis
  size_t points_count = std::distance(i_begin, i_end);
  //return condition
  ++points;
  //construct the box
  m_box = ConstructBox(i_begin, i_end);
  if(1 == points_count)
  {
    return;
  }
  if(m_parent != nullptr)
    assert(m_parent->m_box.FullyContains(m_box));
  size_t axis_index = ChooseAxisForDivision(m_box);

  double median_value = GetSplitValue(i_begin, i_end, axis_index);
  //create predicate
  m_predicate.reset(new DividerPredicate(axis_index, median_value));
  TPointList::iterator median = std::partition(i_begin, i_end, (*m_predicate));
  //create node and construct subtrees
  m_left_node.reset(new KDNode(i_begin, median, this));
  m_right_node.reset(new KDNode(median, i_end, this));
}

bool KDNode::IsLeaf() const
{
  return nullptr == m_left_node && nullptr == m_right_node;
}

TPointList::iterator KDNode::FindBestCandidateRecursive(const Vector3D& i_point, const KDNode*& o_node) const
  {
  if(IsLeaf())
    {
    assert((m_end - m_begin) == 1);
    o_node = this;
    return m_begin;
    }
  if(m_left_node->m_box.DistanceSq(i_point) < m_right_node->m_box.DistanceSq(i_point))
    return m_left_node->FindBestCandidateRecursive(i_point, o_node);
  else
    return m_right_node->FindBestCandidateRecursive(i_point, o_node);
  }

void KDNode::FindNearestNeighbourNode(const Vector3D& i_point, std::set<const KDNode*>& io_visited, TPointList::iterator& io_nearest_neigbour) const
  {
  double upper_bound = (i_point-*io_nearest_neigbour).LengthSq();
  io_visited.insert(this);
  if(IsLeaf())
    {
    assert(std::distance(m_begin,m_end) == 1);
    double dist_to_begin = (i_point - *m_begin).LengthSq();
    double dist_to_best = (i_point-*io_nearest_neigbour).LengthSq();
    if((i_point - *m_begin).LengthSq() <= (i_point-*io_nearest_neigbour).LengthSq())
      io_nearest_neigbour = m_begin;
    return;
    }
  if(io_visited.find(m_left_node.get()) == io_visited.end() && m_left_node->m_box.DistanceSq(i_point) <= (i_point-*io_nearest_neigbour).LengthSq())
    m_left_node->FindNearestNeighbourNode(i_point, io_visited, io_nearest_neigbour);

  if(io_visited.find(m_right_node.get()) == io_visited.end() && m_right_node->m_box.DistanceSq(i_point) <= (i_point-*io_nearest_neigbour).LengthSq())
    m_right_node->FindNearestNeighbourNode(i_point, io_visited, io_nearest_neigbour);

  if(nullptr != m_parent)
    m_parent->FindNearestNeighbourNode(i_point, io_visited, io_nearest_neigbour);
  }
//////////////////////////////////////////////////////////////////////////
//Node implementation methods
namespace NodeImpl
  {
  Box3D ConstructBox(TPointList::iterator i_begin, TPointList::iterator i_end)
    {
    //find the maximum and minimum values for all axis
    //and then construct the box (min, max)
    double max_value = -std::numeric_limits<double>::max();
    double min_value = std::numeric_limits<double>::max();
    //vectors for construction of Box3D
    Vector3D max_vector;
    Vector3D min_vector;
    for(size_t i = 0; i < 3; ++i)
      {
      for(TPointList::iterator j = i_begin; j < i_end; ++j)
        {
        max_value = std::max((*j)[i], max_value);
        min_value = std::min((*j)[i], min_value);
        }//for all points
      max_vector[i] = max_value;
      min_vector[i] = min_value;
      }//for all coordinates
    assert(min_vector[0] <= max_vector[0] && min_vector[1] <= max_vector[1] && min_vector[2] <= max_vector[2]);
    return Box3D(min_vector, max_vector);
    }

  size_t ChooseAxisForDivision(const Box3D& i_box)
    {
    //index of axis by which will be division
    size_t axis_index = -1;
    //choose the longest side of the box -> division by this axis
    double max_value = -std::numeric_limits<double>::max();
    Vector3D box_size = i_box.GetSize();
    for(int i = 0; i < 3; ++i)
      {
      if(max_value < box_size[i])
        {
        max_value = box_size[i];
        axis_index = i;
        }
      }
    return axis_index;
    }

  double GetSplitValue(TPointList::iterator i_begin, TPointList::iterator i_end, size_t i_axis_index)
    {
    //choose median as pivot element
    size_t points_count = std::distance(i_begin, i_end);
    if(2 == points_count)
      {
      double val_first = (*(i_begin+1))[i_axis_index];
      double val_second = (*i_begin)[i_axis_index];
      return (val_second + val_first)/2;
      }
    
    assert(i_end - i_begin > 2);

    double max_value = -std::numeric_limits<double>::max();
    double min_value = std::numeric_limits<double>::max();
      for(TPointList::iterator j = i_begin; j < i_end; ++j)
        {
        max_value = std::max((*j)[i_axis_index], max_value);
        min_value = std::min((*j)[i_axis_index], min_value);
        }//for all points
    return (max_value + min_value)/2.;
    TPointList::iterator median = i_begin + points_count/2;
    std::nth_element(i_begin, median, i_end, [i_axis_index](const Vector3D& i_p1, const Vector3D& i_p2) -> bool
      {
      return i_p1[i_axis_index] < i_p2[i_axis_index];
      });
    return (*median)[i_axis_index];
    }
  }
//////////////////////////////////////////////////////////////////////////
//KDTree
KDTree::KDTree(const TPointList& i_point_list)
{
  //initialize points
  m_points = i_point_list;
  //initialize root node
  m_root_node.reset(new KDNode(m_points.begin(), m_points.end()));
}

const KDNode& KDTree::GetRootNode() const
  {
  return *m_root_node.get();
  }

Vector3D KDTree::FindNearestNeighbour(const Vector3D& i_point) const
  {
  Vector3D neighbour;
  double max = std::numeric_limits<double>::max();

  FindNearestNeighbourImpl(i_point, *m_root_node.get(), max, neighbour);

  return neighbour;
  }

void KDTree::FindNearestNeighbourImpl(const Vector3D& i_point, const KDNode& i_node, double& io_distance, Vector3D& io_point) const
  {
    if(i_node.IsLeaf())
    {
    double length_sq = (i_point - *i_node.m_begin).LengthSq();
    if( length_sq < io_distance)
      {
      io_point = *i_node.m_begin;
      io_distance = length_sq;
      }
    return;
    }

    if(i_node.m_box.DistanceSq(i_point) <= io_distance)
    {
    FindNearestNeighbourImpl(i_point, *i_node.m_right_node, io_distance, io_point);
    FindNearestNeighbourImpl(i_point, *i_node.m_left_node, io_distance, io_point);
    }
  }

Vector3D KDTree::FindNearestNeighbourRecursevly(const Vector3D& i_point) const
  {
  const KDNode *best_cand_node = nullptr;
  TPointList::iterator best_candidate = m_root_node->FindBestCandidateRecursive(i_point, best_cand_node);
  assert(nullptr != best_cand_node);
  std::set<const KDNode*> visited;
  visited.insert(best_cand_node);
  if(nullptr != best_cand_node->m_parent)
    best_cand_node->m_parent->FindNearestNeighbourNode(i_point, visited, best_candidate);
  return *best_candidate;
  }


std::vector<Vector3D> KDTree::SelectPointsInBox(const Box3D& i_box) const
  {
  std::vector<Vector3D> points;
  SelectPointsInBoxImpl(i_box, *m_root_node.get(), points);
  return points;
  }

void KDTree::SelectPointsInBoxImpl(const Box3D& i_box, const KDNode& i_node, std::vector<Vector3D>& io_points_in_box) const
  {
    if(i_node.IsLeaf())
    {
    if(i_box.Contains(*i_node.m_begin))
      io_points_in_box.push_back(*i_node.m_begin);
    return;
    }
   if(i_box.FullyContains(i_node.m_box))
    {
    for(TPointList::iterator i = i_node.m_begin; i != i_node.m_end; ++i)
      io_points_in_box.push_back(*i);
    }
  else
    {
    if(i_box.Intersects(i_node.m_left_node->m_box))
      SelectPointsInBoxImpl(i_box, *i_node.m_left_node, io_points_in_box);
    if(i_box.Intersects(i_node.m_right_node->m_box))
      SelectPointsInBoxImpl(i_box, *i_node.m_right_node, io_points_in_box);
    }
  }

void KDTree::PrintAllPoints(TPointList& o_list, KDNode* node) const
{
  if(NULL == node)
    node = m_root_node.get();

  if(node->IsLeaf() && node->m_begin != m_points.end())
    o_list.push_back(*node->m_begin);
  else
  {
    if(nullptr != node->m_left_node)
      PrintAllPoints(o_list,node->m_left_node.get());
    if(nullptr != node->m_left_node)
      PrintAllPoints(o_list,node->m_right_node.get());
  }
}

void KDTree::PrintAllPoints(std::vector<Box3D*>& o_list, KDNode* node) const
{
  if(NULL == node)
  {
    node = m_root_node.get();
    KDNode::points = 0;
  }
  if(node->IsLeaf() && node->m_begin != m_points.end())
  {
    return;
  }
  else
  {
  o_list.push_back(&node->m_box);
  PrintAllPoints(o_list,node->m_left_node.get());
  o_list.push_back(&node->m_box);
  PrintAllPoints(o_list,node->m_right_node.get());
  }
}