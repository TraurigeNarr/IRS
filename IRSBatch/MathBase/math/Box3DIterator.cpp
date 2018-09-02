#include "Box3DIterator.h"
#include <cassert>

Box3DIterator::Box3DIterator(Box3D* i_box):m_box(i_box), m_currentposition(0)
  {

  }


Box3DIterator::~Box3DIterator()
  {
  }

Box3DIterator Box3DIterator::Begin(Box3D* i_box)
  {
  return Box3DIterator(i_box);
  }

Box3DIterator Box3DIterator::End(Box3D* i_box)
  {
  Box3DIterator temp(i_box);
  temp.m_currentposition=8;
  return temp;
  }

Vector3D Box3DIterator::dereference()const
  {
  int x = m_currentposition & 1;
  int y = m_currentposition & 2;
  int z = m_currentposition & 4;
  return Vector3D(x ? m_box->GetMax()[0] : m_box->GetMin()[0], 
    y ? m_box->GetMax()[1] : m_box->GetMin()[1],
    z ? m_box->GetMax()[2] : m_box->GetMin()[2]);

  } 

bool Box3DIterator::equal(const Box3DIterator& i_iter)const
  {
  assert(m_box==i_iter.m_box);
  return m_currentposition==i_iter.m_currentposition;
  }
void Box3DIterator::increment()
  {
  m_currentposition++;
  }

void Box3DIterator::decrement()
  {
  if(m_currentposition)
    m_currentposition--;
  }
void Box3DIterator::advance(int i_index)
  {
  m_currentposition+=i_index;
  }

int Box3DIterator::distance_to(const Box3DIterator& i_iter)const
  {
  return i_iter.m_currentposition - m_currentposition;
  }

