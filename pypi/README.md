# vrmlx
Toolkit for parsing and traversing VRML files.
Includes a standalone VRML parser library (```vrmlproc```) and a conversion library for transforming VRML into geometry format such as STL (```togeom```), with modular C++ backends and Python bindings (```vrmlxpy```).

The modular architecture allows users to define their own actions—custom functions that process VRML nodes in any desired way. This flexibility enables conversions beyond STL, such as transforming VRML data into a custom geometry JSON format. Simply implement the necessary actions to achieve your desired output.

More information can be found on official [GitHub page](https://github.com/kerrambit/vrmlx).

## License
This project is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later). See the [LICENSE](https://github.com/kerrambit/vrmlx/blob/main/LICENSE) file for more details.

## API
Current API is defined in [this manual](https://github.com/kerrambit/vrmlx/blob/main/docs/manual_python_api.md).