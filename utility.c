/*
 * Author: Joseph Prince Aniekeme
 * Created: 3/July/2024
 * Modified: 6/September/2025
 * Description: Utility functions for CGPA calculator (grade and CSV handling).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

// Convert score to grade point
int scoreToPoint(int score) {
    if (score < 0 || score > 100) return -1;
    if (score >= 70) return 5; // A
    if (score >= 60) return 4; // B
    if (score >= 50) return 3; // C
    if (score >= 45) return 2; // D
    if (score >= 40) return 1; // E
    return 0; // F
}

// Convert grade point to letter grade
char pointToGrade(int point) {
    switch (point) {
        case 5: return 'A';
        case 4: return 'B';
        case 3: return 'C';
        case 2: return 'D';
        case 1: return 'E';
        case 0: return 'F';
        default: return 'F';
    }
}

// Load courses from CSV
int loadCSV(const char* filename, Course courses[]) {
    if (!filename) return 0;
    FILE* file = fopen(filename, "r");
    if (!file) return 0;
    char line[256];
    int count = 0;
    if (fgets(line, sizeof(line), file)) { // Skip header
        while (fgets(line, sizeof(line), file) && count < MAX_COURSES) {
            char* token = strtok(line, ",");
            if (!token) continue;
            courses[count].semester = atoi(token);
            token = strtok(NULL, ",");
            if (!token) continue;
            strncpy(courses[count].course, token, MAX_SUBJECT_LENGTH - 1);
            courses[count].course[MAX_SUBJECT_LENGTH - 1] = '\0';
            token = strtok(NULL, ",");
            if (!token) continue;
            courses[count].unit = atoi(token);
            token = strtok(NULL, ",\n");
            if (!token) continue;
            courses[count].grade = token[0];
            count++;
        }
    }
    fclose(file);
    return count;
}

// Save courses to CSV
void saveCSV(const char* filename, Course courses[], int count) {
    if (!filename) return;
    FILE* file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "Semester,Course,Unit,Grade\n");
    for (int i = 0; i < count && i < MAX_COURSES; i++) {
        fprintf(file, "%d,%s,%d,%c\n", courses[i].semester, courses[i].course,
                courses[i].unit, courses[i].grade);
    }
    fclose(file);
}
