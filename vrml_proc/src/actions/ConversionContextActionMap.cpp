#include "ConversionContextActionMap.hpp"

#include <memory>
#include <string>

#include "ConversionContextAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"

namespace vrml_proc {
	namespace action {

        template<typename ConversionContext>
        void ConversionContextActionMap<ConversionContext>::AddAction(const std::string& key, ActionFunctor action) {
            LOG_INFO() << "Add new action with key <" << key << ">.";
            m_actions[key] = std::move(action);
        }

        template<typename ConversionContext>
        bool ConversionContextActionMap<ConversionContext>::VerifyKey(const std::string& key) const {
            bool result = (m_actions.find(key) != m_actions.end());
            LOG_TRACE() << "Verify key <" << key << ">.";
            if (result) {
                LOG_TRACE() << "Key <" << key << "> was not found!";
            }
            return result;
        }

        template<typename ConversionContext>
        std::shared_ptr<ConversionContextAction<ConversionContext>> ConversionContextActionMap<ConversionContext>::GetAction(
            const std::string& key, const ReferencedArguments& refArgs, const CopiedArguments& copyArgs) const {
            LOG_INFO() << "Retrieve action by key <" << key << ">.";
            auto iterator = m_actions.find(key);
            if (iterator != m_actions.end()) {
                return iterator->second(refArgs, copyArgs);
            }
            LOG_WARNING() << "Action with key <" << key << "> was not found!";
            return nullptr;
        }

        template class ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>;
	}
}