cmake_minimum_required(VERSION 3.10)
project(vrmlxpy)

set(CMAKE_CXX_STANDARD 20)

# Add vendor dependencies.
include_directories(vendor/pybind11/include)
include_directories(vendor/boost_1_86_0)

# Add the pyvrml directory.
add_subdirectory(vrmlproc)