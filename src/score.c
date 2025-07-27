#include <stdio.h>
#include "score.h"

float calculateTotalScore(const Score *s) {
    return s->chinese + s->math + s->english;
}

void displayScore(const Score *s) {
    printf("学号：%d\n语文：%.2f\n数学：%.2f\n英语：%.2f\n总分：%.2f\n",
           s->studentId, s->chinese, s->math, s->english, calculateTotalScore(s));
}
