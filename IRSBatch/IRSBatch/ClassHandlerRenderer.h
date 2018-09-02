#pragma once

#include "IRenderable.h"
#include "Notifications.h"

#include <MathBase\ClassHandler.h>
#include <MathBase\math\Vector3D.h>

class Renderer;

class ClassHandlerRenderer 
  : public IRenderable
  , public INotificationHandler
  , public ClassHandler
{
private:
  float                   m_scale_factor;
  int                     m_color_rgba;

public:
  ClassHandlerRenderer (const std::wstring& i_name);
  ClassHandlerRenderer (const ClassHandler& i_cluster);
  ~ClassHandlerRenderer ();

  //set scale factor
  void SetScale(float i_scale);
  int GetColor() const;
  int GetColorRGBA() const;
  void SetColor(int i_color);

  //overrides  
  virtual void Draw(IRenderer* ip_renderer) const override;

  virtual void HandleNotification(INotificationData* ip_data) override;
  virtual Vector3D GetPosition() const override;
};