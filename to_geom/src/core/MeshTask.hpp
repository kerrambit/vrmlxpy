#pragma once

#include <functional>

#include "CalculatorResult.hpp"

namespace to_geom::core {
  using MeshTask = std::function<to_geom::calculator::CalculatorResult()>;
}
