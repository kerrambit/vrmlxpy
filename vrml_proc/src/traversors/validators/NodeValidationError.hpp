#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>

#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

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

				template<typename T>
				bool CheckErrorType(const NodeValidationError& error) {
					return boost::get<T>(&error) != nullptr;
				}

				template<typename T>
				std::optional<T> ExtractError(const NodeValidationError& error) {
					if (CheckErrorType<T>(error)) {
						return boost::get<T>(&error);
					}
					else {
						return {};
					}
				}
			}
		}
	}
}
