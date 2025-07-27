#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
#include "../include/context.h" 
#include "../include/major.h"
#include "../include/config_persist.h"
#include "../include/student_persist.h" 




/*
 gcc  -Wformat src/*.c   -Iinclude -o a     // -I����ָ��includeĿ¼
 gcc -Wformat -Wall -Werror src/*.c   -Iinclude -o a
 # ʹ�ø��ϸ�ı���ѡ��
gcc -Wall -Wextra -Werror -g -fsanitize=address src/*.c -Iinclude -o a
  
*/

 

int main(int argc, char const *argv[])
{  
    SystemContext *context =malloc(sizeof(SystemContext));       // �������������
    if (!context)
    {
        printf("�ڴ����ʧ��\n");
        return 1;
    }
    printf("ϵͳ��ʼ�����\n"); // ȷ�ϳ�ʼ���ɹ�    

    // ���ú�����ʼ���ṹ���Ա���������䶯̬�ڴ棩 
    if (!initSystemContext(context)) 
    {
    printf("ϵͳ��ʼ��ʧ�ܣ�\n");
    return 1;
    }
    

    // //printf("������ϣ�����\n");
    // //  ������ϣ�����
    // MajorHashTable *majorTable = (MajorHashTable *)malloc(sizeof(MajorHashTable));  *�Ѿ��� initSystemContext() ������˳�ʼ��������ҪҲ���Բ����������³�ʼ��һ�Σ�
    // if (!majorTable) 
    // {
    //     printf("���� MajorHashTable ʧ�ܡ�\n");
    //     return 1;
    // }
    // if (!initMajorHashTable(majorTable, HASH_TABLE_SIZE, hash_func_int)) 
    // {
    //     printf("��ϣ���ʼ��ʧ�ܣ������˳���\n");
    //     return 1;
    // }
    // context->majorTable = majorTable;  //     д��������
    

    // //loginSystem() ;

    //ϵͳ���˵�ѡ��ö��
    int mainChoice = -1;    //MenuOption

    do 
    { 
        // ���˵�ѭ�� 
        printf("--- ��ǰ����'ѧ������ϵͳ��ҳ'���˵�ҳ�� ---\n");
        showMainMenu();   
        printf("��ѡ�����ִ�в�����");  
        //clearInputBuffer();   
        scanf("%d", &mainChoice);    //(int*)
        clearInputBuffer();   
 

        // ע��״̬�仯�ص�
        registerStateChangeListener(onStudentCountChanged);
         
        
        //����ǰϵͳ���˵�
        handleMenuChoice(mainChoice, context );    //st1, &stuNum , major1, &majNum

    } while (mainChoice != EXIT_SYSTEM);

    saveAllData(context);   
    printf("ϵͳ���ڱ����������ݣ����Ժ�...\n");
    //��ϵͳ�˳�ǰ���׳־û���������

    // �ͷ��ڴ�
    freeSystemContext(context);

    // �������ǰע���ص�����ѡ��
    unregisterStateChangeListener(onStudentCountChanged);    
    return 0;
    
}

    