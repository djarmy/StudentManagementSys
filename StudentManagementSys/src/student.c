#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdbool.h>

#include "../include/utils.h"
#include "../include/student.h"
#include "../include/state.h"
#include "../include/student_persist.h"


 
// '学生信息管理模块'菜单页面
void showStudentManagementMenu() 
{ 
    //StudentMenuOption moduleMenuChoice;  
  
    printf("%d. 增加学生信息\n", STUDENT_MENU_ADD);
    printf("%d. 删除学生信息\n", STUDENT_MENU_DELETE);
    printf("%d. 查询学生信息\n", STUDENT_MENU_FIND);
    printf("%d. 修改学生信息\n", STUDENT_MENU_UPDATE);  
    printf("%d. 展示所有学生信息\n", STUDENT_MENU_DISPLAY_ALL);
    printf("%d. 返回上级菜单\n", STUDENT_MENU_BACK);
}


// 生成唯一学生ID（限制在int类型以内）
int generateUniqueStudentId() 
{
    // 获取当前时间戳（秒）
    time_t timestamp = time(NULL);
    
    // 计算相对时间戳（从2023年1月1日开始）
    // 2023-01-01 00:00:00 的Unix时间戳约为 1672531200
    const time_t baseTime = 1672531200;
    int relativeTime = (int)(timestamp - baseTime);
    
    // 生成0-999的随机数
    srand((unsigned int)timestamp);
    int randomNum = rand() % 1000;
    
    // 组合ID：相对时间戳 + 随机数
    // 相对时间戳最大：约10年 = 315,360,000
    // 随机数范围：0-999
    // 组合后最大：315,360,000 + 999 = 315,360,999 < 14亿
    return relativeTime + randomNum;
}

// 检查学生数量是否为0
bool checkStudentCountEmpty(int count)
{
    if (count == 0)
    {
        printf("当前没有学生记录，请先使用[1. 增加学生信息]！\n");
        fflush(stdout);  // 强制刷新输出缓冲区
        return true;     // 表示学生数量为空
    }
    return false ;     // 表示学生数量不为空
    
}


void addStudent(Student *student, int *count) 
{
    int actualCount = 0;  // 实际添加的学生数
    
    for (int i = 0; i < *count; i++) 
    {
        printf("请输入第%d个学生信息（姓名、年龄、性别、院系),学生信息ID自动生成：\n" ,i + 1);
        
        int temp;
        temp =scanf("%s %d %s %s", 
            student[i].name, 
            &student[i].age, 
            student[i].gender, 
            student[i].department) ;
        clearInputBuffer();
        
        if (temp !=4)
        {
            printf("录入学生信息字段数量缺失，请检查！实际数量：%d \n",temp);
        }
        

        // 读取学生信息
        if (temp == 4) 
        {
            
            // 验证输入是否有效（示例）
            if (student[i].age > 0 && student[i].age < 100) 
            {
                student[i].id = generateUniqueStudentId();
                actualCount++;
                incrementStudentCount();
            } 
            else 
            {
                printf("    添加失败，无效的年龄\n");
            }
        } 
        else 
        {
            printf("    输入格式错误，跳过该学生\n");
            while (getchar() != '\n');  // 清除输入缓冲区
        }
    }
    
    *count = actualCount;  // 更新实际添加的数量
    //持久化保存
    if (saveStudents(student,*count))
    {
        printf("[持久化]学生信息保存成功\n");
    }else
    {
        printf("[持久化]保存失败！请检查/data目录权限。\n");
    }  
}


