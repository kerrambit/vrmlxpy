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

			inline bool IsEmpty() const {
				return m_data.empty();
			}

			void Merge(Mergeable* other) override {

				auto* otherContext = dynamic_cast<VectorConversionContext<T>*>(other);
				if (otherContext) {
					m_data.insert(m_data.end(), std::make_move_iterator(otherContext->m_data.begin()), std::make_move_iterator(otherContext->m_data.end()));
					otherContext->m_data.clear();
				}
				else {
					// Wish not to use exceptions, this could be replaces by returning bool, or Result type in the future
					throw std::runtime_error("Merge called with incompatible type!");
				}
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
