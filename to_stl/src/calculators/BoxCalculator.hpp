#pragma once

#include <memory>

#include <result.hpp>

#include "BoxAction.hpp"
#include "Error.hpp"
#include "Mesh.hpp"
#include "VrmlToStlExport.hpp"

namespace to_stl {
	namespace calculator {
		class VRMLTOSTL_API BoxCalculator {
		public:
			cpp::result<std::shared_ptr<core::Mesh>, std::shared_ptr<vrml_proc::core::error::Error>> Generate3DMesh(const vrml_proc::action::BoxAction::BoxProperties& properties);
		};
	}
}
