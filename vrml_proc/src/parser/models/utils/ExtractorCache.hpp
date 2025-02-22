#pragma once

#include <cstdint>

#include <boost/unordered/concurrent_node_map.hpp>

#include "VrmlUnits.hpp"

namespace vrml_proc::parser::model::utils {

	/**
	 * @brief Represents non-templated simple cache for concurrent map-like storage.
	 */
	class ExtractorCache {
	public:
		ExtractorCache() = default;

		void StoreFloat(std::uintptr_t key, vrml_proc::parser::float32_t value) {
			m_cachedFloats.insert_or_assign(key, value);
		}

		void StoreInt(std::uintptr_t key, int32_t value) {
			m_cachedInts.insert_or_assign(key, value);
		}

		std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> GetFloat(std::uintptr_t key) {

			std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> result;

			m_cachedFloats.visit(key, [&](const auto& value) {
				result = std::make_optional(std::cref(value.second));
				});

			return result;
		}

		std::optional<std::reference_wrapper<const int32_t>> GetInt(std::uintptr_t key) {

			std::optional<std::reference_wrapper<const int32_t>> result;

			m_cachedInts.visit(key, [&](const auto& value) {
				result = std::make_optional(std::cref(value.second));
				});

			return result;
		}

	private:
		boost::unordered::concurrent_node_map<std::uintptr_t, vrml_proc::parser::float32_t> m_cachedFloats;
		boost::unordered::concurrent_node_map<std::uintptr_t, int32_t> m_cachedInts;
	};
}
