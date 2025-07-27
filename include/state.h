// state.h
#include <stdbool.h>
#ifndef STATE_H
#define STATE_H

//总数状态
typedef struct {
    int studentCount; 
    int dormitoryCount; // 宿舍总数 
    int majorCount; // 专业总数 
    int courseCount; // 课程数 
    int graduateCount; // 毕业学生人数 
    int activeCardCount; // 正在使用校园卡的学生数
    // 可扩展更多
} SysCountState; 




// 自定义数据类型： 是一个函数指针类型定义，它定义了一种特定签名的函数指针类型 
typedef void (*StateChangeCallback)(const SysCountState* oldState, const SysCountState* newState);     //函数的参数列表，接受两个指向 SysCountState 结构体的常量指针。StateChangeCallback属于这个自定义数据类型的一个变量；

extern SysCountState countState1;

// 观察者管理函数:注册回调函数
bool registerStateChangeListener(StateChangeCallback callback);

//注销回调函数
bool unregisterStateChangeListener(StateChangeCallback callback);
void clearAllStateChangeListeners();

//调用回调函数
void notifyStateChange(const SysCountState* oldState);

// 回调函数：当状态变化时被调用.可以在数据更新时自动通知 UI 或日志记录模块。
void onStudentCountChanged(const SysCountState* oldState, const SysCountState* newState);

// 状态操作函数
void incrementStudentCount();  // +1
void decrementStudentCount();  // -1
void resetStudentCount();      // 归零
void setStudentCount(int count);
int getStudentCount();

#endif




