#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "TypeToString.hpp"
#include "VrmlField.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {
			/**
			 * @brief Class representing a generic NodeValidator. Subclasses have to implement Validate() method.
			 * The interface offers also several static helper methods.
			 * Static methods work as bridge between raw outputs of VrmlFieldExtractor and nicely formatted output of NodeValidationError entities.
			 */
			class NodeValidator {
			public:

				/**
				 * @brief Pure virtual method. Method validates an entity and returns NodeValidationError.
				 * 
				 * @returns result type representing NodeValidationError
				 */
				virtual cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> Validate() = 0;

				/**
				 * @brief Static helper method which checks that all field names are from the given list of allowed names. Method also
				 * verifies that each field name is at most once and that no unknown field name is used either.
				 * 
				 * @param validFieldNames set of all allowed field names
				 * @param fields list of fields to verify
				 * @returns result type where error stores the NodeValidationError subtype
				 */
				static cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> CheckForOnlyUniqueAllowedFieldNames(const std::unordered_set<std::string>& validFieldNames, const std::vector<vrml_proc::parser::VrmlField>& fields) {
					
					std::unordered_set<std::string> alreadyFoundFieldNames;
					for (const auto& field : fields) {
						if (validFieldNames.find(field.name) == validFieldNames.end()) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidVrmlFieldName(field.name));
						}

						if (alreadyFoundFieldNames.find(field.name) != alreadyFoundFieldNames.end()) {
							return cpp::fail(vrml_proc::traversor::validator::error::DuplicatedVrmlFieldName(field.name));
						}
						alreadyFoundFieldNames.insert(field.name);
					}
					return {};
				}

				/**
				 * @brief Static helper method which checks if node has valid header. Basically this method checks the "kind" of generic VRML node.
				 * 
				 * @param validHeaders set of allowed header names
				 * @param node node to verify
				 * @param field field which node belongs to in the parent node
				 * 
				 * @returns result type where error stores the NodeValidationError subtype
				 */
				static cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> CheckForOnlyAllowedVrmlNodeHeaders(const std::unordered_set<std::string>& validHeaders, const vrml_proc::parser::VrmlNode& node, const std::string& field) {

					if (validHeaders.find(node.header) == validHeaders.end()) {
						return cpp::fail(vrml_proc::traversor::validator::error::InvalidVrmlNodeForGivenField(field, node.header, validHeaders));
					}					

					return {};
				}

				/**
				 * @brief Static helper method which validates field entry. Field should be of type VrmlNode (or UseNode which is evaluated to VrmlNode in the aftermath).
				 * 
				 * @param name name of the field
				 * @param fields list of all node's fields
				 * @param manager manager where associations between UseNodes and VrmlNodes will be searched for
				 * 
				 * @returns NodeValidationError if failure occurs (invalid field type, missing VrmlNode for UseNode id); otherwise it returns empty optional (field is missing) or optional containing const reference to VRML node
				 */
				static cpp::result<std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>, vrml_proc::traversor::validator::error::NodeValidationError> ExtractVrmlNodeWithValidation(const std::string& fieldName, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager) {

					std::string invalidType = ""; std::string missingUseId = "";
					auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(fieldName, fields, manager, invalidType, missingUseId);

					if (value.has_error()) {
						if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound) {
							return std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
						}
						else if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType(vrml_proc::core::utils::TypeToString<vrml_proc::parser::VrmlNode>() + " or " + vrml_proc::core::utils::TypeToString<vrml_proc::parser::UseNode>(), invalidType));
						}
						else if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
							return cpp::fail(vrml_proc::traversor::validator::error::MissingDefNodeForUseNode(missingUseId));
						}
					}

					return std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(value.value());
				}

				/**
				 * @brief Static helper method which validates field entry. Field type is templated.
				 *
				 * @param name name of the field
				 * @param fields list of all node's fields
				 *
				 * @returns NodeValidationError if failure occurs (invalid field type); otherwise it returns empty optional (field is missing) or optional containing const reference to T
				 */
				template<typename T>
				static cpp::result<std::optional<std::reference_wrapper<const T>>, vrml_proc::traversor::validator::error::NodeValidationError> ExtractFieldByNameWithValidation(const std::string& fieldName, const std::vector<vrml_proc::parser::VrmlField>& fields) {

					std::string invalidType = "";
					auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<T>(fieldName, fields, invalidType);

					if (value.has_error()) {
						if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::FieldNotFound) {
							return std::optional<std::reference_wrapper<const T>>{};
						}
						else if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType(vrml_proc::core::utils::TypeToString<T>(), invalidType));
						}
					}

					return std::optional<std::reference_wrapper<const T>>(value.value());
				}
			};
		}
	}
}
