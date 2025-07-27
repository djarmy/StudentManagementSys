#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>    // for access()  
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include "utils.h"
#include "../include/major.h"


//?????
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){}
}

void pauseAndClear() {
    printf("??? Enter ?��?????...\n");
    getchar();
 
}


//是否文件存在
bool fileExist(const char *path)
{
    //文件存在则access返回0，转换为bool类型的true
    return access(path, F_OK) == 0;
}

//如果否存在文件路径，是否创建文件路径
bool createDirectory(const char *path)
{
#ifdef _WIN32
    // Windows使用_mkdir，权限由系统设
    if (mkdir(path) == -1)
    {
        // 文件路径已存在时不报错
        if (errno != EEXIST)
        {
            perror("(文件路径已存在时不报错)，创建文件路径失败\n");
            return false;
        } 
    }
#else
    //Linux/macOS使用mkdir，可指定权限
    if (mkdir(path, 0755) != 0) 
    {
        if (errno != EEXIST) {
            perror("创建目录失败");
            return false;
        }
    }
#endif
    return true;
}

//获取当前日期函数
Date getCurrentDate()
{
    Date date;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    date.year = t->tm_year + 1900;
    date.month = t->tm_mon + 1;
    date.day = t->tm_mday;
    date.hour = t->tm_hour;
    date.minute = t->tm_min;
    date.second = t->tm_sec;
    return date; 
}