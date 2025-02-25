cmake_minimum_required(VERSION 3.10)
project(vrmlxpy)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    # On Windows, use static Boost libraries to avoid DLL dependency issues.
    # This ensures that Boost is linked statically into the executable, making distribution easier.
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
else()
    # On Linux, prefer dynamic Boost libraries for better compatibility and smaller binaries.
    # Most Linux distributions provide Boost as shared libraries, making this the default.
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

# CMake will look for Boost libraries specified by BOOST_ROOT variable.
find_package(Boost REQUIRED COMPONENTS iostreams log date_time log_setup thread system)

if (Boost_FOUND)
    message(STATUS "Boost found!")
    message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost was not found. Please set BOOST_ROOT to your Boost installation.")
endif()

set(CGAL_Boost_INCLUDE_DIR ${Boost_INCLUDE_DIRS})
set(CGAL_Boost_LIBRARY_DIR ${Boost_LIBRARY_DIRS})
#if(WIN32)
set(CGAL_Boost_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS})  # Ensure CGAL links Boost the same way
#endif()
set(CGAL_Boost_USE_MULTITHREADED ${Boost_USE_MULTITHREADED})

# Find CGAL.
find_package(CGAL CONFIG REQUIRED)
if(CGAL_FOUND)
    message(STATUS "CGAL found!")
    message(STATUS "CGAL version: ${CGAL_VERSION}")
    message(STATUS "CGAL include directory: ${CGAL_INCLUDE_DIRS}")
    message(STATUS "CGAL libraries: ${CGAL_LIBRARIES}")
    include_directories(${CGAL_INCLUDE_DIRS})
    link_directories(${CGAL_LIBRARIES})
else()
    message(FATAL_ERROR "CGAL not found. Please set CGAL_DIR to the correct path.")
endif()

# Add the vrml_proc and to_stl directories.
add_subdirectory(vrml_proc)
add_subdirectory(to_stl)

# Add pybind11 from local vendor folder.
add_subdirectory(vendor/pybind11)

# Create the Python bindings module.
pybind11_add_module(vrmlxpy SHARED src/vrmlxpy.cpp)

# Link against your C++ libraries.
target_link_libraries(vrmlxpy PRIVATE vrmlproc tostl pybind11::module Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread)

# Ensure vrmlxpy.cpp can access headers from vrml_proc, to_stl and pybind11 projects.
target_include_directories(vrmlxpy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vrml_proc
    ${CMAKE_CURRENT_SOURCE_DIR}/to_stl
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/pybind11/include/pybind11
)

# Copy vrmlproc.dll and tostl.dll to the folder for the binding to run.
add_custom_command(TARGET vrmlxpy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:vrmlxpy>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:tostl>"
        "$<TARGET_FILE_DIR:vrmlxpy>"
)
