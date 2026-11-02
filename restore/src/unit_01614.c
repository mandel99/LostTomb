#include "high_scores_menu.h"
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561ca1(void *,uint32_t,uint32_t,restore_legacy_FILE *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);
uint8_t restore_high_scores_read(RestoreHighScores *self,const char *path)
{
    uint32_t read;
    restore_legacy_FILE *file=FUN_00561a0a(path,(const char *)0x5763bcu);
    if(!file)return 0;
    read=FUN_00561ca1(self,0x1db0,1,file);
    FUN_00561994(file);
    return read==1;
}
uint8_t FUN_004c5880(const char *path)
{
    return restore_high_scores_read(RESTORE_HIGH_SCORES,path);
}
