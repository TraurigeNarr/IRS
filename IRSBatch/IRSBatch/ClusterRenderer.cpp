#include "stdafx.h"

#include "ClusterRenderer.h"

#include "Database.h"
#include "Grid.h"
#include "IRenderer.h"
#include "NotificationManager.h"


ClusterRenderer::ClusterRenderer (const std::wstring& i_name)
  : m_color_rgba(Color(255,0,0,0).m_color)
  , m_scale_factor(1.f)
  , m_draw_center_point(false)
  , Cluster(i_name)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
  StartListen();
}

ClusterRenderer::ClusterRenderer (const Cluster& i_cluster)
  : Cluster(i_cluster)
  , m_scale_factor(1.f)
  , m_color_rgba(Color(255,0,0,0).m_color)
  , m_draw_center_point(false)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
  StartListen();
}

ClusterRenderer::~ClusterRenderer()
{
  NotificationManager::GetInstance()->UnregisterHandler(this);
}

void ClusterRenderer::SetScale(float i_scale)
{
  m_scale_factor = i_scale;
}

int ClusterRenderer::GetColor() const
{
  return RGB(m_color_rgba>>24, m_color_rgba>>16, m_color_rgba>>8);
}

void ClusterRenderer::SetColor(int i_color)
{
  m_color_rgba = i_color;
}

void ClusterRenderer::DrawCenterPoint(bool i_draw /* = true */)
{
  m_draw_center_point = i_draw;
}

void ClusterRenderer::Draw(IRenderer* ip_renderer) const
  {
  if (m_linked_points.size() == 0)
    return;

  Grid* grid = IRS::DatabaseInstance->GetGrid();

  D2D1_POINT_2F first_point = m_draw_center_point ? grid->TransformToRendererCoords(GetCenter()) : grid->TransformToRendererCoords(*m_linked_points[0]);

  if (m_draw_center_point)
    {
    D2D1_POINT_2F center = grid->TransformToRendererCoords(GetCenter());
    ip_renderer->RenderCircle(Vector3D(center.x, center.y, 0.), 5, m_color_rgba);
    }
  else
    {
    ip_renderer->RenderCircle(Vector3D(first_point.x, first_point.y, 0.), 5, m_color_rgba);
    }

  MathBase::TPointCloud::const_iterator it = m_linked_points.begin();
  MathBase::TPointCloud::const_iterator it_end = m_linked_points.end();

  for ( ; it != it_end; ++it)
    {
    D2D1_POINT_2F linked_point = grid->TransformToRendererCoords(**it);
    ip_renderer->RenderLine(Vector3D(first_point.x, first_point.y, 0.), Vector3D(linked_point.x, linked_point.y, 0.), m_color_rgba, 1.5f);
    }
  }


void ClusterRenderer::HandleNotification(INotificationData* ip_data)
{
  if (ItemChanged* p_notification = dynamic_cast<ItemChanged*>(ip_data))
  {
    if ( p_notification->m_state != ItemChanged::IS_REMOVED)
      return;
    if (Vector3D* p_vertex = dynamic_cast<Vector3D*>(p_notification->mp_renderable))
      RemovePoint(p_vertex);
  }
}


Vector3D ClusterRenderer::GetPosition() const 
{
  return GetCenter();
}