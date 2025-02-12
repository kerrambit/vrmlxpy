#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {
			class SwitchNodeValidator : public NodeValidator {
			public:
				SwitchNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "choice", "whichChoice" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto whichChoiceResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<int32_t>("whichChoice", m_node.fields);
					if (whichChoiceResult.has_error()) {
						return cpp::fail(whichChoiceResult.error());
					}

					m_whichChoice = whichChoiceResult.value();

					// --------------------------------------------------------

					auto choiceResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeArrayWithValidation("choice", m_node.fields, m_manager, true);
					if (choiceResult.has_error()) {
						return cpp::fail(choiceResult.error());
					}

					m_choice = choiceResult.value();

					// --------------------------------------------------------

					return {};
				}

				std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> GetCachedChoice() {
					if (m_choice.has_value()) {
						return m_choice.value();
					}
					return {};
				}

				std::reference_wrapper<const int32_t> GetCachedWhichChoice(const int32_t& defaultValue) {
					if (m_whichChoice.has_value()) {
						return m_whichChoice.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_choice = std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>{};
				std::optional<std::reference_wrapper<const int32_t>> m_whichChoice = std::optional<std::reference_wrapper<const int32_t>>{};
			};
		}
	}
}
