#pragma once

#include "Mergeable.hpp"

namespace vrml_proc {
	namespace conversion_context {

		class BaseConversionContext : public Mergeable {
		public:
			virtual ~BaseConversionContext() = default;
		protected:
			BaseConversionContext() = default;
		private:
			BaseConversionContext(const BaseConversionContext&) = delete;
			BaseConversionContext& operator=(const BaseConversionContext&) = delete;
			BaseConversionContext(BaseConversionContext&&) = delete;
			BaseConversionContext& operator=(BaseConversionContext&&) = delete;
		};
	}
}
