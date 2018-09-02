#include "stdafx.h"

#include "NotificationManager.h"

#include "Notifications.h"

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

NotificationManager::NotificationManager()
{

}

NotificationManager::~NotificationManager()
{

}

NotificationManager* NotificationManager::GetInstance()
{
  static NotificationManager instance;
  return &instance;
}


void NotificationManager::RegisterHandler(THandler ip_handler)
{
  THandlers::iterator it = std::find(m_handlers.begin(), m_handlers.end(), ip_handler);

  if (it == m_handlers.end())
    m_handlers.push_back(ip_handler);
}

void NotificationManager::UnregisterHandler(THandler ip_handler)
{
  THandlers::iterator it = std::find(m_handlers.begin(), m_handlers.end(), ip_handler);

  if (it != m_handlers.end())
    m_handlers.erase(it);
}

void NotificationManager::SendNotification(INotificationData* ip_notification)
{
  std::for_each(m_handlers.begin(), m_handlers.end(), [ip_notification](THandler ip_handler)
  {
    if (ip_handler->IsListening())
      ip_handler->HandleNotification(ip_notification);
  });
}
