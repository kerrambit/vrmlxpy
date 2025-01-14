#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/variant.hpp>

#include <result.hpp>

#include "VrmlField.hpp"
#include "VrmlNode.hpp"
#include "NodeValidator.hpp"
#include "VrmlFieldExtractor.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class ShapeNodeValidator : public NodeValidator {
			public:

				ShapeNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, vrml_proc::traversor::validator::error::NodeValidationError> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "appearance", "geometry" }, m_node.fields);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					m_appearance = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNode("appearance", m_node.fields, m_manager);

					if (m_appearance.has_error() && m_appearance.error() != vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound) {
						if (m_appearance.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("VrmlNode", vrml_proc::parser::model::utils::VrmlFieldExtractor::StaticExtractByNameTypeValue));
						}
						if (m_appearance.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
							return cpp::fail(vrml_proc::traversor::validator::error::MissingDefNodeForUseNode(vrml_proc::parser::model::utils::VrmlFieldExtractor::StaticExtractVrmlNodeUseNodeId));
						}
					}

					if (m_appearance.has_value()) {
						auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Appearance" }, m_appearance.value(), "appearance");
						if (fieldsResult.has_error()) {
							return fieldsResult;
						}
					}

					// --------------------------------------------------------

					m_geometry = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNode("geometry", m_node.fields, m_manager);

					if (m_geometry.has_error() && m_geometry.error() != vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::FieldNotFound) {
						if (m_geometry.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError) {
							return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("VrmlNode", vrml_proc::parser::model::utils::VrmlFieldExtractor::StaticExtractByNameTypeValue));
						}
						if (m_geometry.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::UnknownUseNode) {
							return cpp::fail(vrml_proc::traversor::validator::error::MissingDefNodeForUseNode(vrml_proc::parser::model::utils::VrmlFieldExtractor::StaticExtractVrmlNodeUseNodeId));
						}
					}

					if (m_geometry.has_value()) {
						auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Box",
																							"Cone", "Cylinder", "ElevationGrid", "Extrusion",
																							"IndexedLineSet", "IndexedFaceSet", "PointSet",
																							"Sphere", "Text" }, m_geometry.value(), "appearance");
						if (fieldsResult.has_error()) {
							return fieldsResult;
						}
					}

					return {};
				}

				vrml_proc::parser::VrmlNode& GetCachedAppearance(vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_appearance.has_value()) {
						return m_appearance.value();
					}

					return defaultValue;
				}

				vrml_proc::parser::VrmlNode& GetCachedGeometry(vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_geometry.has_value()) {
						return m_geometry.value();
					}

					return defaultValue;
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				cpp::result<vrml_proc::parser::VrmlNode&, vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError> m_appearance = cpp::fail(vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError);
				cpp::result<vrml_proc::parser::VrmlNode&, vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError> m_geometry = cpp::fail(vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractVrmlNodeError::ValidationError);
			};
		}
	}
}
