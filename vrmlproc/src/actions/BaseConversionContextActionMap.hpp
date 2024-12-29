#pragma once

#include <any>
#include <functional>
#include <memory>

#include "BaseConversionContextAction.hpp"
#include "BaseConversionContext.hpp"

namespace vrml_proc {
	namespace action {

		class BaseConversionContextActionMap {

		public:

			using Arguments = std::vector<std::any>;
			using ActionFunctor = std::function<std::shared_ptr<BaseConversionContextAction>(Arguments)>;

			inline void AddAction(const std::string& key, ActionFunctor action) {
				m_actions[key] = std::move(action);
			}

			inline bool VerifyKey(const std::string& key) const {
				auto iterator = m_actions.find(key);
				if (iterator != m_actions.end()) {
					return true;
				}
				return false;
			}

			std::shared_ptr<BaseConversionContextAction> GetAction(const std::string& key, const Arguments& args) const {
				auto iterator = m_actions.find(key);
				if (iterator != m_actions.end()) {
					return iterator->second(args);
				}
				return nullptr;
			}

		private:
			std::unordered_map<std::string, ActionFunctor> m_actions;
		};
	}
}
