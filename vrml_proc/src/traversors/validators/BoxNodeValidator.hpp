#pragma once

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "Vec3f.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class BoxNodeValidator : public NodeValidator {
			public:

				BoxNodeValidator(const vrml_proc::parser::VrmlNode& node)
					: m_node(node) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "size" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto sizeResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("size", m_node.fields);
					if (sizeResult.has_error()) {
						return cpp::fail(sizeResult.error());
					}

					m_size = sizeResult.value();

					return {};
				}

				const vrml_proc::parser::Vec3f& GetCachedSize(const vrml_proc::parser::Vec3f& defaultValue) {
					if (m_size.has_value()) {
						return m_size.value().get();
					}

					return defaultValue;
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;

				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_size = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
			};
		}
	}
}

