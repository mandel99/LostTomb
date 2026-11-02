#include "tool_selector_input.h"
int32_t FUN_00403e00(void)
{
    uint32_t original=TI_U(0xf53f64)-258u,x=original,y,rectangle=0x5c28a8u,index=0,selection;
    int32_t previous=-1,count,hover;
    if(TI_U(0xf53f78)==1)x=TI_U(0xf53f60)-258u;
    if(TI_U(0x605a3c)==1)x=original;
    y=TI_U(0x7056e8)+110u;
    x+=161u-(uint32_t)FUN_004d5ed0();
    hover=FUN_00426450((int32_t)x,(int32_t)y,(const RestoreMenuButton *)(uintptr_t)0x5c28a8u,TI_I(0x7056f0));
    count=TI_I(0x7056f0);
    TI_U(0x7056c8)=(uint32_t)hover;
    if(count<=0)return 0;
    do{
        int32_t mouse,bound;
        previous=FUN_00403f10(TI_I(0x7056ec),previous);
        bound=(int32_t)((uint32_t)TI_S(rectangle)+x);
        mouse=TI_I(0xe38e34);
        if(bound<=mouse &&
           mouse<(int32_t)((uint32_t)TI_S(rectangle+4u)+x)){
            bound=(int32_t)((uint32_t)TI_S(rectangle+2u)+y);
            mouse=TI_I(0xe38e30);
            if(bound<=mouse &&
               mouse<(int32_t)((uint32_t)TI_S(rectangle+6u)+y) && TI_B(0xe38e03)!=0){
                selection=(uint32_t)previous+1u;
                TI_U(0xd3b198)=selection;
                FUN_00405200();
                return(int32_t)selection;
            }
        }
        count=TI_I(0x7056f0);++index;rectangle+=28u;
    }while((int32_t)index<count);
    return 0;
}
