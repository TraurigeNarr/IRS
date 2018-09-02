#pragma once
#include "Box3d.h"
#include "Vector3D.h"
#include <boost/iterator/iterator_facade.hpp>

class Box3DIterator:public boost::iterator_facade<
  Box3DIterator
  , Vector3D
  , boost::random_access_traversal_tag
  , Vector3D
  , int
>

  {
  public:
    Box3DIterator():m_box(nullptr)
      {
      };
    Box3DIterator(Box3D* i_box);
    ~Box3DIterator();
    static Box3DIterator Begin(Box3D*);
    static Box3DIterator End(Box3D*);
    Vector3D dereference()const;
    bool equal(const Box3DIterator&)const;
    void increment();
    void decrement();
    void advance(int);
    int distance_to(const Box3DIterator&)const;
  private:
    Box3D* m_box;
    int m_currentposition;
  };
