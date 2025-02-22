#pragma once

#include <string>

#include "ParserResult.hpp"

namespace vrml_proc {
	namespace parser {

		template <typename SourceType, typename ResultType>
		class Parser {

		public:

			Parser() = default;

			virtual ~Parser() = default;

			virtual ParserResult<ResultType> Parse(SourceType source) = 0;
		};
	}
}
