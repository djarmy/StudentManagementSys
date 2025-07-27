#ifndef COURSE_H
#define COURSE_H

typedef struct {
    int courseId;
    char courseName[50];
    int credit;
} Course;

void displayCourse(const Course *c);

#endif // COURSE_H
