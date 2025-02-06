#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

// https://stackoverflow.com/questions/115703/storing-c-template-function-definitions-in-a-cpp-file.

#include "GroupHandler.cpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"
#include "MeshConversionContext.hpp"

#include "VrmlProcessingExport.hpp"

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace GroupHandler {

                template cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}
