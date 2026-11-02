#ifndef RESTORE_HIGH_SCORE_SUBMISSION_H
#define RESTORE_HIGH_SCORE_SUBMISSION_H
#include "high_scores_menu.h"
void FUN_004c5a40(void);
void FUN_004c5970(int32_t level,uint32_t completed,int32_t score,uint32_t months);
void FUN_0041c600(int32_t level,uint32_t completed,int32_t score,uint32_t months);
uint8_t restore_high_scores_write(RestoreHighScores *self,const char *path);
uint8_t FUN_004c58d0(const char *path);
#endif
