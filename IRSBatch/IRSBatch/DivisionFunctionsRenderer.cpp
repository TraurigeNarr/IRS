#include "stdafx.h"

#include "ClassHandlerRenderer.h"
#include "Database.h"
#include "DivisionFunctionsRenderer.h"
#include "IRenderer.h"
#include "Grid.h"
#include "NotificationManager.h"
#include "Notifications.h"

#include <MathBase/math/Vector3D.h>

DivisionFunctionsRenderer::DivisionFunctionsRenderer()
  : m_draw_class_division_functions(true)
  , m_draw_class_decision_functions(false)
{
  NotificationManager::GetInstance()->RegisterHandler(this);
  StartListen();
}

DivisionFunctionsRenderer::~DivisionFunctionsRenderer()
{
  StopListen();
  NotificationManager::GetInstance()->UnregisterHandler(this);
}

void DivisionFunctionsRenderer::HandleNotification(INotificationData* ip_data)
{
  if (ItemChanged* p_item_changed = dynamic_cast<ItemChanged*>(ip_data))
  {
    if (dynamic_cast<ClassHandlerRenderer*>(p_item_changed->mp_renderable))
    {
      m_classes = IRS::DatabaseInstance->GetItems<ClassHandlerRenderer>();
      UpdateCash();
    }
  }
}

void DivisionFunctionsRenderer::UpdateCash()
{
  Grid* grid = IRS::DatabaseInstance->GetGrid();

  float max_x = .0f, 
        min_x = .0f;

  grid->GetMaxMin_X_Axis(min_x, max_x);

  m_cash_data.clear();
  Vector3D temp_point;
  D2D1_POINT_2F division_line[2];
  for (size_t i = 0; i < m_classes.size(); ++i)
  {
    ClassHandlerRenderer* p_first_class = m_classes.at(i);
    m_cash_data.push_back(DivisionDrawingInformation());
    DivisionDrawingInformation& p_first = m_cash_data.back();
    
    temp_point[0] = min_x;
    temp_point[1] = p_first_class->GetDivisionFunction()->GetCoordinate(min_x);
    division_line[0] = grid->TransformToRendererCoords(temp_point);
    temp_point[0] = max_x;
    temp_point[1] = p_first_class->GetDivisionFunction()->GetCoordinate(max_x);
    division_line[1] = grid->TransformToRendererCoords(temp_point);

    p_first.mp_class_renderer = p_first_class;
    p_first.m_division_line[0] = division_line[0];
    p_first.m_division_line[1] = division_line[1];

    for (size_t j = i + 1; j < m_classes.size(); ++j)
    {
      ClassHandlerRenderer* p_second_class = m_classes.at(j); 
      std::shared_ptr<IDivisionFunction> p_between = p_first_class->GetDivisionFunction()->Minus(p_second_class->GetDivisionFunction());
      if (p_between == nullptr)
        continue;

      temp_point[0] = min_x;
      temp_point[1] = p_between->GetCoordinate(min_x);
      division_line[0] = grid->TransformToRendererCoords(temp_point);
      temp_point[0] = max_x;
      temp_point[1] = p_between->GetCoordinate(max_x);
      division_line[1] = grid->TransformToRendererCoords(temp_point);

      p_first.m_division_between_classes.push_back(division_line[0]);
      p_first.m_division_between_classes.push_back(division_line[1]);
    }
  }
}

void DivisionFunctionsRenderer::Draw(IRenderer* ip_renderer) const
  {
  for (size_t i = 0; i < m_cash_data.size(); ++i)
  {
    const DivisionDrawingInformation& p_drawing_info = m_cash_data.at(i);

    if (m_draw_class_decision_functions)
      ip_renderer->RenderLine(Vector3D(p_drawing_info.m_division_line[0].x,p_drawing_info.m_division_line[0].y, 0.),
                                Vector3D(p_drawing_info.m_division_line[1].x,p_drawing_info.m_division_line[1].y, 0.),
                                p_drawing_info.mp_class_renderer->GetColorRGBA());

    if (m_draw_class_division_functions)
    {
      for (size_t j = 0; j < p_drawing_info.m_division_between_classes.size(); j+=2)
      {
      ip_renderer->RenderLine(Vector3D(p_drawing_info.m_division_between_classes.at(j).x,p_drawing_info.m_division_between_classes.at(j).y, 0.),
        Vector3D(p_drawing_info.m_division_between_classes.at(j+1).x,p_drawing_info.m_division_between_classes.at(j+1).y, 0.),
        p_drawing_info.mp_class_renderer->GetColorRGBA(), 1.5f);
      }
    }

  }
  }

Vector3D DivisionFunctionsRenderer::GetPosition() const 
{
  return Vector3D();
}

void DivisionFunctionsRenderer::DrawClassDivisionFunctions(bool i_draw /* = false */)
{
  m_draw_class_division_functions = i_draw;
}

void DivisionFunctionsRenderer::DrawClassDecisionFunctions(bool i_draw /* = true */)
{
  m_draw_class_decision_functions = i_draw;
}