#pragma once

#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class WorldInfoNodeValidator : public NodeValidator {
			public:

				WorldInfoNodeValidator(const vrml_proc::parser::VrmlNode& node)
					: m_node(node) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "info", "title"}, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto infoResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<std::string>("info", m_node.fields);
					if (infoResult.has_error()) {
						return cpp::fail(infoResult.error());
					}

					m_info = infoResult.value();

					auto titleResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<std::string>("title", m_node.fields);
					if (titleResult.has_error()) {
						return cpp::fail(titleResult.error());
					}

					m_title = titleResult.value();

					return {};
				}

				std::reference_wrapper<const std::string> GetCachedInfo(const std::string& defaultValue) {
					if (m_info.has_value()) {
						return m_info.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const std::string> GetCachedTitle(const std::string& defaultValue) {
					if (m_title.has_value()) {
						return m_title.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;

				std::optional<std::reference_wrapper<const std::string>> m_info = std::optional<std::reference_wrapper<const std::string>>{};
				std::optional<std::reference_wrapper<const std::string>> m_title = std::optional<std::reference_wrapper<const std::string>>{};
			};
		}
	}
}

