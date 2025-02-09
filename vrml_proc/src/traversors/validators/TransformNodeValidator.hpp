#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include <result.hpp>

#include "NodeValidationError.hpp"
#include "NodeValidator.hpp"
#include "Vec3f.hpp"
#include "Vec4f.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc::traversor::validator {

	class TransformNodeValidator : public NodeValidator {
	public:
		TransformNodeValidator(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
			: m_node(node), m_manager(manager) {}

		cpp::result<void, std::shared_ptr<error::NodeValidationError>> Validate() override {

			if (m_node.fields.empty()) {
				return {};
			}

			auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames({
				"children",
				"bboxCenter",
				"bboxSize",
				"center",
				"rotation",
				"scale",
				"scaleOrientation",
				"translation"
				}, m_node.fields, m_node.header);

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

			auto childrenResult = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeArrayWithValidation("children", m_node.fields, m_manager);
			if (childrenResult.has_error()) {
				return cpp::fail(childrenResult.error());
			}

			m_children = childrenResult.value();

			// --------------------------------------------------------

			auto centerResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("center", m_node.fields);
			if (centerResult.has_error()) {
				return cpp::fail(centerResult.error());
			}

			m_center = centerResult.value();

			// --------------------------------------------------------

			auto rotationResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec4f>("rotation", m_node.fields);
			if (rotationResult.has_error()) {
				return cpp::fail(rotationResult.error());
			}

			m_rotation = rotationResult.value();

			// --------------------------------------------------------

			auto scaleResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("scale", m_node.fields);
			if (scaleResult.has_error()) {
				return cpp::fail(scaleResult.error());
			}

			m_scale = scaleResult.value();

			// --------------------------------------------------------

			auto scaleOrientationResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec4f>("scaleOrientation", m_node.fields);
			if (scaleOrientationResult.has_error()) {
				return cpp::fail(scaleOrientationResult.error());
			}

			m_scaleOrientation = scaleOrientationResult.value();

			// --------------------------------------------------------

			auto translationResult = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>("translation", m_node.fields);
			if (translationResult.has_error()) {
				return cpp::fail(translationResult.error());
			}

			m_translation = translationResult.value();

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

		std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedCenter(const vrml_proc::parser::Vec3f& defaultValue) {
			if (m_center.has_value()) {
				return m_center.value();
			}

			return std::cref(defaultValue);
		}

		std::reference_wrapper<const vrml_proc::parser::Vec4f> GetCachedRotation(const vrml_proc::parser::Vec4f& defaultValue) {
			if (m_rotation.has_value()) {
				return m_rotation.value();
			}

			return std::cref(defaultValue);
		}

		std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedScale(const vrml_proc::parser::Vec3f& defaultValue) {
			if (m_scale.has_value()) {
				return m_scale.value();
			}

			return std::cref(defaultValue);
		}

		std::reference_wrapper<const vrml_proc::parser::Vec4f> GetCachedScaleOrientation(const vrml_proc::parser::Vec4f& defaultValue) {
			if (m_scaleOrientation.has_value()) {
				return m_scaleOrientation.value();
			}

			return std::cref(defaultValue);
		}

		std::reference_wrapper<const vrml_proc::parser::Vec3f> GetCachedTranslation(const vrml_proc::parser::Vec3f& defaultValue) {
			if (m_translation.has_value()) {
				return m_translation.value();
			}

			return std::cref(defaultValue);
		}

	private:
		const vrml_proc::parser::VrmlNode& m_node;
		const vrml_proc::parser::VrmlNodeManager& m_manager;

		std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_children = std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_bboxSize = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_bboxCenter = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_center = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>> m_rotation = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_scale = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>> m_scaleOrientation = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>{};
		std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>> m_translation = std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>{};
	};
}
