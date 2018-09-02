#pragma once

#include "IRenderable.h"
#include "Notifications.h"

#include <MathBase\Cluster.h>
#include <MathBase\math\Vector3D.h>

class Renderer;
class Grid;

class ClusterRenderer 
  : public IRenderable
  , public INotificationHandler
  , public Cluster
{
  float                   m_scale_factor;
  int                     m_color_rgba;
  bool                    m_draw_center_point;

public:
  ClusterRenderer (const std::wstring& i_name);
  ClusterRenderer (const Cluster& i_cluster);
  ~ClusterRenderer ();

  //set scale factor
  void SetScale(float i_scale);
  int  GetColor() const;
  void SetColor(int i_color);
  void DrawCenterPoint(bool i_draw = true);

  //overrides
  virtual void Draw(IRenderer* ip_renderer) const override;

  virtual void HandleNotification(INotificationData* ip_data) override;
  virtual Vector3D GetPosition() const override;
};