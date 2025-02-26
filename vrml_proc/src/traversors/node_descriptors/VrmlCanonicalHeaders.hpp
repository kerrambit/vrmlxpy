#pragma once

#include <string>
#include <map>

namespace vrml_proc::traversor::node_descriptor {

    std::map<std::string, std::string> HeaderToCanonicalName = {

        {"Group", "Group"},
        {"VRMLGroup", "Group"},

        {"Transform", "Transform"},
        {"VRMLTransform", "Transform"},

        {"Switch", "Switch"},
        {"VRMLSwitch", "Switch"},

        {"IndexedFaceSet", "IndexedFaceSet"},
        {"VRMLIndexedFaceSet", "IndexedFaceSet"},

        {"Shape", "Shape"},
        {"VRMLShape", "Shape"},
    };
}