// 删除学生（按学号ID）
void deleteStudent(Student *students, int *count, int id) {
    // 先检查学生数量是否为0
    if (*count == 0) {
        printf("当前没有学生记录，请先使用[1. 增加学生信息]！\n");
        showStudentManagementMenu();
        fflush(stdout);  // 强制刷新输出缓冲区
        
        return;
    }
    
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (students[i].id == id) {
            found = 1;
            // 用最后一个元素覆盖当前
            students[i] = students[*count - 1];
            (*count)--;
            printf("学号为 %d 的学生已成功删除。\n", id);
            getStudentCount();        // 
            decrementStudentCount();
            printf("-----------------");
            break;
        }
    }
    if (!found) {
        printf("未找到学号为 %d 的学生，删除失败！请检查学号ID的正确性。\n", id);
    }
    //持久化保存
    if (saveStudents(students,*count))
    {
        printf("[持久化]学生信息保存成功\n");
    }else
    {
        printf("[持久化]保存失败！请检查/data目录权限。\n");
    } 
}


// 查询学生信息
void findStudent(const Student *students, int count, int id)
{
    for (size_t i = 0; i < count; i++)
    {
        // 先检查学生数量是否为0
        if (count == 0) 
        {
            printf("当前没有学生记录，请先使用[1. 增加学生信息]！\n");
            showStudentManagementMenu();
            fflush(stdout);  // 强制刷新输出缓冲区
            
            return;
        }
        
        if (id == students[i].id)
        {
            printf("有查询结果，学生信息学号ID为：%d\n  姓名：%s\n  年龄：%d\n  性别：%s\n  院系：%s\n ",
                        (students+i)->id, (students+i)->name, (students+i)->age, (students+i)->gender, (students+i)->department);
            return;
        }
        
    }
    // 如果循环结束后未找到学生
    printf("未找到学号为%d的学生记录！请检查学号ID正确性！\n", id);
    
}

int displayStudent(const Student *s, int count) 
{
    if (checkStudentCountEmpty(count) == false)
    {
        for (size_t i = 0; i < count; i++)
        {
        printf("第%d个学生信息学号ID：%d\n  姓名：%s\n  年龄：%d\n  性别：%s\n  院系：%s\n ", 
                i+1,    (s+i)->id, (s+i)->name, (s+i)->age, (s+i)->gender, (s+i)->department);
        }
        
        return count;
    }
   return -1 ;   
}

