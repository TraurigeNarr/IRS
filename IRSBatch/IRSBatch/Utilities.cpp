#include "stdafx.h"

#include "Utilities.h"

#include "Database.h"
#include "IRenderable.h"
#include "Vertex.h"

#include <limits>

#ifdef max 
#undef max
#endif

namespace IRS
{
  IRenderable* GetNearestItem(const Vector3D& i_position)
  {
    IRS::TRenderableItems items = IRS::DatabaseInstance->GetItems<IRenderable>();

    IRenderable* nearest = nullptr;

    double min_dist = std::numeric_limits<double>::max();

    for (size_t i = 0; i < items.size(); ++i)
    {
      double dist = i_position.DistanceSq(items[i]->GetPosition());

      if (dist < min_dist && dist < 20)
      {
        min_dist = dist;
        nearest = items[i];
      }
    }

    return nearest;
  }
}