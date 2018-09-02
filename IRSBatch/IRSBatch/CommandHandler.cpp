#include "stdafx.h"

#include "CommandHandler.h"

#include "MainFrm.h"

#include "RendererFrame.h"


CommandHandler::CommandHandler()
  : m_previous_wnd(nullptr)
{

}

CommandHandler::~CommandHandler()
{

}

CommandHandler* CommandHandler::GetInstance()
{
  static CommandHandler instance;

  return &instance;
}

void CommandHandler::TrackMouseLeave(HWND ip_wnd, WPARAM wParam, LPARAM lParam)
{
  if (nullptr == m_previous_wnd)
  {
    m_previous_wnd = ip_wnd;
    return;
  }

  CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(tme);
  tme.hwndTrack = m_previous_wnd;
  tme.dwFlags = TME_LEAVE;        
  _TrackMouseEvent(&tme);
  m_previous_wnd = ip_wnd;
}

