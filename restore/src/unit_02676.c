#include "sprite_controller.h"

#define SC_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* The native virtual method receives the embedded receiver self+4, with
   EAX=vtable, EDX=source->data, ESI=self and EDI=cached descriptor. Its ST0
   result is spilled directly to binary32, exactly as FSTP [ESP] at 55D86F.
   There is no C float/double return boundary or binary64 intermediate. */
static __declspec(noinline) void sprite_angle_bits(RestoreSpriteCycle *self,
    uint32_t source_data,uint32_t vtable,uint32_t descriptor,uint32_t *output)
{
    uint32_t bits;
    __asm { mov esi,self }
    __asm { mov eax,vtable }
    __asm { lea ecx,[esi+4] }
    __asm { mov edx,source_data }
    __asm { mov edi,descriptor }
    __asm { call dword ptr[eax] }
    __asm { fstp dword ptr bits }
    *output=bits;
}

uint16_t restore_sprite_cycle_direction(RestoreSpriteCycle *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,source,source_data,vtable,descriptor;
    uint32_t packed,mirrored,flags;
    union { uint32_t bits;float value; } angle;
    RestoreSpriteDirection output,*returned;
    source=SC_U(base+0x38u);
    if(!source)return 0;
    source_data=SC_U(source+8u);
    if(!source_data)return 0;
    vtable=SC_U(base+4u);
    descriptor=SC_U(base+0x40u);
    sprite_angle_bits(self,source_data,vtable,descriptor,&angle.bits);
    returned=restore_sprite_direction_from_angle(&output,
        (const RestoreSpriteAction *)(uintptr_t)descriptor,
        /* The mapper's padding high WORD belongs only to this private local;
           neither recorded return consumer uses it. No early stack sample. */
        angle.value,0,NULL);
    packed=SC_U((uint32_t)(uintptr_t)returned);
    mirrored=SC_U((uint32_t)(uintptr_t)returned+4u);
    flags=SC_U(base+0x34u);
    if(mirrored)flags|=1u;
    else flags&=0xfffffffeu;
    SC_U(base+0x34u)=flags;
    return (uint16_t)packed;
}
