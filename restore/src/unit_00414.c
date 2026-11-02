#include "dropdown_menu.h"
int32_t FUN_00426100(const RestoreDropdownBar *bars,int32_t count)
{
    int32_t i,mx,my;uintptr_t row=(uintptr_t)bars;
    if(count<1)return 0;
    mx=DM_I(0xe38e34u);my=DM_I(0xe38e30u);
    for(i=1;i<=count;++i,row+=16u)if(DM_S(row)<=mx&&mx<DM_S(row+2u)&&DM_S(row+4u)<=my&&my<dm_add(DM_S(row+4u),12))return i;
    return 0;
}
