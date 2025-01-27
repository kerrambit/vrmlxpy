#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>

#include "VrmlNode.hpp"
#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace parser {

        class VRMLPROCESSING_API VrmlNodeManager {
        public:
            VrmlNodeManager();

            void AddDefinitionNode(const std::string& id, const VrmlNode& node);

            std::optional<std::reference_wrapper<const VrmlNode>> GetDefinitionNode(const std::string& id) const;

            inline size_t GetDefNodesTotalCount() const { return m_definitionNodes.size(); }

        private:
            std::unordered_map<std::string, std::reference_wrapper<const VrmlNode>> m_definitionNodes;
        };
    }
}
