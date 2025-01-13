#pragma once

#include <string>

#include "ParserResult.hpp"

namespace vrml_proc {

	namespace parser {

		template <typename T> class Parser {

		public:

			Parser() = default;

			virtual ~Parser() = default;

			virtual ParserResult<T> Parse(std::string& string) = 0;
		};
	}
}
