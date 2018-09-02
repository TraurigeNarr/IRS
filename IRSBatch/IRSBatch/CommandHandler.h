#pragma once

class CommandHandler
{
  HWND m_previous_wnd;
private:
  CommandHandler();
  ~CommandHandler();
  CommandHandler(const CommandHandler& i_other);
  CommandHandler& operator=(const CommandHandler& i_other);
public:
  static CommandHandler* GetInstance();

  void TrackMouseLeave(HWND ip_wnd, WPARAM wParam, LPARAM lParam);
};

#define CommandHandlerInstance CommandHandler::GetInstance()