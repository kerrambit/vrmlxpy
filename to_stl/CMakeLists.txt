cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(tostl)

# Add Catch2 unit tests using FetchContent.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# --------------------------------------------------------------------------------------- #

# Create the DLL.
add_library(tostl SHARED

    # Third party libraries.
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result/result.hpp"

    # DLL export macro & VrmlToStlConvertor.
    "src/VrmlToStlExport.hpp"
    "src/VrmlToStlConvertor.hpp"

    # Core.
    "src/core/Mesh.hpp"
    "src/core/MeshTask.hpp"
    "src/core/io/StlFileWriter.hpp"

    # Actions.
    "src/actions/GeometryAction.hpp"
    "src/actions/IndexedFaceSetAction.hpp"
    "src/actions/IndexedFaceSetAction.cpp"      
    "src/actions/GroupAction.hpp"
    "src/actions/GroupAction.cpp"
    "src/actions/BoxAction.hpp"
    "src/actions/BoxAction.cpp"
    "src/actions/ShapeAction.hpp"
    "src/actions/ShapeAction.cpp"
    "src/actions/TransformAction.hpp"
    "src/actions/TransformAction.cpp"
    "src/actions/SwitchAction.hpp"
    "src/actions/SwitchAction.cpp"

    # Conversion contexts
    "src/conversion_context/Vec3fArrayConversionContext.hpp"
    "src/conversion_context/MeshTaskConversionContext.hpp"
    "src/conversion_context/StlActionMap.hpp"
    #"src/conversion_context/ConversionContextMap.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/VrmlNodeTraversor.Vec3fArrayConversionContext.cpp"

    #"src/conversion_context/TransformHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/GroupHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/BoxHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/WorldInfoHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/BoxHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/ColorHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/CoordinateHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/IndexedFaceSetHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/NormalHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/ShapeHandler.Vec3fArrayConversionContext.cpp"
    #"src/conversion_context/TextureCoordinateHandler.Vec3fArrayConversionContext.cpp"

    # Calculators.
    "src/calculators/CalculatorResult.hpp"
    "src/calculators/BoxCalculator.hpp"
    "src/calculators/BoxCalculator.cpp"
    "src/calculators/IndexedFaceSetCalculator.hpp"
    "src/calculators/IndexedFaceSetCalculator.cpp"
    "src/calculators/MeshSimplificator.hpp"
    "src/calculators/errors/CalculatorError.hpp"
    "src/calculators/test.hpp"
)

# Add source files and vendor dependencies.
target_include_directories(tostl PUBLIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/actions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/conversion_context
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calculators
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calculators/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/io

    ${CMAKE_CURRENT_SOURCE_DIR}/../vrml_proc/src

    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result
    ${CGAL_INCLUDE_DIRS}
)

# Two build types - Debug and Release.
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Offer two macros based on the build type.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)
endif()

# Link the tostl library with vrmlproc shared library.
target_link_libraries(tostl PRIVATE vrmlproc)

# --------------------------------------------------------------------------------------- #

# Add the test executables.
add_executable(VrmlFileTraversorUnitTests "tests/VrmlFileTraversorTest.cpp")
add_executable(CalculatorsUnitTests "tests/CalculatorsTest.cpp")

# Apply /bigobj flag for MSVC.
if (MSVC)
    target_compile_options(VrmlFileTraversorUnitTests PRIVATE /bigobj)
endif()

# Link Catch2 with the test executables.
target_link_libraries(VrmlFileTraversorUnitTests PRIVATE Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread Catch2::Catch2WithMain vrmlproc tostl)
target_link_libraries(CalculatorsUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc tostl)

# Enable testing (use CTest to run the unit tests).
enable_testing()
add_test(NAME VrmlFileTraversorUnitTests COMMAND VrmlFileTraversorUnitTests)
add_test(NAME CalculatorsUnitTests COMMAND CalculatorsUnitTests)

# Copy vrmlproc.dll to the folder for tests to run.
add_custom_command(TARGET VrmlFileTraversorUnitTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:tostl>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
)
