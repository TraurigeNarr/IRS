#include "stdafx.h"

#include "SettingsDialog.h"

#include "CPointItem.h"
#include "CustomItem.h"
#include "Grid.h"
#include "IMode.h"
#include "MainFrm.h"
#include "ModeInformation.h"
#include "Renderer.h"
#include "Database.h"

CSettingsDialog::CSettingsDialog(IMode* ip_owner, CWnd* pParent /* = NULL */)
	: CDialogEx(CSettingsDialog::IDD, pParent)
	, m_log_on_each_iteration(true)
	, m_show_point_numbers(true)
	, mp_owner(ip_owner)
	, mp_grid_scale_custom_item(nullptr)
{
}

CSettingsDialog::~CSettingsDialog()
  {
  if (nullptr != mp_grid_scale_custom_item)
    delete mp_grid_scale_custom_item;
  }

BEGIN_MESSAGE_MAP(CSettingsDialog, CDialogEx)
  ON_MESSAGE(WM_PG_ITEMCHANGED, OnItemChanged)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
  ON_BN_CLICKED(IDC_SETTINGS_RESET_ALL, OnBnClickedResetAll)
END_MESSAGE_MAP()

void CSettingsDialog::_Initialize()
  {
  ModeInformation& information = mp_owner->GetInformation();
  m_property_grid.ResetContents();
  if (mp_grid_scale_custom_item)
    delete mp_grid_scale_custom_item;

  mp_grid_scale_custom_item = new CPointItem(Vector3D(information.GetGridXScaleMin(), information.GetGridXScaleMax(), .0));

  CString res_string;
  res_string.LoadString(IDS_SETTINGS);
  HSECTION hs = m_property_grid.AddSection(res_string.GetString());

  res_string.LoadString(IDS_SETTINGS_GRID_SCALE);
  m_grid_scale = m_property_grid.AddCustomItem(hs, res_string.GetString(), mp_grid_scale_custom_item);

  res_string.LoadString(IDS_SETTINGS_GRID_DIVISIONS);
  m_grid_divisions_number = m_property_grid.AddIntegerItem(hs, res_string.GetString(), information.GetGridDivisionsNumber());

  res_string.LoadString(IDS_SETTINGS_DRAW_DECISION_FUNCTIONS);
  m_bool_draw_decision_functions = m_property_grid.AddBoolItem(hs, res_string.GetString(), information.DrawDecisionFunctions());
  res_string.LoadString(IDS_SETTINGS_DRAW_DIVISION_FUNCTIONS);
  m_bool_draw_division_functions = m_property_grid.AddBoolItem(hs, res_string.GetString(), information.DrawDivisionFunctions());

  res_string.LoadString(IDS_SETTINGS_LOG_ON_EACH_ITERATION);
  m_log_on_each_iteration = m_property_grid.AddBoolItem(hs, res_string.GetString(), information.LogOnEachIteration());

  res_string.LoadString(IDS_SETTINGS_SHOW_POINT_NUMBERS);
  m_show_point_numbers = m_property_grid.AddBoolItem(hs, res_string.GetString(), information.ShowPointNumbers());

  res_string.LoadString(IDS_SETTINGS_POINT_RADIUS);
  m_point_radius = m_property_grid.AddDoubleItem(hs, res_string.GetString(), information.PointRadius());
  }

void CSettingsDialog::_SetInformation()
{
	ModeInformation& information = mp_owner->GetInformation();

	//grid scale
	CPointItem* temp = nullptr;
	m_property_grid.GetItemValue<CPointItem>(m_grid_scale, temp);
	if (nullptr != temp)
	{
		Vector3D vec = temp->GetPosition();
		Grid* p_grid = IRS::DatabaseInstance->GetGrid();
		double scale[] = { vec.GetX(), vec.GetY() };
		if (scale[0] > scale[1])
			std::swap(scale[0], scale[1]);
		information.SetGridScale(scale);
		p_grid->SetMaxMin_X_Axis(scale[0], scale[1]);
	}
	size_t grid_divisions = 10;
	m_property_grid.GetItemValue(m_grid_divisions_number, (int&)grid_divisions);
	information.SetGridDivisionsNumber(grid_divisions);
	//division functions
	bool draw_decision = false,
		draw_division = true;
	m_property_grid.GetItemValue(m_bool_draw_decision_functions, draw_decision);
	information.SetDrawFunctions(draw_decision, draw_division);
	//logging
	bool log_on_each_iteration = true;
	m_property_grid.GetItemValue(m_log_on_each_iteration, log_on_each_iteration);
	information.SetLog(log_on_each_iteration);
	// show point number
	bool show_point_number = true;
	m_property_grid.GetItemValue(m_show_point_numbers, show_point_number);
	information.SetShowPointNumbers(show_point_number);
	// point radius
	double point_radius = 7.0f;
	m_property_grid.GetItemValue(m_point_radius, point_radius);
	information.SetPointRadius(static_cast<float>(point_radius));
}

BOOL CSettingsDialog::OnInitDialog()
{
	BOOL result = CDialogEx::OnInitDialog();
	_Initialize();
	return result;
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX)
  {
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SETTINGS_PROPERTIES, m_property_grid);
  }

void CSettingsDialog::OnBnClickedOk()
  {
  _SetInformation();
  mp_owner->HandleSettingsUpdate();
  CDialogEx::OnOK();
  }

void CSettingsDialog::OnBnClickedCancel()
  {
  CDialogEx::OnCancel();
  }

void CSettingsDialog::OnBnClickedResetAll()
  {
  mp_owner->GetInformation().Flush();
  _Initialize();
  }

LRESULT CSettingsDialog::OnItemChanged(WPARAM wParam, LPARAM lParam)
  {
  return 0;
  }