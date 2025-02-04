#pragma once

#include <concepts>
#include <type_traits>

namespace vrml_proc {
	namespace core {
		namespace contract {
			template <typename T>
			concept Comparable = requires(T a, T b) {
				{ a < b } -> std::convertible_to<bool>;
				{ a > b } -> std::convertible_to<bool>;
			};
		}
	}
}
