#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <result.hpp>

#include "Int32Array.hpp"
#include "Logger.hpp"
#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "FormatString.hpp"
#include "VrmlUnits.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {

			class IndexedFaceSetNodeValidator : public NodeValidator {
			public:

				IndexedFaceSetNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					//LOG_INFO() << "Validate IndexedFaceSet VRML node.";

					if (m_node.fields.empty()) {
						//LOG_INFO() << "No fields were detected in the node.";
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "coord",
						"color",
						"normal",
						"texCoord",
						"ccw",
						"colorIndex", 
						"colorPerVertex",
						"convex",
						"coordIndex",
						"creaseAngle",
						"normalIndex",
						"normalPerVertex",
						"solid",
						"texCoordIndex" }, m_node.fields, m_node.header);

					if (fieldsResult.has_error()) {
						//LOG_ERROR() << "Node contains field with invalid name!";
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto coordResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("coord", m_node.fields, m_manager);
					if (coordResult.has_error()) {
						//LOG_ERROR() << "Field <coord> could not be extracted from the VRML node!";
						return cpp::fail(coordResult.error());
					}

					if (coordResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Coordinate" }, coordResult.value().value().get(), "coord");
						if (headerResult.has_error()) {
							//OG_ERROR() << "VRML node <" << coordResult.value().value().get().header << "> is not valid node for <coord> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_coord = coordResult.value();

					// --------------------------------------------------------

					auto colorResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("color", m_node.fields, m_manager);
					if (colorResult.has_error()) {
						//LOG_ERROR() << "Field <color> could not be extracted from the VRML node!";
						return cpp::fail(colorResult.error());
					}

					if (colorResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Color" }, colorResult.value().value().get(), "color");
						if (headerResult.has_error()) {
							//LOG_ERROR() << "VRML node <" << colorResult.value().value().get().header << "> is not valid node for <color> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_color = colorResult.value();

					// --------------------------------------------------------

					auto normalResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("normal", m_node.fields, m_manager);
					if (normalResult.has_error()) {
						//LOG_ERROR() << "Field <normal> could not be extracted from the VRML node!";
						return cpp::fail(normalResult.error());
					}

					if (normalResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "Normal" }, colorResult.value().value().get(), "normal");
						if (headerResult.has_error()) {
							//LOG_ERROR() << "VRML node <" << normalResult.value().value().get().header << "> is not valid node for <normal> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_normal = normalResult.value();

					// --------------------------------------------------------

					auto texCoordResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation("texCoord", m_node.fields, m_manager);
					if (texCoordResult.has_error()) {
						//LOG_ERROR() << "Field <texCoord> could not be extracted from the VRML node!";
						return cpp::fail(texCoordResult.error());
					}

					if (texCoordResult.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders({ "TextureCoordinate" }, colorResult.value().value().get(), "texCoord");
						if (headerResult.has_error()) {
							//LOG_ERROR() << "VRML node <" << texCoordResult.value().value().get().header << "> is not valid node for <texCoord> field.";
							return cpp::fail(headerResult.error());
						}
					}

					m_texCoord = texCoordResult.value();

					// --------------------------------------------------------

					auto ccwResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>("ccw", m_node.fields);
					if (ccwResult.has_error()) {
						vrml_proc::core::logger::LogError("Field <ccw> could not be extracted from the VRML node!", LOGGING_INFO);
						return cpp::fail(ccwResult.error());
					}

					m_ccw = ccwResult.value();

					// --------------------------------------------------------

					auto colorIndexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Int32Array>("colorIndex", m_node.fields);
					if (colorIndexResult.has_error()) {
						//LOG_ERROR() << "Field <colorIndex> could not be extracted from the VRML node!";
						return cpp::fail(colorIndexResult.error());
					}

					m_colorIndex = colorIndexResult.value();

					// --------------------------------------------------------

					auto colorPerVertexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>("colorPerVertex", m_node.fields);
					if (colorPerVertexResult.has_error()) {
						//LOG_ERROR() << "Field <colorPerVertex> could not be extracted from the VRML node!";
						return cpp::fail(colorPerVertexResult.error());
					}

					m_colorPerVertex = colorPerVertexResult.value();

					// --------------------------------------------------------

					auto convexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>("convex", m_node.fields);
					if (convexResult.has_error()) {
						//LOG_ERROR() << "Field <convex> could not be extracted from the VRML node!";
						return cpp::fail(convexResult.error());
					}

					m_convex = convexResult.value();

					// --------------------------------------------------------

					auto coordIndexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Int32Array>("coordIndex", m_node.fields);
					if (coordIndexResult.has_error()) {
						//LOG_ERROR() << "Field <coordIndex> could not be extracted from the VRML node!";
						return cpp::fail(coordIndexResult.error());
					}

					m_coordIndex = coordIndexResult.value();

					// --------------------------------------------------------

					auto creaseAngleResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::float32_t>("creaseAngle", m_node.fields);
					if (creaseAngleResult.has_error()) {
						//LOG_ERROR() << "Field <creaseAngle> could not be extracted from the VRML node!";
						return cpp::fail(creaseAngleResult.error());
					}

					m_creaseAngle = creaseAngleResult.value();

					// --------------------------------------------------------

					auto normalIndexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Int32Array>("normalIndex", m_node.fields);
					if (normalIndexResult.has_error()) {
						//LOG_ERROR() << "Field <normalIndex> could not be extracted from the VRML node!";
						return cpp::fail(normalIndexResult.error());
					}

					m_normalIndex = normalIndexResult.value();

					// --------------------------------------------------------

					auto normalPerVertexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>("normalPerVertex", m_node.fields);
					if (normalPerVertexResult.has_error()) {
						//LOG_ERROR() << "Field <normalPerVertex> could not be extracted from the VRML node!";
						return cpp::fail(normalPerVertexResult.error());
					}

					m_normalPerVertex = normalPerVertexResult.value();

					// --------------------------------------------------------

					auto solidResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>("solid", m_node.fields);
					if (solidResult.has_error()) {
						//LOG_ERROR() << "Field <solid> could not be extracted from the VRML node!";
						return cpp::fail(solidResult.error());
					}

					m_solid = solidResult.value();

					// --------------------------------------------------------

					auto texCoordIndexResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Int32Array>("texCoordIndex", m_node.fields);
					if (texCoordIndexResult.has_error()) {
						//LOG_ERROR() << "Field <texCoordIndex> could not be extracted from the VRML node!";
						return cpp::fail(texCoordIndexResult.error());
					}

					m_texCoordIndex = texCoordIndexResult.value();

					// --------------------------------------------------------

					//LOG_INFO() << "Validation was successfull. Data are cached.";
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedCoord(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_coord.has_value()) {
						return m_coord.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedColor(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_color.has_value()) {
						return m_color.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedNormal(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_normal.has_value()) {
						return m_normal.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetCachedTexCoord(const vrml_proc::parser::VrmlNode& defaultValue) {
					if (m_texCoord.has_value()) {
						return m_texCoord.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const bool> GetCachedCcw(const bool& defaultValue) {
					if (m_ccw.has_value()) {
						return m_ccw.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::Int32Array> GetCachedColorIndex(const vrml_proc::parser::Int32Array& defaultValue) {
					if (m_colorIndex.has_value()) {
						return m_colorIndex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const bool> GetCachedColorPerVertex(const bool& defaultValue) {
					if (m_colorPerVertex.has_value()) {
						return m_colorPerVertex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const bool> GetCachedConvex(const bool& defaultValue) {
					if (m_convex.has_value()) {
						return m_convex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::Int32Array> GetCachedCoordIndex(const vrml_proc::parser::Int32Array& defaultValue) {
					if (m_coordIndex.has_value()) {
						return m_coordIndex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::float32_t> GetCachedCreaseAngle(const vrml_proc::parser::float32_t& defaultValue) {
					if (m_creaseAngle.has_value()) {
						return m_creaseAngle.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::Int32Array> GetCachedNormalIndex(const vrml_proc::parser::Int32Array& defaultValue) {
					if (m_normalIndex.has_value()) {
						return m_normalIndex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const bool> GetCachedNormalPerVertex(const bool& defaultValue) {
					if (m_normalPerVertex.has_value()) {
						return m_normalPerVertex.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const bool> GetCachedSolid(const bool& defaultValue) {
					if (m_solid.has_value()) {
						return m_solid.value();
					}
					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::Int32Array> GetCachedTexCoordIndex(const vrml_proc::parser::Int32Array& defaultValue) {
					if (m_texCoordIndex.has_value()) {
						return m_texCoordIndex.value();
					}
					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_coord = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_color = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_normal = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> m_texCoord = std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>{};

				std::optional<std::reference_wrapper<const bool>> m_ccw = std::optional<std::reference_wrapper<const bool>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>> m_colorIndex = std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>{};
				std::optional<std::reference_wrapper<const bool>> m_colorPerVertex = std::optional<std::reference_wrapper<const bool>>{};
				std::optional<std::reference_wrapper<const bool>> m_convex = std::optional<std::reference_wrapper<const bool>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>> m_coordIndex = std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>> m_creaseAngle = std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>> m_normalIndex = std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>{};
				std::optional<std::reference_wrapper<const bool>> m_normalPerVertex = std::optional<std::reference_wrapper<const bool>>{};
				std::optional<std::reference_wrapper<const bool>> m_solid = std::optional<std::reference_wrapper<const bool>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>> m_texCoordIndex = std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>{};
			};
		}
	}
}
