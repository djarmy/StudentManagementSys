// state.c
#include "../include/state.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
   
// 最大回调函数数量
#define MAX_CALLBACKS 200

// 结构体全局状态
SysCountState countState = {0};

// 回调函数数组和计数器
static StateChangeCallback callbacks[MAX_CALLBACKS];
static int callbackCount = 0;

// 注册回调函数
bool registerStateChangeListener(StateChangeCallback callback) {
    if (callback == NULL || callbackCount >= MAX_CALLBACKS) {
        return false;
    }
    
    callbacks[callbackCount++] = callback;
    return true;
}

// 注销回调函数
bool unregisterStateChangeListener(StateChangeCallback callback) {
    if (callback == NULL) {
        return false;
    }
    
    for (int i = 0; i < callbackCount; i++) {
        if (callbacks[i] == callback) {
            // 移除回调函数（通过移动数组元素）
            for (int j = i; j < callbackCount - 1; j++) {
                callbacks[j] = callbacks[j + 1];
            }
            callbackCount--;
            return true;
        }
    }
    
    return false;
}

// 清除所有回调函数
void clearAllStateChangeListeners() {
    callbackCount = 0;
}


// 回调函数：当状态变化时被调用
void onStudentCountChanged(const SysCountState* oldState, const SysCountState* newState) {
    printf("\n=== 学生数量变化通知 ===\n");
    printf("旧学生数量: %d\n", oldState->studentCount);
    printf("新学生数量: %d\n", newState->studentCount);
    printf("变化差值: %+d\n", newState->studentCount - oldState->studentCount);
    printf("========================\n\n");
}

// 通知所有观察者状态已更改
void notifyStateChange(const SysCountState* oldState) //static 
{
    for (int i = 0; i < callbackCount; i++) {
        if (callbacks[i] != NULL) {
            callbacks[i](oldState, &countState);
        }
    }
}

// 状态操作函数实现自增+1
void incrementStudentCount() {
    SysCountState oldState = countState;
    countState.studentCount++;
    notifyStateChange(&oldState);
}

// 状态操作函数实现自减-1
void decrementStudentCount() {
    if (countState.studentCount > 0) {
        SysCountState oldState = countState;
        countState.studentCount--;
        notifyStateChange(&oldState);
    }
}

void resetAllCounts() {
    SysCountState oldState = countState;
    memset(&countState, 0, sizeof(SysCountState));
    notifyStateChange(&oldState);
}

void setStudentCount(int count) {
    if (count >= 0) {
        SysCountState oldState = countState;
        countState.studentCount = count;
        notifyStateChange(&oldState);
    }
}

int getStudentCount() {
    printf("当前获取在校学生数量刷新后计数：%d\n",countState.studentCount);
    return countState.studentCount;
}