#ifndef RESTORE_HIGH_SCORES_MENU_H
#define RESTORE_HIGH_SCORES_MENU_H
#include "restore_types.h"
typedef struct RestoreHighScore {
    int32_t score,mission;
    char family[32];
    int32_t detail[6],difficulty,unknown_44,valid;
} RestoreHighScore;
typedef struct RestoreHighScores { RestoreHighScore records[100]; } RestoreHighScores;
typedef char restore_high_score_size[sizeof(RestoreHighScore)==0x4c?1:-1];
typedef char restore_high_scores_size[sizeof(RestoreHighScores)==0x1db0?1:-1];
#define RESTORE_HIGH_SCORES ((RestoreHighScores *)(uintptr_t)0xd38708u)
/* 004c5880 consumes ECX as the complete destination object. */
uint8_t restore_high_scores_read(RestoreHighScores *self,const char *path);
uint8_t FUN_004c5880(const char *path);
uint8_t FUN_004c5920(void);
void FUN_004c5400(void);
void FUN_004c5450(void);
void FUN_004c54c0(void);
void FUN_004c5660(void);
int32_t FUN_004c57d0(void);
/* The complete mode-0x53 input branch of 004d8310. */
void restore_high_scores_input(void);
#endif
