#pragma once

class EditBox : public CEdit
{
public:
  enum EditBoxMode
  {
    EM_LONG,
    EM_FLOAT,
    EM_TEXT,
  };

public:
  EditBox();
  ~EditBox();

  void SetMode();

public:
  afx_msg void OnUpdate();
  afx_msg void OnKillFocus();
};