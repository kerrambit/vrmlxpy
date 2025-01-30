#pragma once

#include <memory>
#include <vector>

#include "ConversionContextAction.hpp"
#include "MeshConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace action {
		/**
		 * @brief Represents a concrete type of Action.
		 *
		 * @implements ConversionContextAction
		 */
		class VRMLPROCESSING_API ShapeAction : public ConversionContextAction<vrml_proc::conversion_context::MeshConversionContext> {

		public:
			/**
			 * @brief Default constructor.
			 *
			 * @param children vector of shared pointers which own objects inherited from `BaseConversionContext`
			 * @param appearance data representing appearance of the shape
			 * @param geometry data representing geometry of the shape
			 */
			ShapeAction(std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> appearance, std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> geometry);
			/**
			 * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on `m_geometry` member field.
			 *
			 * @returns shared pointer owning the object of merged ConversionContext objects
			 */
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> Execute() override;

		private:
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> m_appearance;
			std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> m_geometry;
		};
	}
}
