/*
 * Author: Joseph Prince Aniekeme
 * Created: 3/July/2024
 * Modified: 6/September/2025
 * Description: Core CGPA calculator functions with CSV storage.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

// View semester results
char* viewSemester(int semester, const char* filename) {
    Course courses[MAX_COURSES];
    int count = loadCSV(filename, courses);
    static char result[4096] = {0}; // Static to persist return value
    result[0] = '\0'; // Clear buffer
    int totalUnits = 0, totalPoints = 0;
    char temp[256];
    snprintf(result, sizeof(result), "Semester %d Results:\n----------------------------------\n", semester);
    for (int i = 0; i < count && i < MAX_COURSES; i++) {
        if (courses[i].semester == semester) {
            int gp = scoreToPoint(courses[i].grade == 'A' ? 70 : courses[i].grade == 'B' ? 60 :
                                  courses[i].grade == 'C' ? 50 : courses[i].grade == 'D' ? 45 :
                                  courses[i].grade == 'E' ? 40 : 0);
            if (gp == -1) continue;
            totalUnits += courses[i].unit;
            totalPoints += courses[i].unit * gp;
            snprintf(temp, sizeof(temp), "Course: %s | Unit: %d | Grade: %c\n",
                     courses[i].course, courses[i].unit, courses[i].grade);
            if (strlen(result) + strlen(temp) < sizeof(result) - 1) {
                strncat(result, temp, sizeof(result) - strlen(result) - 1);
            }
        }
    }
    if (totalUnits > 0) {
        float cgpa = (float)totalPoints / totalUnits;
        snprintf(temp, sizeof(temp), "----------------------------------\nCGPA for Semester %d = %.2f\n",
                 semester, cgpa);
        if (strlen(result) + strlen(temp) < sizeof(result) - 1) {
            strncat(result, temp, sizeof(result) - strlen(result) - 1);
        }
    } else {
        strncat(result, "No records found.\n", sizeof(result) - strlen(result) - 1);
    }
    return result;
}

// Add a course
int addCourse(int semester, const char* course, int unit, int score, const char* filename) {
    if (!course || !filename || score < 0 || score > 100 || unit <= 0) return -1;
    Course courses[MAX_COURSES];
    int count = loadCSV(filename, courses);
    if (count >= MAX_COURSES) return -1;
    courses[count].semester = semester;
    strncpy(courses[count].course, course, MAX_SUBJECT_LENGTH - 1);
    courses[count].course[MAX_SUBJECT_LENGTH - 1] = '\0';
    courses[count].unit = unit;
    int gp = scoreToPoint(score);
    if (gp == -1) return -1;
    courses[count].grade = pointToGrade(gp);
    count++;
    saveCSV(filename, courses, count);
    return 0;
}

// Update a course
int updateCourse(const char* course_name, int new_semester, int new_unit, int new_score, const char* filename) {
    if (!course_name || !filename || new_score < 0 || new_score > 100 || new_unit <= 0) return -1;
    Course courses[MAX_COURSES];
    int count = loadCSV(filename, courses);
    for (int i = 0; i < count; i++) {
        if (strcmp(courses[i].course, course_name) == 0) {
            courses[i].semester = new_semester;
            courses[i].unit = new_unit;
            int gp = scoreToPoint(new_score);
            if (gp == -1) return -1;
            courses[i].grade = pointToGrade(gp);
            saveCSV(filename, courses, count);
            return 0;
        }
    }
    return -1;
}

// Delete a course
int deleteCourse(const char* course_name, const char* filename) {
    if (!course_name || !filename) return -1;
    Course courses[MAX_COURSES];
    int count = loadCSV(filename, courses);
    for (int i = 0; i < count; i++) {
        if (strcmp(courses[i].course, course_name) == 0) {
            for (int j = i; j < count - 1; j++) {
                courses[j] = courses[j + 1];
            }
            count--;
            saveCSV(filename, courses, count);
            return 0;
        }
    }
    return -1;
}
