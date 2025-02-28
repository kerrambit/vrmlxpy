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
#include "Vec2f.hpp"
#include "Vec2fArray.hpp"
#include "VrmlUnits.hpp"
#include "Vec4f.hpp"
#include "Int32Array.hpp"
#include "VrmlField.hpp"
#include "NodeValidator.hpp"
#include "VrmlNode.hpp"
#include "NodeValidationError.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc::traversor::node_descriptor {

  class NodeDescriptor {
   public:
    enum class FieldType {
      Unknown,
      Bool,
      String,
      Float32,
      Int32,
      Vec2f,
      Vec3f,
      Vec4f,
      Vec2fArray,
      Vec3fArray,
      Int32Array,
      Node,
      NodeArray
    };

    NodeDescriptor(const std::string& id) : m_id(id) {}

    NodeDescriptor(const std::string& id, const std::string& additionalId)
        : m_id(id), m_additionalIds({additionalId}) {}

    NodeDescriptor(const std::string& id, const std::unordered_set<std::string>& additionalIds)
        : m_id(id), m_additionalIds(additionalIds) {}

    template <typename Type>
    void BindField(const std::string& fieldName, const Type& defaultValue);

    void BindVrmlNode(const std::string& fieldName, const std::unordered_set<std::string>& validNodeHeaders,
                      const vrml_proc::parser::VrmlNode& defaultNode) {
      m_fieldTypes[fieldName] = FieldType::Node;
      m_validHeaderNames[fieldName] = validNodeHeaders;
      m_nodeFields[fieldName] = std::cref(defaultNode);
    }

    void BindVrmlNodeArray(const std::string& fieldName) { m_fieldTypes[fieldName] = FieldType::NodeArray; }

    static bool CheckHeader(const NodeDescriptor& nd, const vrml_proc::parser::VrmlNode& node) {
      return node.header == nd.m_id || nd.m_additionalIds.find(node.header) != nd.m_additionalIds.end();
    }

    cpp::result<void, std::shared_ptr<vrml_proc::traversor::validator::error::NodeValidationError>> Validate(
        const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager,
        bool checkName = false) {
      if (checkName) {
        CheckHeader(*this, node);
      }

      if (node.fields.empty()) {
        return {};
      }

      auto fieldsResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyUniqueAllowedFieldNames(
          ExtractFieldNames(m_fieldTypes), node.fields, node.header);
      if (fieldsResult.has_error()) {
        return fieldsResult;
      }

      for (const auto& field : node.fields) {
        FieldType type = m_fieldTypes[field.name];
        switch (type) {
          case FieldType::Bool:

          {
            auto boolean = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<bool>(
                field.name, node.fields);
            if (boolean.has_error()) {
              return cpp::fail(boolean.error());
            }
            m_boolFields[field.name] = boolean.value();
          } break;

          case FieldType::String:

          {
            auto string = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<std::string>(
                field.name, node.fields);
            if (string.has_error()) {
              return cpp::fail(string.error());
            }
            m_stringFields[field.name] = string.value();
          } break;

          case FieldType::Float32:

          {
            auto float32_t = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::float32_t>(field.name, node.fields);
            if (float32_t.has_error()) {
              return cpp::fail(float32_t.error());
            }
            m_float32Fields[field.name] = float32_t.value();
          } break;

          case FieldType::Int32:

          {
            auto int32 = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<int32_t>(
                field.name, node.fields);
            if (int32.has_error()) {
              return cpp::fail(int32.error());
            }
            m_int32Fields[field.name] = int32.value();
          } break;

          case FieldType::Vec2f:

          {
            auto vec2f = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::Vec2f>(field.name, node.fields);
            if (vec2f.has_error()) {
              return cpp::fail(vec2f.error());
            }
            m_vec2fFields[field.name] = vec2f.value();
          } break;

          case FieldType::Vec3f:

          {
            auto vec3f = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::Vec3f>(field.name, node.fields);
            if (vec3f.has_error()) {
              return cpp::fail(vec3f.error());
            }
            m_vec3fFields[field.name] = vec3f.value();
          } break;

          case FieldType::Vec4f:

          {
            auto vec4f = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::Vec4f>(field.name, node.fields);
            if (vec4f.has_error()) {
              return cpp::fail(vec4f.error());
            }
            m_vec4fFields[field.name] = vec4f.value();
          } break;

          case FieldType::Vec2fArray:

          {
            auto value = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::Vec2fArray>(field.name, node.fields);
            if (value.has_error()) {
              return cpp::fail(value.error());
            }
            m_vec2fArrayFields[field.name] = value.value();
          } break;

          case FieldType::Vec3fArray:

          {
            auto value = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                vrml_proc::parser::Vec3fArray>(field.name, node.fields);
            if (value.has_error()) {
              return cpp::fail(value.error());
            }
            m_vec3fArrayFields[field.name] = value.value();
          } break;

            {
              auto value = vrml_proc::traversor::validator::NodeValidator::ExtractFieldByNameWithValidation<
                  vrml_proc::parser::Int32Array>(field.name, node.fields);
              if (value.has_error()) {
                return cpp::fail(value.error());
              }
              m_int32ArrayFields[field.name] = value.value();
            }
            break;

          case NodeDescriptor::FieldType::Node:

          {
            auto vrmlNode = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeWithValidation(
                field.name, node.fields, manager);
            if (vrmlNode.has_error()) {
              return cpp::fail(vrmlNode.error());
            }

            if (vrmlNode.value().has_value()) {
              auto headerResult = vrml_proc::traversor::validator::NodeValidator::CheckForOnlyAllowedVrmlNodeHeaders(
                  m_validHeaderNames[field.name], vrmlNode.value().value().get(), field.name);
              if (headerResult.has_error()) {
                return cpp::fail(headerResult.error());
              }
            }
            m_nodeFields[field.name] = vrmlNode.value();
          } break;

          case NodeDescriptor::FieldType::NodeArray:

          {
            auto vrmlNodeArray = vrml_proc::traversor::validator::NodeValidator::ExtractVrmlNodeArrayWithValidation(
                field.name, node.fields, manager, true);
            if (vrmlNodeArray.has_error()) {
              return cpp::fail(vrmlNodeArray.error());
            }

            m_nodeArrayFields[field.name] = vrmlNodeArray.value();
          } break;

          default:
            break;
        }
      }

      return {};
    }

    bool FieldExists(const std::string& fieldName) const { return m_fieldTypes.find(fieldName) != m_fieldTypes.end(); }

    FieldType GetFieldType(const std::string& fieldName) const {
      if (FieldExists(fieldName)) {
        return m_fieldTypes.at(fieldName);
      }
      return FieldType::Unknown;
    }

    template <typename T>
    T GetField(const std::string& fieldName) const;

    std::string GetId() const { return m_id; }

   private:
    std::string m_id;
    std::unordered_set<std::string> m_additionalIds;

    std::map<std::string, FieldType> m_fieldTypes;
    std::map<std::string, std::unordered_set<std::string>> m_validHeaderNames;

    std::map<std::string, std::optional<std::reference_wrapper<const bool>>> m_boolFields;
    std::map<std::string, std::optional<std::reference_wrapper<const std::string>>> m_stringFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::float32_t>>> m_float32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const int32_t>>> m_int32Fields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2f>>> m_vec2fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3f>>> m_vec3fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec4f>>> m_vec4fFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>>
        m_vec2fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>>
        m_vec3fArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::Int32Array>>>
        m_int32ArrayFields;
    std::map<std::string, std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>> m_nodeFields;
    std::map<std::string, std::optional<std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>>
        m_nodeArrayFields;

    static std::unordered_set<std::string> ExtractFieldNames(const std::map<std::string, FieldType>& fieldTypes) {
      std::unordered_set<std::string> fieldNames;
      for (const auto& [key, _] : fieldTypes) {
        fieldNames.insert(key);
      }
      return fieldNames;
    }
  };
}  // namespace vrml_proc::traversor::node_descriptor

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const bool& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Bool;
  m_boolFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const std::string& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::String;
  m_stringFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::float32_t& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Float32;
  m_float32Fields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const int32_t& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Int32;
  m_int32Fields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const vrml_proc::parser::Vec2f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec2f;
  m_vec2fFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const vrml_proc::parser::Vec3f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec3f;
  m_vec3fFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(const std::string& fieldName,
                                                                      const vrml_proc::parser::Vec4f& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec4f;
  m_vec4fFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec2fArray& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec2fArray;
  m_vec2fArrayFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Vec3fArray& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Vec3fArray;
  m_vec3fArrayFields[fieldName] = std::cref(defaultValue);
}

