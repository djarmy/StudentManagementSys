#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdbool.h>

#include "../include/utils.h"
#include "../include/student.h"
#include "../include/state.h"
#include "../include/student_persist.h"


 
// 'ѧ����Ϣ����ģ��'�˵�ҳ��
void showStudentManagementMenu() 
{ 
    //StudentMenuOption moduleMenuChoice;  
  
    printf("%d. ����ѧ����Ϣ\n", STUDENT_MENU_ADD);
    printf("%d. ɾ��ѧ����Ϣ\n", STUDENT_MENU_DELETE);
    printf("%d. ��ѯѧ����Ϣ\n", STUDENT_MENU_FIND);
    printf("%d. �޸�ѧ����Ϣ\n", STUDENT_MENU_UPDATE);  
    printf("%d. չʾ����ѧ����Ϣ\n", STUDENT_MENU_DISPLAY_ALL);
    printf("%d. �����ϼ��˵�\n", STUDENT_MENU_BACK);
}


// ����Ψһѧ��ID��������int�������ڣ�
int generateUniqueStudentId() 
{
    // ��ȡ��ǰʱ������룩
    time_t timestamp = time(NULL);
    
    // �������ʱ�������2023��1��1�տ�ʼ��
    // 2023-01-01 00:00:00 ��Unixʱ���ԼΪ 1672531200
    const time_t baseTime = 1672531200;
    int relativeTime = (int)(timestamp - baseTime);
    
    // ����0-999�������
    srand((unsigned int)timestamp);
    int randomNum = rand() % 1000;
    
    // ���ID�����ʱ��� + �����
    // ���ʱ������Լ10�� = 315,360,000
    // �������Χ��0-999
    // ��Ϻ����315,360,000 + 999 = 315,360,999 < 14��
    return relativeTime + randomNum;
}

// ���ѧ�������Ƿ�Ϊ0
bool checkStudentCountEmpty(int count)
{
    if (count == 0)
    {
        printf("��ǰû��ѧ����¼������ʹ��[1. ����ѧ����Ϣ]��\n");
        fflush(stdout);  // ǿ��ˢ�����������
        return true;     // ��ʾѧ������Ϊ��
    }
    return false ;     // ��ʾѧ��������Ϊ��
    
}


void addStudent(Student *student, int *count) 
{
    int actualCount = 0;  // ʵ����ӵ�ѧ����
    
    for (int i = 0; i < *count; i++) 
    {
        printf("�������%d��ѧ����Ϣ�����������䡢�Ա�Ժϵ),ѧ����ϢID�Զ����ɣ�\n" ,i + 1);
        
        int temp;
        temp =scanf("%s %d %s %s", 
            student[i].name, 
            &student[i].age, 
            student[i].gender, 
            student[i].department) ;
        clearInputBuffer();
        
        if (temp !=4)
        {
            printf("¼��ѧ����Ϣ�ֶ�����ȱʧ�����飡ʵ��������%d \n",temp);
        }
        

        // ��ȡѧ����Ϣ
        if (temp == 4) 
        {
            
            // ��֤�����Ƿ���Ч��ʾ����
            if (student[i].age > 0 && student[i].age < 100) 
            {
                student[i].id = generateUniqueStudentId();
                actualCount++;
                incrementStudentCount();
            } 
            else 
            {
                printf("    ���ʧ�ܣ���Ч������\n");
            }
        } 
        else 
        {
            printf("    �����ʽ����������ѧ��\n");
            while (getchar() != '\n');  // ������뻺����
        }
    }
    
    *count = actualCount;  // ����ʵ����ӵ�����
    //�־û�����
    if (saveStudents(student,*count))
    {
        printf("[�־û�]ѧ����Ϣ����ɹ�\n");
    }else
    {
        printf("[�־û�]����ʧ�ܣ�����/dataĿ¼Ȩ�ޡ�\n");
    }  
}


// ɾ��ѧ������ѧ��ID��
void deleteStudent(Student *students, int *count, int id) {
    // �ȼ��ѧ�������Ƿ�Ϊ0
    if (*count == 0) {
        printf("��ǰû��ѧ����¼������ʹ��[1. ����ѧ����Ϣ]��\n");
        showStudentManagementMenu();
        fflush(stdout);  // ǿ��ˢ�����������
        
        return;
    }
    
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (students[i].id == id) {
            found = 1;
            // �����һ��Ԫ�ظ��ǵ�ǰ
            students[i] = students[*count - 1];
            (*count)--;
            printf("ѧ��Ϊ %d ��ѧ���ѳɹ�ɾ����\n", id);
            getStudentCount();        // 
            decrementStudentCount();
            printf("-----------------");
            break;
        }
    }
    if (!found) {
        printf("δ�ҵ�ѧ��Ϊ %d ��ѧ����ɾ��ʧ�ܣ�����ѧ��ID����ȷ�ԡ�\n", id);
    }
    //�־û�����
    if (saveStudents(students,*count))
    {
        printf("[�־û�]ѧ����Ϣ����ɹ�\n");
    }else
    {
        printf("[�־û�]����ʧ�ܣ�����/dataĿ¼Ȩ�ޡ�\n");
    } 
}


