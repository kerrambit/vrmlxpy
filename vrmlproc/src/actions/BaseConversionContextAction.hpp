#pragma once

#include <memory>

#include "BaseConversionContext.hpp"

namespace vrml_proc {
	namespace action {

		class BaseConversionContextAction {
		public:
			virtual ~BaseConversionContextAction() = default;
			virtual std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Execute() = 0;
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
