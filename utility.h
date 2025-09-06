/*
 * Author: Joseph Prince Aniekeme
 * Created: 3/July/2024
 * Modified: 6/September/2025
 * Description: Header file for CGPA calculator shared library.
 */
#ifndef UTILITY_H
#define UTILITY_H

#define MAX_COURSES 100
#define MAX_SUBJECT_LENGTH 50

typedef struct {
    int semester;
    char course[MAX_SUBJECT_LENGTH];
    int unit;
    char grade;
} Course;

char* viewSemester(int semester, const char* file_path);
int addCourse(int semester, const char* course_code, int unit, int score, const char* file_path);
int updateCourse(const char* course_code, int new_semester, int new_unit, int new_score, const char* file_path);
int deleteCourse(const char* course_code, const char* file_path);

#endif
