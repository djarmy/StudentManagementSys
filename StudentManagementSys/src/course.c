#include <stdio.h>
#include "course.h"

void displayCourse(const Course *c) {
    printf("�γ̱�ţ�%d\n�γ����ƣ�%s\nѧ�֣�%d\n",
           c->courseId, c->courseName, c->credit);
}
