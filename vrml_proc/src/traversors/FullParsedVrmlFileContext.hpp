#pragma once

#include "VrmlFile.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
	namespace traversor {

		struct VRMLPROCESSING_API FullParsedVrmlFileContext {

			FullParsedVrmlFileContext(const vrml_proc::parser::VrmlFile& file, const vrml_proc::parser::VrmlNodeManager& manager)
				: file(file), manager(manager) {}

			const vrml_proc::parser::VrmlFile& file;
			const vrml_proc::parser::VrmlNodeManager& manager;
		};
	}
}

