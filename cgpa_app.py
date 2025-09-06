"""
Author: Joseph Prince Aniekeme
Created: 3/July/2024
Modified: 6/September/2025
Description: Python wrapper for CGPA calculator shared library with user-specific file handling.
"""
import ctypes
import streamlit as st
import logging

logging.basicConfig(level=logging.DEBUG)

try:
    lib = ctypes.cdll.LoadLibrary("./cgpa_lib.so")
    logging.info("Successfully loaded cgpa_lib.so")
except OSError as e:
    st.error(f"Failed to load cgpa_lib.so: {e}")
    raise

# Define function signatures
lib.viewSemester.argtypes = [ctypes.c_int, ctypes.c_char_p]
lib.viewSemester.restype = ctypes.c_char_p
lib.addCourse.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
lib.addCourse.restype = ctypes.c_int
lib.updateCourse.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
lib.updateCourse.restype = ctypes.c_int
lib.deleteCourse.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.deleteCourse.restype = ctypes.c_int

def to_c_string(s):
    if not s:
        st.error("Input string cannot be empty")
        raise ValueError("Input string cannot be empty")
    return s.encode('utf-8')

def view_semester(semester, filename):
    try:
        result = lib.viewSemester(semester, to_c_string(filename))
        return result.decode('utf-8')
    except Exception as e:
        st.error(f"Error in view_semester: {e}")
        raise

def add_course(semester, course, unit, score, filename):
    try:
        return lib.addCourse(semester, to_c_string(course), unit, score, to_c_string(filename))
    except Exception as e:
        st.error(f"Error in add_course: {e}")
        raise

def update_course(course_name, new_semester, new_unit, new_score, filename):
    try:
        return lib.updateCourse(to_c_string(course_name), new_semester, new_unit, new_score, to_c_string(filename))
    except Exception as e:
        st.error(f"Error in update_course: {e}")
        raise

def delete_course(course_name, filename):
    try:
        return lib.deleteCourse(to_c_string(course_name), to_c_string(filename))
    except Exception as e:
        st.error(f"Error in delete_course: {e}")
        raise
