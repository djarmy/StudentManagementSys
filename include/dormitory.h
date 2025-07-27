//dorm 宿舍管理模块（选填）
#ifndef DORM_H
#define DORM_H

typedef struct {
    int dormId;
    int studentIds[6];
    int numResidents;    //居住人数
} Dorm;

void assignDorm(Dorm *d, int *ids, int count);       //  当数组作为函数参数时，会自动退化为指向首元素的指针，进而丢失了大小长度信息，所以必须额外增加一个count参数，循环就有了边界
void displayDorm(const Dorm *d);

#endif // DORM_H
