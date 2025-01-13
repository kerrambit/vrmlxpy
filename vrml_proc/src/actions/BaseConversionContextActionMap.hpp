#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>

#include "BaseConversionContext.hpp"
#include "BaseConversionContextAction.hpp"

namespace vrml_proc {
	namespace action {
		/**
		 * @brief Represents class which is responsible for storing and mapping `Action` functor value to a string key.
		 */
		class BaseConversionContextActionMap {
		public:
			/**
			 * @brief Arguments passed into the function responsible for creating the given Action.
			 */
			using Arguments = std::vector<std::any>;
			/**
			 * @brief The basic data type stored in the ActionMap. It is a function pointer to a function, which
			 * takes some, for now unknown, arguments and returns a shared pointer owning BaseConversionContextAction.
			 */
			using ActionFunctor = std::function<std::shared_ptr<BaseConversionContextAction>(Arguments)>;
			/**
			 * @brief Adds new action to the ActionMap mapped to a string key.
			 * Duplicated key will be rewritten.
			 * 
			 * @param key key which will be mapped to the Action
			 * @param action ActionFunctor representing the given Action
			 */
			inline void AddAction(const std::string& key, ActionFunctor action) {
				m_actions[key] = std::move(action);
			}
			/**
			 * @brief Verifies if the given key exists in the ActionMap.
			 * 
			 * @param key key identifying the given Action
			 * @return true if the key is present, false if the key is not present.
			 */
			inline bool VerifyKey(const std::string& key) const {
				auto iterator = m_actions.find(key);
				if (iterator != m_actions.end()) {
					return true;
				}
				return false;
			}
			/**
			 * @brief Gets a shared pointer owning a BaseConversionContextAction object. The function creates new object using functor
			 * stored in the ActionMap and passed `args` which are passed in to the Action constructor.
			 * 
			 * @param key key identifying the given Action
			 * @param args arguments passed into the function responsible for creating the given Action
			 * @returns nullptr if the key does not exists, otherwise shared pointer owning the given BaseConversionContextAction object
			 */
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
