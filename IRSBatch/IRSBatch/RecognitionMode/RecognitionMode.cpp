#include "stdafx.h"

#include "RecognitionMode.h"

#include "ClusterRenderer.h"
#include "Database.h"
#include "Grid.h"
#include "MainFrm.h"
#include "MouseController.h"
#include "NotificationManager.h"
#include "Renderer.h"
#include "Vertex.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/ClusteringK_Means.h>
#include <Algorithms/Reporter.h>

#include <functional>


//////////////////////////////////////////////////////////////////////////

RecognitionMode::RecognitionMode(CWnd* pParent /*= NULL*/)
	: IMode(MT_RECOGNITION)
	, m_algo_page(this, pParent)
	, m_points_page(this, pParent)
	, m_results_page(std::make_shared<Algorithms::Reporters::Reporter>(), this, pParent)
{
	NotificationManager::GetInstance()->RegisterHandler(this);
}

RecognitionMode::~RecognitionMode()
{
	NotificationManager::GetInstance()->UnregisterHandler(this);
	AlgorithmsInstance->SetNameGenerator(nullptr);
}

int RecognitionMode::InitializeMode(CMFCTabCtrl* ip_tab_ctrl)
{
	ASSERT(ip_tab_ctrl != nullptr);
	CRect rectDummy;
	CString strTabName;
	BOOL bNameValid = FALSE;

	ip_tab_ctrl->GetTabsRect(rectDummy);
	ip_tab_ctrl->RemoveAllTabs();
	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	//points page
	m_points_page.DestroyWindow();
	if (!m_points_page.Create(PointsPage::IDD, ip_tab_ctrl) || !m_points_page.ModifyStyleEx(0, dwStyle))
		return FALSE;

	bNameValid = strTabName.LoadString(IDS_POINTS_PAGE);
	ASSERT(bNameValid);
	ip_tab_ctrl->AddTab(&m_points_page, strTabName, (UINT)0);
	//algorithms page
	m_algo_page.DestroyWindow();
	if (!m_algo_page.Create(RecognitionPage::IDD, ip_tab_ctrl) || !m_algo_page.ModifyStyleEx(0, dwStyle))
		return FALSE;
	bNameValid = strTabName.LoadString(IDS_CLUSTERING_ALGO_PAGE);
	ASSERT(bNameValid);
	ip_tab_ctrl->AddTab(&m_algo_page, strTabName, (UINT)1);
	//results page
	m_results_page.DestroyWindow();
	if (!m_results_page.Create(Results_Page::IDD, ip_tab_ctrl) || !m_results_page.ModifyStyleEx(0, dwStyle))
		return FALSE;
	bNameValid = strTabName.LoadString(IDS_CLUSTERING_RESULT_PAGE);
	ASSERT(bNameValid);
	ip_tab_ctrl->AddTab(&m_results_page, strTabName, (UINT)2);

	//clear points (we can already have clusters)
	IRS::DatabaseInstance->Clear();
	//start listen to notifications
	StartListen();
	MouseControllerInstance->SetMouse(MOUSE_BRUSH);
	m_algo_page.SetReporter(m_results_page.GetReporter());
	ModeInformation& mode_info = GetInformation();
	AlgorithmsInstance->SetNameGenerator(std::bind<std::wstring>(&ModeInformation::GetNextClusterName, std::ref(mode_info)));
	mode_info.SetCurrentMouse(MOUSE_BRUSH);
	mode_info.SetPointRadius(3.0f);
	mode_info.SetShowPointNumbers(false);
	mode_info.SetGridDivisionsNumber(2u);

	//set grid scale
	Grid* p_grid = IRS::DatabaseInstance->GetGrid();
	p_grid->SetMaxMin_X_Axis(GetInformation().GetGridXScaleMin(), GetInformation().GetGridXScaleMax());
	p_grid->SetDivisionsNumber(GetInformation().GetGridDivisionsNumber());
	return TRUE;
}

void RecognitionMode::Deactivate()
{
	//stop listen to notifications
	StopListen();
	AlgorithmsInstance->SetNameGenerator(nullptr);
}

CString RecognitionMode::GetModeName() const
{
	CString strTabName;

	// Attach list windows to tab:
	BOOL bNameValid = strTabName.LoadString(IDS_RECOGNITION_MODE);
	ASSERT(bNameValid);

	return strTabName;
}

void RecognitionMode::UpdateModeState()
{
	m_results_page.UpdateResults();
}

void RecognitionMode::HandleNotification(INotificationData* ip_data)
{
	if (ItemChanged* p_notification = dynamic_cast<ItemChanged*>(ip_data))
	{

	}
	else if (dynamic_cast<DatabaseCleared*>(ip_data) != nullptr)
		m_points_page.DeleteAllItems();
	else if (dynamic_cast<ClusteringSucceeded*>(ip_data) != nullptr)
		m_points_page.FullUpdate();

}