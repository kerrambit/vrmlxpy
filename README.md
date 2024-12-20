# vrmlxpy
A Python-based toolkit for VRML parsing and STL conversion. Includes a standalone VRML parser library and a conversion library for transforming VRML geometry into STL format, with modular C++ backends and Python bindings.

## Prerequisites
- You need to have the [Boost](https://www.boost.org/) installed on your system. Please follow instructions [here](doc/boost_installation.md).
- The project uses [Ninja](https://ninja-build.org/) build system. Please follow the installation instructions of this software based on your system vendor.
- It comes naturally that you should have [CMake](https://cmake.org/) installed on your system as well.
- C++ compiler.

## Build
### Linux
- You can choose to build the library based on two types of build configurations: **Debug**

  ```
	cmake --preset Debug
  ```
- or **Release**.
  ```
	cmake --preset Release
  ```
- After necessary files are generated, you are able to build.
- Note that ```out/build/Debug``` in the following command is folder for **Debug** built, for **Release**, use ```out/build/Release```.
	```
	cmake --build out/build/Debug
	```

### Windows
- It is recommended to develop in *Visual Studio*.
- To open this project, navigate yourself to ```File - Open - Cmake...``` and choose the root *CMakeLists.txt*.
- *Visual Studio* should take care of the burden connecting with generating and building the library itself.