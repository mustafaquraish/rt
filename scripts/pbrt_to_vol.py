import re
import sys

if len(sys.argv) < 2:
    print("Usage: pbrt_to_vol.py <pbrt_volume_file> [output_file]")
    sys.exit(1)

input_file_name = sys.argv[1]
if len(sys.argv) > 2:
    output_file_name = sys.argv[2]
else:
    output_file_name = input_file_name.replace(".pbrt", ".vol")

input_file = open(input_file_name, "r").read()
output_file = open(output_file_name, "w")

def get_dimension(axis):
    regex_string = fr'"integer n{axis}"\s*\[?\s*(\d+)\s*\]?'
    match = re.search(regex_string, input_file)
    if not match:
        print("Could not find dimension for axis {}".format(axis))
        sys.exit(1)
    return int(match.group(1))

nx = get_dimension("x")
ny = get_dimension("y")
nz = get_dimension("z")

match = re.search(r'"float density"\s*\[', input_file)  
data = input_file[match.end():]
data = data[:data.find("]")].strip()

print("Dimensions: ", nx, ny, nz)

output_file.write("VOL\n")
output_file.write(f"{nx} {ny} {nz}\n")
output_file.write(data)