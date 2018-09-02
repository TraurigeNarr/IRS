#pragma once

#include "ModeTypes.h"
#include "ModeInformation.h"
#include "Notifications.h"

class IMode : public INotificationHandler
{
private:
  ModeType m_type;
  ModeInformation m_information;
public:
  IMode(ModeType i_mode);
  virtual ~IMode();

  ModeType GetType() const;

  virtual int InitializeMode(CMFCTabCtrl* ip_tab_ctrl) = 0;
  virtual void Deactivate() = 0;
  virtual CString GetModeName() const = 0;
  ModeInformation& GetInformation();
  virtual void UpdateModeState(){}
  virtual void HandleSettingsUpdate();
};