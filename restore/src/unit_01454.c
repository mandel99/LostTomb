#include "actor_walking.h"
/* Native4B54F0 counts the signed WORD actor chain rooted at this tile. */
int FUN_004b54f0(int tile)
{
    int32_t actor=AW_S(0xb350dcu+(uint32_t)tile*2u);
    uint32_t count=0;
    while(actor){++count;actor=AW_S(0xbcd600u+(uint32_t)actor*388u);}
    return (int32_t)count;
}
