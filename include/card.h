#ifndef CARD_H
#define CARD_H

typedef struct {
    int studentId;
    char cardId[20];
    float balance;
} Card;

void rechargeCard(Card *c, float amount);
void displayCard(const Card *c);

#endif // CARD_H
