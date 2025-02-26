#pragma once

#include "NodeDescriptor.hpp"

namespace vrml_proc::traversor::node_descriptor {

	using NodeDescriptorFactory = std::function<NodeDescriptor()>;
	using NodeDescriptorMap = std::map<std::string, NodeDescriptorFactory>;

    NodeDescriptorMap CreateNodeDescriptorMap() {
        NodeDescriptorMap validatorMap;

        validatorMap["Group"] = []() {
            auto nd = NodeDescriptor("Group", "VRMLGroup");
            static vrml_proc::parser::Vec3f defaultBoxCenter = { 0.0f, 0.0f, 0.0f };
            static vrml_proc::parser::Vec3f defaultBoxSize = { -1.0f, -1.0f, -1.0f };
            nd.BindField("bboxSize", defaultBoxSize);
            nd.BindField("bboxCenter", defaultBoxCenter);
            nd.BindVrmlNodeArray("children");

            return nd;
        };

        validatorMap["Shape"] = []() {
            auto nd = NodeDescriptor("Shape", "VRMLShape");
            static vrml_proc::parser::VrmlNode defaultAppearance;
            static vrml_proc::parser::VrmlNode defaultGeometry;
            nd.BindVrmlNode("appearance", { "Appearance", "VRMLAppearance" }, defaultAppearance);
            nd.BindVrmlNode("geometry", { "Box", "VRMLBox", "Cone", "VRMLCone", "Cylinder", "VRMLCylinder", "ElevationGrid", "VRMLElevationGrid", "Extrusion", "VRMLExtrusion", "IndexedFaceSet", "VRMLIndexedFaceSet", "IndexedLineSet", "VRMLIndexedLineSet", "PointSet", "VRMLPointSet", "Sphere", "VRMLSphere", "Text", "VRMLText" }, defaultGeometry);

            return nd;
            };

        return validatorMap;
    }
}
