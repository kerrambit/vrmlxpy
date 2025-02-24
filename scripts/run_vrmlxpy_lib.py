from vrmlxpy import *
import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <input_path> <output_directory>")
    sys.exit(1)

input_filename = sys.argv[1]
output_directory = sys.argv[2]

convert_vrml_to_stl(input_filename, output_directory)