#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"
#include "StlBaseStructure.hpp"
#include "Vec3f.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 * 
		 * @implements ConversionContextAction
		 */
		class VRMLPROCESSING_API GroupAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {

		public:
			/**
			 * @brief Default constructor.
			 * 
			 * @param children vector of shared pointers which own objects inherited from `BaseConversionContext`
			 * @param bboxCenter `Vec3f` representing bboxCenter field in VRML 2.0 specification
			 * @param bboxSize `Vec3f` representing bboxSize field in VRML 2.0 specification
			 */
			GroupAction(std::vector<std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext>> children, std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxCenter, std::reference_wrapper<const vrml_proc::parser::Vec3f> bboxSize);
			/**
			 * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on `m_chidlren` member field.
			 * All children are merged together and returned as a pointer.
			 * 
			 * @returns shared pointer owning the object of merged ConversionContext objects
			 */
			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() override;

		private:
			std::vector<std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext>> m_children;
			std::reference_wrapper<const vrml_proc::parser::Vec3f> m_bboxCenter;
			std::reference_wrapper<const vrml_proc::parser::Vec3f> m_bboxSize;
		};
	}
}
