#pragma once

#include "Types.h"

class IRenderer;
class Grid;

namespace IRS
{

  class Database 
  {
  private:
    TRenderableItems        m_items;
    IRenderer*              mp_renderer;

    std::unique_ptr<Grid>   mh_grid;
  private:
    Database();
    ~Database();

    Database(const Database& i_other);
    Database& operator=(const Database& i_other);
  public:
    static Database* GetInstance();

    void Clear();
    void Update();
    void DrawItems();

    void SetRenderer(IRenderer* ip_renderer);
    //add to list for render
    void RegisterItem(TRenderable ip_item);
    //remove from list for render
    void UnregisterItem(TRenderable ip_item);

    IRenderer* GetRenderer() const;
    Grid*      GetGrid() const;

    template <typename Tpo>
    std::vector<Tpo*> GetItems() const;
  };

#define DatabaseInstance Database::GetInstance()

  template <typename Tpo>
  std::vector<Tpo*> Database::GetItems() const
  {
    std::vector<Tpo*> items;

    for (size_t i = 0; i < m_items.size(); ++i)
    {
      Tpo* item = dynamic_cast<Tpo*>(m_items[i]);
      if(nullptr != item)
        items.push_back(item);
    }
    return std::move(items);
  }
}