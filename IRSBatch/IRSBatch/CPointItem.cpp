#include "stdafx.h"

#include "CPointItem.h"
#include "PropertyGrid.h"
#include "EditPoint.h"


CPointItem::CPointItem(const Vector3D& i_position)
  : m_position(i_position)
{

}

CPointItem::~CPointItem()
{

}

CPropertyGrid::EEditMode CPointItem::GetEditMode()
{
  return CPropertyGrid::EM_MODAL;
}

void CPointItem::DrawItem(CDC& dc, CRect rc, bool focused)
{
  CString str;
  str.Format(L"%.3f; %.3f", m_position.GetX(), m_position.GetY());
  rc.left += m_pGrid->GetTextMargin();
  dc.DrawText(str, rc, DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_NOPREFIX);
}

bool CPointItem::OnEditItem()
{
  CEditPoint dlg(m_pGrid);
  dlg.SetPosition(m_position);
   
  if (dlg.DoModal() == IDOK)
  {
    m_position = dlg.GetPosition();
    return true;
  }
  return false;
}

