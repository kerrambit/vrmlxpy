#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "TextureCoordinateHandler.cpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "Vec3fArrayConversionContext.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"

namespace vrml_proc {
    namespace traversor {
        namespace handler {
            namespace TextureCoordinateHandler {

                template cpp::result<std::shared_ptr<to_stl::conversion_context::Vec3fArrayConversionContext>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<to_stl::conversion_context::Vec3fArrayConversionContext>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<to_stl::conversion_context::Vec3fArrayConversionContext>&);
            }
        }
    }
}
