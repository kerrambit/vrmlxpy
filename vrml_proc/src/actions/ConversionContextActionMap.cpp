#include "ConversionContextActionMap.hpp"

#include <memory>
#include <string>

#include "ConversionContextAction.hpp"
#include "Logger.hpp"
#include "MeshConversionContext.hpp"
#include "FormatString.hpp"

using vrml_proc::core::utils::FormatString;
using namespace vrml_proc::core::logger;

namespace vrml_proc {
	namespace action {

        template<typename ConversionContext>
        void ConversionContextActionMap<ConversionContext>::AddAction(const std::string& key, ActionFunctor action) {
            LogInfo(FormatString("Add new action with key <", key, ">."), LOGGING_INFO);
            m_actions[key] = std::move(action);
        }

        template<typename ConversionContext>
        bool ConversionContextActionMap<ConversionContext>::VerifyKey(const std::string& key) const {
            bool result = (m_actions.find(key) != m_actions.end());
            LogTrace(FormatString("Verify key <", key, ">."), LOGGING_INFO);
            if (result) {
                LogTrace(FormatString("Key <", key, "> was not found!"), LOGGING_INFO);
            }
            return result;
        }

        template<typename ConversionContext>
        std::shared_ptr<ConversionContextAction<ConversionContext>> ConversionContextActionMap<ConversionContext>::GetAction(
            const std::string& key, const ReferencedArguments& refArgs, const CopiedArguments& copyArgs) const {
            LogInfo(FormatString("Retrieve action by key <", key, ">."), LOGGING_INFO);
            auto iterator = m_actions.find(key);
            if (iterator != m_actions.end()) {
                return iterator->second(refArgs, copyArgs);
            }
            LogWarning(FormatString("Action with key <", key, "> was not found!"), LOGGING_INFO);
            return nullptr;
        }

        template class ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>;
	}
}