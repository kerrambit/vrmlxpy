#pragma once

#include <memory>

#include "BaseConversionContextAction.hpp"
#include "StlBaseStructure.hpp"
#include "MeshConversionContext.hpp"

namespace vrml_proc {
	namespace action {

		class SpotlightAction : public BaseConversionContextAction {

		public:
			SpotlightAction(float ambientIntensity) :
				m_ambientIntensity(ambientIntensity) {}

			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() override {

				// Temporally code
				auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
				vrml_proc::conversion_context::StlBaseStructure entity = vrml_proc::conversion_context::StlBaseStructure({ 1.0, 1.0, 1.0 }, { {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} });
				result->Add(entity);
				return result;
			}

		private:
			float m_ambientIntensity = 0.0f;
		};
	}
}
