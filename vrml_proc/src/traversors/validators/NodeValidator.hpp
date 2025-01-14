#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <optional>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

#include <result.hpp>

#include "VrmlField.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			namespace error {

				struct InvalidVrmlFieldName {
					std::string invalidName;

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

				using NodeValidationError = boost::variant<InvalidVrmlFieldName, DuplicatedVrmlFieldName, InvalidFieldValueType, MissingDefNodeForUseNode, InvalidVrmlNodeForGivenField>;

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
			};
		}
	}
}
