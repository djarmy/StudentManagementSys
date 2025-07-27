#include <stdio.h>
#include "course.h"

void displayCourse(const Course *c) {
    printf("课程编号：%d\n课程名称：%s\n学分：%d\n",
           c->courseId, c->courseName, c->credit);
}
