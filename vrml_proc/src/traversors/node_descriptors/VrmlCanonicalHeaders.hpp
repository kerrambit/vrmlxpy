#pragma once

#include <unordered_set>
#include <string>
#include <map>

namespace vrml_proc::traversor::node_descriptor {

  /**
   * @brief Groups all posible header names and thei mapping to canonical VRML header name.
   * Each pair is {POSSIBLE_NAME, CANONICAL_FORM}.
   */
  std::map<std::string, std::string> HeaderToCanonicalName = {

      {"Group", "Group"},
      {"VRMLGroup", "Group"},

      {"Transform", "Transform"},
      {"VRMLTransform", "Transform"},

      {"Switch", "Switch"},
      {"VRMLSwitch", "Switch"},

      {"IndexedFaceSet", "IndexedFaceSet"},
      {"VRMLIndexedFaceSet", "IndexedFaceSet"},

      {"Box", "Box"},
      {"VRMLBox", "Box"},

      {"Shape", "Shape"},
      {"VRMLShape", "Shape"},

      {"Color", "Color"},
      {"VRMLColor", "Color"},

      {"Coordinate", "Coordinate"},
      {"VRMLCoordinate", "Coordinate"},

      {"Normal", "Normal"},
      {"VRMLNormal", "Normal"},

      {"TextureCoordinate", "TextureCoordinate"},
      {"VRMLTextureCoordinate", "TextureCoordinate"},

      {"Appearance", "Appearance"},
      {"VRMLAppearance", "Appearance"},

      {"WorldInfo", "WorldInfo"},
      {"VRMLWorldInfo", "WorldInfo"},
  };

  inline std::unordered_set<std::string> GetPossibleNamesForCanonical(const std::string& canonicalName) {
    std::unordered_set<std::string> result;
    for (const auto& [possible, canonical] : HeaderToCanonicalName) {
      if (canonical == canonicalName) {
        result.insert(possible);
      }
    }
    return result;
  }
}  // namespace vrml_proc::traversor::node_descriptor
