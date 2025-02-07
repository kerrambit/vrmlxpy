#pragma once

#include <functional>
#include <optional>

#include "BaseConversionContext.hpp"
#include "Vec3fArray.hpp"

namespace to_stl::conversion_context {

	class CoordConversionContext : public vrml_proc::conversion_context::BaseConversionContext {
	public:
		CoordConversionContext(std::reference_wrapper<const vrml_proc::parser::Vec3fArray> data)
			: m_data(data) {}

		CoordConversionContext() = default;

		void Merge(Mergeable* other) override {
			/*if (other == nullptr) {
				return;
			}
			CoordConversionContext* otherContext = dynamic_cast<CoordConversionContext*>(other);
			if (otherContext) {
				if (otherContext->m_data.get().vectors.size() == 0) {
					return;
				}
				m_data.get().vectors.reserve(m_data.get().vectors.size() + otherContext->m_data.get().vectors.size());
				m_data.get().insert(m_data.get().vectors.end(), std::make_move_iterator(otherContext->m_data.get().vectors.begin()), std::make_move_iterator(otherContext->m_data.get().vectors.end()));
				otherContext->m_data.get().vectors.clear();
			}
			else {
				throw std::invalid_argument("Merge called with incompatible type!");
			}*/
		}

	private:
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>> m_data = std::nullopt;
	};
}
