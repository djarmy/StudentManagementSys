  

#ifndef MAJOR_H
#define MAJOR_H
  

// ǰ������SystemContext����Ҫ #include "context.h"��
typedef struct SystemContext SystemContext;
// #include "utils.h" 

// // ǰ������SystemContext����Ҫ����system.h��
//typedef struct SystemContext SystemContext;

#define HASH_TABLE_SIZE 101




/**
 * ��ϣ�������Ͷ���
 * @param key ��ֵָ�루Ӧǿ��ת��Ϊ��Ӧ���ͣ�
 * @param bucketCount ��ϣ��Ͱ����
 * @return ����õ��Ĺ�ϣ��������Χ��0��bucketCount-1��
 */
typedef   int (*HashFunc)(const void *key, int bucketCount);  // ͳһ��ϣ��������  HashFunc

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} Date;

// רҵ�ṹ����18����Ա
typedef struct {
    int majorId;                  // רҵID��������
    char majorName[50];           // רҵ����
    char majorDesc[200];          // רҵ����������Ŀ�ꡢ�γ���ϵ�ȣ�
    int departmentId;             // ����ԺϵID
    char departmentName[50];      // Ժϵ���ƣ�����洢�������β�ѯ��
    char majorCode[20];           // רҵ���루��"080901"��Ӧ�������ѧ�뼼����
    int status;                   // רҵ״̬��0-ͣ�ã�1-���ã�2-�����У�
    int studentCount;             //��ǰרҵѧ������������ͳ�ƺ�չʾ

    int responsibleTeacherId;     // רҵ�����˽�ʦID
    char contactPhone[20];        // ��ϵ�绰
    char email[50];               // ��ϵ����
    Date createTime;              // ����ʱ�䣨���Զ����������ͣ�
    Date updateTime;              // ������ʱ��
    int currentStudentCount;      // ��ǰѧ�������Զ����㣬���ֶ�ά����
    int maxStudentCapacity;       // �����������
    int annualEnrollment;         // ��������ƻ�
    int graduatedStudentCount;    // �ۼƱ�ҵ����
    
    // רҵ��ɫ��ǩ����ͨ�������ָ��ʵ�֣�
    char tags[10][20];            // ��"���Ҽ�һ��רҵ"��"����"��
} Major;

// רҵ����ģ��˵�ѡ��
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

//רҵ�ڵ�4����Ա
typedef struct MajorNode
{
    void *key;                // ��ֵ���洢majorId��ָ�룩
    void *value;              // ����ֵ���洢Major�ṹ���ָ�룩
    struct MajorNode *prev;   // ǰ���ڵ�ָ�루˫������
    struct MajorNode *next;   // ��̽ڵ�ָ�루˫������
}MajorNode;

 
 

//��ϣ��ͷ�����壬ͨ�ù�ϣ�����д��
typedef struct MajorHashTable 
{
    MajorNode **buckets;          // ����ָ�룬ָ��Ͱ����
    int size;                     // ��ǰרҵ����
    int bucketCount;              // Ͱ�����������ã�Ĭ�� 101��
    HashFunc hashFunc;       // ��ϣ����ָ��
}MajorHashTable;

unsigned int hash_func_str(const void *key, int bucketCount);
int hash_func_int(const void *key, int bucketCount);

bool initMajorHashTable(MajorHashTable *table, int bucketCount, HashFunc func);

 
int generateUniqueMajorId();



//����
bool insertMajor(MajorHashTable *table, Major m);


//����
Major *findMajor(MajorHashTable *table,  int majorId);    //����ֵָ���NULL

//ɾ��
bool deleteMajor(MajorHashTable *table, int majorId);

//����չʾ
void displayAllMajors(MajorHashTable *table);

bool updateMajor(MajorHashTable *table, int majorId);

MajorNode *init_major_node(void *key, void *value);


// // רҵ����ģ�鹦�ܺ�������
// void addMajor(Major *majors, int *count);
// //void deleteMajor(Major *majors, int *count, int id);
// //void updateMajor(Major *majors, int count, int id);
// int displayMajor(const Major *majors, int count);

// // רҵ����ģ��˵�չʾ����
void showMajorManagementMenu(void);

void printMajorDetails(const Major *major);

// // רҵ����ģ���û�ѡ������
MajorMenuOption  handleMajorManagementChoice(MajorMenuOption choice, SystemContext *ctx);           //����������

void freeMajorHashTable(MajorHashTable *table);

#endif // MAJOR_H
