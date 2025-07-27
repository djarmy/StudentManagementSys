#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/major.h"
#include "../include/context.h"
#include "../include/major_persist.h"



//����רҵ���ݵ�������.dat�ļ�
bool saveMajors(MajorHashTable *table)
{
    if (!table || !table->buckets)
    {
        printf("tableΪNULL����bucketsΪ0��\n");
        return false;
    }
    
    FILE *fp = fopen("data/majors.dat", "wb");
    if (!fp) 
    {
        printf("�ڸ�Ŀ¼�£���data/majors.dat�ļ�ʧ�ܣ�\n");
        return false;
    }
    printf("��ǰרҵ������%d\n", table->size);
    // д��רҵ������������ʹ��sizeof(int)ȷ����ȷд��4 �ֽڣ�
    fwrite(&table->size, sizeof(int), 1, fp);
    // д������רҵ���ݣ������� ��
    //����ÿһ��Ͱ��д�����е�Major �ڵ�
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


// �ٴ�����exe,ר�ŷ�װ���� ���ش����ϵ��ѱ����רҵģ�������ļ� -> ��ԭרҵģ���ϣ�����е�Rawԭʼ�������ݵ��ڴ�ṹ���档
static bool insertMajorsRaw(MajorHashTable *table, Major m)
{
    // ��ǿ�氲ȫ���
    if (!table) {
        printf("[���ش���] tableָ��ΪNULL���޷�����\n");
        return false;
    }
    
    // ���table�Ƿ����ͷţ�ͨ��buckets�Ƿ�ΪNULL�жϣ�
    if (!table->buckets) 
    {
        printf("[����] ��ϣ��δ��ʼ�������ͷţ�table��ַ: %p\n", (void*)table);
        return false;
    }
    
    if (table->bucketCount <= 0) {
        printf("[����] ��ϣ��Ͱ������Ч: %d\n", table->bucketCount);
        return false;
    }
    
    if (!table->hashFunc) {
        printf("[����] ��ϣ����δ����\n");
        return false;
    }

    //����Major�ͽڵ�
    Major *newMajor = malloc(sizeof(Major));
    if (!newMajor)
    {
        printf("newMajor�����ڴ�ʧ�ܣ�\n");
        return false;
    }
    *newMajor = m;   //��ֵ���������ļ��ж�ȡ��Major���������ڴ�

    int index = table->hashFunc(&newMajor->majorId,table->bucketCount);   //
    MajorNode *newNode = init_major_node(&newMajor->majorId,newMajor);
    if (!newNode)
    {
        printf("initial major node  failed!\n");
        return false;
    }

    //�������
    newNode->next =table->buckets[index];
    if (table->buckets[index])
    {
        table->buckets[index]->prev =newNode;
    }
    table->buckets[index] =newNode;

    printf("[����] ����ɹ�����ǰ�� size=%d������ID=%d\n", table->size, m.majorId);    
    table->size++;
    return true; 
}


/**
 * �Ӷ������ļ�.dat�������� 
 **/
bool loadMajors(MajorHashTable *table)
{
    if (!table)
    {
        printf(" tableΪ�� ��������ʧ�ܣ�\n");
        return false;
    }
    
    FILE *fp = fopen("data/majors.dat", "rb");    //   ����������Ŀ��Ŀ¼����
    if (!fp) 
    {
        printf("�ڸ�Ŀ¼�£������ļ�data/majors.dat ʧ�ܣ� ���߲����ڸ��ļ���Ӧ������רҵ���� \n");
        return true;   // �ļ������ڲ�����������
    }
    int diskSize  = 0;     //�þֲ�������ȡ�����е� size
    fread(&diskSize,sizeof(int) ,1 ,fp);    //&diskSize    &table->size    �Ӵ����ļ���ͷ ��ȡһ�� int ����ֵ��д�뵽���� diskSize �У����ֵ����֮ǰ�־û�д��� table->size��
    printf("�Ӵ��̶�ȡ��רҵ������%d\n", diskSize);
    for (size_t i = 0; i < diskSize; i++)    //table->bucketCount
    {
        Major m;
        fread(&m, sizeof(Major), 1, fp);
            // debug ��ӡ
        printf("���Բ���רҵID��%d\n", m.majorId);
        if (!insertMajorsRaw(table, m)) 
        {
            printf("[����] ����רҵʧ�ܣ�ID = %d\n", m.majorId);  // ֻ����ṹ�ָ������߼�����
        } 
    }
    fclose(fp);
    return true;
}