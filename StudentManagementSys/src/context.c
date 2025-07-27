#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/context.h"
#include "../include/major.h"  // 添加这一行，引入Major的完整定义
#include "../include/config_persist.h"
#include "../include/student_persist.h"
#include "../include/major_persist.h"

#define INITIAL_STUDENT_CAPACITY 100
#define INITIAL_MAJOR_CAPACITY 20
#define INITIAL_SCORE_CAPACITY 30

  

//初始化上下文，  模块统一调用：只传 SystemContext *，避免参数传来传去。 
bool initSystemContext(SystemContext *ctx) 
{
    if (ctx == NULL) return false; 

    // 为学生数组分配内存
    ctx->students = (Student *)calloc(INITIAL_STUDENT_CAPACITY, sizeof(Student));
    if (ctx->students == NULL) 
    {
        fprintf(stderr, "学生内存分配失败！\n");
        exit(EXIT_FAILURE);
    }

    // 分配学生计数指针并初始化
    ctx->stuCount = (int *)malloc(sizeof(int));
    if (ctx->stuCount == NULL) 
    {
        fprintf(stderr, "指向学生数量指针分配失败！\n");
        exit(EXIT_FAILURE);
    }
    *(ctx->stuCount) = 0;

    //加载学生持久化数据(Binary)
    ctx->students = NULL;
    ctx->stuCount = malloc(sizeof(int));
    *(ctx->stuCount) = 0;
    if (!loadStudents(&ctx->students, ctx->stuCount))
    {
        printf("[警告]未能够加载学生信息，将从空表开始。\n");
    }
    
 

    
     // 正确顺序，先初始化哈希表，加载专业哈希表
    ctx->majorTable = malloc(sizeof(MajorHashTable));
    if (!ctx->majorTable) 
    {
        fprintf(stderr, "分配 majorTable 失败！\n");
        exit(EXIT_FAILURE);
    }
    if (!initMajorHashTable(ctx->majorTable, HASH_TABLE_SIZE, hash_func_int)) 
    {
        fprintf(stderr, "初始化 major 哈希表失败！\n");
        exit(EXIT_FAILURE);
    }
    // // 然后加载专业数据
    // if (!loadMajors(ctx->majorTable))
    // {
    //     printf("[警告]未能够加载专业信息，将从空表开始。\n");
    // }
    // 然后加载专业数据
    if (loadMajors(ctx->majorTable))
    {
        printf("成功加载专业信息。\n");
    }


    //分配分数成绩管理指针，并初始化
    ctx->scores = (Score *)calloc(INITIAL_SCORE_CAPACITY, sizeof(Score));
    if (ctx->scoreCount == NULL)
    {
        fprintf(stderr, "指向 分数成绩的指针的内存分配失败！\n");
        exit(EXIT_FAILURE);  // 保持与其他分配一致
    }
    //scores数组分配成功， scoreCount增加初始化
    ctx->scoreCount = malloc(sizeof(int));
    if (ctx->scoreCount == NULL)
    {
        free(ctx->scores);
        fprintf(stderr, "成绩计数指针分配失败！\n");
         exit(EXIT_FAILURE);
    }
    
    *(ctx->scoreCount) = 0;
    
    //  未来新增模块时，继续类似操作即可



    //加载持久化配置(JSON)
    if (!loadSystemConfig(&ctx->config))
    {
        //加载失败可以设置默认数值
        strcpy(ctx->config.systemMode, "dev");
        ctx->config.maxStudents = 100;
        ctx->config.maxMajors = 20;
        ctx->config.enableLog = 1;
    }
    return true ;
}

//附加建议：包装保存持久化数据接口为 saveAllData(SystemContext *ctx)
void saveAllData(SystemContext *ctx)
{
    saveStudents(ctx->students, *(ctx->stuCount));
    saveMajors(ctx->majorTable);
    // 其他模块...类似函数
    saveSystemConfig(&ctx->config);
}

void freeSystemContext(SystemContext *ctx) 
{
    if (ctx == NULL) return;

    free(ctx->students);
    free(ctx->stuCount);

    if (ctx->majorTable) 
    {
    freeMajorHashTable(ctx->majorTable);
    ctx->majorTable = NULL;
    }

    // 未来添加更多模块也要记得释放内存
}
