#pragma once

#include "IRenderable.h"
#include "Notifications.h"

#include <MathBase/Types.h>

class ClassHandlerRenderer;

class DivisionFunctionsRenderer 
  : public IRenderable
  , public INotificationHandler
{
public:
  typedef std::vector<ClassHandlerRenderer*> TClassRenderers;

private:
  bool  m_draw_class_division_functions;
  bool  m_draw_class_decision_functions;

  TClassRenderers m_classes;

  struct DivisionDrawingInformation
  {
    D2D1_POINT_2F m_division_line[2];
    ClassHandlerRenderer* mp_class_renderer;

    std::vector<D2D1_POINT_2F> m_division_between_classes;
  };

  typedef std::vector<DivisionDrawingInformation> TCashInformation;
  TCashInformation m_cash_data;

public:
  DivisionFunctionsRenderer();
  ~DivisionFunctionsRenderer();

  void DrawClassDivisionFunctions(bool i_draw = false);
  void DrawClassDecisionFunctions(bool i_draw = true);

  void UpdateCash();

  virtual void HandleNotification(INotificationData* ip_data) override;
  
  virtual void Draw(IRenderer* ip_renderer) const override;
  virtual Vector3D GetPosition() const override;

};