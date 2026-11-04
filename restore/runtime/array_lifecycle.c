#include "array_lifecycle.h"
#include <windows.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_005637ee(void);

int32_t restore_array_unwind_exception_filter(uint32_t code)
{
    /* Native563C7F continues searching for non-C++ exceptions and tail-calls
       terminate for an exception thrown while destructing an unwound array. */
    if(code==0xe06d7363u)FUN_005637ee();
    return EXCEPTION_CONTINUE_SEARCH;
}
void restore_array_unwind(void *end,uint32_t stride,int32_t count,RestoreArrayElement destroy)
{
    uint32_t at=(uint32_t)(uintptr_t)end,remaining=(uint32_t)count;
    __try{
        for(;;){
            --remaining;
            if((int32_t)remaining<0)break;
            at-=stride;destroy((void *)(uintptr_t)at);
        }
    }__except(restore_array_unwind_exception_filter((uint32_t)GetExceptionCode())){}
}
void restore_array_destroy(void *base,uint32_t stride,int32_t count,RestoreArrayElement destroy)
{
    uint32_t remaining=(uint32_t)count,at=(uint32_t)(uintptr_t)base+stride*(uint32_t)count;
    int completed=0;
    __try{
        for(;;){
            --remaining;
            /* Native DEC/JS tests the wrapped value, not count>0. */
            if((int32_t)remaining<0)break;
            at-=stride;destroy((void *)(uintptr_t)at);
        }
        completed=1;
    }__finally{
        if(!completed)FUN_005613e3((void *)(uintptr_t)at,stride,(int32_t)remaining,destroy);
    }
}
void restore_array_construct(void *base,uint32_t stride,int32_t count,RestoreArrayElement construct,RestoreArrayElement destroy)
{
    uint32_t at=(uint32_t)(uintptr_t)base,constructed=0;
    int completed=0;
    __try{
        while((int32_t)constructed<count){
            construct((void *)(uintptr_t)at);at+=stride;++constructed;
        }
        completed=1;
    }__finally{
        if(!completed)FUN_005613e3((void *)(uintptr_t)at,stride,(int32_t)constructed,destroy);
    }
}
void restore_array_destroy_cleanup_frame(uint32_t frame)
{
    RestoreArrayElement destroy;uint32_t count,stride,end;
    if(U(frame-28u))return;
    destroy=(RestoreArrayElement)(uintptr_t)U(frame+20u);
    count=U(frame+16u);stride=U(frame+12u);end=U(frame+8u);
    FUN_005613e3((void *)(uintptr_t)end,stride,(int32_t)count,destroy);
}
void restore_array_construct_cleanup_frame(uint32_t frame)
{
    RestoreArrayElement destroy;uint32_t count,stride,end;
    if(U(frame-32u))return;
    destroy=(RestoreArrayElement)(uintptr_t)U(frame+24u);
    count=U(frame-28u);stride=U(frame+12u);end=U(frame+8u);
    FUN_005613e3((void *)(uintptr_t)end,stride,(int32_t)count,destroy);
}
