#pragma once

#include <memory>

#include "BaseConversionContext.hpp"

namespace vrml_proc {
	namespace action {

		/**
		 * @brief Base abstract class representing Action object. The Action defines an interface
		 * with method called Execute() which returns a shared pointer owning a BaseConversionContext object.
		 * The Action object is encapsulation of a "program" or a "function" which returns a BaseConversionContext object.
		 */
		template<typename ConversionContext>
		class BaseConversionContextAction {
		public:
			/**
			 * @brief Virtual default destructor.
			 */
			virtual ~BaseConversionContextAction() = default;
			/**
			 * @brief Pure virtual method which executes on the given Action object.
			 * 
			 * @returns Shared pointer containing the ConversionContext object (of type base class BaseConversionContext).
			 */
			virtual std::shared_ptr<ConversionContext> Execute() = 0;
		protected:
			BaseConversionContextAction() = default;
		private:
			BaseConversionContextAction(const BaseConversionContextAction&) = delete;
			BaseConversionContextAction& operator=(const BaseConversionContextAction&) = delete;
			BaseConversionContextAction(BaseConversionContextAction&&) = delete;
			BaseConversionContextAction& operator=(BaseConversionContextAction&&) = delete;
		};
	}
}
