#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>

#include "ConversionContextAction.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace action {
		/**
		 * @brief Represents class which is responsible for storing and mapping `Action` functor value to a string key.
		 */
		template<typename ConversionContext>
		class VRMLPROCESSING_API ConversionContextActionMap {
		public:
			/**
			 * @brief Arguments passed into the function responsible for creating the given Action.
			 * These argument will be taken as a reference.
			 */
			using ReferencedArguments = std::vector<std::reference_wrapper<const std::any>>;
			/**
			 * @brief Arguments passed into the function responsible for creating the given Action.
			 * These argument will be copied.
			 */
			using CopiedArguments = std::vector<std::any>;
			/**
			 * @brief The basic data type stored in the ActionMap. It is a function pointer to a function, which
			 * takes some, for now unknown, arguments and returns a shared pointer owning ConversionContextAction.
			 */
			using ActionFunctor = std::function<std::shared_ptr<ConversionContextAction<ConversionContext>>(ReferencedArguments, CopiedArguments)>;
			/**
			 * @brief Adds new action to the ActionMap mapped to a string key.
			 * Duplicated key will be rewritten.
			 * 
			 * @param key key which will be mapped to the Action
			 * @param action ActionFunctor representing the given Action
			 */
			void AddAction(const std::string& key, ActionFunctor action);
			/**
			 * @brief Verifies if the given key exists in the ActionMap.
			 * 
			 * @param key key identifying the given Action
			 * @return true if the key is present, false if the key is not present.
			 */
			bool VerifyKey(const std::string& key) const;
			/**
			 * @brief Gets a shared pointer owning a ConversionContextAction object. The function creates new object using functor
			 * stored in the ActionMap and passed `args` which are passed in to the Action constructor.
			 * 
			 * @param key key identifying the given Action
			 * @param args arguments passed into the function responsible for creating the given Action
			 * @returns nullptr if the key does not exists, otherwise shared pointer owning the given ConversionContextAction object
			 */
			std::shared_ptr<ConversionContextAction<ConversionContext>> GetAction(const std::string& key, const ReferencedArguments& refArgs, const CopiedArguments& copyArgs) const;

		private:
			std::unordered_map<std::string, ActionFunctor> m_actions;
		};
	}
}
