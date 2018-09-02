#pragma once

#include <MathBase/math/Vector3D.h>

class IRenderable;

namespace IRS
{
  IRenderable* GetNearestItem(const Vector3D& i_position);
}