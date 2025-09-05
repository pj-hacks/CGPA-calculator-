/*
author: JOSEPH PRINCE ANIEKEME
CREATED ON: 3/JULY/2024
MODIFIED ON: 5/SEPTEMBER/2025
DESCRIPTION: Shared library for CGPA calculator with CSV storage and course management.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COURSES 100
#define MAX_SUBJECT_LENGTH 50

typedef struct {
  int semester;
  char course[MAX_SUBJECT_LENGTH];
  int unit;
  char grade;
} Course;

// Convert score to grade point
int scoreToPoint(int score)
{
  if (score >= 70) return 5; // A
  else if (score >= 60) return 4; // B
  else if (score >= 50) return 3; // C
  else if (score >= 45) return 2; // D
  else if (score >= 40) return 1; // E
  else return 0; // F
}

// Convert grade point to letter grade
char pointToGrade(int point)
{
  switch (point) {
  case 5:
    return 'A';
  case 4:
    return 'B';
  case 3:
    return 'C';
  case 2:
    return 'D';
  case 1:
    return 'E';
  case 0:
    return 'F';
  default:
    return 'F';
  }
}

// Load courses from CSV
int loadCSV(const char *filename, Course courses[])
{
  FILE *file = fopen(filename, "r");
  if (!file) return 0;
  char line[256];
  int count = 0;
  fgets(line, sizeof(line), file); // Skip header
  while (fgets(line, sizeof(line), file) && count < MAX_COURSES) {
    char *token = strtok(line, ",");
    courses[count].semester = atoi(token);
    token = strtok(NULL, ",");
    strncpy(courses[count].course, token, MAX_SUBJECT_LENGTH - 1);
    courses[count].course[MAX_SUBJECT_LENGTH - 1] = '\0';
    token = strtok(NULL, ",");
    courses[count].unit = atoi(token);
    token = strtok(NULL, ",\n");
    courses[count].grade = token[0];
    count++;
  }
  fclose(file);
  return count;
}

// Save courses to CSV
void saveCSV(const char *filename, Course courses[], int count)
{
  FILE *file = fopen(filename, "w");
  if (!file) return;
  fprintf(file, "Semester,Course,Unit,Grade\n");
  for (int i = 0; i < count; i++) {
    fprintf(file, "%d,%s,%d,%c\n", courses[i].semester,
            courses[i].course, courses[i].unit, courses[i].grade);
  }
  fclose(file);
}

// View semester results
char* viewSemester(int semester, const char *filename)
{
  Course courses[MAX_COURSES];
  int count = loadCSV(filename, courses);
  static char result[4096] = {0};
  int totalUnits = 0, totalPoints = 0;
  char temp[256];
  snprintf(result, sizeof(result), "Semester %d Results:\n----------------------------------\n", semester);
  for (int i = 0; i < count; i++) {
    if (courses[i].semester == semester) {
      int gp = scoreToPoint(courses[i].grade == 'A' ? 70 : courses[i].grade == 'B' ? 60 :
                            courses[i].grade == 'C' ? 50 : courses[i].grade == 'D' ? 45 :
                            courses[i].grade == 'E' ? 40 : 0);
      totalUnits += courses[i].unit;
      totalPoints += courses[i].unit * gp;
      snprintf(temp, sizeof(temp), "Course: %s | Unit: %d | Grade: %c\n",
               courses[i].course, courses[i].unit, courses[i].grade);
      strncat(result, temp, sizeof(result) - strlen(result) - 1);
    }
  }
  if (totalUnits > 0) {
    float cgpa = (float)totalPoints / totalUnits;
    snprintf(temp, sizeof(temp), "----------------------------------\nCGPA for Semester %d = %.2f\n",
             semester, cgpa);
    strncat(result, temp, sizeof(result) - strlen(result) - 1);
  } else {
    strncat(result, "No records found.\n", sizeof(result) - strlen(result) - 1);
  }
  return result;
}

// Add a course
int addCourse(int semester, const char *course, int unit, int score, const char *filename)
{
  if (score < 0 || score > 100) return -1;
  Course courses[MAX_COURSES];
  int count = loadCSV(filename, courses);
  if (count >= MAX_COURSES) return -1;
  courses[count].semester = semester;
  strncpy(courses[count].course, course, MAX_SUBJECT_LENGTH - 1);
  courses[count].course[MAX_SUBJECT_LENGTH - 1] = '\0';
  courses[count].unit = unit;
  courses[count].grade = pointToGrade(scoreToPoint(score));
  count++;
  saveCSV(filename, courses, count);
  return 0;
}

// Update a course
int updateCourse(const char *course_name, int new_semester, int new_unit, int new_score, const char *filename)
{
  if (new_score < 0 || new_score > 100) return -1;
  Course courses[MAX_COURSES];
  int count = loadCSV(filename, courses);
  for (int i = 0; i < count; i++) {
    if (strcmp(courses[i].course, course_name) == 0) {
      courses[i].semester = new_semester;
      courses[i].unit = new_unit;
      courses[i].grade = pointToGrade(scoreToPoint(new_score));
      saveCSV(filename, courses, count);
      return 0;
    }
  }
  return -1;
}

// Delete a course
int deleteCourse(const char *course_name, const char *filename)
{
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