template <>
void vrml_proc::traversor::node_descriptor::NodeDescriptor::BindField(
    const std::string& fieldName, const vrml_proc::parser::Int32Array& defaultValue) {
  m_fieldTypes[fieldName] = FieldType::Int32Array;
  m_int32ArrayFields[fieldName] = std::cref(defaultValue);
}

template <>
std::reference_wrapper<const bool>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const bool>>(
    const std::string& fieldName) const {
  return m_boolFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const std::string>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const std::string>>(
    const std::string& fieldName) const {
  return m_stringFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::float32_t> vrml_proc::traversor::node_descriptor::NodeDescriptor::
    GetField<std::reference_wrapper<const vrml_proc::parser::float32_t>>(const std::string& fieldName) const {
  return m_float32Fields.at(fieldName).value();
}

template <>
std::reference_wrapper<const int32_t>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const int32_t>>(
    const std::string& fieldName) const {
  return m_int32Fields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Vec2f>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const vrml_proc::parser::Vec2f>>(
    const std::string& fieldName) const {
  return m_vec2fFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Vec3f>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const vrml_proc::parser::Vec3f>>(
    const std::string& fieldName) const {
  return m_vec3fFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Vec4f>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<std::reference_wrapper<const vrml_proc::parser::Vec4f>>(
    const std::string& fieldName) const {
  return m_vec4fFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Vec2fArray> vrml_proc::traversor::node_descriptor::NodeDescriptor::
    GetField<std::reference_wrapper<const vrml_proc::parser::Vec2fArray>>(const std::string& fieldName) const {
  return m_vec2fArrayFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Vec3fArray> vrml_proc::traversor::node_descriptor::NodeDescriptor::
    GetField<std::reference_wrapper<const vrml_proc::parser::Vec3fArray>>(const std::string& fieldName) const {
  return m_vec3fArrayFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::Int32Array> vrml_proc::traversor::node_descriptor::NodeDescriptor::
    GetField<std::reference_wrapper<const vrml_proc::parser::Int32Array>>(const std::string& fieldName) const {
  return m_int32ArrayFields.at(fieldName).value();
}

template <>
std::reference_wrapper<const vrml_proc::parser::VrmlNode> vrml_proc::traversor::node_descriptor::NodeDescriptor::
    GetField<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>(const std::string& fieldName) const {
  return m_nodeFields.at(fieldName).value();
}

template <>
std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>
vrml_proc::traversor::node_descriptor::NodeDescriptor::GetField<
    std::vector<std::reference_wrapper<const vrml_proc::parser::VrmlNode>>>(const std::string& fieldName) const {
  return m_nodeArrayFields.at(fieldName).value();
}