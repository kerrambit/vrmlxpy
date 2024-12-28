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

					std::cout << "VRML Node - Group" << std::endl;

					// TODO: check if fields are empty, then skip all the checking

					auto children = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", context.node.fields);
					if (children.has_value()) {

						std::cout << "children " << std::endl;

						for (const auto& child : children.value()) {

							auto vrmlNode = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);
							if (vrmlNode.has_value()) {
								// VRML node
								std::cout << "VRML Node child " << vrmlNode.value().get() << std::endl;
								// return vrml_proc::traversor::VrmlNodeTraversor::Traverse({vrmlNode.value().get(), context.manager});
							}
							auto useNode = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::UseNode>>(child);
							if (useNode.has_value()) {
								// USE node
								std::cout << "USE Node child " << useNode.value().get() << std::endl;
								// I have found USE node, I will need the aproproate action for it and take the covnersion conetxt and nodes manager.
								// I will look for the node in the manager, when it is found, I will verify what it is.
								// It should be VRML node - so I should traverse it again, or it has been already traversed, look for the data associated with it.
							}
						}
					}

					auto bboxCentre = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxCenter", context.node.fields);
					if (bboxCentre.has_value()) {
						std::cout << "bboxCenter " << bboxCentre.value() << std::endl;
					}

					auto bboxSize = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<vrml_proc::parser::Vec3f>("bboxSize", context.node.fields);
					if (bboxSize.has_value()) {
						std::cout << "bboxSize " << bboxSize.value() << std::endl;
					}


				}
				else if (context.node.header == "Spotlight") {

					std::cout << "VRML Node - Spotlight" << std::endl;

					auto ambientIntensity = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
					if (ambientIntensity.has_value()) {
						std::cout << "ambientIntensity" << ambientIntensity.value() << std::endl;
					}
				}

				return nullptr;

			}
		};
	}
}
