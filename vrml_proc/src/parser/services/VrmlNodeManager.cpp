#include "VrmlNodeManager.hpp"

#include <optional>
#include <functional>
#include <string>
#include "Logger.hpp"

#include "VrmlNode.hpp"

vrml_proc::parser::VrmlNodeManager::VrmlNodeManager() {
    LOG_INFO() << "Construct new VrmlNodeManager.";
}

void vrml_proc::parser::VrmlNodeManager::AddDefinitionNode(const std::string& id, const vrml_proc::parser::VrmlNode& node)
{
    LOG_INFO() << "Add DEF node with id <" << id << "> into the map.";
    LOG_DEBUG() << "Address of Vrml node: <" << &node << ">.";

    auto result = m_definitionNodes.insert_or_assign(id, std::reference_wrapper<const vrml_proc::parser::VrmlNode>(std::cref(node)));
    if (!result.second) {
        LOG_INFO() << "Id <" << id << "> was already present in the map. The value was replaced with the new entity.";
    }
}

std::optional<std::reference_wrapper<const vrml_proc::parser::VrmlNode>> vrml_proc::parser::VrmlNodeManager::GetDefinitionNode(const std::string& id) const
{
    LOG_INFO() << "Retrieve DEF node with id <" << id << "> from the map.";
    auto it = m_definitionNodes.find(id);
    if (it != m_definitionNodes.end()) {
        return it->second;
    }

    LOG_WARNING() << "DEF node with id <" << id << "> was not found in the map!";
    return {};
}
