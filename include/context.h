/*
��ģ�����ͳһ��װ����
����Է�װһ���ṹ�� SystemContext   ��ͳһ�������ģ���ָ���������
*/



#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
 
#include "context.h" 
#include "student.h"  
// // ǰ������������ѭ��������    
// typedef struct Major Major; 
#include "utils.h"
#include "state.h"
#include "course.h"
#include "score.h"
#include "dormitory.h"
#include "card.h"
#include "health.h"
#include "major.h"
#include "system.h"

//�½�SystemConfig������ṹ��(���ڴ洢ϵͳȫ�����ò���)Ŀǰ4����Ա
typedef struct SystemConfig
{
    char systemMode[20];    //ϵͳ����ģʽ(���翪������"dev",��������"prod")
    int maxStudents;        //ϵͳ֧�ֵ����ѧ������
    int maxMajors;          //ϵͳ֧�ֵ����רҵ����
    int enableLog;          //��־���ܿ���(0�ر� or 1����)
}SystemConfig;


// ϵͳ������,����ģ��Ĳ�������״̬
typedef struct SystemContext
{
    Student *students;    //ѧ����Ϣ�ṹ��(�洢����ѧ���Ļ���������Ϣ)Ŀǰ5����Ա
    int *stuCount;     //ָ��ʵ��ѧ������������ָ��

    // Major *majors;
    // int *majorCount;
    MajorHashTable *majorTable;

   

    Course *courses;
    int *courseCount;

    Score *scores;
    int *scoreCount;    //ָ������ɼ�����

    Card *cards;
    int *cardCount;

    // Dormitory *dormitories;
    // int *dormitoryCount; 

    // HealthRecord *healthRecords;
    // int *healthCount;

    //���������� �����Ա
    SystemConfig config;
} SystemContext;
 
bool initSystemContext(SystemContext *ctx);    //��ָ������ö������������޸�ͬһ������ֵ

void saveAllData(SystemContext *ctx);

void freeSystemContext(SystemContext *ctx);  

#endif // CONTEXT_H