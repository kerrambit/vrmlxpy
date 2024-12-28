#pragma once

#include <memory>

#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"
#include "VrmlProcessingExport.hpp"
#include "BaseConversionContext.hpp"
#include "MeshConversionContext.hpp"
#include "VrmlNodeTraversor.hpp"

namespace vrml_proc {
	namespace traversor {

		struct VRMLPROCESSING_API FullParsedVrmlFileContext {

			FullParsedVrmlFileContext(const vrml_proc::parser::VrmlFile& file, const vrml_proc::parser::VrmlNodeManager& manager)
				: file(file), manager(manager) {}

			const vrml_proc::parser::VrmlFile& file;
			const vrml_proc::parser::VrmlNodeManager& manager;
		};

		class VRMLPROCESSING_API VrmlFileTraversor {
		public:
			static std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> Traverse(FullParsedVrmlFileContext context) {

				for (const auto& root : context.file) {

					if (root.header == "Anchor") {
						std::cout << "Found Anchor" << std::endl;
					}
					else if (root.header == "Billboard") {
						std::cout << "Found Billboard" << std::endl;
					}
					else if (root.header == "Collision") {
						std::cout << "Found Collision" << std::endl;
					}
					else if (root.header == "Group") {
						std::cout << "Found Group" << std::endl;
						std::shared_ptr<vrml_proc::conversion_context::BaseConversionContext> result =
							vrml_proc::traversor::VrmlNodeTraversor::Traverse(vrml_proc::traversor::FullParsedVrmlNodeContext(root, context.manager));
					}
					else if (root.header == "Transform") {
						std::cout << "Found Transform" << std::endl;
					}
					else if (root.header == "WorldInfo") {
						std::cout << "Found WorldInfo" << std::endl;
					}
					else {
						std::cout << "Invalid grouping/children node!" << std::endl;
						return nullptr;
					}
				}
			}
		};
	}
}
