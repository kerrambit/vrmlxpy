cmake_minimum_required(VERSION 3.10)
project(vrmlxpy)

set(CMAKE_CXX_STANDARD 20)

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

# Add vendor dependencies.
# include_directories(vendor/pybind11/include)

# Add the pyvrml directory.
add_subdirectory(vrml_proc)