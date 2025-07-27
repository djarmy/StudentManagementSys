  

#ifndef MAJOR_H
#define MAJOR_H
  

// 前向声明SystemContext（不要 #include "context.h"）
typedef struct SystemContext SystemContext;
// #include "utils.h" 

// // 前向声明SystemContext（不要包含system.h）
//typedef struct SystemContext SystemContext;

#define HASH_TABLE_SIZE 101




/**
 * 哈希函数类型定义
 * @param key 键值指针（应强制转换为对应类型）
 * @param bucketCount 哈希表桶数量
 * @return 计算得到的哈希索引（范围：0到bucketCount-1）
 */
typedef   int (*HashFunc)(const void *key, int bucketCount);  // 统一哈希函数类型  HashFunc

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} Date;

// 专业结构体有18个成员
typedef struct {
    int majorId;                  // 专业ID（主键）
    char majorName[50];           // 专业名称
    char majorDesc[200];          // 专业描述（培养目标、课程体系等）
    int departmentId;             // 所属院系ID
    char departmentName[50];      // 院系名称（冗余存储，避免多次查询）
    char majorCode[20];           // 专业代码（如"080901"对应计算机科学与技术）
    int status;                   // 专业状态（0-停用，1-启用，2-招生中）
    int studentCount;             //当前专业学生数量，用于统计和展示

    int responsibleTeacherId;     // 专业负责人教师ID
    char contactPhone[20];        // 联系电话
    char email[50];               // 联系邮箱
    Date createTime;              // 创建时间（需自定义日期类型）
    Date updateTime;              // 最后更新时间
    int currentStudentCount;      // 当前学生数（自动计算，非手动维护）
    int maxStudentCapacity;       // 最大招生容量
    int annualEnrollment;         // 年度招生计划
    int graduatedStudentCount;    // 累计毕业人数
    
    // 专业特色标签（可通过数组或指针实现）
    char tags[10][20];            // 如"国家级一流专业"、"工科"等
} Major;

// 专业管理模块菜单选项
typedef enum 
{
    MAJOR_MENU_BACK=0,
    MAJOR_MENU_ADD = 1,
    MAJOR_MENU_DELETE,
    MAJOR_MENU_FIND,
    MAJOR_MENU_UPDATE,
    MAJOR_MENU_DISPLAY_ALL,
    MAJOR_EXIT
} MajorMenuOption;

//专业节点4个成员
typedef struct MajorNode
{
    void *key;                // 键值（存储majorId的指针）
    void *value;              // 数据值（存储Major结构体的指针）
    struct MajorNode *prev;   // 前驱节点指针（双向链表）
    struct MajorNode *next;   // 后继节点指针（双向链表）
}MajorNode;

 
 

//哈希表头管理体，通用哈希表抽象写法
typedef struct MajorHashTable 
{
    MajorNode **buckets;          // 二级指针，指向桶数组
    int size;                     // 当前专业数量
    int bucketCount;              // 桶总数（可配置，默认 101）
    HashFunc hashFunc;       // 哈希函数指针
}MajorHashTable;

unsigned int hash_func_str(const void *key, int bucketCount);
int hash_func_int(const void *key, int bucketCount);

bool initMajorHashTable(MajorHashTable *table, int bucketCount, HashFunc func);

 
int generateUniqueMajorId();



//插入
bool insertMajor(MajorHashTable *table, Major m);


//查找
Major *findMajor(MajorHashTable *table,  int majorId);    //返回值指针或NULL

//删除
bool deleteMajor(MajorHashTable *table, int majorId);

//遍历展示
void displayAllMajors(MajorHashTable *table);

bool updateMajor(MajorHashTable *table, int majorId);

MajorNode *init_major_node(void *key, void *value);


// // 专业管理模块功能函数声明
// void addMajor(Major *majors, int *count);
// //void deleteMajor(Major *majors, int *count, int id);
// //void updateMajor(Major *majors, int count, int id);
// int displayMajor(const Major *majors, int count);

// // 专业管理模块菜单展示函数
void showMajorManagementMenu(void);

void printMajorDetails(const Major *major);

// // 专业管理模块用户选择处理函数
MajorMenuOption  handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx);           //参数可满足

void freeMajorHashTable(MajorHashTable *table);

#endif // MAJOR_H
