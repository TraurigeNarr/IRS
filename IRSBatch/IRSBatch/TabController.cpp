#include "stdafx.h"

#include "ClusteringMode.h"
#include "Grid.h"
#include "MainFrm.h"
#include "NotificationManager.h"
#include "Renderer.h"
#include "SimpleMode.h"
#include "TabController.h"

#include "RecognitionMode/RecognitionMode.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/SimpleSpaceDivider.h>
#include <Algorithms/ClusteringK_Means.h>
#include <Algorithms/Clustering_Maxmin.h>
#include <Algorithms/Clustering_HeuristicBorder.h>
#include <Algorithms/Recognition/Recognition_KNN.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CTabController, CPane)

CTabController::CTabController()
  : mp_current_mode(nullptr)
{

}

CTabController::~CTabController()
{
  for (size_t i = 0; i < m_modes.size(); ++i)
    delete m_modes[i];
  m_modes.clear();
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTabController, CPane)
  ON_WM_SIZE()
  ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void CTabController::SetMode(ModeType i_mode)
{
  IMode* p_new_mode = _GetMode(i_mode);

  if (p_new_mode == mp_current_mode || p_new_mode == nullptr)
    return;
   
  IMode* p_prev_mode = mp_current_mode;
  //deactivate first
  if (p_prev_mode != nullptr)
    p_prev_mode->Deactivate();
  //switch mode
  mp_current_mode = p_new_mode;
  //try to initialize
  CString mode_name = p_new_mode->GetModeName();
  if (!p_new_mode->InitializeMode(&m_wndTabs))
  {
    CString error_msg;
    error_msg.Format(IDS_ERROR_MODE_SWITCH, mode_name);
    if (AfxMessageBox(error_msg, MB_YESNO) != IDYES)
      exit(1);
    mp_current_mode = p_prev_mode;
    if (mp_current_mode == nullptr && !mp_current_mode->InitializeMode(&m_wndTabs))
    {
      CString error_msg;
      error_msg.Format(IDS_ERROR_MODE_SWITCH, mp_current_mode->GetModeName());
      AfxMessageBox(error_msg, MB_OK);
      exit(2);
    }
    return;
  }

  SetWindowText(mode_name);
}

IMode* CTabController::GetCurrentMode()
{
  return mp_current_mode;
}

int CTabController::_InitializeModes()
{
  IMode* p_mode = new SimpleMode(nullptr);
  m_modes.push_back(p_mode);

  p_mode = new ClusteringMode(nullptr);
  m_modes.push_back(p_mode);

  p_mode = new RecognitionMode(nullptr);
  m_modes.push_back(p_mode);

  //initialize algorithms
  //simple space divider
  AlgorithmManager::TAnalyzerImpl p_analyzer = std::make_shared<Algorithms::SimpleSpaceDivider>(L"Simple space", MT_SIMPLE);
  AlgorithmsInstance->AddAnalyzer(p_analyzer);
  //clustering k means
  p_analyzer = std::make_shared<Algorithms::ClusteringK_Means>(L"K Means", MT_CLUSTERING);
  AlgorithmsInstance->AddAnalyzer(p_analyzer);
  //max min
  p_analyzer = std::make_shared<Algorithms::CLustering_Maxmin>(L"Maxmin", MT_CLUSTERING);
  AlgorithmsInstance->AddAnalyzer(p_analyzer);
  //heuristic border dependent
  p_analyzer = std::make_shared<Algorithms::Clustering_HeuristicBorder>(L"Simple", MT_CLUSTERING);
  AlgorithmsInstance->AddAnalyzer(p_analyzer);
  // recognition
  p_analyzer = std::make_shared<Algorithms::Recognition_KNN>(L"KNN char recognition", MT_RECOGNITION);
  AlgorithmsInstance->AddAnalyzer(p_analyzer);
  //set mode
  SetMode(MT_SIMPLE);

  return 0;
}

IMode* CTabController::_GetMode(ModeType i_mode)
{
  std::vector<IMode*>::iterator it = std::find_if(m_modes.begin(), m_modes.end(), [i_mode](IMode* p_mode) -> bool
  {
    return p_mode->GetType() == i_mode;
  });

  if (it != m_modes.end())
    return *it;

  return nullptr;
}

int CTabController::OnCreate(LPCREATESTRUCT lpsc)
{
  if (CPane::OnCreate(lpsc) == -1)
    return -1;

  CRect rectDummy;
  rectDummy.SetRectEmpty();

  // Create tabs window:
  if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
  {
    TRACE0("Failed to create output tab window\n");
    return -1;      // fail to create
  }
  
  _InitializeModes();

  return 0;
}

void CTabController::Initialize()
{
  _InitializeModes();
}

void CTabController::OnSize(UINT nType, int cx, int cy)
{
  CPane::OnSize(nType, cx, cy);
  
  // Tab control should cover the whole client area:
  m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
}