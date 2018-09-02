#include "stdafx.h"

#include "MouseBlocker.h"
#include "ResultsPage.h"

#include <Algorithms/Reporter.h>
#include <fstream>


Results_Page::Results_Page(std::shared_ptr<Algorithms::Reporters::Reporter> ih_reporter, IMode* ip_owner, CWnd* pParent /* = NULL */)
  : CDialog(Results_Page::IDD, pParent)
  , mp_owner(ip_owner)
  , mh_reporter(ih_reporter)
{

}

Results_Page::~Results_Page()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(Results_Page, CDialog)
  ON_WM_CREATE()
  ON_WM_SIZE()
  ON_COMMAND(IDC_SAVE_REPORT_IN_FILE, SaveReportInFile)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void Results_Page::UpdateResults()
{
  UpdateData(FALSE);
}

std::shared_ptr<Algorithms::Reporters::Reporter> Results_Page::GetReporter() const
{
  return mh_reporter;
}

void Results_Page::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  CString result_string = L"No calculations were proceeded";
  if (!mh_reporter->IsEmpty())
    result_string = mh_reporter->GetState(L"\r\n", L"  ", L":").c_str();

  DDX_Text(pDX, IDC_RESULTS_BOX, result_string);
}

int Results_Page::OnCreate(LPCREATESTRUCT lpsc)
{
  return CDialog::OnCreate(lpsc);
}

BOOL Results_Page::OnInitDialog()
{
  return CDialog::OnInitDialog();
}

void Results_Page::OnSize(UINT nType, int cx, int cy)
  {  
  CDialog::OnSize(nType, cx, cy);

  CRect dlg_rc;
  GetClientRect(&dlg_rc);

  CWnd* item = nullptr;
  CRect rc;
  
  //save report button
  item = GetDlgItem(IDC_SAVE_REPORT_IN_FILE);
  if (item != nullptr)
    {
    item->GetWindowRect(&rc);
    ScreenToClient(&rc);
    rc.top = cy - 30;
    rc.bottom = cy - 7;
    rc.left = cx - 95;
    rc.right = rc.left + 90;
    item->MoveWindow(rc);
    }

  //report box
  item = GetDlgItem(IDC_RESULTS_BOX);
  if (item != nullptr)
    {
    item->GetWindowRect(&rc);
    ScreenToClient(&rc);
    rc.right = rc.left + cx - 13;
    rc.bottom = cy - 34;
    item->MoveWindow(rc);
    }
  }

void Results_Page::SaveReportInFile()
{
  ActiveMouseBlocker mouse_blocker;

  static wchar_t report_filter[] = L"Text file (*.txt)|*.txt|Excel file (*.csv)|*.csv|";
  CFileDialog dlg(false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, report_filter);
  if (dlg.DoModal() != IDOK)
    return;
  std::wofstream file(dlg.GetPathName().GetString());
  mh_reporter->Write(file, L"\n", L";", L";");
  file.close();
}