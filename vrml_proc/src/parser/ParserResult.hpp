#pragma once

#include <optional>

namespace vrml_proc {
	namespace parser {
		template <typename T>
		using ParserResult = std::optional<T>;
	}
}
