//dorm �������ģ�飨ѡ�
#ifndef DORM_H
#define DORM_H

typedef struct {
    int dormId;
    int studentIds[6];
    int numResidents;    //��ס����
} Dorm;

void assignDorm(Dorm *d, int *ids, int count);       //  ��������Ϊ��������ʱ�����Զ��˻�Ϊָ����Ԫ�ص�ָ�룬������ʧ�˴�С������Ϣ�����Ա����������һ��count������ѭ�������˱߽�
void displayDorm(const Dorm *d);

#endif // DORM_H
