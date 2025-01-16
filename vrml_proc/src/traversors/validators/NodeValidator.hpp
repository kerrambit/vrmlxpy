#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <functional>
#include <optional>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <result.hpp>

#include "VrmlField.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlFieldExtractor.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			namespace error {

				struct InvalidVrmlFieldName {
					std::string invalidName;

					InvalidVrmlFieldName()
						: invalidName("") {}

					InvalidVrmlFieldName(std::string invalidName)
						: invalidName(invalidName) {}
				};

				struct InvalidVrmlNodeForGivenField {
					std::string field;
					std::string expectedHeaders;
					std::string actualHeader;

					InvalidVrmlNodeForGivenField(std::string field, std::string actualHeader, std::unordered_set<std::string> expectedHeaders)
						: field(field), actualHeader(actualHeader) {
						AddExpectedHeaders(expectedHeaders);
					}
					
					void AddExpectedHeaders(const std::unordered_set<std::string>& headers) {
						std::ostringstream stream;
						for (auto it = headers.begin(); it != headers.end(); ++it) {
							if (it != headers.begin()) {
								stream << ", ";
							}
							stream << *it;
						}
						expectedHeaders = stream.str();
					}
				};

				struct DuplicatedVrmlFieldName {
					std::string duplicatedName;

					DuplicatedVrmlFieldName(std::string duplicatedName)
						: duplicatedName(duplicatedName) {}
				};

				struct InvalidFieldValueType {
					std::string expectedType;
					std::string actualType;

					InvalidFieldValueType(std::string expectedType, std::string actualType)
						: expectedType(expectedType), actualType(actualType) {}
				};

				struct MissingDefNodeForUseNode {
					std::string useNodeId;

					MissingDefNodeForUseNode(std::string useNodeId)
						: useNodeId(useNodeId) {}
				};

				struct InvalidVrmlNodeVector {
					boost::variant<InvalidFieldValueType, MissingDefNodeForUseNode> innerError;

					InvalidVrmlNodeVector(boost::variant<InvalidFieldValueType, MissingDefNodeForUseNode> innerError)
						: innerError(std::move(innerError)) {}
				};

				using NodeValidationError = boost::variant<InvalidVrmlFieldName, DuplicatedVrmlFieldName, InvalidFieldValueType, MissingDefNodeForUseNode, InvalidVrmlNodeForGivenField, InvalidVrmlNodeVector>;

				/*template<typename T>
				bool CheckErrorType(const NodeValidationError& error) {
					return boost::get<T>(&error) != nullptr;
				}

				template<typename T>
				std::optional<T> ExtractError(const NodeValidationError& error) {
					if (vrml_proc::traversor::validator::error::CheckErrorType<T>(error)) {
						return boost::get<T>(&error);
					}
					else {
						return {};
					}
				}*/
			}

			class NodeValidator {
			public:

				virtual cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> Validate() = 0;

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

				static cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> CheckForOnlyAllowedVrmlNodeHeaders(const std::unordered_set<std::string>& validHeaders, const vrml_proc::parser::VrmlNode& node, const std::string& field) {

					if (validHeaders.find(node.header) == validHeaders.end()) {
						return cpp::fail(vrml_proc::traversor::validator::error::InvalidVrmlNodeForGivenField(field, node.header, validHeaders));
					}					

					return {};
				}

				/**
				 * @brief Validates field entry. Field should be of type VrmlNode (or UseNode which is evaluated to VrmlNode in the aftermath).
				 * 
				 * @param name name of the field
				 * @param fields list of all node's fields
				 * @param manager manager where associations between UseNodes and VrmlNodes will be looked for
				 * 
				 * @returns NodeValidationError if failure occurs (invalid field type, missing VrmlNode for UseNode id); otherwise it returns empty optional (field is missing) or optional containing reference to found VrmlNode
				 */
				static cpp::result<std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>>, vrml_proc::traversor::validator::error::NodeValidationError> ValidateVrmlNode(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager) {

					std::string invalidType = ""; std::string missingUseId = "";
					auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeExtended(name, fields, manager, invalidType, missingUseId);

					if (value.has_error()) {
						if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound) {
							return std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>>{};
						}
						if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("VrmlNode", invalidType));
						}
						if (value.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
							return cpp::fail(vrml_proc::traversor::validator::error::MissingDefNodeForUseNode(missingUseId));
						}
					}

					return std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>>(value.value());
				}

				/*template<typename T>
				static cpp::result<std::optional<std::reference_wrapper<T>>, vrml_proc::traversor::validator::error::NodeValidationError> ValidateField(const std::string& name, const std::vector<vrml_proc::parser::VrmlField>& fields, const vrml_proc::parser::VrmlNodeManager& manager) {

					std::string invalidType = "";
					auto value = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameExtended<T>(name, fields, manager, invalidType);

				}*/
			};
		}
	}
}
