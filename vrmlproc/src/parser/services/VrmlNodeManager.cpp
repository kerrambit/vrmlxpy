#include "VrmlNodeManager.hpp"
#include "VrmlNodeManager.hpp"

void vrml_proc::parser::VrmlNodeManager::AddDefinitionNode(const std::string& id, std::shared_ptr<VrmlNode> node)
{
    m_definitionNodes[id] = node;
}

std::shared_ptr<vrml_proc::parser::VrmlNode> vrml_proc::parser::VrmlNodeManager::GetDefinitionNode(const std::string& id) const {
    auto it = m_definitionNodes.find(id);
    if (it != m_definitionNodes.end()) {
        return it->second;
    }
    return nullptr;
}
