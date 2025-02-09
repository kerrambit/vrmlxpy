import os

TYPES = [
    "to_stl::conversion_context::Vec3fArrayConversionContext",
]

HANDLERS = ["WorldInfoHandler", "BoxHandler", "GroupHandler", "TransformationHandler", "ShapeHandler", "IndexedFaceSetHandler", "CoordinateHandler", "NormalHandler", "TextureCoordinateHandler", "ColorHandler"]

CPP_TEMPLATE = """#if __INCLUDE_LEVEL__
#error "Don't include this file"
#endif

#include "{handler}.cpp"

#include <memory>

#include <result.hpp>

#include "ConversionContextActionMap.hpp"
#include "{filename}.hpp"
#include "Error.hpp"
#include "FullParsedVrmlNodeContext.hpp"

namespace vrml_proc {{
    namespace traversor {{
        namespace handler {{
            namespace {handler} {{

                template cpp::result<std::shared_ptr<{context}>, std::shared_ptr<vrml_proc::core::error::Error>>
                    Handle<{context}>(
                        vrml_proc::traversor::FullParsedVrmlNodeContext,
                        const vrml_proc::action::ConversionContextActionMap<{context}>&);
            }}
        }}
    }}
}}
"""

print("Output dir:", end=" ")
OUTPUT_DIR = str(input())

os.makedirs(OUTPUT_DIR, exist_ok=True)

for handler in HANDLERS:
    for t in TYPES:
        type_name = t.split("::")[-1]
        filename = f"{OUTPUT_DIR}/{handler}.{type_name}.cpp"
        with open(filename, "w") as f:
            f.write(CPP_TEMPLATE.format(handler=handler, context=t, filename=type_name))
        print(f"Generated: {filename}")

print("✅ Instantiations generated.")
