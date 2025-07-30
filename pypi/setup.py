import os
import platform
from setuptools import setup, find_packages

if platform.system() == "Windows":
    binary_files = ["vrmlxpy/vrmlxpy.pyd", "vrmlxpy/vrmlproc.dll", "vrmlxpy/tostl.dll"]
elif platform.system() == "Linux":
    binary_files = ["vrmlxpy/vrmlxpy.so", "vrmlxpy/libvrmlproc.so", "vrmlxpy/libtogeom.so", "vrmlxpy/libboost_iostreams.so.1.74.0", "vrmlxpy/libboost_log.so.1.74.0", "vrmlxpy/libboost_thread.so.1.74.0", "vrmlxpy/libboost_filesystem.so.1.74.0"]
else:
    binary_files = []

setup(
    name="vrmlxpy",
    version="1.0.4",
    author="Marek Eibel",
    description="Toolkit for VRML parsing and traversing. Includes a standalone VRML parser library and a conversion library for transforming VRML geometry into geometry format such as STL, with modular C++ backends and Python bindings.",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    url="https://github.com/kerrambit/vrmlxpy",
    include_package_data=True,
    classifiers=[
         "Programming Language :: Python :: 3",
         "Programming Language :: C++",
         "Operating System :: Microsoft :: Windows",
         "Operating System :: POSIX :: Linux",
    ],
    python_requires=">=3.10",
    zip_safe=False,
)
