#include "sound_file_lookup.h"
#include "owned_buffer.h"
#include "array_lifecycle.h"
#include <windows.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(void *allocation);
RestoreSoundFileTable *restore_sound_file_table_initialize(RestoreSoundFileTable *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    U(0x12663b0u)=U(0x12663b0u)+12u;
    U(base+8u)=0;U(base+4u)=0;U(base)=0;
    return self;
}
void restore_sound_file_table_clear(RestoreSoundFileTable *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,entries;
    if(U(base+8u)){
        entries=U(base+8u);
        if(entries)(void)FUN_00560210((RestoreOwnedBuffer *)(uintptr_t)entries,3);
        U(base+8u)=0;
    }
}
void restore_sound_file_table_destroy(RestoreSoundFileTable *self)
{
    U(0x12663b0u)=U(0x12663b0u)-12u;
    FUN_0055dc18(self);
}
int restore_sound_file_table_resize(RestoreSoundFileTable *self,int32_t count)
{
    uint32_t base=(uint32_t)(uintptr_t)self,allocation,entries=0;
    if(count<1)return 0;
    FUN_0055dc18(self);
    allocation=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*8u+4u);
    __try{
        if(allocation){
            U(allocation)=(uint32_t)count;
            FUN_00561441((void *)(uintptr_t)(allocation+4u),8,count,
                         restore_owned_buffer_construct_element,restore_owned_buffer_destroy_element);
            entries=allocation+4u;
        }
    }__finally{
        if(AbnormalTermination())FUN_00561a2b((void *)(uintptr_t)allocation);
    }
    U(base+8u)=entries;
    if(!U(base+8u))return 0;
    U(base)=(uint32_t)count;U(base+4u)=0;
    return 1;
}
void restore_sound_file_allocation_cleanup_frame(uint32_t frame)
{FUN_00561a2b((void *)(uintptr_t)U(frame-24u));}
