from vrmlxpy import *
import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <input_file> <output_file>")
    sys.exit(1)

input_filename = sys.argv[1]
output_filename = sys.argv[2]

print_version()
convert_vrml_to_stl(input_filename, output_filename)