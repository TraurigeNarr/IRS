#pragma once

class INotificationHandler;
class INotificationData;

class NotificationManager
{
public:
  typedef INotificationHandler* THandler;
  typedef std::vector<THandler> THandlers;
private:
  THandlers m_handlers;

private:
  NotificationManager();
  ~NotificationManager();

  NotificationManager(const NotificationManager& i_other);
  NotificationManager& operator=(const NotificationManager& i_other);

public:
  static NotificationManager* GetInstance();

  void RegisterHandler(THandler ip_handler);
  void UnregisterHandler(THandler ip_handler);

  void SendNotification(INotificationData* ip_notification);
};