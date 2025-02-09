#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "TransformationHandler.cpp"

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
            namespace TransformationHandler {

                template cpp::result<std::shared_ptr<vrml_proc::conversion_context::MeshConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    VRMLPROCESSING_API Handle<vrml_proc::conversion_context::MeshConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<vrml_proc::conversion_context::MeshConversionContext>&);
            }
        }
    }
}
