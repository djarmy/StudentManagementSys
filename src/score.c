#include <stdio.h>
#include "score.h"

float calculateTotalScore(const Score *s) {
    return s->chinese + s->math + s->english;
}

void displayScore(const Score *s) {
    printf("ѧ�ţ�%d\n���ģ�%.2f\n��ѧ��%.2f\nӢ�%.2f\n�ܷ֣�%.2f\n",
           s->studentId, s->chinese, s->math, s->english, calculateTotalScore(s));
}
