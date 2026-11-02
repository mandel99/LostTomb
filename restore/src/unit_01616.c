#include "high_scores_menu.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define s_save_highscore_jas_005e02a8 (*(char (*)[19])0x005e02a8u)
uint8_t FUN_004c5920(void)
{
    int32_t i,count=0;
    I(0xd3a4bc)=0;
    if(!restore_high_scores_read(RESTORE_HIGH_SCORES,s_save_highscore_jas_005e02a8))return 0;
    for(i=0;i<I(0x700994);++i)if(RESTORE_HIGH_SCORES->records[i].valid)++count;
    if(I(0x700994)>0)I(0xd3a4bc)=count;
    return 1;
}
