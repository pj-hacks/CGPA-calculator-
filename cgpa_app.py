import ctypes

# Load the shared library
lib = ctypes.cdll.LoadLibrary("./cgpa_lib.so")  # Use cgpa_lib.dll on Windows

# Define function signatures
lib.viewSemester.argtypes = [ctypes.c_int, ctypes.c_char_p]
lib.viewSemester.restype = ctypes.c_char_p
lib.addCourse.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
lib.addCourse.restype = ctypes.c_int
lib.updateCourse.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
lib.updateCourse.restype = ctypes.c_int
lib.deleteCourse.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.deleteCourse.restype = ctypes.c_int

# Helper to convert Python string to C string
def to_c_string(s):
    return s.encode('utf-8')

# Wrapper functions
def view_semester(semester):
    result = lib.viewSemester(semester, to_c_string("results.csv"))
    return result.decode('utf-8')

def add_course(semester, course, unit, score):
    return lib.addCourse(semester, to_c_string(course), unit, score, to_c_string("results.csv"))

def update_course(course_name, new_semester, new_unit, new_score):
    return lib.updateCourse(to_c_string(course_name), new_semester, new_unit, new_score, to_c_string("results.csv"))

def delete_course(course_name):
    return lib.deleteCourse(to_c_string(course_name), to_c_string("results.csv"))
