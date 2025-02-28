#pragma once

#include <memory>
#include <sstream>
#include <string>

#include "Error.hpp"

namespace vrml_proc {
  namespace core {
    namespace error {
      class UnsupportedOperationError : public Error {
       public:
        UnsupportedOperationError(std::string message) : m_message(message) {}

       protected:
        std::string GetMessageInternal() const override {
          std::ostringstream stream;
          stream << "[UnsupportedOperationError]:" << m_message << "\n";
          return stream.str();
        }

       private:
        std::string m_message;
      };
    }  // namespace error
  }    // namespace core
}  // namespace vrml_proc
