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

			class CoordinateNodeValidator : public NodeValidator {
			public:

				CoordinateNodeValidator(const vrml_proc::parser::VrmlNode& node)
					: m_node(node) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					//LOG_INFO() << "Validate Coordinate VRML node.";

					if (m_node.fields.empty()) {
						//LOG_INFO() << "No fields were detected in the node.";
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "point" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						//LOG_ERROR() << "Node contains field with invalid name!";
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto pointResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3fArray>("point", m_node.fields);
					if (pointResult.has_error()) {
						//LOG_ERROR() << "Field <point> could not be extracted from the VRML node!";
						return cpp::fail(pointResult.error());
					}

					m_point = pointResult.value();

					//LOG_INFO() << "Validation was successfull. Data are cached.";
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::Vec3fArray> GetCachedPoint(const vrml_proc::parser::Vec3fArray& defaultValue) {
					if (m_point.has_value()) {
						return m_point.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>> m_point = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>{};
			};
		}
	}
}

