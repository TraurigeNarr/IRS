#pragma once

#include "resource.h"

#include "PropertyGrid.h"

class IMode;
class Results_Page;

namespace Algorithms
{
	struct Recognition_Parameters;
	namespace Reporters
	{
		class Reporter;
	}

}

class RecognitionPage : public CDialogEx
{
private:
	CPropertyGrid m_property_grid;

	CString m_algo_name;
	std::shared_ptr<Algorithms::Reporters::Reporter> mh_reporter;

	HITEM m_algo_combobox;
	HITEM m_show_recognition_contours;
	HITEM m_max_samples_per_symbol;

	HITEM m_path_to_train_folder;
	HITEM m_classification_file;
	HITEM m_images_file;

	IMode* mp_owner;

private:
	bool _SetParameters(Algorithms::Recognition_Parameters* op_parameters);

public:
	RecognitionPage(IMode* ip_owner, CWnd* pParent = NULL);
	virtual ~RecognitionPage();

	void SetReporter(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter);

	enum { IDD = IDD_RECOGNITION_PAGE };

public:
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpsc);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAnalyze();
	afx_msg void OnTrain();
	virtual BOOL OnInitDialog() override;
protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
};