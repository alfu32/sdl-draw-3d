import ctypes
import sys

# Load the shared library (DLL)
main_program = ctypes.CDLL('C:\\Users\\alferio\\Development\\sdl-draw-3d\\build\\win64\\voxd31.dll')

# Define the argument types and return type for the C main function
main_program.main.argtypes = (ctypes.c_int, ctypes.POINTER(ctypes.POINTER(ctypes.c_char)))
main_program.main.restype = ctypes.c_int

# Get the command-line arguments passed to Python (sys.argv)
# sys.argv includes the script name as the first argument, just like in C
args = sys.argv

# Convert the Python list of arguments to a list of bytes (C strings)
args_c = [arg.encode('utf-8') for arg in args]

# Calculate argc (number of arguments)
argc = len(args_c)

# Create argv: a ctypes array of char* (char pointers)
argv = (ctypes.POINTER(ctypes.c_char) * argc)(*map(ctypes.create_string_buffer, args_c))

# Call the C main function with argc and argv
main_program.main(argc, argv)
