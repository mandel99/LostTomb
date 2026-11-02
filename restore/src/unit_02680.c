#include "sprite_direction.h"
#include "map_x87_conversion.h"

/* Whole55D9C0/450 bytes. Small arithmetic boundaries retain x87 PC/RC,
   extended intermediates and the two intentional binary32 spills. MSVC's
   binary64 long double changes direction choices around sector edges. */
static const float full_turn=360.0f,half_turn=180.0f,unmirrored_origin=225.0f;
static const float mirrored_origin=135.0f,half=0.5f,one=1.0f,zero=0.0f;

static void direction_origin(uint8_t value[10],int32_t sector_offset,float angle,int mirrored)
{
    __asm { fild sector_offset }
    __asm { fld angle }
    if(mirrored){ __asm { fsub mirrored_origin } }
    else{ __asm { fadd unmirrored_origin } }
    __asm { fsubr full_turn }
    __asm { faddp st(1),st(0) }
    __asm { mov eax,value }
    __asm { fstp tbyte ptr[eax] }
}
static uint16_t direction_compare(const uint8_t value[10],float limit_value)
{
    uint16_t status;
    __asm { mov eax,value }
    __asm { fld tbyte ptr[eax] }
    __asm { fcomp limit_value }
    __asm { fnstsw status }
    return status;
}
static void direction_adjust(uint8_t value[10],int subtract)
{
    __asm { mov eax,value }
    __asm { fld tbyte ptr[eax] }
    if(subtract){ __asm { fsub full_turn } }
    else{ __asm { fadd full_turn } }
    __asm { mov eax,value }
    __asm { fstp tbyte ptr[eax] }
}
static uint16_t direction_mirrored_half(const uint8_t value[10],int32_t steps,
                                      float *step_size,float *rounded_ratio)
{
    uint16_t status;
    __asm { mov eax,value }
    __asm { fld tbyte ptr[eax] }
    __asm { fild steps }
    __asm { fld full_turn }
    __asm { fdiv st(0),st(1) }
    __asm { mov eax,step_size }
    __asm { fstp dword ptr[eax] }
    __asm { fld st(1) }
    __asm { fdiv dword ptr[eax] }
    __asm { mov eax,rounded_ratio }
    __asm { fst dword ptr[eax] }
    __asm { fxch st(1) }
    __asm { fmul half }
    __asm { fxch st(1) }
    __asm { fcompp }
    __asm { fnstsw status }
    __asm { fstp st(0) }
    return status;
}
static void direction_reflected_ratio(uint8_t result[10],const uint8_t value[10],float step_size)
{
    __asm { mov eax,value }
    __asm { fld tbyte ptr[eax] }
    __asm { fld full_turn }
    __asm { fsub st(0),st(1) }
    __asm { fdiv step_size }
    __asm { fadd one }
    __asm { mov eax,result }
    __asm { fstp tbyte ptr[eax] }
    __asm { fstp st(0) }
}
static void direction_unmirrored_ratio(uint8_t result[10],const uint8_t value[10],int32_t count)
{
    __asm { mov eax,value }
    __asm { fld tbyte ptr[eax] }
    __asm { fild count }
    __asm { fdivr full_turn }
    __asm { fdivr st(0),st(1) }
    __asm { mov eax,result }
    __asm { fstp tbyte ptr[eax] }
    __asm { fstp st(0) }
}
static void direction_float80(uint8_t result[10],float value)
{
    __asm { fld value }
    __asm { mov eax,result }
    __asm { fstp tbyte ptr[eax] }
}
RestoreSpriteDirection *restore_sprite_direction_from_angle(
    RestoreSpriteDirection *output,const RestoreSpriteAction *action,
    float angle,uint32_t inherited_padding,uint32_t *argument_slots)
{
    uint8_t value[10],ratio[10];
    uint32_t index=0,mirrored=0;
    uint16_t status;
    int32_t count=action->directions,steps;
    float step_size,rounded_ratio;
    if(count<=1){
        direction_float80(value,angle);
        mirrored=(direction_compare(value,half_turn)&0x100u)==0;
    }else if(action->flags&0x80u){
        steps=count*2-2;
        if(argument_slots)argument_slots[0]=(uint32_t)steps;
        direction_origin(value,360/(steps*2),angle,1);
        while(direction_compare(value,zero)&0x100u)direction_adjust(value,0);
        while(!(direction_compare(value,full_turn)&0x100u))direction_adjust(value,1);
        status=direction_mirrored_half(value,steps,&step_size,&rounded_ratio);
        if(argument_slots){
            argument_slots[0]=*(uint32_t *)(void *)&rounded_ratio;
            argument_slots[1]=*(uint32_t *)(void *)&step_size;
        }
        if(status&0x4100u){
            direction_float80(ratio,rounded_ratio);
        }else{
            direction_reflected_ratio(ratio,value,step_size);mirrored=1;
        }
        index=(uint32_t)restore_map_trunc80(ratio)&0xffffu;
    }else{
        if(argument_slots)argument_slots[0]=(uint32_t)count;
        direction_origin(value,360/(count*2),angle,0);
        /* Native does ONE correction here, unlike its mirrored loop. */
        if(direction_compare(value,zero)&0x100u)direction_adjust(value,0);
        else if(!(direction_compare(value,full_turn)&0x100u))direction_adjust(value,1);
        direction_unmirrored_ratio(ratio,value,count);
        index=(uint32_t)restore_map_trunc80(ratio)&0xffffu;
    }
    *(uint32_t *)(void *)output=index|((inherited_padding&0xffffu)<<16);
    output->mirrored=mirrored;
    return output;
}
/* cdecl(out,action,angle), EAX returns out. Capture the original private WORD
   at entry ESP-6; an uninitialized C local would be undefined behavior. */
__declspec(naked) void FUN_0055d9c0(void)
{
    __asm { movzx eax,word ptr[esp-6] }
    __asm { lea edx,[esp+8] }
    __asm { push edx }
    __asm { push eax }
    __asm { push dword ptr[esp+20] }
    __asm { push dword ptr[esp+20] }
    __asm { push dword ptr[esp+20] }
    __asm { call restore_sprite_direction_from_angle }
    __asm { add esp,20 }
    __asm { ret }
}
