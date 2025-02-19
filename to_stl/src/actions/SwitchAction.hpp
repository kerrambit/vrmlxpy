#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {

		class VRMLPROCESSING_API SwitchAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {

		public:

			struct SwitchProperties {
				std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> choice;
			};

			SwitchAction(SwitchProperties properties);

			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() override;

		private:
			SwitchProperties m_properties;
		};
	}
}
