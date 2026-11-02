#include "production_updates.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* 488630: DWORD months/mode, four DWORDs per row, explicit ECX input. */
void FUN_00488630(volatile uint8_t *schedule)
{
    int32_t previous=(int32_t)(U(0x78d7c0u)-1u)%11;
    uint32_t row=0,column;int32_t month;
    if(U(0x78d7c4u))return;
    while(row<10u){
        column=0;
        while(column<4u){
            month=(int32_t)U(0x56f5b8u+(row*4u+column)*4u);
            if(month==previous&&schedule[row]){
                schedule[row]=0;row=0;
            }else{
                if(month==-1)break;
                if(month==(int32_t)U(0x78d7c0u)){schedule[row]=1;break;}
            }
            ++column;
        }
        ++row;
    }
}
