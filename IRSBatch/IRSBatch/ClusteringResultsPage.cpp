#include "stdafx.h"

#include "ClusteringResultsPage.h"

ClusteringResults_Page::ClusteringResults_Page(IMode* ip_owner, CWnd* pParent /* = NULL */)
  : CDialog(IDD, pParent)
  , mp_owner(ip_owner)
{

}

ClusteringResults_Page::~ClusteringResults_Page()
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(ClusteringResults_Page, CDialog)
  ON_WM_CREATE()
  ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void ClusteringResults_Page::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

int ClusteringResults_Page::OnCreate(LPCREATESTRUCT lpsc)
{
  if (CDialog::OnCreate(lpsc) == -1)
    return -1;

  return 0;
}

BOOL ClusteringResults_Page::OnInitDialog()
{
  if (CDialog::OnInitDialog() != TRUE)
    return FALSE;

  return TRUE;
}

void ClusteringResults_Page::OnSize(UINT nType, int cx, int cy)
{
  /*
  CWnd* item = GetDlgItem(IDC_ITEMS_TREE);

  CRect rc;

  item->GetWindowRect(&rc);
  ScreenToClient(&rc);

  rc.right = rc.left + cx - 15;
  rc.bottom = rc.top + cy;

  item->MoveWindow(rc);
  */
}