#include "menu_keyboard_modal.h"
#include "media_bindings.h"
#define s_MEM_Max_alloc_after___005f8fe4 (*(char (*)[23])0x005f8fe4u)
#define s_OK__SMK_user_ends_005f9044 (*(char (*)[19])0x005f9044u)
extern int FUN_004ce100(void *context);
extern void FUN_004ce2d0(void);
extern void FUN_004ce6e0(char *message,uint32_t context,uint32_t number);
extern uint32_t FUN_004cff20(void);
void FUN_00539170(void)
{
    if(MM_U(0xf54054)){
        uint32_t handle;
        if(!MM_U(0xf5404c))FUN_004ce100((void *)0xf5400cu);
        handle=MM_U(0xf54114);
        if(handle){_SmackClose_4((uintptr_t)handle);MM_U(0xf54114)=0;}
        FUN_004ce2d0();
        FUN_004ce6e0(s_OK__SMK_user_ends_005f9044,0xf5400c,0);
        {uint32_t available=FUN_004cff20();FUN_004ce6e0(s_MEM_Max_alloc_after___005f8fe4,0,available+1u);}
        FUN_00539220(1);
    }
}
