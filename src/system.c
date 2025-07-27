#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 

#include "../include/system.h"
#include "../include/context.h"
#include "../include/utils.h"

#include "../include/student.h" 
#include "../include/major.h"  // ��srcĿ¼����һ����include
#include "../include/student_persist.h"

/*
gcc -I../include     .\system.c    ../src/student.c ../src/score.c ../src/dormitory.c          -o a.exe

���ߣ�    ����Ŀ���̸�Ŀ¼�£�gcc src/*.c   -Iinclude -o a
gcc src/*.c -Iinclude -o a -mconsole
*/

//--- ��ǰ����ѧ������ϵͳ��ҳ���˵�ҳ�� ---
void showMainMenu()
 {
    printf("%d. ѧ����Ϣ����\n", STUDENT_MANAGEMENT); 
    printf("%d. רҵ����\n", MAJOR_MANAGEMENT);
    printf("%d. �γ���Ϣ\n", COURSE_INFORMATION);
    printf("%d. �ɼ�����\n", SCORE_MANAGEMENT);
    printf("%d. ������Ϣ\n", DORMITORY_INFORMATION);
    printf("%d. У԰������\n", CARD_MANAGEMENT);
    printf("%d. ����¼\n", HEALTH_RECORD);
    printf("%d. �˳�ϵͳ\n", EXIT_SYSTEM);
 
    return ;
 
}


// �����û�ѡ��:��ǰϵͳ���˵�
void handleMenuChoice(MenuOption choice ,SystemContext *ctx) 
{  
    //'ѧ������ģ��'�˵�ѡ��ö�� ��������
    StudentMenuOption moduleMenuChoice;      
    MajorMenuOption majorModuleEnum;
    do
    {
        switch (choice)
         {       

                
            case STUDENT_MANAGEMENT:  
                printf("\n--- ��ǰ����'1.ѧ����Ϣ����'ģ��ҳ��--- \n");    //--- �ɹ�����'1.ѧ����Ϣ����'ģ��ҳ��---
                showStudentManagementMenu(); 
                printf("��ѡ�����ִ�У�"); 
                scanf("%d",(int*)&moduleMenuChoice);    
                clearInputBuffer();

                handleStudentManagementChoice(moduleMenuChoice,ctx->students,ctx->stuCount);   
  
                break; 

            case MAJOR_MANAGEMENT:
                printf("\n��ǰλ�ô���2. רҵ���� ģ��ҳ�棬����ѡ���С�1:����2:ɾ��3:�飻4:�ġ�4�����ܣ���ѡ����ȷ���");
                showMajorManagementMenu();

                
                int tempChoice; 
                //�û�ѡ��רҵģ��Ĳ˵���
                MajorMenuOption majModuleEnum;
                scanf("%d",&tempChoice);
                majModuleEnum = tempChoice;
                clearInputBuffer();

                //����רҵ����ģ���ѡ��
                majorModuleEnum = handleMajorManagementChoice(majModuleEnum, ctx);
                if (majorModuleEnum == MAJOR_EXIT)
                {
                    moduleMenuChoice =STUDENT_MENU_BACK;  // �˳�רҵ����ģ��
                }
                
                // ����רҵ������ϵͳ 
                break;

            case SCORE_MANAGEMENT:
                printf("\n�ɹ�����ɼ�����ģ��\n");
                // ���óɼ�������ϵͳ
                break;
            case COURSE_INFORMATION:
                printf("\n�ɹ�����γ���Ϣģ��\n");
                // ���ÿγ���Ϣ��ϵͳ
                break;
            case DORMITORY_INFORMATION:
                printf("\n�ɹ�����������Ϣģ��\n");
                // ����������Ϣ��ϵͳ
                break;
            case HEALTH_RECORD:
                printf("\n�ɹ���������¼ģ��\n");
                // ��������¼��ϵͳ
                break;
            case CARD_MANAGEMENT:
                printf("\n�ɹ�����У԰������ģ��\n");
                // ����У԰��������ϵͳ
                break;

            case EXIT_SYSTEM:
                printf("\n�����˳�ϵͳ...\n");
                // �˳��߼�
                saveStudents(ctx->students, *(ctx->stuCount));
                return;
            default:
                printf("\n��Чѡ�������ѡ��\n");
                break; 
        }
    } while (moduleMenuChoice !=STUDENT_MENU_BACK);    //moduleMenuChoice !=EXIT_SYSTEM
    if (moduleMenuChoice == STUDENT_MENU_BACK)
    {
        //�û����˳�ѧ������ģ�顱����һ�μ��б���   ��ģ��˵��˳�ʱ�������棨�Ƽ����� system.c �У�
        saveStudents(ctx->students, *(ctx->stuCount));
    } 
}






int loginSystem() {
    char username[20], password[20];
    printf("�������û�����");
    scanf("%s", username);
    printf("���������룺");
    scanf("%s", password);

    // ģ�����Ա��֤
    if (strcmp(username, "admin") == 0 && strcmp(password, "123456") == 0) {
        printf("��¼�ɹ���\n");
        return 1;
    } else {
        printf("��¼ʧ�ܣ������ԡ�\n");
        return 0;
    }
}
