#include "stdafx.h"

#include "RecognitionPage.h"

#include "ClusterRenderer.h"
#include "Database.h"
#include "IMode.h"
#include "ModeInformation.h"
#include "MouseController.h"
#include "NotificationManager.h"
#include "Vertex.h"
#include "VertexSet.h"
#include "ResultsPage.h"
#include "Grid.h"

#include <Algorithms/AlgorithmManager.h>
#include <Algorithms/Recognition/Recognition_KNN.h>
#include <Algorithms/Reporter.h>


//////////////////////////////////////////////////////////////////////////

RecognitionPage::RecognitionPage(IMode* ip_owner, CWnd* pParent /*= NULL*/)
	: CDialogEx(RecognitionPage::IDD, pParent)
	, mp_owner(ip_owner)
	, mh_reporter(nullptr)
{

}

RecognitionPage::~RecognitionPage()
{
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(RecognitionPage, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDC_RECOGNIZE_BUTTON, OnAnalyze)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void RecognitionPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALGORITHM_PROPERTIES, m_property_grid);
}

BOOL RecognitionPage::OnInitDialog()
{
	if (TRUE != CDialogEx::OnInitDialog())
		return FALSE;

	m_property_grid.ResetContents();

	CString res_string;
	res_string.LoadString(IDC_ALGO_COMBOBOX);
	HSECTION hs = m_property_grid.AddSection(res_string.GetString());
	//algorithms  
	res_string.LoadString(IDS_ALGORITHM_TYPE);
	m_algo_combobox = m_property_grid.AddComboItem(hs, res_string.GetString(),
												   AlgorithmsInstance->GetAnalyzerNames(mp_owner->GetType()),
												   0);

	return TRUE;
}

int RecognitionPage::OnCreate(LPCREATESTRUCT lpsc)
{
	if (CDialogEx::OnCreate(lpsc) == -1)
		return -1;

	return 0;
}

void RecognitionPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}

void RecognitionPage::OnAnalyze()
{
	Algorithms::Recognition_Parameters params;
	if (!_SetParameters(&params))
		return;


	for (VertexSet* vertex_set : IRS::DatabaseInstance->GetItems<VertexSet>())
	{
		auto& vertices = vertex_set->GetVertices();
		for (const Vertex& v : vertices)
		{
			D2D1_POINT_2F coordinates = IRS::DatabaseInstance->GetGrid()->TransformToRendererCoords(v);
			params.m_points.push_back(Vector3D(coordinates.x, coordinates.y, 0.0));
		}
	}

	mh_reporter->Flush();
	std::wstring algo_name;
	m_property_grid.GetItemValue(m_algo_combobox, algo_name);
	VERIFY(AlgorithmsInstance->SetCurrentAnalyzer(algo_name, mp_owner->GetType()));
	AlgorithmsInstance->Analyze(params, mh_reporter);
	mp_owner->UpdateModeState();

	IRS::DatabaseInstance->Update();

	NotificationManager::GetInstance()->SendNotification(new RecognitionSucceded());
}

bool RecognitionPage::_SetParameters(Algorithms::Recognition_Parameters* op_parameters)
{
	UpdateData(TRUE);
	return true;
}

void RecognitionPage::SetReporter(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter)
{
	mh_reporter = ih_reporter;
}