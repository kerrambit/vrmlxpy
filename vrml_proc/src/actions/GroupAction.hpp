#pragma once

#include <memory>
#include <vector>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextAction.hpp"
#include "MeshConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "Vec3f.hpp"

namespace vrml_proc {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 * 
		 * @implements BaseConversionContextAction
		 */
		class GroupAction : public BaseConversionContextAction<vrml_proc::conversion_context::MeshConversionContext> {

		public:
			/**
			 * @brief Default constructor.
			 * 
			 * @param children vector of shared pointers which own objects inherited from `BaseConversionContext`
			 * @param bboxCenter `Vec3f` representing bboxCenter field in VRML 2.0 specification
			 * @param bboxSize `Vec3f` representing bboxSize field in VRML 2.0 specification
			 */
			GroupAction(std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>> children, vrml_proc::parser::Vec3f bboxCenter, vrml_proc::parser::Vec3f bboxSize) :
				m_children(children), m_bboxCenter(bboxCenter), m_bboxSize(bboxSize) {}
			/**
			 * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on `m_chidlren` member field.
			 * All children are merged together and returned as a pointer.
			 * 
			 * @returns shared pointer owning the object of merged ConversionContext objects
			 */
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override {

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
			std::vector<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>> m_children;
			vrml_proc::parser::Vec3f m_bboxCenter;
			vrml_proc::parser::Vec3f m_bboxSize;
		};
	}
}
