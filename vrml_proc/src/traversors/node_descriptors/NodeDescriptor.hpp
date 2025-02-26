#pragma once

#include <string>
#include <unordered_set>
#include <memory>
#include <optional>
#include <functional>
#include <map>
#include <vector>

#include <result.hpp>

#include "Vec3f.hpp"
#include "Vec3fArray.hpp"
#include "VrmlUnits.hpp"
#include "Vec4f.hpp"
#include "Int32Array.hpp"
#include "VrmlField.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"
#include "NodeValidationError.hpp"
#include "VrmlNodeManager.hpp"
#include <type_traits>

namespace vrml_proc::traversor::node_descriptor {

	class NodeDescriptor {
	public:

		enum class FieldType {
			Unknown,
			Vec3f,
			Node,
			NodeArray
		};

		NodeDescriptor(const std::string& id)
			: m_id(id) {}

		NodeDescriptor(const std::string& id, const std::string& additionalId)
			: m_id(id), m_additionalIds({ additionalId }) {}

		NodeDescriptor(const std::string& id, const std::unordered_set<std::string>& additionalIds)
			: m_id(id), m_additionalIds(additionalIds) {}

		template<typename Type>
		void BindField(const std::string& fieldName, const Type& defaultValue);
		
		template <>
		void BindField(const std::string& fieldName, const vrml_proc::parser::Vec3f& defaultValue) {
			m_fieldTypes[fieldName] = FieldType::Vec3f;
			m_vec3fFields[fieldName] = std::cref(defaultValue);
		}

		void BindVrmlNode(const std::string& fieldName, const std::unordered_set<std::string>& validNodeHeaders, const vrml_proc::parser::VrmlNode& defaultNode) {
			m_fieldTypes[fieldName] = FieldType::Node;
			m_validHeaderNames[fieldName] = validNodeHeaders;
			m_nodeFields[fieldName] = std::cref(defaultNode);
		}

		void BindVrmlNodeArray(const std::string& fieldName) {
			m_fieldTypes[fieldName] = FieldType::NodeArray;
		}

		static bool CheckHeader(const NodeDescriptor& nd, const vrml_proc::parser::VrmlNode& node) {
			return node.header == nd.m_id || nd.m_additionalIds.find(node.header) != nd.m_additionalIds.end();
		}

		cpp::result<void, std::shared_ptr<vrml_proc::traversor::validator::error::NodeValidationError>> Validate(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager, bool checkName = false) {

			if (checkName) {
				CheckHeader(*this, node);
			}

			if (node.fields.empty()) {
				return {};
			}

			auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames(ExtractFieldNames(m_fieldTypes), node.fields, node.header);
			if (fieldsResult.has_error()) {
				return fieldsResult;
			}

			for (const auto& field : node.fields) {

				FieldType type = m_fieldTypes[field.name];
				switch (type)
				{
				case FieldType::Vec3f:
					
				{
					auto vec3f = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<vrml_proc::parser::Vec3f>(field.name, node.fields);
					if (vec3f.has_error()) {
						return cpp::fail(vec3f.error());
					}
					m_vec3fFields[field.name] = vec3f.value();
				}
					break;

				case NodeDescriptor::FieldType::Node:

				{
					auto vrmlNode = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation(field.name, node.fields, manager);
					if (vrmlNode.has_error()) {
						return cpp::fail(vrmlNode.error());
					}

					if (vrmlNode.value().has_value()) {
						auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders(m_validHeaderNames[field.name], vrmlNode.value().value().get(), field.name);
						if (headerResult.has_error()) {
							return cpp::fail(headerResult.error());
						}
					}
					m_nodeFields[field.name] = vrmlNode.value();
				}
					break;

				case NodeDescriptor::FieldType::NodeArray:

				{
					auto vrmlNodeArray = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeArrayWithValidation(field.name, node.fields, manager, true);
					if (vrmlNodeArray.has_error()) {
						return cpp::fail(vrmlNodeArray.error());
					}

					m_nodeArrayFields[field.name] = vrmlNodeArray.value();
				}
					break;

				default:
					break;
				}
			}

			return {};
		}

		bool FieldExists(const std::string& fieldName) const {
			return m_fieldTypes.find(fieldName) != m_fieldTypes.end();
		}

		FieldType GetFieldType(const std::string& fieldName) const {
			if (FieldExists(fieldName)) {
				return m_fieldTypes.at(fieldName);
			}
			return FieldType::Unknown;
		}

		template <typename T>
		T GetField(const std::string& fieldName) const;

		template <>
		std::reference_wrapper<const vrml_proc::parser::Vec3f> GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(const std::string& fieldName) const {
			return m_vec3fFields.at(fieldName).value();
		}

		template <>
		std::reference_wrapper<const vrml_proc::parser::VrmlNode> GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(const std::string& fieldName) const {
			return m_nodeFields.at(fieldName).value();
		}

		template <>
		std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> GetField<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>(const std::string& fieldName) const {
			return m_nodeArrayFields.at(fieldName).value();
		}


	private:

		std::string m_id;
		std::unordered_set<std::string> m_additionalIds;

		std::map<std::string, FieldType> m_fieldTypes;
		std::map<std::string, std::unordered_set<std::string>> m_validHeaderNames;

		std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>> m_vec3fFields;
		std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_nodeFields;
		std::map<std::string, std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>> m_nodeArrayFields;

		static std::unordered_set<std::string> ExtractFieldNames(const std::map<std::string, FieldType>& fieldTypes) {
			std::unordered_set<std::string> fieldNames;
			for (const auto& [key, _] : fieldTypes) {
				fieldNames.insert(key);
			}
			return fieldNames;
		}
	};
}
