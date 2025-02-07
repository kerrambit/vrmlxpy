#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "GroupHandler.cpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "CoordConversionContext.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace GroupHandler {

                template cpp::result<std::shared_ptr<to_stl::conversion_context::CoordConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<to_stl::conversion_context::CoordConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::CoordConversionContext>&);
            }
        }
    }
}

#include "MeshConversionContext.hpp"

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
