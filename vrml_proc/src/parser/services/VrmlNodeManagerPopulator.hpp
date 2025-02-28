#pragma once

#include "VrmlNode.hpp"
#include "VrmlNodeManager.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
  namespace parser {
    namespace VrmlNodeManagerPopulator {
      VRMLPROCESSING_API void Populate(VrmlNodeManager& manager, const VrmlNode& node);
    }
  }  // namespace parser
}  // namespace vrml_proc
