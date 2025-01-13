#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "VrmlField.hpp"

namespace vrml_proc {
	namespace traversor {
		namespace utils {
			namespace VrmlFieldChecker {

                static bool CheckFields(const std::unordered_set<std::string>& validFieldNames, const std::vector<vrml_proc::parser::VrmlField>& fields) {

                    std::unordered_set<std::string> alreadyFoundFieldNames;
                    for (const auto& field : fields) {
                        if (validFieldNames.find(field.name) == validFieldNames.end()) {
                            return false;
                        }

                        if (alreadyFoundFieldNames.find(field.name) != alreadyFoundFieldNames.end()) {
                            return false;
                        }

                        alreadyFoundFieldNames.insert(field.name);
                    }

                    return true;
                }
			}
		}
	}
}