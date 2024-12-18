cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(vrmlproc)

# Add Catch2 unit tests using FetchContent.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# --------------------------------------------------------------------------------------- #

# Add source files and vendor dependencies.
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/models 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammars
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/pybind11/include/pybind11
)

# Create the DLL.
add_library(vrmlproc SHARED
    "src/parser/vrml_processing.hpp"
    "src/parser/vrml_processing.cpp"

    "src/parser/models/VRMLUnits.hpp"

    "src/parser/models/VRMLField.hpp"
    "src/parser/models/VRMLField.cpp"

    "src/parser/models/Vec3f.hpp"
    "src/parser/models/Vec3f.cpp"

    "src/parser/models/Vec4f.hpp"
    "src/parser/models/Vec4f.cpp"

    "src/parser/models/Vec3fArray.hpp"
    "src/parser/models/Vec3fArray.cpp"

    "src/parser/models/Int32Array.hpp"
    "src/parser/models/Int32Array.cpp"

    "src/parser/models/VRMLNode.hpp"
    "src/parser/models/VRMLNode.cpp"

    "src/parser/models/USENode.hpp"
    "src/parser/models/USENode.cpp"

    "src/parser/VRMLNodeManager.hpp"

    "src/parser/interfaces/Printable.hpp"
    "src/parser/interfaces/Printable.cpp"

    "src/parser/VRMLParser.cpp"

    "src/parser/grammars/CommentSkipper.hpp"
    "src/parser/grammars/IdentifierGrammar.hpp"
    "src/parser/grammars/Vec3fGrammar.hpp"
    "src/parser/grammars/Vec3fArrayGrammar.hpp"
    "src/parser/grammars/Vec4fGrammar.hpp"
    "src/parser/grammars/Int32ArrayGrammar.hpp"
    "src/parser/grammars/QuotedStringGrammar.hpp"
    "src/parser/grammars/BooleanGrammar.hpp"
    "src/parser/grammars/VRMLNodeGrammar.hpp"

    "src/parser/SetupIntegrity.cpp"
)

# --------------------------------------------------------------------------------------- #

# Create the Python bindings module.
#pybind11_add_module(pypyvrml SHARED bindings/MyLibBindings.cpp)

# Link pybind11 and Python libraries to the bindings module.
#target_link_libraries(pypyvrml PRIVATE
#    pyvrml
#)

# Add include directories for the bindings module.
#target_include_directories(pypyvrml PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/src
#)

# --------------------------------------------------------------------------------------- #

# Add the test executables.
add_executable(SetupIntegrityTests "tests/SetupIntegrityTest.cpp")
add_executable(VRMLFileGrammarUnitTests "tests/VRMLFileGrammarTest.cpp")
add_executable(Vec3fGrammarUnitTests "tests/Vec3fGrammarTest.cpp")
add_executable(Vec4fGrammarUnitTests "tests/Vec4fGrammarTest.cpp")
add_executable(Vec3fArrayGrammarUnitTests "tests/Vec3fArrayGrammarTest.cpp")
add_executable(Int32ArrayGrammarUnitTests "tests/Int32ArrayGrammarTest.cpp")

# Link Catch2 with the test executables.
target_link_libraries(SetupIntegrityTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(VRMLFileGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec3fGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec4fGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec3fArrayGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Int32ArrayGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)

# Enable testing (use CTest to run the unit tests).
enable_testing()
add_test(NAME SetupIntegrityTests COMMAND SetupIntegrityTests)
add_test(NAME VRMLFileGrammarUnitTests COMMAND VRMLFileGrammarUnitTests)
add_test(NAME Vec3fGrammarUnitTests COMMAND Vec3fGrammarUnitTests)
add_test(NAME Vec4fGrammarUnitTests COMMAND Vec4fGrammarUnitTests)
add_test(NAME Vec3fArrayGrammarUnitTests COMMAND Vec3fArrayGrammarUnitTests)
add_test(NAME Int32ArrayGrammarUnitTests COMMAND Int32ArrayGrammarUnitTests)