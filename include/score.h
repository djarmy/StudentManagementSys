#ifndef SCORE_H
#define SCORE_H



//score �����ɼ�����ģ��
typedef struct {
    int studentId;
    float chinese;
    float math;
    float english;
} Score;

float calculateTotalScore(const Score *s);
void displayScore(const Score *s);

#endif // SCORE_H
