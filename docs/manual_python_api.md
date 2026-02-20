## API of vrmlxpy - Python bindings library

### Version

A function that prints a current vrmlx version.

```bash
print_version() -> None
```

### Single Conversion

Converts a VRML file to a geometry format based on a configuration file

```bash
convert_vrml(input_filename: str, output_filename: str, config_filename: str) -> bool
```

### Bulk Conversion

```(WIP)```

<!-- > **Note**: The `<config_file>` must be present in the `<input_folder>` when using bulk conversion. -->

---

## Configuration File

The `<config_file>` is a JSON file that defines behavior for the application. All options have default values. For default values, and examples of configuration files, please visit [this manual](manual.md).