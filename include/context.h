/*
（模块参数统一封装）：
你可以封装一个结构体 SystemContext   来统一管理各个模块的指针和数量：
*/



#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
 
#include "context.h" 
#include "student.h"  
// // 前向声明（打破循环依赖）    
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

//新建SystemConfig配置项结构体(用于存储系统全局配置参数)目前4个成员
typedef struct SystemConfig
{
    char systemMode[20];    //系统运行模式(比如开发环境"dev",生产环境"prod")
    int maxStudents;        //系统支持的最大学生数量
    int maxMajors;          //系统支持的最大专业数量
    int enableLog;          //日志功能开关(0关闭 or 1开启)
}SystemConfig;


// 系统上下文,所有模块的参数共享状态
typedef struct SystemContext
{
    Student *students;    //学生信息结构体(存储单个学生的基本档案信息)目前5个成员
    int *stuCount;     //指向实际学生计数变量的指针

    // Major *majors;
    // int *majorCount;
    MajorHashTable *majorTable;

   

    Course *courses;
    int *courseCount;

    Score *scores;
    int *scoreCount;    //指向分数成绩计数

    Card *cards;
    int *cardCount;

    // Dormitory *dormitories;
    // int *dormitoryCount; 

    // HealthRecord *healthRecords;
    // int *healthCount;

    //新增配置项 对象成员
    SystemConfig config;
} SystemContext;
 
bool initSystemContext(SystemContext *ctx);    //用指针可以让多个函数共享和修改同一个计数值

void saveAllData(SystemContext *ctx);

void freeSystemContext(SystemContext *ctx);  

#endif // CONTEXT_H