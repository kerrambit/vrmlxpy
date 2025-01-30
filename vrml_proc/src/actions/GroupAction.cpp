#include "GroupAction.hpp"

#include <memory>
#include <vector>

#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "Vec3f.hpp"

namespace vrml_proc {
	namespace action {
		
		GroupAction::GroupAction(std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>> children, std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter, std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize) :
			m_children(children), m_bboxCenter(bboxCenter), m_bboxSize(bboxSize) {}

		std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> GroupAction::Execute() {

			LOG_INFO() << "Execute GroupAction.";

			auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
			for (const auto& child : m_children) {
				if (child != nullptr) {
					std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> meshContextPtr = std::dynamic_pointer_cast<vrml_proc::conversion_context::MeshConversionContext>(child);
					if (meshContextPtr != nullptr) {
						result->Merge(meshContextPtr.get());
						meshContextPtr.reset();
					}
				}
			}

			return result;
		}
	}
}
