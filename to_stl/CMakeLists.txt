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

# Find CGAL.
find_package(CGAL CONFIG REQUIRED)
if(CGAL_FOUND)
    message(STATUS "CGAL found!")
    message(STATUS "CGAL version: ${CGAL_VERSION}")
    message(STATUS "CGAL include directory: ${CGAL_INCLUDE_DIRS}")
    message(STATUS "CGAL libraries: ${CGAL_LIBRARIES}")
else()
    message(FATAL_ERROR "CGAL not found. Please set CGAL_DIR to the correct path.")
endif()

# --------------------------------------------------------------------------------------- #

# Create the DLL.
add_library(tostl SHARED

    # Third party libraries.
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result/result.hpp"

    # DLL export macro.
    "src/VrmlToStlExport.hpp"

    # Actions.
    "src/actions/IndexedFaceSetAction.hpp"
    "src/actions/IndexedFaceSetAction.cpp"
    "src/actions/CoordinateAction.hpp"
    "src/actions/CoordinateAction.cpp"
    "src/actions/NormalAction.hpp"
    "src/actions/NormalAction.cpp"
    "src/actions/TextureCoordinateAction.hpp"
    "src/actions/TextureCoordinateAction.cpp"
    "src/actions/GroupAction.hpp"
    "src/actions/GroupAction.cpp"
    "src/actions/BoxAction.hpp"
    "src/actions/BoxAction.cpp"
    "src/actions/ShapeAction.hpp"
    "src/actions/ShapeAction.cpp"

    "src/calculators/test.hpp"
)

# Add source files and vendor dependencies.
target_include_directories(tostl PUBLIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/actions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calculators

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
target_link_libraries(tostl PRIVATE CGAL::CGAL vrmlproc)

# --------------------------------------------------------------------------------------- #

# Add the test executables.
add_executable(VrmlFileTraversorUnitTests "tests/VrmlFileTraversorTest.cpp")

# Link Catch2 with the test executables.
target_link_libraries(VrmlFileTraversorUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc tostl)

# Enable testing (use CTest to run the unit tests).
enable_testing()
add_test(NAME VrmlFileTraversorUnitTests COMMAND VrmlFileTraversorUnitTests)

# Copy vrmlproc.dll to the folder for tests to run.
add_custom_command(TARGET VrmlFileTraversorUnitTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:tostl>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
)
