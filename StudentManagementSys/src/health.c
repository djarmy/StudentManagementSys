#include <stdio.h>
#include "health.h"

void displayHealth(const Health *h) {
    printf("学生ID：%d\n身高：%.1f cm\n体重：%.1f kg\n血型：%s\n健康状况：%s\n",
           h->studentId, h->height, h->weight, h->bloodType, h->healthStatus);
}
