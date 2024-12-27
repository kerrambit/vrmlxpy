#pragma once

namespace vrml_proc {
	namespace conversion_context {

		class BaseConversionContext {
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
