#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/major.h"
#include "../include/context.h"
#include "../include/major_persist.h"



//保存专业数据到二进制.dat文件
bool saveMajors(MajorHashTable *table)
{
    if (!table || !table->buckets)
    {
        printf("table为NULL或者buckets为0！\n");
        return false;
    }
    
    FILE *fp = fopen("data/majors.dat", "wb");
    if (!fp) 
    {
        printf("在根目录下，打开data/majors.dat文件失败！\n");
        return false;
    }
    printf("当前专业数量：%d\n", table->size);
    // 写入专业数量（修正：使用sizeof(int)确保正确写入4 字节）
    fwrite(&table->size, sizeof(int), 1, fp);
    // 写入所有专业数据（修正： ）
    //遍历每一个桶，写入所有的Major 节点
    for (size_t i = 0; i < table->bucketCount; i++)
    {
        MajorNode *node =table->buckets[i];
        while (node)
        {
            Major *m = (Major *)node->value;
            fwrite(m, sizeof(Major), 1, fp);
            node = node->next;
        }
    } 
    fclose(fp);
    return true; 
}


// 再次运行exe,专门封装用于 加载磁盘上的已保存的专业模块数据文件 -> 还原专业模块哈希表已有的Raw原始数据内容到内存结构里面。
static bool insertMajorsRaw(MajorHashTable *table, Major m)
{
    // 增强版安全检查
    if (!table) {
        printf("[严重错误] table指针为NULL，无法插入\n");
        return false;
    }
    
    // 检查table是否已释放（通过buckets是否为NULL判断）
    if (!table->buckets) 
    {
        printf("[错误] 哈希表未初始化或已释放，table地址: %p\n", (void*)table);
        return false;
    }
    
    if (table->bucketCount <= 0) {
        printf("[错误] 哈希表桶数量无效: %d\n", table->bucketCount);
        return false;
    }
    
    if (!table->hashFunc) {
        printf("[错误] 哈希函数未设置\n");
        return false;
    }

    //分配Major和节点
    Major *newMajor = malloc(sizeof(Major));
    if (!newMajor)
    {
        printf("newMajor分配内存失败！\n");
        return false;
    }
    *newMajor = m;   //赋值动作，把文件中读取的Major拷贝到新内存

    int index = table->hashFunc(&newMajor->majorId,table->bucketCount);   //
    MajorNode *newNode = init_major_node(&newMajor->majorId,newMajor);
    if (!newNode)
    {
        printf("initial major node  failed!\n");
        return false;
    }

    //链表插入
    newNode->next =table->buckets[index];
    if (table->buckets[index])
    {
        table->buckets[index]->prev =newNode;
    }
    table->buckets[index] =newNode;

    printf("[调试] 插入成功，当前表 size=%d，插入ID=%d\n", table->size, m.majorId);    
    table->size++;
    return true; 
}


/**
 * 从二进制文件.dat加载数据 
 **/
bool loadMajors(MajorHashTable *table)
{
    if (!table)
    {
        printf(" table为空 参数条件失败！\n");
        return false;
    }
    
    FILE *fp = fopen("data/majors.dat", "rb");    //   假设程序从项目根目录运行
    if (!fp) 
    {
        printf("在根目录下，加载文件data/majors.dat 失败！ 或者不存在该文件，应当跳过专业加载 \n");
        return true;   // 文件不存在不算致命错误
    }
    int diskSize  = 0;     //用局部变量读取磁盘中的 size
    fread(&diskSize,sizeof(int) ,1 ,fp);    //&diskSize    &table->size    从磁盘文件开头 读取一个 int 整数值，写入到变量 diskSize 中，这个值就是之前持久化写入的 table->size。
    printf("从磁盘读取到专业数量：%d\n", diskSize);
    for (size_t i = 0; i < diskSize; i++)    //table->bucketCount
    {
        Major m;
        fread(&m, sizeof(Major), 1, fp);
            // debug 打印
        printf("尝试插入专业ID：%d\n", m.majorId);
        if (!insertMajorsRaw(table, m)) 
        {
            printf("[警告] 插入专业失败，ID = %d\n", m.majorId);  // 只负责结构恢复，无逻辑限制
        } 
    }
    fclose(fp);
    return true;
}