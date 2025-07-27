 
#ifndef SYSTEM_H
#define SYSTEM_H

// // 先包含context.h（定义SystemContext） 
typedef struct SystemContext SystemContext;

// 系统主菜单选项枚举
typedef enum {
    EXIT_SYSTEM = 0,           // 退出系统
    STUDENT_MANAGEMENT = 1,    // 学生信息管理
    MAJOR_MANAGEMENT = 2,      //   专业管理
    COURSE_INFORMATION = 3,    // 课程信息
    SCORE_MANAGEMENT = 4,       // 成绩管理
    DORMITORY_INFORMATION = 5,    //  宿舍信息
    CARD_MANAGEMENT = 6,       // 校园卡管理
    HEALTH_RECORD = 7        // 体检记录
} MenuOption;

void showMainMenu();
void handleMenuChoice(MenuOption choice ,SystemContext *ctx);   //参数可满足   Student * students, int * count
// 生成唯一学生ID
int generateUniqueStudentId();
int loginSystem();

#endif // SYSTEM_H
