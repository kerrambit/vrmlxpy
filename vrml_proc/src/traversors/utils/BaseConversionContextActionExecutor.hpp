#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "BaseConversionContextActionMap.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace utils {

			namespace BaseConversionContextActionExecutor {

				template<typename EmptyDefaultConversionContextType>
				static std::shared_ptr<EmptyDefaultConversionContextType> TryToExecute(const vrml_proc::action::BaseConversionContextActionMap<EmptyDefaultConversionContextType>& actionMap, const std::string& key, const std::vector<std::any>& args) {
					if (actionMap.VerifyKey(key)) {
						std::shared_ptr<EmptyDefaultConversionContextType> result = actionMap.GetAction(key, args)->Execute();
						auto derivedContext = std::dynamic_pointer_cast<EmptyDefaultConversionContextType>(result);
						if (derivedContext != nullptr) {
							return derivedContext;
						}
					}
					return std::make_shared<EmptyDefaultConversionContextType>();
				}
			}
		}
	}
}
