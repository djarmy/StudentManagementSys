#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
    int studentId;
    float height;
    float weight;
    char bloodType[4];
    char healthStatus[20];
} Health;

void displayHealth(const Health *h);

#endif // HEALTH_H
