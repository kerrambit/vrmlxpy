#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "ConversionContextActionMap.cpp"

#include "Vec3fArrayConversionContext.hpp"

namespace vrml_proc {
    namespace action {
        template class ConversionContextActionMap<to_stl::conversion_context::Vec3fArrayConversionContext>;
    }
}