#pragma once

#include <memory>

#include "BaseConversionContextAction.hpp"
#include "MeshConversionContext.hpp"
#include "StlBaseStructure.hpp"

namespace vrml_proc {
	namespace action {
		/**
		 * @todo This class will be deleted.
		 */
		class SpotlightAction : public BaseConversionContextAction {

		public:
			SpotlightAction(float ambientIntensity) :
				m_ambientIntensity(ambientIntensity) {}

			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() override {
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
