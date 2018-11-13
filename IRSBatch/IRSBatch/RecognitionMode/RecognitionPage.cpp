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
#include <Algorithms/NeuralNetworks/Network.h>
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
	ON_COMMAND(IDC_TRAIN_BUTTON, OnTrain)
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
	
	res_string.LoadString(IDS_SHOW_RECOGNITION_CONTOURS);
	m_show_recognition_contours = m_property_grid.AddBoolItem(hs, res_string.GetString(), false);

	res_string.LoadString(IDS_MAX_SAMPLES_NUMBER);
	m_max_samples_per_symbol = m_property_grid.AddIntegerItem(hs, res_string.GetString(), 1000);

	res_string.LoadString(IDS_PATH_TO_TRAIN_FOLDER);
	m_path_to_train_folder = m_property_grid.AddStringItem(hs, res_string.GetString(), L"");

	res_string.LoadString(IDS_CLASSIFICATION_FILE);
	m_classification_file = m_property_grid.AddStringItem(hs, res_string.GetString(), L"classifications.xml");

	res_string.LoadString(IDS_IMAGES_FILE);
	m_images_file = m_property_grid.AddStringItem(hs, res_string.GetString(), L"images.xml");
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

void RecognitionPage::OnTrain()
{
	Algorithms::Recognition_Parameters params;
	if (!_SetParameters(&params))
		return;

	mh_reporter->Flush();
	std::wstring algo_name;
	m_property_grid.GetItemValue(m_algo_combobox, algo_name);
	VERIFY(AlgorithmsInstance->SetCurrentAnalyzer(algo_name, mp_owner->GetType()));
	AlgorithmsInstance->Train(params, mh_reporter);
	mp_owner->UpdateModeState();

	IRS::DatabaseInstance->Update();

	NotificationManager::GetInstance()->SendNotification(new RecognitionSucceded());
}

bool RecognitionPage::_SetParameters(Algorithms::Recognition_Parameters* op_parameters)
{
	UpdateData(TRUE);
	m_property_grid.GetItemValue(m_max_samples_per_symbol, (int&)op_parameters->max_samples_for_symbol);
	m_property_grid.GetItemValue(m_show_recognition_contours, (bool&)op_parameters->m_show_contours);

	m_property_grid.GetItemValue(m_path_to_train_folder, (std::wstring&)op_parameters->path_to_samples);
	m_property_grid.GetItemValue(m_classification_file, (std::wstring&)op_parameters->classification_file);
	m_property_grid.GetItemValue(m_images_file, (std::wstring&)op_parameters->images_file);

	return true;
}

void RecognitionPage::SetReporter(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter)
{
	mh_reporter = ih_reporter;
}