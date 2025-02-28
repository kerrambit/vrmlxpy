#pragma once

#include <memory>

#include <result.hpp>

#include "Error.hpp"

namespace vrml_proc {
  namespace parser {
    template <typename T>
    using ParserResult = cpp::result<T, std::shared_ptr<vrml_proc::core::error::Error>>;
  }
}  // namespace vrml_proc
