#pragma once

class IRenderable;

namespace IRS
{
  typedef IRenderable* TRenderable;
  typedef std::vector<TRenderable> TRenderableItems;
}