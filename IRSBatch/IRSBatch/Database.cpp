#include "stdafx.h"

#include "Database.h"
#include "Grid.h"
#include "IRenderable.h"
#include "IRenderer.h"
#include "MainFrm.h"
#include "NotificationManager.h"
#include "Notifications.h"

#include <algorithm>
#include <functional>

using namespace IRS;

Database::Database()
  : mp_renderer(nullptr)
  , mh_grid(new Grid)
  {  }

Database::~Database()
  {
  mp_renderer = nullptr;
  Clear();
  }

Database* Database::GetInstance()
  {
  static Database instance;
  return &instance;
  }

void Database::Clear()
  {
  std::for_each(m_items.begin(), m_items.end(), [](TRenderable i_item)
  {
    delete i_item;
  });
  
  m_items.clear();

  Update();
  
  NotificationManager::GetInstance()->SendNotification(new DatabaseCleared());
  }

IRenderer* Database::GetRenderer() const
  {
  return mp_renderer;
  }

Grid* Database::GetGrid() const
  {
  return mh_grid.get();
  }

void Database::Update()
  {
  if (CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
    {
    if (CRendererFrame* p_renderer_frame = pMainFrame->GetRendererFrame())
      p_renderer_frame->Invalidate();
    }
  }

void Database::DrawItems()
  {
  if (nullptr == mp_renderer)
    return;

  mh_grid->Draw(mp_renderer);

  std::for_each(m_items.begin(), m_items.end(), [this](IRenderable* ip_item)
    {
    ip_item->Draw(mp_renderer);
    });
  }

void Database::SetRenderer(IRenderer* ip_renderer)
  {
  mp_renderer = ip_renderer;
  }

void Database::RegisterItem(IRenderable* ip_item)
{
  TRenderableItems::iterator it = std::find(m_items.begin(), m_items.end(), ip_item);

  if (it == m_items.end())
  {
    m_items.push_back(ip_item);
    //send notification
    NotificationManager::GetInstance()->SendNotification(new ItemChanged(ip_item, ItemChanged::IS_ADDED));
  }
}

void Database::UnregisterItem(IRenderable* ip_item)
{
  TRenderableItems::iterator it = std::find(m_items.begin(), m_items.end(), ip_item);

  if (it != m_items.end())
  {
    IRenderable* p_item = *it;
    m_items.erase(it);
    NotificationManager::GetInstance()->SendNotification(new ItemChanged(ip_item, ItemChanged::IS_REMOVED));
    delete p_item;
  }

}