#include "trade_updates.h"
/* Complete559C60, including the signed lower bound and full resource switch. */
int32_t FUN_00559c60(uint32_t resource,int32_t value)
{
    int32_t limit;
    if(value<0)return 0;
    switch(resource){
    case 10:case 28:limit=10000;break;
    case 12:case 24:case 25:case 26:case 30:limit=30000;break;
    default:limit=15000;break;
    }
    return value>limit?limit:value;
}
