#include "dropdown_menu.h"
int32_t FUN_00426150(const RestoreDropdownItem *items,int32_t count,int32_t x,int32_t y)
{
    uintptr_t row=(uintptr_t)items;int32_t i;
    for(i=1;i<=count;++i,row+=16u){int32_t mx=DM_I(0xe38e34u),top=dm_add(dm_add(DM_S(row),y),30);
        if(x<=mx){int32_t width=FUN_004d5f60((int32_t)(DM_U(0x705730u)-1u));mx=DM_I(0xe38e34u);
            if(dm_add((int32_t)((uint32_t)width<<4),x)>mx){int32_t my=DM_I(0xe38e30u);if(top<=my&&my<dm_add(top,15))return i;}
        }
    }
    return 0;
}
