#pragma once

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include <result.hpp>

#include "Logger.hpp"
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

					LOG_INFO() << "Validate Box VRML node.";

					if (m_node.fields.empty()) {
						LOG_INFO() << "No fields were detected in the node.";
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "size" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						LOG_ERROR() << "Node contains field with invalid name!";
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto sizeResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("size", m_node.fields);
					if (sizeResult.has_error()) {
						LOG_ERROR() << "Field <size> could not be extracted from the VRML node!";
						return cpp::fail(sizeResult.error());
					}

					m_size = sizeResult.value();

					LOG_INFO() << "Validation was successfull. Data are cached.";
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedSize(const vrml_proc::parser::Vec3f& defaultValue) {
					if (m_size.has_value()) {
						return m_size.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_size = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
			};
		}
	}
}

