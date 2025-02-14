#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "Vec3f.hpp"
#include "VrmlNode.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace validator {
			class GroupNodeValidator : public NodeValidator {
			public:
				GroupNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
					: m_node(node), m_manager(manager) {}

				cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

					if (m_node.fields.empty()) {
						return {};
					}

					auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({ "children", "bboxCenter", "bboxSize"}, m_node.fields, m_node.header);
					if (fieldsResult.has_error()) {
						return fieldsResult;
					}

					// --------------------------------------------------------

					auto bboxSizeResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("bboxSize", m_node.fields);
					if (bboxSizeResult.has_error()) {
						return cpp::fail(bboxSizeResult.error());
					}

					m_bboxSize = bboxSizeResult.value();

					// --------------------------------------------------------

					auto bboxCenterResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("bboxCenter", m_node.fields);
					if (bboxCenterResult.has_error()) {
						return cpp::fail(bboxCenterResult.error());
					}

					m_bboxCenter = bboxCenterResult.value();

					// --------------------------------------------------------

					auto childrenResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeArrayWithValidation("children", m_node.fields, m_manager, true);
					if (childrenResult.has_error()) {
						return cpp::fail(childrenResult.error());
					}

					m_children = childrenResult.value();

					// --------------------------------------------------------

					return {};
				}

				std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> GetCachedChildren() {
					if (m_children.has_value()) {
						return m_children.value();
					}
					return {};
				}

				std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedBoxSize(const vrml_proc::parser::Vec3f& defaultValue) {
					if (m_bboxSize.has_value()) {
						return m_bboxSize.value();
					}

					return std::cref(defaultValue);
				}

				std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedBoxCenter(const vrml_proc::parser::Vec3f& defaultValue) {
					if (m_bboxCenter.has_value()) {
						return m_bboxCenter.value();
					}

					return std::cref(defaultValue);
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_children = std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_bboxSize = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
				std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_bboxCenter = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
			};
		}
	}
}
