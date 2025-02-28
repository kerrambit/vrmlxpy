#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"
#include "Mesh.hpp"

namespace to_stl::calculator {
  using CalculatorResult =
      cpp::result<std::shared_ptr<to_stl::core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>>;
}
