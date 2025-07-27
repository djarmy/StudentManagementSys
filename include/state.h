// state.h
#include <stdbool.h>
#ifndef STATE_H
#define STATE_H

//����״̬
typedef struct {
    int studentCount; 
    int dormitoryCount; // �������� 
    int majorCount; // רҵ���� 
    int courseCount; // �γ��� 
    int graduateCount; // ��ҵѧ������ 
    int activeCardCount; // ����ʹ��У԰����ѧ����
    // ����չ����
} SysCountState; 




// �Զ����������ͣ� ��һ������ָ�����Ͷ��壬��������һ���ض�ǩ���ĺ���ָ������ 
typedef void (*StateChangeCallback)(const SysCountState* oldState, const SysCountState* newState);     //�����Ĳ����б���������ָ�� SysCountState �ṹ��ĳ���ָ�롣StateChangeCallback��������Զ����������͵�һ��������

extern SysCountState countState1;

// �۲��߹�����:ע��ص�����
bool registerStateChangeListener(StateChangeCallback callback);

//ע���ص�����
bool unregisterStateChangeListener(StateChangeCallback callback);
void clearAllStateChangeListeners();

//���ûص�����
void notifyStateChange(const SysCountState* oldState);

// �ص���������״̬�仯ʱ������.���������ݸ���ʱ�Զ�֪ͨ UI ����־��¼ģ�顣
void onStudentCountChanged(const SysCountState* oldState, const SysCountState* newState);

// ״̬��������
void incrementStudentCount();  // +1
void decrementStudentCount();  // -1
void resetStudentCount();      // ����
void setStudentCount(int count);
int getStudentCount();

#endif




