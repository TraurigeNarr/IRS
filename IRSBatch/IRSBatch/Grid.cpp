#include "stdafx.h"

#include "Grid.h"

#include "Database.h"
#include "IRenderer.h"

#include "Renderer.h"

#include <sstream>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  void _DrawAxis(IRenderer* ip_renderer)
    {
    CRect rect = ip_renderer->GetTargetRectangle();
    float width = rect.Width();
    float height = rect.Height();
    //x axis
    ip_renderer->RenderLine(
      Vector3D(0,     height/2.f, 0),
      Vector3D(width, height/2.f, 0),
      IRS::CL_BLACK, 1.5f);
    //y axis
    ip_renderer->RenderLine(
      Vector3D(width/2.f, 0,      0),
      Vector3D(width/2.f, height, 0),
      IRS::CL_BLACK, 1.5f);
    }

  } // namespace

//////////////////////////////////////////////////////////////////////////

Grid::Grid ()
{
  _SetDefaults();
  Reset();
}

Grid::~Grid()
{
}

void Grid::_SetDefaults()
{
  m_scale_factor[0] = 1.f;
  m_scale_factor[1] = 1.f;
  m_divisions_number = 10;
  m_minimum[0] = m_minimum[1] = -10;
  m_maximum[0] = m_maximum[1] = 10;
}

void Grid::OnSize(size_t i_x, size_t i_y)
{
  m_size.x = i_x;
  m_size.y = i_y;
  
  float y_scale = i_y/(i_x*1.f);
  m_minimum[1] = m_minimum[0]*y_scale;
  m_maximum[1] = m_maximum[0]*y_scale;

  float size[2];
  size[0] = m_maximum[0] - m_minimum[0];//by x
  size[1] = m_maximum[1] - m_minimum[1];//by y

  m_scale_factor[0] = size[0] / m_size.x;//by x
  m_scale_factor[1] = size[1] / m_size.y;//by y
  
}

void Grid::GetMaxMin_X_Axis(float& o_min, float& o_max) const
{
  o_min = m_minimum[0];
  o_max = m_maximum[0];
}

void Grid::GetMaxMin_Y_Axis(float& o_min, float& o_max) const
{
  o_min = m_minimum[1];
  o_max = m_maximum[1];
}

void Grid::SetMaxMin_X_Axis(float i_min, float i_max)
{
  m_minimum[0] = i_min;
  m_maximum[0] = i_max;
}

void Grid::SetMaxMin_Y_Axis(float i_min, float i_max)
{
  m_minimum[1] = i_min;
  m_maximum[1] = i_max;
}

void Grid::SetDivisionsNumber(size_t i_number)
{
  m_divisions_number = i_number;
}

void Grid::TransformToCurrent(Vector3D& o_position) const
{
  o_position[0] = m_minimum[0] + o_position[0]*m_scale_factor[0];
  o_position[1] = m_size.y - o_position[1];
  o_position[1] = m_minimum[1] + o_position[1]*m_scale_factor[1];
  o_position[2] = 0.0;
}

Vector3D Grid::GetPoint(int i_x, int i_y) const
{
  Vector3D res(i_x, i_y, 0);
  TransformToCurrent(res);
  return res;
}

D2D1_POINT_2F Grid::TransformToRendererCoords(const Vector3D& i_position) const
{
  D2D1_POINT_2F point;
  float size = m_maximum[0] - m_minimum[0];
  point.x = m_size.x - (size - (i_position[0] - m_minimum[0])) / m_scale_factor[0];
  size = m_maximum[1] - m_minimum[1];
  point.y = (size - (i_position[1] - m_minimum[1])) / m_scale_factor[1];

  return point;
}

void Grid::Reset()
  {
  if (IRenderer* ip_renderer = IRS::DatabaseInstance->GetRenderer())
    {
    CRect rc = ip_renderer->GetTargetRectangle();
    OnSize(rc.Width(), rc.Height());
    }
  }

void Grid::Draw(IRenderer* ip_renderer) const
  {
  _DrawAxis(ip_renderer);

   CRect rect = ip_renderer->GetTargetRectangle();
   float width = rect.Width();
   float height = rect.Height();
   float step = (m_maximum[0] - m_minimum[0]) / m_divisions_number;
  
   Vector3D pos_temp;
   char text[10];

   pos_temp[0] = m_minimum[0];
   //y axis
   for (size_t i = 0; i < m_divisions_number; ++i, pos_temp[0] += step)
     {
     float x_position = pos_temp[0];
     FLOAT x_renderer_position = TransformToRendererCoords(pos_temp).x;
     ip_renderer->RenderLine(
       Vector3D(x_renderer_position, 0,      0),
       Vector3D(x_renderer_position, height, 0),
       IRS::CL_BLACK, 0.5f);
     sprintf_s(text, "%.1f", x_position);
     ip_renderer->RenderText(Vector3D(x_renderer_position,height/2.,0.), text, IRS::CL_BLACK);
     }

  step = (m_maximum[1] - m_minimum[1]) / m_divisions_number;
  pos_temp[1] = m_minimum[1];
  for (size_t i = 0; i < m_divisions_number; ++i, pos_temp[1] += step)
    {
    float y_position = pos_temp[1];
    FLOAT y_renderer_position = TransformToRendererCoords(pos_temp).y;
    ip_renderer->RenderLine(
      Vector3D(0.,    y_renderer_position, 0),
      Vector3D(width, y_renderer_position, 0),
      IRS::CL_BLACK, 0.5f);
    
    sprintf_s(text, "%.1f", y_position);
    ip_renderer->RenderText(Vector3D(width/2., y_renderer_position, 0.), text, IRS::CL_BLACK);
    }
  }

Vector3D Grid::GetPosition() const
{
  return Vector3D();
}