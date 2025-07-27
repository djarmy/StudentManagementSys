#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/student.h"
#include "../include/student_persist.h"
#include "../include/utils.h"



//����ѧ�����ݵ�������.dat�ļ�
bool saveStudents(Student *students, int count)
{
    FILE *fp = fopen("data/students.dat", "wb");
    if (!fp) 
    {
        printf("�ڸ�Ŀ¼�£���data/students.dat�ļ�ʧ�ܣ�\n");
        return false;
    }
    // д��ѧ��������������ʹ��sizeof(int)ȷ����ȷд��4�ֽڣ�
    fwrite(&count, sizeof(int), 1, fp);
    // д������ѧ�����ݣ�������ʹ��sizeof(Student)ȷ��д�������ṹ�壩
    fwrite(students, sizeof(Student), count,fp);
    fclose(fp);
    return true; 
}

/**
 * �Ӷ������ļ�.dat��������
 * @param students ���ڴ洢���غ�ѧ������Ķ���ָ��(����ֵ)
 * @param count ���ڴ洢ѧ��������ָ��(����ֵ)
 * @return ���سɹ�����true, ����ʧ�ܷ���false
 **/
bool loadStudents(Student **students, int *count)
{
    const char *filePath = "data/students.dat";
    //�Ƿ��ļ�����
    if (!fileExist(filePath))    //�������ļ�
    {
        //�򴴽��ļ�dataĿ¼
        if (!createDirectory("data"))   
        {
            //�񴴽�Ŀ¼
            printf("���Դ���dataĿ¼ʧ�ܣ�����ʧ�ܣ�\n");
            return false;
        }
        //�������ļ� 
        FILE *fp = fopen("data/students.dat", "wb");    //   ����������Ŀ��Ŀ¼����
        if (!fp) 
        {
            printf("�ڸ�Ŀ¼�£����Դ����������ļ�students.datʧ�ܣ������ļ�data/students.dat ʧ�ܣ�\n");
            return false;
        }
        printf("�Ѿ������������ļ�students.dat\n");
        //��ʼ��������
        *students = NULL;
        *count = 0;
        fclose(fp);
        return true;
    }



    //�ļ����ڣ���������
    FILE *fp = fopen(filePath, "rb");
    if (!fp)
    {
        printf("����:%sʧ�ܣ�\n",filePath);
        perror("");
        return false;
    }
    fread(count,sizeof(int) ,1 ,fp);
    *students = malloc(sizeof(Student) * (*count));
    if ((*students) == NULL)
    {
        fclose(fp);
        printf("�����ڴ�ʧ��\n");
        perror("������:\n");
        return false;
    }
    
    size_t readCount = fread(*students,sizeof(Student) ,*count ,fp);    //�ɹ�������ʵ�ʶ�ȡ��nmemb���������ֽ�����
    fclose(fp);

    if (readCount != ((size_t) *count))
    {
        free(*students);
        *students = NULL;
        *count = 0;
        if (feof(fp))
        {
            printf("�ļ���ȡʧ��:��ǰ�����ļ�ĩβ�����ݿ��ܲ�������\n");
        }
        else if (ferror(fp))
        {
            printf("�ļ���ȡʧ�ܣ�IO�����룬�������״̬��\n");
        }else
        { 
        printf("�ļ���ȡʧ�ܣ����ݿ����𻵣�\n");       
        }
        
        return false;
    }
    
    return true;
}