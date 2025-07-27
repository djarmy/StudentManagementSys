#include <stdio.h>
#include "card.h"

void rechargeCard(Card *c, float amount) {
    c->balance += amount;
}

void displayCard(const Card *c) {
    printf("ѧ��ID��%d\n���ţ�%s\n��%.2f\n",
           c->studentId, c->cardId, c->balance);
}
