#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "VrmlNode.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        class VRMLPROCESSING_API VrmlNodeManager {
        public:
            VrmlNodeManager() = default;

            void AddDefinitionNode(const std::string& id, std::shared_ptr<VrmlNode> node);

            std::shared_ptr<VrmlNode> GetDefinitionNode(const std::string& id) const;

            inline size_t GetDefNodesTotalCount() const { return m_definitionNodes.size(); }

        private:
            std::unordered_map<std::string, std::shared_ptr<VrmlNode>> m_definitionNodes;
        };
    }
}
