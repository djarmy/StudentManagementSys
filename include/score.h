#ifndef SCORE_H
#define SCORE_H



//score 分数成绩管理模块
typedef struct {
    int studentId;
    float chinese;
    float math;
    float english;
} Score;

float calculateTotalScore(const Score *s);
void displayScore(const Score *s);

#endif // SCORE_H
