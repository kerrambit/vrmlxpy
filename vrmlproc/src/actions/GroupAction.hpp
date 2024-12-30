#pragma once

#include <memory>
#include <vector>

#include "BaseConversionContextAction.hpp"
#include "StlBaseStructure.hpp"
#include "MeshConversionContext.hpp"
#include "BaseConversionContext.hpp"
#include "Vec3f.hpp"

namespace vrml_proc {
	namespace action {

		class GroupAction : public BaseConversionContextAction {

		public:
			GroupAction(std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>> children, vrml_proc::parser::Vec3f bboxCenter, vrml_proc::parser::Vec3f bboxSize) :
				m_children(children), m_bboxCenter(bboxCenter), m_bboxSize(bboxSize) {}

			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() override {

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

		private:
			std::vector<std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext>> m_children;
			vrml_proc::parser::Vec3f m_bboxCenter;
			vrml_proc::parser::Vec3f m_bboxSize;
		};
	}
}
