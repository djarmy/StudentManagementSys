/*
# ����test.c����������Դ�ļ�
gcc -I../include     .\test.c    ../src/student.c ../src/score.c ../src/dormitory.c          -o a.exe

���ߣ�    ����Ŀ���̸�Ŀ¼�£�gcc src/*.c test/test.c -Iinclude -o a
*/

#include <stdio.h>
#include "../include/score.h"
#include "../include/student.h"

#include "../include/dormitory.h"

int main() {
    Student s;
    Score sc;
    Dorm d;

    addStudent(&s);
    displayStudent(&s);

    printf("\n������ɼ������� ��ѧ Ӣ���\n");
    scanf("%f %f %f", &sc.chinese, &sc.math, &sc.english);
    sc.studentId = s.id;

    displayScore(&sc);

    int ids[] = {s.id};
    assignDorm(&d, ids, 1);
    displayDorm(&d);

    return 0;
}
