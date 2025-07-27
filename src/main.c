#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
#include "../include/context.h" 
#include "../include/major.h"
#include "../include/config_persist.h"
#include "../include/student_persist.h" 




/*
 gcc  -Wformat src/*.c   -Iinclude -o a     // -I参数指定include目录
 gcc -Wformat -Wall -Werror src/*.c   -Iinclude -o a
 # 使用更严格的编译选项
gcc -Wall -Wextra -Werror -g -fsanitize=address src/*.c -Iinclude -o a
  
*/

 

int main(int argc, char const *argv[])
{  
    SystemContext *context =malloc(sizeof(SystemContext));       // 共享参数上下文
    if (!context)
    {
        printf("内存分配失败\n");
        return 1;
    }
    printf("系统初始化完成\n"); // 确认初始化成功    

    // 调用函数初始化结构体成员（包括分配动态内存） 
    if (!initSystemContext(context)) 
    {
    printf("系统初始化失败！\n");
    return 1;
    }
    

    // //printf("声明哈希表变量\n");
    // //  声明哈希表变量
    // MajorHashTable *majorTable = (MajorHashTable *)malloc(sizeof(MajorHashTable));  *已经在 initSystemContext() 里完成了初始化，不需要也绝对不允许再重新初始化一次！
    // if (!majorTable) 
    // {
    //     printf("分配 MajorHashTable 失败。\n");
    //     return 1;
    // }
    // if (!initMajorHashTable(majorTable, HASH_TABLE_SIZE, hash_func_int)) 
    // {
    //     printf("哈希表初始化失败，程序退出。\n");
    //     return 1;
    // }
    // context->majorTable = majorTable;  //     写入上下文
    

    // //loginSystem() ;

    //系统主菜单选项枚举
    int mainChoice = -1;    //MenuOption

    do 
    { 
        // 主菜单循环 
        printf("--- 当前处于'学生管理系统首页'主菜单页面 ---\n");
        showMainMenu();   
        printf("请选择序号执行操作：");  
        //clearInputBuffer();   
        scanf("%d", &mainChoice);    //(int*)
        clearInputBuffer();   
 

        // 注册状态变化回调
        registerStateChangeListener(onStudentCountChanged);
         
        
        //处理当前系统主菜单
        handleMenuChoice(mainChoice, context );    //st1, &stuNum , major1, &majNum

    } while (mainChoice != EXIT_SYSTEM);

    saveAllData(context);   
    printf("系统正在保存所有数据，请稍后...\n");
    //在系统退出前兜底持久化所有数据

    // 释放内存
    freeSystemContext(context);

    // 程序结束前注销回调（可选）
    unregisterStateChangeListener(onStudentCountChanged);    
    return 0;
    
}

    