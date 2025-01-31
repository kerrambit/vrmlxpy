#pragma once

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include <result.hpp>

#include "Logger.hpp"
#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "Vec3fArray.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class NormalNodeValidator : public NodeValidator {
			public:

				NormalNodeValidator(const vrml_proc::parser::VrmlNode& node)
					: m_node(node) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					LOG_INFO() << "Validate Normal VRML node.";

					if (m_node.fields.empty()) {
						LOG_INFO() << "No fields were detected in the node.";
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "vector" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						LOG_ERROR() << "Node contains field with invalid name!";
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto vectorResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3fArray>("vector", m_node.fields);
					if (vectorResult.has_error()) {
						LOG_ERROR() << "Field <vector> could not be extracted from the VRML node!";
						return cpp::fail(vectorResult.error());
					}

					m_vector = vectorResult.value();

					LOG_INFO() << "Validation was successfull. Data are cached.";
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::Vec3fArray> GetCachedVector(const vrml_proc::parser::Vec3fArray& defaultValue) {
					if (m_vector.has_value()) {
						return m_vector.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>> m_vector = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>{};
			};
		}
	}
}

