#pragma once

#include <memory>
#include <string>

#define ErrorPositionStringMaxLength 16

namespace vrml_proc {
  namespace parser {
    namespace error {

      struct ParserError {
        size_t bytesCountFromStringStartToErrorPosition;
        size_t bytesCountFromErrorPositionToStringEnd;
        std::string expectedRule;
        std::string errorPosition;
      };
    }  // namespace error
  }    // namespace parser
}  // namespace vrml_proc
