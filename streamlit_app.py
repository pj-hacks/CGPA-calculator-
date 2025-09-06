"""
Author: Joseph Prince Aniekeme
Created: 3/July/2024
Modified: 6/September/2025
Description: Streamlit frontend for CGPA calculator.
"""
import os
import subprocess
import streamlit as st
from cgpa_app import view_semester, add_course, update_course, delete_course

# Compile cgpa_lib.so if not present
if not os.path.exists("cgpa_lib.so"):
    try:
        subprocess.run(["gcc", "-shared", "-fPIC", "-o", "cgpa_lib.so", "CGPA_CALCULATOR.c", "utility.c"], check=True)
        st.success("Compiled cgpa_lib.so successfully")
    except subprocess.CalledProcessError as e:
        st.error(f"Failed to compile cgpa_lib.so: {e}")
        raise

st.title("CGPA Manager")
option = st.sidebar.selectbox("Choose an action", ["View Semester", "Add Course", "Update Course", "Delete Course"])

if option == "View Semester":
    semester = st.number_input("Enter semester number", min_value=1, step=1)
    if st.button("View Results"):
        try:
            result = view_semester(semester)
            st.text(result)
        except Exception as e:
            st.error(f"Error viewing semester: {e}")

elif option == "Add Course":
    semester = st.number_input("Enter semester", min_value=1, step=1)
    course = st.text_input("Enter course code")
    unit = st.number_input("Enter unit", min_value=1, step=1)
    score = st.number_input("Enter score (0-100)", min_value=0, max_value=100, step=1)
    if st.button("Add Course"):
        if not course:
            st.error("Course code cannot be empty")
        else:
            try:
                result = add_course(semester, course, unit, score)
                st.success("Course added!" if result == 0 else "Error: Course limit reached or invalid score.")
            except Exception as e:
                st.error(f"Error adding course: {e}")

elif option == "Update Course":
    course = st.text_input("Enter course code to update")
    new_semester = st.number_input("Enter new semester", min_value=1, step=1)
    new_unit = st.number_input("Enter new unit", min_value=1, step=1)
    new_score = st.number_input("Enter new score (0-100)", min_value=0, max_value=100, step=1)
    if st.button("Update Course"):
        if not course:
            st.error("Course code cannot be empty")
        else:
            try:
                result = update_course(course, new_semester, new_unit, new_score)
                st.success("Course updated!" if result == 0 else "Error: Course not found or invalid score.")
            except Exception as e:
                st.error(f"Error updating course: {e}")

elif option == "Delete Course":
    course = st.text_input("Enter course code to delete")
    if st.button("Delete Course"):
        if not course:
            st.error("Course code cannot be empty")
        else:
            try:
                result = delete_course(course)
                st.success("Course deleted!" if result == 0 else "Error: Course not found.")
            except Exception as e:
                st.error(f"Error deleting course: {e}")
