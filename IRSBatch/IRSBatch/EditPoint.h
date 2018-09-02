#pragma once

#include "resource.h"

#include <MathBase/math/Vector3D.h>


class CEditPoint 
  : public CDialogEx
{
private:
  Vector3D m_position;

public:
  CEditPoint(CWnd* ip_parent = NULL);

  enum {IDD = IDD_EDIT_POINT };

  void     SetPosition(Vector3D i_position);
  Vector3D GetPosition();

public:
  DECLARE_MESSAGE_MAP()

  virtual void DoDataExchange(CDataExchange* pDX) override;
};