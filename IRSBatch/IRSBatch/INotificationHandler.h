#pragma once

//inherit this class
//and then by dynamic casts ;)
class INotificationData
{

};

class INotificationHandler
{
public:
  virtual void HandleNotification(INotificationData* ip_data) = 0;
};