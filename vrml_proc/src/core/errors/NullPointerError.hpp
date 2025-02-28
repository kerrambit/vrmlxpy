#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc {
  namespace core {
    namespace error {
      class NullPointerError : public Error {
       protected:
        std::string GetMessageInternal() const override {
          return "[NullPointerError]: (smart) pointer contained unexpectedly null pointer!\n";
        }
      };
    }  // namespace error
  }    // namespace core
}  // namespace vrml_proc
