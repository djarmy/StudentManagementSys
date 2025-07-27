#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dormitory.h"

void assignDorm(Dorm *d, int *ids, int count) {
    d->dormId = rand() % 1000 + 1;
    d->numResidents = count;
    for (int i = 0; i < count; i++) {
        d->studentIds[i] = ids[i];
    }
}

void displayDorm(const Dorm *d) {
    printf("�����ţ�%d\n������%d\n��ԱID��", d->dormId, d->numResidents);
    for (int i = 0; i < d->numResidents; i++) {
        printf("%d ", d->studentIds[i]);
    }
    printf("\n");
}
