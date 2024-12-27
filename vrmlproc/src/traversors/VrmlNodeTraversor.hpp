#pragma once

#include <memory>
#include <vector>

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include "VrmlNode.hpp"
#include "UseNode.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlFieldExtractor.hpp"
#include "BaseConversionContext.hpp"

namespace vrml_proc {
	namespace traversor {

		struct FullParsedVrmlNodeContext {

			FullParsedVrmlNodeContext(const vrml_proc::parser::VrmlNode& node, const vrml_proc::parser::VrmlNodeManager& manager)
				: node(node), manager(manager) {}

			const vrml_proc::parser::VrmlNode& node;
			const vrml_proc::parser::VrmlNodeManager& manager;
		};

		class VrmlNodeTraversor {
		public:
			static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlNodeContext context) {

				if (context.node.header == "Group") {

					auto result = vrml_proc::traversors::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", context.node.fields);
					if (result.has_value()) {
													
					}

					auto result3 = vrml_proc::traversors::utils::VrmlFieldExtractor::ExtractByName<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>("shape", context.node.fields);
					if (result3.has_value()) {

					}

					auto result2 = vrml_proc::traversors::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
					if (result2.has_value()) {

					}

				}
				else if (context.node.header == "Shape") {
					auto result2 = vrml_proc::traversors::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
				}

				return nullptr;

			}
		};
	}
}
