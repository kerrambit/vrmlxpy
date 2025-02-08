#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <result.hpp>

#include "Logger.hpp"
#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class ShapeNodeValidator : public NodeValidator {
			public:

				ShapeNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					//LOG_INFO() << "Validate Shape VRML node.";

					if (m_node.fields.empty()) {
						//LOG_INFO() << "No fields were detected in the node.";
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "appearance", "geometry" }, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						//LOG_ERROR() << "Node contains field with invalid name!";
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto appearanceResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("appearance", m_node.fields, m_manager);
					if (appearanceResult.has_error()) {
						//LOG_ERROR() << "Field <appearance> could not be extracted from the VRML node!";
						return cpp::fail(appearanceResult.error());
					}

					if (appearanceResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Appearance" }, appearanceResult.value().value().get(), "appearance");
						if (headerResult.has_error()) {
							//LOG_ERROR() << "VRML node <" << appearanceResult.value().value().get().header << "> is not valid node for <appearance> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_appearance = appearanceResult.value();

					// --------------------------------------------------------

					auto geometryResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("geometry", m_node.fields, m_manager);
					if (geometryResult.has_error()) {
						//LOG_ERROR() << "Field <geometry> could not be extracted from the VRML node!";
						return cpp::fail(geometryResult.error());
					}

					if (geometryResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Box", "Cone", "Cylinder", "ElevationGrid", "Extrusion", "IndexedFaceSet", "IndexedLineSet", "PointSet", "Sphere", "Text" }, geometryResult.value().value().get(), "geometry");
						if (headerResult.has_error()) {
							//LOG_ERROR() << "VRML node <" << geometryResult.value().value().get().header << "> is not valid node for <geometry> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_geometry = geometryResult.value();

					//LOG_INFO() << "Validation was successfull. Data are cached.";
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedAppearance(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_appearance.has_value()) {
						return m_appearance.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedGeometry(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_geometry.has_value()) {
						return m_geometry.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_appearance = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_geometry = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
			};
		}
	}
}
