#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshTaskConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace to_stl {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 *
		 * @implements ConversionContextAction
		 */
		class VRMLPROCESSING_API ShapeAction : public vrml_proc::action::ConversionContextAction<to_stl::conversion_context::MeshTaskConversionContext> {

		public:
			/**
			 * @brief Default constructor.
			 *
			 * @param children vector of shared pointers which own objects inherited from `BaseConversionContext`
			 * @param appearance data representing appearance of the shape
			 * @param geometry data representing geometry of the shape
			 */
			ShapeAction(std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> appearance, std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> geometry);
			/**
			 * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on `m_geometry` member field.
			 *
			 * @returns shared pointer owning the object of merged ConversionContext objects
			 */
			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> Execute() override;

		private:
			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> m_appearance;
			std::shared_ptr<to_stl::conversion_context::MeshTaskConversionContext> m_geometry;
		};
	}
}
