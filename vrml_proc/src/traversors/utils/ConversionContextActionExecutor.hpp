#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "ConversionContextActionMap.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace utils {

			namespace ConversionContextActionExecutor {

				template<typename ConversionContextType>
				static std::shared_ptr<ConversionContextType> TryToExecute(const vrml_proc::action::ConversionContextActionMap<ConversionContextType>& actionMap, const std::string& key, const std::vector<std::reference_wrapper<const std::any>>& refArgs, const std::vector<std::any>& copyArgs) {
					if (actionMap.VerifyKey(key)) {
						std::shared_ptr<ConversionContextType> result = actionMap.GetAction(key, refArgs, copyArgs)->Execute();
						auto derivedContext = std::dynamic_pointer_cast<ConversionContextType>(result);
						if (derivedContext != nullptr) {
							return derivedContext;
						}
					}
					return std::make_shared<ConversionContextType>();
				}
			}
		}
	}
}
