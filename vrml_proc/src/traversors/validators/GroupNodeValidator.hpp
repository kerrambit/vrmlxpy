#pragma once

#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <result.hpp>

#include "FullParsedVrmlNodeContext.hpp"
#include "NodeValidator.hpp"
#include "UseNode.hpp"
#include "Vec3f.hpp"
#include "VrmlFieldExtractor.hpp"
#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"
#include "NodeValidationError.hpp"
#include <memory>

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

					//// --------------------------------------------------------

					//m_children = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", m_node.fields);
					//if (m_children.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
					//	return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>", ""));
					//}
					//
					//if (m_children.has_value()) {

					//	for (const auto& child : m_children.value()) {

					//		auto vrmlNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);

					//		auto useNode = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(child);
					//		if (useNode.has_value()) {

					//			auto managerFound = m_manager.GetDefinitionNode(useNode.value().get_pointer()->identifier);
					//			if (managerFound == nullptr) {
					//				return cpp::fail(vrml_proc::traversor::validator::error::InvalidVrmlNodeVector(boost::variant<vrml_proc::traversor::validator::error::InvalidFieldValueType, vrml_proc::traversor::validator::error::MissingDefNodeForUseNode>(vrml_proc::traversor::validator::error::MissingDefNodeForUseNode(""))));
					//			}
					//		}

					//		if (!vrmlNode.has_value() && !useNode.has_value()) {
					//			return cpp::fail(vrml_proc::traversor::validator::error::InvalidVrmlNodeVector(boost::variant<vrml_proc::traversor::validator::error::InvalidFieldValueType, vrml_proc::traversor::validator::error::MissingDefNodeForUseNode>(vrml_proc::traversor::validator::error::InvalidFieldValueType("vrml_proc::parser::VrmlNode || vrml_proc::parser::UseNode", "..."))));
					//		}
					//	}
					//}

					//// --------------------------------------------------------

					//m_bboxCenter = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxCenter", m_node.fields);
					//if (m_bboxCenter.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
					//	return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("Vec3f", "TODO"));
					//}

					//// --------------------------------------------------------

					//m_bboxSize = vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxSize", m_node.fields);
					//if (m_bboxSize.error() == vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError) {
					//	return cpp::fail(vrml_proc::traversor::validator::error::InvalidFieldValueType("Vec3f", "TODO"));
					//}

					//return {};
				}

				std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& GetCachedChildren(std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>& defaultValue) {
					if (m_children.has_value()) {
						return m_children.value();
					}
					return defaultValue;
				}

				vrml_proc::parser::Vec3f& GetCachedBoxSize(vrml_proc::parser::Vec3f& defaultValue) {
					if (m_bboxSize.has_value()) {
						return m_bboxSize.value();
					}

					return defaultValue;
				}

				vrml_proc::parser::Vec3f& GetCachedBoxCenter(vrml_proc::parser::Vec3f& defaultValue) {
					if (m_bboxCenter.has_value()) {
						return m_bboxCenter.value();
					}

					return defaultValue;
				}

			private:
				const vrml_proc::parser::VrmlNode& m_node;
				const vrml_proc::parser::VrmlNodeManager& m_manager;

				cpp::result<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>, vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError> m_children = cpp::fail(vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError);
				cpp::result<vrml_proc::parser::Vec3f, vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError> m_bboxSize = cpp::fail(vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError);
				cpp::result<vrml_proc::parser::Vec3f, vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError> m_bboxCenter = cpp::fail(vrml_proc::parser::model::utils::VrmlFieldExtractor::ExtractByNameError::ValidationError);
			};
		}
	}
}
