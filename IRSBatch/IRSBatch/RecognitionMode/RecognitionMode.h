#pragma once

#include "IMode.h"

#include "RecognitionPage.h"
#include "ResultsPage.h"
#include "PointsPage.h"

class RecognitionMode
	: public IMode
{
public:
	enum RecognitionTabs
	{
		CT_POINTS,
		CT_ALGO,
		CT_RESULTS
	};
private:
	CString m_algo_name;

	int    m_first_clusters_type;
	double m_distance;
	size_t m_number_of_clusters;
	size_t m_iteration_number;

	RecognitionPage     m_algo_page;
	PointsPage          m_points_page;
	Results_Page        m_results_page;

private:
	void _SetParameters(Algorithms::Recognition_Parameters* op_parameters);

public:
	RecognitionMode(CWnd* ip_parent = NULL);
	virtual ~RecognitionMode();

	void EnableTab(RecognitionTabs);
	void DisableTab();

	//overrides
public:
	virtual int InitializeMode(CMFCTabCtrl* ip_tab_ctrl) override;
	virtual void Deactivate() override;
	virtual CString GetModeName() const override;
	virtual void UpdateModeState() override;
	virtual void HandleNotification(INotificationData* ip_data) override;
};