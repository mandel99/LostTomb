#include "dropdown_menu.h"
void FUN_00425e80(int32_t x,int32_t y,const RestoreSquareButton *buttons,int32_t count,int32_t base)
{
    uintptr_t row=(uintptr_t)buttons;int32_t i,mx=DM_I(0xe38e34u),my=DM_I(0xe38e30u);
    for(i=0;i<count;++i,row+=24u){int32_t left=dm_add(DM_S(row),x),top=dm_add(DM_S(row+2u),y),size=DM_S(row+6u);
        if(left<=mx&&mx<dm_add(left,size)&&top<=my&&my<dm_add(top,size)){
            DM_U(0xe38e2cu)=(uint32_t)mx;DM_U(0xe38e28u)=(uint32_t)my;DM_U(0x7056b0u)=(uint32_t)i+(uint32_t)base;return;
        }
    }
}
