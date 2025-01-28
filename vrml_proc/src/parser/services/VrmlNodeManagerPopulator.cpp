#include "VrmlNodeManagerPopulator.hpp"

#include "VrmlFieldExtractor.hpp"
#include "vrmlNode.hpp"
#include "VrmlNodeManager.hpp"

namespace vrml_proc {
	namespace parser {
		namespace VrmlNodeManagerPopulator{

			void Populate(VrmlNodeManager& manager, const VrmlNode& node) {

				if (node.definitionName.has_value() && node.definitionName.value() != "") {
					manager.AddDefinitionNode(node.definitionName.value(), node);
				}

				if (node.fields.size() == 0) {
					return;
				}

				for (const auto& child : node.fields) {

					auto nodeResult = model::utils::VrmlFieldExtractor::Extract<VrmlNode>(child.value);
					if (nodeResult.has_value()) {
						Populate(manager, nodeResult.value().get());
						continue;
					}

					auto arrayResult = model::utils::VrmlFieldExtractor::Extract<VrmlNodeArray>(child.value);
					if (arrayResult.has_value() && arrayResult.value().get().size() != 0) {
						for (const auto& variant : arrayResult.value().get()) {
							auto variantResult = model::utils::VrmlFieldExtractor::ExtractFromVariant<VrmlNode>(variant);
							if (variantResult.has_value()) {
								Populate(manager, variantResult.value().get());
							}
						}
					}
				}
			}
		}
	}
}

