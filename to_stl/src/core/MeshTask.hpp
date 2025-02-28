#pragma once

#include <functional>

#include "CalculatorResult.hpp"

namespace to_stl::core {
  using MeshTask = std::function<to_stl::calculator::CalculatorResult()>;
}
