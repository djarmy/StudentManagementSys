#ifndef STUDENT_H
#define STUDENT_H

//学生信息结构体(存储单个学生的基本档案信息)目前5个成员
typedef struct {
    int id;         // 学生ID（使用时间戳+随机数）
    char name[50];
    int age;
    char gender[10];
    char department[50];    //所属院系eg:"计算机科学与技术系"
} Student;

// '学生管理模块'菜单选项枚举
typedef enum {
    STUDENT_MENU_BACK = 0,       // 从模块页面返回到主菜单
    STUDENT_MENU_ADD = 1,        // 增加学生信息
    STUDENT_MENU_DELETE = 2,     // 删除学生信息
    STUDENT_MENU_FIND = 3,      // 查询学生信息
    STUDENT_MENU_UPDATE = 4,     // 修改学生信息
    STUDENT_MENU_DISPLAY_ALL = 5 // 显示所有学生
} StudentMenuOption;





// '学生信息管理模块'菜单
void showStudentManagementMenu();


int generateUniqueStudentId();


bool checkStudentCountEmpty(int count);

// 增加学生
void addStudent(Student *student, int *count);

// 显示所有学生信息
int displayStudent(const Student *s, int count);

// 删除学生
void deleteStudent(Student *s, int *count, int id);

// 查询学生信息
void findStudent(const Student *students, int count, int id);

// 修改学生信息
void updateStudent(Student *students, int count, int id); 





// 处理学生信息管理模块的选择
bool handleStudentManagementChoice(StudentMenuOption choice, Student *students, int *count);

#endif // STUDENT_H


  