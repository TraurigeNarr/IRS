#include "stdafx.h"

#include "ClassHandlerRenderer.h"

#include "Database.h"
#include "Grid.h"
#include "IRenderer.h"
#include "NotificationManager.h"

//////////////////////////////////////////////////////////////////////////

ClassHandlerRenderer::ClassHandlerRenderer (const std::wstring& i_name)
  : m_scale_factor(1.f)
  , m_color_rgba(Color(255,0,0,0).m_color)
  , ClassHandler(i_name)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
  StartListen();
}

ClassHandlerRenderer::ClassHandlerRenderer (const ClassHandler& i_class)
  : ClassHandler(i_class)
  , m_scale_factor(1.f)
  , m_color_rgba(Color(255,0,0,0).m_color)
  {
  NotificationManager::GetInstance()->RegisterHandler(this);
  StartListen();
  }

ClassHandlerRenderer::~ClassHandlerRenderer()
{
  NotificationManager::GetInstance()->UnregisterHandler(this);
}

void ClassHandlerRenderer::SetScale(float i_scale)
{
  m_scale_factor = i_scale;
}

int ClassHandlerRenderer::GetColor() const
{
  return RGB(m_color_rgba>>24, m_color_rgba>>16, m_color_rgba>>8);
}

int ClassHandlerRenderer::GetColorRGBA() const
  {
  return m_color_rgba;
  }

void ClassHandlerRenderer::SetColor(int i_color)
{
  m_color_rgba = i_color;
}

void ClassHandlerRenderer::Draw(IRenderer* ip_renderer) const
  {
  Grid* grid = IRS::DatabaseInstance->GetGrid();

  D2D1_POINT_2F center = grid->TransformToRendererCoords(GetCenter());
  Vector3D center_vec(center.x, center.y, 0.);
  //linked points
  MathBase::TPointCloud::const_iterator it = m_linked_points.begin();
  MathBase::TPointCloud::const_iterator it_end = m_linked_points.end();
  for ( ; it != it_end; ++it)
    {
    D2D1_POINT_2F linked_point = grid->TransformToRendererCoords(**it);
    ip_renderer->RenderLine(center_vec, Vector3D(linked_point.x, linked_point.y,0.), m_color_rgba, 1.5f);
    }

  //center points
  it = m_center_points.begin();
  it_end = m_center_points.end();
  for ( ; it != it_end; ++it)
    {
    D2D1_POINT_2F center_point = grid->TransformToRendererCoords(**it);
    Vector3D center_point_vec(center_point.x, center_point.y,0.);
    
    //draw point itself
    ip_renderer->RenderRectangle(center_point_vec, 6, 6, m_color_rgba);
    //draw line between center and point
    ip_renderer->RenderLine(center_vec, center_point_vec, m_color_rgba, 1.5f);
    }

  if (!m_center_points.empty())
    ip_renderer->RenderCircle(center_vec, 6, m_color_rgba);
  }

void ClassHandlerRenderer::HandleNotification(INotificationData* ip_data)
{
  if (ItemChanged* p_notification = dynamic_cast<ItemChanged*>(ip_data))
  {
    if ( p_notification->m_state == ItemChanged::IS_ADDED)
      return;
    if (Vector3D* p_vertex = dynamic_cast<Vector3D*>(p_notification->mp_renderable))
      RemovePoint(p_vertex);
  }
}

Vector3D ClassHandlerRenderer::GetPosition() const 
{
  return GetCenter();
}