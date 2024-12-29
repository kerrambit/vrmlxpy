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
#include "StlBaseStructure.hpp"
#include "BaseConversionContextActionMap.hpp"
#include "BaseConversionContextActionExecutor.hpp"

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
			static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlNodeContext context, const vrml_proc::action::BaseConversionContextActionMap& actionMap) {

				if (context.node.header == "WorldInfo") {
					std::cout << "VRML Node - WorldInfo" << std::endl;

					auto info = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::string>("info", context.node.fields);
					if (info.has_value()) {
						std::cout << "info" << info.value() << std::endl;
					}

					auto title = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::string>("title", context.node.fields);
					if (title.has_value()) {
						std::cout << "title" << title.value() << std::endl;
					}

					if (info.has_value() && title.has_value()) {
						return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "WorldInfo", { info.value(), title.value()});
					}

					return std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
				}
				else if (context.node.header == "Group") {
					std::cout << "VRML Node - Group" << std::endl;

					auto result = std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();

					if (context.node.fields.empty()) {
						return result;
					}

					auto children = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<std::vector<boost::variant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>, boost::recursive_wrapper<vrml_proc::parser::UseNode>>>>("children", context.node.fields);
					if (children.has_value()) {

						std::cout << "children " << std::endl;

						for (const auto& child : children.value()) {

							auto vrmlNode = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractFromVariant<boost::recursive_wrapper<vrml_proc::parser::VrmlNode>>(child);
							if (vrmlNode.has_value()) {
								// VRML node
								std::cout << "VRML Node child " << vrmlNode.value().get() << std::endl;

							    auto vrmlNodeTraversorResult = vrml_proc::traversor::VrmlNodeTraversor::Traverse({vrmlNode.value().get(), context.manager}, actionMap);
								if (vrmlNodeTraversorResult != nullptr) {
									std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext> meshContextPtr = std::dynamic_pointer_cast<vrml_proc::conversion_context::MeshConversionContext>(vrmlNodeTraversorResult);
									if (meshContextPtr != nullptr) {
										result->Merge(meshContextPtr.get());
										meshContextPtr.reset();
									}
								}
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

					return result;

				}
				else if (context.node.header == "Spotlight") {

					std::cout << "VRML Node - Spotlight" << std::endl;

					auto ambientIntensity = vrml_proc::traversor::utils::VrmlFieldExtractor::ExtractByName<float>("ambientIntensity", context.node.fields);
					if (ambientIntensity.has_value()) {
						std::cout << "ambientIntensity" << ambientIntensity.value() << std::endl;
						return vrml_proc::traversor::utils::BaseConversionContextActionExecutor::TryToExecute<vrml_proc::conversion_context::MeshConversionContext>(actionMap, "Spotlight", { ambientIntensity.value() });
					}

					return std::make_shared<vrml_proc::conversion_context::MeshConversionContext>();
				}

				std::cout << "Error! Unknown VRML Node." << std::endl;
				return nullptr;
			}
		};
	}
}
