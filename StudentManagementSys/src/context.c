#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/context.h"
#include "../include/major.h"  // �����һ�У�����Major����������
#include "../include/config_persist.h"
#include "../include/student_persist.h"
#include "../include/major_persist.h"

#define INITIAL_STUDENT_CAPACITY 100
#define INITIAL_MAJOR_CAPACITY 20
#define INITIAL_SCORE_CAPACITY 30

  

//��ʼ�������ģ�  ģ��ͳһ���ã�ֻ�� SystemContext *���������������ȥ�� 
bool initSystemContext(SystemContext *ctx) 
{
    if (ctx == NULL) return false; 

    // Ϊѧ����������ڴ�
    ctx->students = (Student *)calloc(INITIAL_STUDENT_CAPACITY, sizeof(Student));
    if (ctx->students == NULL) 
    {
        fprintf(stderr, "ѧ���ڴ����ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }

    // ����ѧ������ָ�벢��ʼ��
    ctx->stuCount = (int *)malloc(sizeof(int));
    if (ctx->stuCount == NULL) 
    {
        fprintf(stderr, "ָ��ѧ������ָ�����ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }
    *(ctx->stuCount) = 0;

    //����ѧ���־û�����(Binary)
    ctx->students = NULL;
    ctx->stuCount = malloc(sizeof(int));
    *(ctx->stuCount) = 0;
    if (!loadStudents(&ctx->students, ctx->stuCount))
    {
        printf("[����]δ�ܹ�����ѧ����Ϣ�����ӿձ�ʼ��\n");
    }
    
 

    
     // ��ȷ˳���ȳ�ʼ����ϣ������רҵ��ϣ��
    ctx->majorTable = malloc(sizeof(MajorHashTable));
    if (!ctx->majorTable) 
    {
        fprintf(stderr, "���� majorTable ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }
    if (!initMajorHashTable(ctx->majorTable, HASH_TABLE_SIZE, hash_func_int)) 
    {
        fprintf(stderr, "��ʼ�� major ��ϣ��ʧ�ܣ�\n");
        exit(EXIT_FAILURE);
    }
    // // Ȼ�����רҵ����
    // if (!loadMajors(ctx->majorTable))
    // {
    //     printf("[����]δ�ܹ�����רҵ��Ϣ�����ӿձ�ʼ��\n");
    // }
    // Ȼ�����רҵ����
    if (loadMajors(ctx->majorTable))
    {
        printf("�ɹ�����רҵ��Ϣ��\n");
    }


    //��������ɼ�����ָ�룬����ʼ��
    ctx->scores = (Score *)calloc(INITIAL_SCORE_CAPACITY, sizeof(Score));
    if (ctx->scoreCount == NULL)
    {
        fprintf(stderr, "ָ�� �����ɼ���ָ����ڴ����ʧ�ܣ�\n");
        exit(EXIT_FAILURE);  // ��������������һ��
    }
    //scores�������ɹ��� scoreCount���ӳ�ʼ��
    ctx->scoreCount = malloc(sizeof(int));
    if (ctx->scoreCount == NULL)
    {
        free(ctx->scores);
        fprintf(stderr, "�ɼ�����ָ�����ʧ�ܣ�\n");
         exit(EXIT_FAILURE);
    }
    
    *(ctx->scoreCount) = 0;
    
    //  δ������ģ��ʱ���������Ʋ�������



    //���س־û�����(JSON)
    if (!loadSystemConfig(&ctx->config))
    {
        //����ʧ�ܿ�������Ĭ����ֵ
        strcpy(ctx->config.systemMode, "dev");
        ctx->config.maxStudents = 100;
        ctx->config.maxMajors = 20;
        ctx->config.enableLog = 1;
    }
    return true ;
}

//���ӽ��飺��װ����־û����ݽӿ�Ϊ saveAllData(SystemContext *ctx)
void saveAllData(SystemContext *ctx)
{
    saveStudents(ctx->students, *(ctx->stuCount));
    saveMajors(ctx->majorTable);
    // ����ģ��...���ƺ���
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

    // δ����Ӹ���ģ��ҲҪ�ǵ��ͷ��ڴ�
}
