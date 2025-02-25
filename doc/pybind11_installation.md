# pybind11 Setup Guide

For the creation of Python bindings, some library is needed. The pybind11 is a lightweight header-only library that exposes C++ types in Python.
While Boost.Python could have been used for this task, pybind11 was chosen as it is considered a better option for future compatibility. For more information, click [here](https://pybind11.readthedocs.io/en/stable/).

### Installation

1. **Download pybind11**:
   - Use [official releases](https://github.com/pybind/pybind11/releases/tag/v2.13.6) and download the archive.
   - For smooth cooperation with *CMake*, it is needed to extract the archive to ```vendor``` folder and rename it from e.g. ```pybind11-2.13.6``` only to ```pybind11```.
   - The structure should like this: ```vendor/pybind11```. Inside this folder, you should see ```CMakeLists.txt```  file and ```include``` directory (apart from many other files).

2. **Installation**:
   - There is no installation needed, *Cmake* will take care of everything from this point.
   - After the project itself and bindings are built, you should see in ```out/build/Debug``` (or ```out/build/Release```),
   a file called similiar to ```vrmlxpy.cpython-312-x86_64-linux-gnu.so``` on *Linux*, on *Windows* ```vrmlxpy.cp310-win_amd64.pyd```.
   - This is the Python binding. Note that it needs also ```vrmlproc.dll``` (```libvrmlproc.so```) and ```tostl.dll``` (```libtostl.so```) libraries.
   - How to use it in Python code, come back to main [README.md](../README.md).
