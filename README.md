# vrmlxpy
Toolkit for VRML parsing and traversing.
Includes a standalone VRML parser library and a conversion library for transforming VRML geometry into STL format, with modular C++ backends and Python bindings.

## License
This project is licensed under the **GNU General Public License v3.0 or later (GPL-3.0-or-later)**.  
See the [LICENSE](LICENSE) file for details.

## Run  as Python library
- The easiest way how to use this library is to use *vrmlxpy* as a Python library - you do not need to build anything yourself.
- Please visit [official vrmlxpy PyPi page](https://pypi.org/project/vrmlxpy/) to read more.
- Basically, the steps are to install vrmlxpy library via ```pip install vrmlxpy``` command.
- To get the idea how to use the library in action, have a look at example [script](scripts/run_vrmlxpy_lib.py).

## Run as Docker container
- Another recommended way how to run vrmlxpy is to build *Docker* image and then run it.
- **General steps**:
	- Make sure your systems has *Docker* installed.
	- Download our [Dockerfile](Dockerfile) - copy/paste should be enough.
	- Build the image using ```docker build -t <IMAGE_NAME> .```.
	  It is possible to specify the branch from which this reposity which will be cloned by adding ```--build-arg BRANCH=<BRANCH_NAME_>```.
	- Before running the container, make sure you have created two folders ```input``` and ```output``` on your current path.
	- Insert into ```input``` folder your ```.wrl``` files the configuration file (it can look like [this](vrmlxpyConfig.json.example)).
	- Use the following command to run the conversion (command is quite long, as we need to bind the fodlers into container).
	- ```docker run -v $(pwd)/input:/app/input -v $(pwd)/output:/app/output vrmlxpy python3 /app/out/build/Release/run_vrmlxpy_from_docker.py /app/input/<YOUR_VRML_FILE> /app/output/<OUTPUT_FILENAME> /app/input/<CONFIG_FILE_>```

## Prerequisites
- You need to have the [Boost](https://www.boost.org/) installed on your system. Please follow instructions [here](doc/boost_installation.md).
- Other third part library used is called [CGAL](https://www.cgal.org/). Follow instructions [here](doc/cgal_installation.md).
- For Python bindings creation, the solution uses [pybind11](https://github.com/pybind/pybind11). See the steps [here](doc/pybind11_installation.md).
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
- To open the project, navigate yourself to ```File - Open - Cmake...``` and choose the root *CMakeLists.txt*.
- *Visual Studio* should handle the process of generating and building the library automatically.

## Run C++ executable files
### Linux
- If your system has *Valgrind* installed, you can run the following commands like this:

  ```
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes <YOUR PROGRAM>
  ```
- You can use *CTest* which is a program that comes with *CMake*; it handles running the tests for the project.
- Before running the command below, make sure that tests have access to configuration file ```testConfig.json``` which should
  be inside ```vrmlxpy\out\build\Release\to_geom``` folder (or ```vrmlxpy\out\build\Debug\to_geom```) as one of the tests uses it.
  The paths in the configuration file should point to directories with test data.
- Run the following command (once again, note that you might need to exchange *Debug* with *Release* if you have built for this type):

	```
	ctest --test-dir out/build/Debug/vrml_proc
	```
- You might want to use ```--verbose``` for the command above.

### Windows
- TODO

## Troubleshooting