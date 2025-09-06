"""
Author: Joseph Prince Aniekeme
Created: 3/July/2024
Modified: 6/September/2025
Description: Streamlit frontend for CGPA calculator with user-specific file handling and persistent access.
"""
import os
import glob
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

# Initialize session state for username
if 'username' not in st.session_state:
    st.session_state.username = ""

st.title("CGPA Manager")

# Get list of existing user files
user_files = glob.glob("*_results.csv")
existing_users = [os.path.splitext(os.path.basename(f))[0].replace("_results", "").replace("_", " ").title() for f in user_files]

# User input for name or selection from existing users
col1, col2 = st.columns([2, 1])
with col1:
    username_input = st.text_input("Enter your name", value=st.session_state.username)
with col2:
    selected_user = st.selectbox("Or select existing user", [""] + existing_users, index=0)

# Determine the username to use
username = username_input if username_input else selected_user
if username:
    st.session_state.username = username
    csv_filename = f"{username.lower().replace(' ', '_')}_results.csv"
    # Check if the user's file exists
    if os.path.exists(csv_filename):
        st.success(f"Welcome back, {username}! Your data file ({csv_filename}) was found.")
    else:
        st.info(f"New user, {username}. A new data file ({csv_filename}) will be created when you add a course.")

if not username:
    st.warning("Please enter your name or select an existing user to proceed.")
else:
    option = st.sidebar.selectbox("Choose an action", ["View Semester", "Add Course", "Update Course", "Delete Course"])

    if option == "View Semester":
        semester = st.number_input("Enter semester number", min_value=1, step=1)
        if st.button("View Results"):
            try:
                result = view_semester(semester, csv_filename)
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
                    result = add_course(semester, course, unit, score, csv_filename)
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
                    result = update_course(course, new_semester, new_unit, new_score, csv_filename)
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
                    result = delete_course(course, csv_filename)
                    st.success("Course deleted!" if result == 0 else "Error: Course not found.")
                except Exception as e:
                    st.error(f"Error deleting course: {e}")
