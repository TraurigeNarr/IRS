#include "stdafx.h"

#include "IMode.h"

#include "Database.h"
#include "DivisionFunctionsRenderer.h"
#include "Grid.h"
#include "MainFrm.h"
#include "Renderer.h"

IMode::IMode(ModeType i_mode)
  : m_type(i_mode)
{
  //do not listen at begin; only between Initialization and Deactivation of mode
  StopListen();
}

IMode::~IMode()
{

}

ModeType IMode::GetType() const
{
  return m_type;
}

ModeInformation& IMode::GetInformation()
{
  return m_information;
}

void IMode::HandleSettingsUpdate()
  {
  //set grid scale
  Grid* p_grid = IRS::DatabaseInstance->GetGrid();
  p_grid->SetMaxMin_X_Axis(GetInformation().GetGridXScaleMin(), GetInformation().GetGridXScaleMax());
  p_grid->SetDivisionsNumber(GetInformation().GetGridDivisionsNumber());
  p_grid->Reset();

  std::vector<DivisionFunctionsRenderer*> div_funcs = IRS::DatabaseInstance->GetItems<DivisionFunctionsRenderer>();

  for (size_t i = 0; i < div_funcs.size(); ++i)
    {
    div_funcs[i]->UpdateCash();
    div_funcs[i]->DrawClassDivisionFunctions(GetInformation().DrawDivisionFunctions());
    div_funcs[i]->DrawClassDecisionFunctions(GetInformation().DrawDecisionFunctions());
    }

  }