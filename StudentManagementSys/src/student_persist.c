#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/student.h"
#include "../include/student_persist.h"
#include "../include/utils.h"



//保存学生数据到二进制.dat文件
bool saveStudents(Student *students, int count)
{
    FILE *fp = fopen("data/students.dat", "wb");
    if (!fp) 
    {
        printf("在根目录下，打开data/students.dat文件失败！\n");
        return false;
    }
    // 写入学生数量（修正：使用sizeof(int)确保正确写入4字节）
    fwrite(&count, sizeof(int), 1, fp);
    // 写入所有学生数据（修正：使用sizeof(Student)确保写入完整结构体）
    fwrite(students, sizeof(Student), count,fp);
    fclose(fp);
    return true; 
}

/**
 * 从二进制文件.dat加载数据
 * @param students 用于存储加载后学生数组的二级指针(返回值)
 * @param count 用于存储学生数量的指针(返回值)
 * @return 加载成功返回true, 加载失败返回false
 **/
bool loadStudents(Student **students, int *count)
{
    const char *filePath = "data/students.dat";
    //是否文件存在
    if (!fileExist(filePath))    //不存在文件
    {
        //则创建文件data目录
        if (!createDirectory("data"))   
        {
            //否创建目录
            printf("尝试创建data目录失败，加载失败！\n");
            return false;
        }
        //创建空文件 
        FILE *fp = fopen("data/students.dat", "wb");    //   假设程序从项目根目录运行
        if (!fp) 
        {
            printf("在根目录下，尝试创建空数据文件students.dat失败，加载文件data/students.dat 失败！\n");
            return false;
        }
        printf("已经创建空数据文件students.dat\n");
        //初始化空数据
        *students = NULL;
        *count = 0;
        fclose(fp);
        return true;
    }



    //文件存在，正常加载
    FILE *fp = fopen(filePath, "rb");
    if (!fp)
    {
        printf("加载:%s失败！\n",filePath);
        perror("");
        return false;
    }
    fread(count,sizeof(int) ,1 ,fp);
    *students = malloc(sizeof(Student) * (*count));
    if ((*students) == NULL)
    {
        fclose(fp);
        printf("分配内存失败\n");
        perror("错误码:\n");
        return false;
    }
    
    size_t readCount = fread(*students,sizeof(Student) ,*count ,fp);    //成功：返回实际读取的nmemb数量（非字节数）
    fclose(fp);

    if (readCount != ((size_t) *count))
    {
        free(*students);
        *students = NULL;
        *count = 0;
        if (feof(fp))
        {
            printf("文件读取失败:提前到达文件末尾，数据可能不完整。\n");
        }
        else if (ferror(fp))
        {
            printf("文件读取失败，IO错误码，请检查磁盘状态！\n");
        }else
        { 
        printf("文件读取失败！数据可能损坏！\n");       
        }
        
        return false;
    }
    
    return true;
}