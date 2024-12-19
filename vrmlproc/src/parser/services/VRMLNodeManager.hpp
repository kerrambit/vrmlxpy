#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "VRMLNode.hpp"

namespace vrml_proc {
    namespace parser {
        class VRMLNodeManager {

        public:
            VRMLNodeManager() = default;

            void AddDefinitionNode(const std::string& id, std::shared_ptr<VRMLNode> node) {
                m_definition_nodes[id] = node;
            }

            std::shared_ptr<VRMLNode> GetDefinitionNode(const std::string& id) const {
                auto it = m_definition_nodes.find(id);
                if (it != m_definition_nodes.end()) {
                    return it->second;
                }
                return nullptr;
            }

            size_t GetDefNodesTotalCount() const {
                return m_definition_nodes.size();
            }

            void Display(std::ostream &output_stream) {
                if (m_definition_nodes.size() == 0) {
                    output_stream << "There are no DEF nodes stored." << std::endl;
                    return;
                }
                for (auto& [id, node_reference] : m_definition_nodes) {
                    if (node_reference != nullptr) {
                        output_stream << *node_reference << std::endl;
                    }
                }
            }


        private:
            std::unordered_map<std::string, std::shared_ptr<VRMLNode>> m_definition_nodes;
        };
    }
}
