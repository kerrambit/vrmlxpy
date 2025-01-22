#pragma once

#include <vector>
#include <optional>
#include <functional>

#include <result.hpp>

#include "NodeValidator.hpp"
#include "VrmlNode.hpp"
#include <memory>
#include "NodeValidationError.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class ShapeNodeValidator : public NodeValidator {
			public:

				ShapeNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "appearance", "geometry" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					//auto appearanceResult = vrml_proc::traversor::validator::NodeValidator::ValidateVrmlNode("appearance", m_node.fields, m_manager);
					//if (appearanceResult.has_error()) {
					//	return cpp::fail(appearanceResult.error());
					//}
			
					//m_appearance = appearanceResult.value();

					//if (m_appearance.has_value()) {
					//	auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Appearance" }, m_appearance.value(), "appearance");
					//	if (fieldsResult.has_error()) {
					//		return fieldsResult;
					//	}
					//}

					//// --------------------------------------------------------

					//auto geometryResult = vrml_proc::traversor::validator::NodeValidator::ValidateVrmlNode("geometry", m_node.fields, m_manager);
					//if (geometryResult.has_error()) {
					//	return cpp::fail(geometryResult.error());
					//}

					//m_geometry = geometryResult.value();

					//if (m_geometry.has_value()) {
					//	auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Box",
					//																		"Cone", "Cylinder", "ElevationGrid", "Extrusion",
					//																		"IndexedLineSet", "IndexedFaceSet", "PointSet",
					//																		"Sphere", "Text" }, m_geometry.value(), "appearance");
					//	if (fieldsResult.has_error()) {
					//		return fieldsResult;
					//	}
					//}

					//return {};
				}

				vrml_proc::parser::VrmlNode& GetCachedAppearance(vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_appearance.has_value()) {
						return m_appearance.value().get();
					}

					return defaultValue;
				}

				vrml_proc::parser::VrmlNode& GetCachedGeometry(vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_geometry.has_value()) {
						return m_geometry.value().get();
					}

					return defaultValue;
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>> m_appearance = std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>>{};
				std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>> m_geometry = std::optional<std::reference_wrapper<vrml_proc::parser::VrmlNode>>{};
			};
		}
	}
}
