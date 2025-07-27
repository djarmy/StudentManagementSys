#ifndef STUDENT_H
#define STUDENT_H

//ѧ����Ϣ�ṹ��(�洢����ѧ���Ļ���������Ϣ)Ŀǰ5����Ա
typedef struct {
    int id;         // ѧ��ID��ʹ��ʱ���+�������
    char name[50];
    int age;
    char gender[10];
    char department[50];    //����Ժϵeg:"�������ѧ�뼼��ϵ"
} Student;

// 'ѧ������ģ��'�˵�ѡ��ö��
typedef enum {
    STUDENT_MENU_BACK = 0,       // ��ģ��ҳ�淵�ص����˵�
    STUDENT_MENU_ADD = 1,        // ����ѧ����Ϣ
    STUDENT_MENU_DELETE = 2,     // ɾ��ѧ����Ϣ
    STUDENT_MENU_FIND = 3,      // ��ѯѧ����Ϣ
    STUDENT_MENU_UPDATE = 4,     // �޸�ѧ����Ϣ
    STUDENT_MENU_DISPLAY_ALL = 5 // ��ʾ����ѧ��
} StudentMenuOption;





// 'ѧ����Ϣ����ģ��'�˵�
void showStudentManagementMenu();


int generateUniqueStudentId();


bool checkStudentCountEmpty(int count);

// ����ѧ��
void addStudent(Student *student, int *count);

// ��ʾ����ѧ����Ϣ
int displayStudent(const Student *s, int count);

// ɾ��ѧ��
void deleteStudent(Student *s, int *count, int id);

// ��ѯѧ����Ϣ
void findStudent(const Student *students, int count, int id);

// �޸�ѧ����Ϣ
void updateStudent(Student *students, int count, int id); 





// ����ѧ����Ϣ����ģ���ѡ��
bool handleStudentManagementChoice(StudentMenuOption choice, Student *students, int *count);

#endif // STUDENT_H


  