// ��ѯѧ����Ϣ
void findStudent(const Student *students, int count, int id)
{
    for (size_t i = 0; i < count; i++)
    {
        // �ȼ��ѧ�������Ƿ�Ϊ0
        if (count == 0) 
        {
            printf("��ǰû��ѧ����¼������ʹ��[1. ����ѧ����Ϣ]��\n");
            showStudentManagementMenu();
            fflush(stdout);  // ǿ��ˢ�����������
            
            return;
        }
        
        if (id == students[i].id)
        {
            printf("�в�ѯ�����ѧ����Ϣѧ��IDΪ��%d\n  ������%s\n  ���䣺%d\n  �Ա�%s\n  Ժϵ��%s\n ",
                        (students+i)->id, (students+i)->name, (students+i)->age, (students+i)->gender, (students+i)->department);
            return;
        }
        
    }
    // ���ѭ��������δ�ҵ�ѧ��
    printf("δ�ҵ�ѧ��Ϊ%d��ѧ����¼������ѧ��ID��ȷ�ԣ�\n", id);
    
}

int displayStudent(const Student *s, int count) 
{
    if (checkStudentCountEmpty(count) == false)
    {
        for (size_t i = 0; i < count; i++)
        {
        printf("��%d��ѧ����Ϣѧ��ID��%d\n  ������%s\n  ���䣺%d\n  �Ա�%s\n  Ժϵ��%s\n ", 
                i+1,    (s+i)->id, (s+i)->name, (s+i)->age, (s+i)->gender, (s+i)->department);
        }
        
        return count;
    }
   return -1 ;   
}

//�޸�ѧ����Ϣ
void updateStudent(Student *students, int count, int id)
{
    //�ȼ��ѧ�������Ƿ�Ϊ0
    if (count == 0)
    {
        printf("��ǰû��ѧ����¼������0������ʹ��[1.����ѧ����Ϣ]��\n");
        showStudentManagementMenu();
        fflush(stdout);
        return;
    } 

    int found = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (students[i].id == id)
        {
            found = 1;
            printf("�ҵ�ѧ�����Ϊ%d��ѧ����Ϣ����ǰ��Ϣ���£�\n", id);
            printf("    ����:%s\n   ����:%d\n   �Ա�:%s\n   Ժϵ:%s\n", 
            students[i].name, students[i].age, students[i].gender, students[i].department);

            printf("\n�������µ�ѧ����Ϣ(���������䡢�Ա�Ժϵ)�������򱣳�ԭ��ֵ\n");
            char tempName[50] = {0};
            int tempAge = 0;
            char ageBuf[50] = {0};
            int inputValid = 0;
            char tempGender[10] = {0};
            char tempDepartment[50] = {0};

            //��ȡ����Ϣ(ʹ��fgets()��������scanf()����������������)
            printf("����(��ǰ:%s):", students[i].name);
            fgets(tempName, sizeof(tempName), stdin);
            if (tempName[0] != '\n')    //�û������� ���ı�
            {
                strncpy(students[i].name, tempName, sizeof(students[i].name - 1));
                //�Ƴ��ַ���ĩβ�Ļ��з�
                size_t len = strlen(students[i].name);
                if (len > 0 && students[i].name[len - 1] == '\n')
                {
                    students[i].name[len - 1] = '\0';    //�����з� '\n' �滻Ϊ�ַ�����ֹ�� '\0'
                } 
            }

            clearInputBuffer();
            printf("���䣨��ǰ��%d����", students[i].age);
            fgets(ageBuf,sizeof(ageBuf),stdin);
            //�Ƴ����з�
            size_t len = strlen(ageBuf);
            if (len > 0 && ageBuf[len - 1] == '\n')
            {
                ageBuf[len - 1] = '\0';
            }                
            if (ageBuf[0] == '\0')    //
            { 
                // ������س�������������Ч(����ԭ��ֵ)  
                printf("���䱣��ԭ��ֵ:%d",students[i].age);
                inputValid = 1;   // 
            }
            else  //�û������� ���ı� ���Խ���Ϊ����
            {
                if ( sscanf(ageBuf,"%d",&tempAge) == 1)    //sscanf �� ageBuffer �ַ����ж�ȡһ��������������洢�� tempAge �����С�
                {
                    if (tempAge > 0 && tempAge < 100) 
                    {
                        students[i].age = tempAge;
                        inputValid = 1;
                    } else {
                        printf("��Ч�����䷶Χ(��Ҫ1-99)������ԭֵ\n");
                    }
                } 
            }  
            if (!inputValid) 
            {
                printf("�������봦��ʧ�ܣ�����ԭֵ\n");            
            }  
            clearInputBuffer();  // ������뻺����
            
            printf("�Ա𣨵�ǰ��%s����", students[i].gender);
            fgets(tempGender, sizeof(tempGender), stdin);
            if (tempGender[0] != '\n') 
            {
                strncpy(students[i].gender, tempGender, sizeof(students[i].gender) - 1);
                // �Ƴ��ַ���ĩβ�Ļ��з�
                size_t len = strlen(students[i].gender);
                if (len > 0 && students[i].gender[len - 1] == '\n') 
                {
                    students[i].gender[len - 1] = '\0';
                }
            }
            
            printf("Ժϵ����ǰ��%s����", students[i].department);
            fgets(tempDepartment, sizeof(tempDepartment), stdin);
            if (tempDepartment[0] != '\n') 
            {
                strncpy(students[i].department, tempDepartment, sizeof(students[i].department) - 1);
                // �Ƴ��ַ���ĩβ�Ļ��з�
                size_t len = strlen(students[i].department);
                if (len > 0 && students[i].department[len - 1] == '\n') 
                {
                    students[i].department[len - 1] = '\0';
                }
            }
            printf("ѧ�����Ϊ:%d����Ϣ���³ɹ���\n", id);

            //�־û�����
            if (saveStudents(students, count))
            {
                printf("[�־û�]ѧ����Ϣ����ɹ�\n");
            }else
            {
                printf("[�־û�]ѧ����Ϣ����ʧ�ܣ�����/dataĿ¼Ȩ�ޡ�\n");
            }
            break; 
        } 

    } 
    if (!found)
    {
        printf("δ�ҵ�ѧ�����Ϊ:%d��ѧ����Ϣ������ʧ�ܣ�����ѧ�����ID����ȷ�ԡ�\n",id);
    } 
}

