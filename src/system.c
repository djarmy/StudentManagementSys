#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 

#include "../include/system.h"
#include "../include/context.h"
#include "../include/utils.h"

#include "../include/student.h" 
#include "../include/major.h"  // 从src目录向上一级到include
#include "../include/student_persist.h"

/*
gcc -I../include     .\system.c    ../src/student.c ../src/score.c ../src/dormitory.c          -o a.exe

或者：    在项目工程根目录下：gcc src/*.c   -Iinclude -o a
gcc src/*.c -Iinclude -o a -mconsole
*/

//--- 当前处于学生管理系统首页主菜单页面 ---
void showMainMenu()
 {
    printf("%d. 学生信息管理\n", STUDENT_MANAGEMENT); 
    printf("%d. 专业管理\n", MAJOR_MANAGEMENT);
    printf("%d. 课程信息\n", COURSE_INFORMATION);
    printf("%d. 成绩管理\n", SCORE_MANAGEMENT);
    printf("%d. 宿舍信息\n", DORMITORY_INFORMATION);
    printf("%d. 校园卡管理\n", CARD_MANAGEMENT);
    printf("%d. 体检记录\n", HEALTH_RECORD);
    printf("%d. 退出系统\n", EXIT_SYSTEM);
 
    return ;
 
}


// 处理用户选择:当前系统主菜单
void handleMenuChoice(MenuOption choice ,SystemContext *ctx) 
{  
    //'学生管理模块'菜单选项枚举 变量名称
    StudentMenuOption moduleMenuChoice;      
    MajorMenuOption majorModuleEnum;
    do
    {
        switch (choice)
         {       

                
            case STUDENT_MANAGEMENT:  
                printf("\n--- 当前处于'1.学生信息管理'模块页面--- \n");    //--- 成功进入'1.学生信息管理'模块页面---
                showStudentManagementMenu(); 
                printf("请选择序号执行："); 
                scanf("%d",(int*)&moduleMenuChoice);    
                clearInputBuffer();

                handleStudentManagementChoice(moduleMenuChoice,ctx->students,ctx->stuCount);   
  
                break; 

            case MAJOR_MANAGEMENT:
                printf("\n当前位置处于2. 专业管理 模块页面，功能选项有【1:增；2:删；3:查；4:改】4个功能，请选择正确序号");
                showMajorManagementMenu();

                
                int tempChoice; 
                //用户选择专业模块的菜单项
                MajorMenuOption majModuleEnum;
                scanf("%d",&tempChoice);
                majModuleEnum = tempChoice;
                clearInputBuffer();

                //处理专业管理模块的选择
                majorModuleEnum = handleMajorManagementChoice(majModuleEnum, ctx);
                if (majorModuleEnum == MAJOR_EXIT)
                {
                    moduleMenuChoice =STUDENT_MENU_BACK;  // 退出专业管理模块
                }
                
                // 调用专业管理子系统 
                break;

            case SCORE_MANAGEMENT:
                printf("\n成功进入成绩管理模块\n");
                // 调用成绩管理子系统
                break;
            case COURSE_INFORMATION:
                printf("\n成功进入课程信息模块\n");
                // 调用课程信息子系统
                break;
            case DORMITORY_INFORMATION:
                printf("\n成功进入宿舍信息模块\n");
                // 调用宿舍信息子系统
                break;
            case HEALTH_RECORD:
                printf("\n成功进入体检记录模块\n");
                // 调用体检记录子系统
                break;
            case CARD_MANAGEMENT:
                printf("\n成功进入校园卡管理模块\n");
                // 调用校园卡管理子系统
                break;

            case EXIT_SYSTEM:
                printf("\n正在退出系统...\n");
                // 退出逻辑
                saveStudents(ctx->students, *(ctx->stuCount));
                return;
            default:
                printf("\n无效选项，请重新选择！\n");
                break; 
        }
    } while (moduleMenuChoice !=STUDENT_MENU_BACK);    //moduleMenuChoice !=EXIT_SYSTEM
    if (moduleMenuChoice == STUDENT_MENU_BACK)
    {
        //用户“退出学生管理模块”，做一次集中保存   ：模块菜单退出时批量保存（推荐加在 system.c 中）
        saveStudents(ctx->students, *(ctx->stuCount));
    } 
}






int loginSystem() {
    char username[20], password[20];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);

    // 模拟管理员验证
    if (strcmp(username, "admin") == 0 && strcmp(password, "123456") == 0) {
        printf("登录成功！\n");
        return 1;
    } else {
        printf("登录失败，请重试。\n");
        return 0;
    }
}
