#include <stdio.h>
#include "card.h"

void rechargeCard(Card *c, float amount) {
    c->balance += amount;
}

void displayCard(const Card *c) {
    printf("Ñ§ÉúID£º%d\n¿¨ºÅ£º%s\nÓà¶î£º%.2f\n",
           c->studentId, c->cardId, c->balance);
}