// ����ѧ����Ϣ����ģ���ѡ��
bool handleStudentManagementChoice(StudentMenuOption moduleMenuChioce, Student *students, int *count)
{
    int id;  // ���ڴ洢ѧ��ID,�ֲ�������ֻ����Ҫʱʹ��
   
   
    switch (moduleMenuChioce)   

        {
            case STUDENT_MENU_ADD:
                printf("����ѧ��������Ч��,���������ѧ������:");
                //���ѧ��������Ӧ�ûᴥ��ѧ�������仯��
                scanf("%d",count);
                clearInputBuffer();
                addStudent(students, count); 
                break;    //��������� switch ���,�������˳� do-while ѭ����ѭ�������ִ��

            case STUDENT_MENU_DELETE:
                if ( displayStudent(students,*count) ==0)
                {
                    printf("��ǰ����ѧ����Ϣ������Ϊ0����ѡ��[1. ����ѧ����Ϣ]��\n"); 
                    break;
                }
                else if (*count >0 )    //displayStudent(students,*count) >0 
                {
                    int tempID =0 ;
                    printf("ɾ��ѧ��������Ч�У�������ѧ�����ID:");
                    scanf("%d",&tempID);
                    clearInputBuffer();
                    deleteStudent(students,count,tempID);
                }  
                break;

            case STUDENT_MENU_DISPLAY_ALL:
                loadStudents(&students, count);
                displayStudent(students,*count);
                break;

            case STUDENT_MENU_FIND: 
                
                if (checkStudentCountEmpty(*count) == false)
                {
                    printf("findStudent()������ѯѧ����Ч��,������ѧ�����ID:");
                    scanf("%d",&id);
                    clearInputBuffer();
                    findStudent(students,*count,id);
                } 
                break;  
            case STUDENT_MENU_UPDATE: 
                
                if (checkStudentCountEmpty(*count) == false)
                {
                    printf("updateStudent()�����޸�ѧ����Ч��,������ѧ�����ID:");
                    scanf("%d",&id);
                    clearInputBuffer();
                    updateStudent(students,*count,id);
                } 
                break;                
                
            case STUDENT_MENU_BACK:
                printf("����������һ��ҳ�棡\n");
                
                return true;
            
            default:
                break;
    }   
    return false;
}