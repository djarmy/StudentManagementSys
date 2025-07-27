 
#ifndef SYSTEM_H
#define SYSTEM_H

// // �Ȱ���context.h������SystemContext�� 
typedef struct SystemContext SystemContext;

// ϵͳ���˵�ѡ��ö��
typedef enum {
    EXIT_SYSTEM = 0,           // �˳�ϵͳ
    STUDENT_MANAGEMENT = 1,    // ѧ����Ϣ����
    MAJOR_MANAGEMENT = 2,      //   רҵ����
    COURSE_INFORMATION = 3,    // �γ���Ϣ
    SCORE_MANAGEMENT = 4,       // �ɼ�����
    DORMITORY_INFORMATION = 5,    //  ������Ϣ
    CARD_MANAGEMENT = 6,       // У԰������
    HEALTH_RECORD = 7        // ����¼
} MenuOption;

void showMainMenu();
void handleMenuChoice(MenuOption choice ,SystemContext *ctx);   //����������   Student * students, int * count
// ����Ψһѧ��ID
int generateUniqueStudentId();
int loginSystem();

#endif // SYSTEM_H
