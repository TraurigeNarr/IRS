#pragma once

#include "IMouse.h"

class ModeInformation
{
private:
  size_t        m_next_color;
  std::wstring  m_name_template;
  size_t        m_next_core_point_index;
  size_t        m_next_point_index;
  size_t        m_next_name_index;
  bool          m_draw_class_division_functions;
  bool          m_draw_class_decision_functions;
  bool          m_log_on_each_iteration;
  bool			m_show_point_numbers;
  double        m_grid_x_scale[2];
  float         m_point_radius;
  size_t        m_grid_divisions_number;

protected:
  MouseType m_mouse_type;

public:
  ModeInformation ();
  ~ModeInformation();

  void            Flush();
  void            SetCorePointNameTemplate(const std::wstring& i_template);

  std::wstring    GetNextClusterName();
  int             GetNextColorRGBA();
  size_t          GetNextPointIndex();
  MouseType       GetMouseType();
  void            SetCurrentMouse(MouseType i_type);
  
  void            SetGridScale(double i_scale[2]);
  void            SetDrawFunctions(bool i_draw_decision, bool i_draw_division);
  void            SetLog(bool i_log_on_each_iteration);
  void            SetGridDivisionsNumber(size_t i_number);
  void            SetShowPointNumbers(bool i_show_point_numbers);
  void            SetPointRadius(float i_radius);

  double          GetGridXScaleMin() const;
  double          GetGridXScaleMax() const;
  bool            DrawDecisionFunctions() const;
  bool            DrawDivisionFunctions() const;
  bool            LogOnEachIteration() const;
  bool            ShowPointNumbers() const;
  float           PointRadius() const;
  size_t          GetGridDivisionsNumber() const;
};