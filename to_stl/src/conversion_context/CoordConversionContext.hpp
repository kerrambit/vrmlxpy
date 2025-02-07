#pragma once

#include <functional>
#include <vector>

#include "BaseConversionContext.hpp"
#include "Vec3fArray.hpp"

namespace to_stl::conversion_context {

	class CoordConversionContext : public vrml_proc::conversion_context::BaseConversionContext {
	public:
		CoordConversionContext(std::reference_wrapper<const vrml_proc::parser::Vec3fArray> data) {
			m_data.push_back(data);
		}

		CoordConversionContext() = default;

		void Merge(Mergeable* other) override {
			if (other == nullptr) {
				return;
			}
			CoordConversionContext* otherContext = dynamic_cast<CoordConversionContext*>(other);
			if (otherContext) {
				if (otherContext->m_data.size() == 0) {
					return;
				}
				m_data.reserve(m_data.size() + otherContext->m_data.size());
				m_data.insert(m_data.end(), std::make_move_iterator(otherContext->m_data.begin()), std::make_move_iterator(otherContext->m_data.end()));
				otherContext->m_data.clear();
			}
			else {
				throw std::invalid_argument("Merge called with incompatible type!");
			}
		}

	private:
		std::vector<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>> m_data = {};
	};
}
