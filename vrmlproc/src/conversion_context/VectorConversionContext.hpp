#pragma once

#include <vector>

#include "BaseConversionContext.hpp"

namespace vrml_proc {
	namespace conversion_context {

		template<typename T>
		class VectorConversionContext : public BaseConversionContext {

		public:
			
			VectorConversionContext() = default;
			virtual ~VectorConversionContext() = default;

			inline const std::vector<T>& GetData() const {
				return m_data;
			}

			inline void Add(T entity) {
				this->m_data.push_back(entity);
			}

		protected:
			std::vector<T> m_data;

		private:
			VectorConversionContext(const VectorConversionContext&) = delete;
			VectorConversionContext& operator=(const VectorConversionContext&) = delete;
			VectorConversionContext(VectorConversionContext&&) = delete;
			VectorConversionContext& operator=(VectorConversionContext&&) = delete;
		};
	}
}
