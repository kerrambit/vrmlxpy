#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace vrml_proc {
	namespace core {
		namespace error {

			class Error {
			public:
				virtual ~Error() = default;

				virtual std::string GetMessage() const {

					std::ostringstream stream;
					stream << GetMessageInternal();

					if (m_innerError != nullptr) {
						m_innerError->m_indentation = m_indentation + 1;
						stream << std::string(m_innerError->m_indentation, '\t') << m_innerError->GetMessage();
						m_innerError->m_indentation = 0;
					}

					return stream.str();
				}
				void SetInnerError(std::shared_ptr<Error> inner) {
					m_innerError = inner;
				}
				const std::shared_ptr<Error> GetInnerError() const {
					return m_innerError;
				}
			protected:
				virtual std::string GetMessageInternal() const = 0;
				size_t m_indentation = 0;
			private:
				std::shared_ptr<Error> m_innerError = nullptr;
			};

		}
	}
}
