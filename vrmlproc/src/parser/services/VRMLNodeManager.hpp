#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "VrmlNode.hpp"

namespace vrml_proc {
    namespace parser {

        class VrmlNodeManager {
        public:
            VrmlNodeManager() = default;

            void AddDefinitionNode(const std::string& id, std::shared_ptr<VrmlNode> node) {
                m_definitionNodes[id] = node;
            }

            std::shared_ptr<VrmlNode> GetDefinitionNode(const std::string& id) const {
                auto it = m_definitionNodes.find(id);
                if (it != m_definitionNodes.end()) {
                    return it->second;
                }
                return nullptr;
            }

            inline size_t GetDefNodesTotalCount() const {
                return m_definitionNodes.size();
            }

            void Display(std::ostream &output_stream) const {
                if (GetDefNodesTotalCount() == 0) {
                    output_stream << "There are no DEF nodes stored." << std::endl;
                    return;
                }
                for (auto& [id, node_reference] : m_definitionNodes) {
                    if (node_reference != nullptr) {
                        output_stream << *node_reference << std::endl;
                    }
                }
            }

        private:
            std::unordered_map<std::string, std::shared_ptr<VrmlNode>> m_definitionNodes;
        };
    }
}
