#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 *
		 * @implements ConversionContextAction
		 */
		class VRMLPROCESSING_API SwitchAction : public vrml_proc::action::ConversionContextAction<vrml_proc::conversion_context::MeshConversionContext> {

		public:

			struct SwitchProperties {
				std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> choice;
			};

			SwitchAction(SwitchProperties properties);

			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			SwitchProperties m_properties;
		};
	}
}
