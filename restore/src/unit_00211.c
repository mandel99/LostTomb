#include "event_publisher.h"
extern void FUN_00417a70(void),FUN_00404350(void),FUN_004051a0(void),FUN_00417aa0(void),FUN_004179f0(void),FUN_0051e990(void);
void FUN_00417ae0(int32_t index)
{
    *(uint8_t *)(uintptr_t)(0x7027b8u+(uint32_t)index)=1;
    FUN_00417a70();FUN_00404350();FUN_004051a0();FUN_00417aa0();FUN_004179f0();
    if(*(uint8_t *)0xe38ddau==1)FUN_0051e990();
    *(uint32_t *)0xe950dcu=1;
    FUN_004e0cf0(*(int32_t *)(uintptr_t)(0x5794c4u+(uint32_t)index*28u),0,0,NULL,0);
}
