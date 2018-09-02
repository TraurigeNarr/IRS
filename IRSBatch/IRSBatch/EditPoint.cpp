#include "stdafx.h"

#include "EditPoint.h"

#include "PropertyGrid.h"

//////////////////////////////////////////////////////////////////////////

CEditPoint::CEditPoint(CWnd* ip_parent /*= NULL*/)
  : CDialogEx(CEditPoint::IDD, ip_parent)
{

}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CEditPoint, CDialogEx)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void CEditPoint::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_EDIT_X, m_position[0]);
  DDX_Text(pDX, IDC_EDIT_Y, m_position[1]);
}

Vector3D CEditPoint::GetPosition()
{
  return m_position;
}

void CEditPoint::SetPosition(Vector3D i_position)
{
  m_position = i_position;
}