//修改学生信息
void updateStudent(Student *students, int count, int id)
{
    //先检查学生数量是否为0
    if (count == 0)
    {
        printf("当前没有学生记录，数量0，请先使用[1.增加学生信息]！\n");
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
            printf("找到学生编号为%d的学生信息，当前信息如下：\n", id);
            printf("    姓名:%s\n   年龄:%d\n   性别:%s\n   院系:%s\n", 
            students[i].name, students[i].age, students[i].gender, students[i].department);

            printf("\n请输入新的学生信息(姓名、年龄、性别、院系)，留空则保持原数值\n");
            char tempName[50] = {0};
            int tempAge = 0;
            char ageBuf[50] = {0};
            int inputValid = 0;
            char tempGender[10] = {0};
            char tempDepartment[50] = {0};

            //读取新信息(使用fgets()函数避免scanf()函数缓冲区的问题)
            printf("姓名(当前:%s):", students[i].name);
            fgets(tempName, sizeof(tempName), stdin);
            if (tempName[0] != '\n')    //用户输入了 新文本
            {
                strncpy(students[i].name, tempName, sizeof(students[i].name - 1));
                //移除字符串末尾的换行符
                size_t len = strlen(students[i].name);
                if (len > 0 && students[i].name[len - 1] == '\n')
                {
                    students[i].name[len - 1] = '\0';    //将换行符 '\n' 替换为字符串终止符 '\0'
                } 
            }

            clearInputBuffer();
            printf("年龄（当前：%d）：", students[i].age);
            fgets(ageBuf,sizeof(ageBuf),stdin);
            //移除换行符
            size_t len = strlen(ageBuf);
            if (len > 0 && ageBuf[len - 1] == '\n')
            {
                ageBuf[len - 1] = '\0';
            }                
            if (ageBuf[0] == '\0')    //
            { 
                // 空输入回车键换行输入有效(保持原数值)  
                printf("年龄保持原数值:%d",students[i].age);
                inputValid = 1;   // 
            }
            else  //用户输入了 新文本 尝试解析为整数
            {
                if ( sscanf(ageBuf,"%d",&tempAge) == 1)    //sscanf 从 ageBuffer 字符串中读取一个整数，并将其存储到 tempAge 变量中。
                {
                    if (tempAge > 0 && tempAge < 100) 
                    {
                        students[i].age = tempAge;
                        inputValid = 1;
                    } else {
                        printf("无效的年龄范围(需要1-99)，保持原值\n");
                    }
                } 
            }  
            if (!inputValid) 
            {
                printf("年龄输入处理失败，保持原值\n");            
            }  
            clearInputBuffer();  // 清除输入缓冲区
            
            printf("性别（当前：%s）：", students[i].gender);
            fgets(tempGender, sizeof(tempGender), stdin);
            if (tempGender[0] != '\n') 
            {
                strncpy(students[i].gender, tempGender, sizeof(students[i].gender) - 1);
                // 移除字符串末尾的换行符
                size_t len = strlen(students[i].gender);
                if (len > 0 && students[i].gender[len - 1] == '\n') 
                {
                    students[i].gender[len - 1] = '\0';
                }
            }
            
            printf("院系（当前：%s）：", students[i].department);
            fgets(tempDepartment, sizeof(tempDepartment), stdin);
            if (tempDepartment[0] != '\n') 
            {
                strncpy(students[i].department, tempDepartment, sizeof(students[i].department) - 1);
                // 移除字符串末尾的换行符
                size_t len = strlen(students[i].department);
                if (len > 0 && students[i].department[len - 1] == '\n') 
                {
                    students[i].department[len - 1] = '\0';
                }
            }
            printf("学生编号为:%d的信息更新成功。\n", id);

            //持久化保存
            if (saveStudents(students, count))
            {
                printf("[持久化]学生信息保存成功\n");
            }else
            {
                printf("[持久化]学生信息保存失败！请检查/data目录权限。\n");
            }
            break; 
        } 

    } 
    if (!found)
    {
        printf("未找到学生编号为:%d的学生信息，更新失败！请检查学生编号ID的正确性。\n",id);
    } 
}

// 处理学生信息管理模块的选择
bool handleStudentManagementChoice(StudentMenuOption moduleMenuChioce, Student *students, int *count)
{
    int id;  // 用于存储学生ID,局部变量，只在需要时使用
   
   
    switch (moduleMenuChioce)   

        {
            case STUDENT_MENU_ADD:
                printf("增加学生函数生效中,请输入添加学生数量:");
                //添加学生（这里应该会触发学生数量变化）
                scanf("%d",count);
                clearInputBuffer();
                addStudent(students, count); 
                break;    //程序会跳出 switch 语句,但不会退出 do-while 循环，循环会继续执行

            case STUDENT_MENU_DELETE:
                if ( displayStudent(students,*count) ==0)
                {
                    printf("当前所有学生信息，数量为0，请选择[1. 增加学生信息]！\n"); 
                    break;
                }
                else if (*count >0 )    //displayStudent(students,*count) >0 
                {
                    int tempID =0 ;
                    printf("删除学生函数生效中，请输入学生编号ID:");
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
                    printf("findStudent()函数查询学生生效中,请输入学生编号ID:");
                    scanf("%d",&id);
                    clearInputBuffer();
                    findStudent(students,*count,id);
                } 
                break;  
            case STUDENT_MENU_UPDATE: 
                
                if (checkStudentCountEmpty(*count) == false)
                {
                    printf("updateStudent()函数修改学生生效中,请输入学生编号ID:");
                    scanf("%d",&id);
                    clearInputBuffer();
                    updateStudent(students,*count,id);
                } 
                break;                
                
            case STUDENT_MENU_BACK:
                printf("即将返回上一个页面！\n");
                
                return true;
            
            default:
                break;
    }   
    return false;
}