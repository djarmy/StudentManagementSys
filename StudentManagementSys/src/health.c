#include <stdio.h>
#include "health.h"

void displayHealth(const Health *h) {
    printf("ѧ��ID��%d\n��ߣ�%.1f cm\n���أ�%.1f kg\nѪ�ͣ�%s\n����״����%s\n",
           h->studentId, h->height, h->weight, h->bloodType, h->healthStatus);
}
