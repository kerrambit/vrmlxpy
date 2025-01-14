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
		class ShapeAction : public BaseConversionContextAction {

		public:
			/**
			 * @brief Default constructor.
			 *
			 * @param children vector of shared pointers which own objects inherited from `BaseConversionContext`
			 * @param appearance data representing appearance of the shape
			 * @param geometry data representing geometry of the shape
			 */
			ShapeAction(std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> appearance, std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> geometry) :
				m_appearance(appearance), m_geometry(geometry) {}
			/**
			 * @brief Overriden implemented interface method from `BaseConversionContextAction`. The method is focused only on `m_geometry` member field.
			 *
			 * @returns shared pointer owning the object of merged ConversionContext objects
			 */
			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() override {

				return m_geometry;
			}
		private:
			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> m_appearance;
			std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> m_geometry;
		};
	}
}
