// state.c
#include "../include/state.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
   
// ���ص���������
#define MAX_CALLBACKS 200

// �ṹ��ȫ��״̬
SysCountState countState = {0};

// �ص���������ͼ�����
static StateChangeCallback callbacks[MAX_CALLBACKS];
static int callbackCount = 0;

// ע��ص�����
bool registerStateChangeListener(StateChangeCallback callback) {
    if (callback == NULL || callbackCount >= MAX_CALLBACKS) {
        return false;
    }
    
    callbacks[callbackCount++] = callback;
    return true;
}

// ע���ص�����
bool unregisterStateChangeListener(StateChangeCallback callback) {
    if (callback == NULL) {
        return false;
    }
    
    for (int i = 0; i < callbackCount; i++) {
        if (callbacks[i] == callback) {
            // �Ƴ��ص�������ͨ���ƶ�����Ԫ�أ�
            for (int j = i; j < callbackCount - 1; j++) {
                callbacks[j] = callbacks[j + 1];
            }
            callbackCount--;
            return true;
        }
    }
    
    return false;
}

// ������лص�����
void clearAllStateChangeListeners() {
    callbackCount = 0;
}


// �ص���������״̬�仯ʱ������
void onStudentCountChanged(const SysCountState* oldState, const SysCountState* newState) {
    printf("\n=== ѧ�������仯֪ͨ ===\n");
    printf("��ѧ������: %d\n", oldState->studentCount);
    printf("��ѧ������: %d\n", newState->studentCount);
    printf("�仯��ֵ: %+d\n", newState->studentCount - oldState->studentCount);
    printf("========================\n\n");
}

// ֪ͨ���й۲���״̬�Ѹ���
void notifyStateChange(const SysCountState* oldState) //static 
{
    for (int i = 0; i < callbackCount; i++) {
        if (callbacks[i] != NULL) {
            callbacks[i](oldState, &countState);
        }
    }
}

// ״̬��������ʵ������+1
void incrementStudentCount() {
    SysCountState oldState = countState;
    countState.studentCount++;
    notifyStateChange(&oldState);
}

// ״̬��������ʵ���Լ�-1
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
    printf("��ǰ��ȡ��Уѧ������ˢ�º������%d\n",countState.studentCount);
    return countState.studentCount;
}