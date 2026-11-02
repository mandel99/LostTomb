#include "population_updates.h"
#include "god_effects.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Whole 4BECF0: subtract once, then correct that same wrapped result.
   Both native callers supply D05720; raw callers retain their actual ECX. */
static uint32_t flood_month_before(uint32_t self)
{
    uint32_t month=U(self+0x32fb0u)-1u;
    if((int32_t)month<0)month+=12u;
    return month;
}
int32_t FUN_004becf0(void){return (int32_t)flood_month_before(0xd05720u);}

/* Complete4E8630/6 instructions. Publish its true DWORD flag before the
   explicit player argument, and discard incidental returned EAX. */
void FUN_004e8630(void)
{
    uint32_t player=U(0xeaac28u);U(0xea895cu)=1;(void)FUN_00458f80((int32_t)player);
}
/* The whole159instruction4EA000 implementation already exists in the
   god/festival graph. Both native callers discard GPR results;
   activate that accepted C body through the existing nullary public API. */
void FUN_004ea000(void){restore_god_city_004ea000();}

/* Both original 202-byte remaps (4BC8E4/4BC9B8) select exactly these
   nineteen kinds. Preserve their overlap with 6E..72 and their exclusion
   of 5F in the second pass; do not broaden the native dispatch table. */
static int destination_counted(int32_t kind)
{
    if(kind>=0x6e&&kind<=0x72)return 1;
    switch(kind){
    case 0x20:case 0x2f:case 0x33:case 0x35:case 0x4a:case 0x71:
    case 0xb1:case 0xb2:case 0xbb:case 0xbc:case 0xbd:case 0xc2:
    case 0xcb:case 0xcc:case 0xcd:case 0xe2:case 0xe7:case 0xe8:case 0xe9:return 1;
    default:return 0;
    }
}

/* Whole 4BC7A0. The fixed scans advance by 264/388 bytes. Counters and
   actor payload are WORDs; the destination ID and building kind are signed. */
void FUN_004bc7a0(void)
{
    uint32_t p,offset;
    int32_t kind;
    uint16_t payload;
    uint8_t action;
    for(p=0x799af0u;p<0x89b6e8u;p+=264u){
        if(B(p-16u)!=1)continue;
        kind=(int16_t)W(p);
        if(destination_counted(kind)||kind==0x47||kind==0x5f){
            W(p+0xf4u)=0;W(p+0xf6u)=0;
        }
    }
    for(p=0xbcd787u;p<0xc8ad43u;p+=388u){
        if(B(p+3u)==0)continue;
        payload=W(p+0x69u);if(payload==0)continue;
        offset=(uint32_t)(int32_t)(int16_t)W(p+0x53u)*264u;
        kind=(int16_t)W(0x7999e8u+offset);
        if(destination_counted(kind)){
            if(kind==0x5f){
                action=B(p);
                if(action==0x1c)W(0x799adeu+offset)=(uint16_t)(W(0x799adeu+offset)+1u);
                else if(action==0xa)W(0x799adcu+offset)=(uint16_t)(W(0x799adcu+offset)+1u);
            }else if(kind==0xcc){
                action=B(p);
                if(action==9)W(0x799adeu+offset)=(uint16_t)(W(0x799adeu+offset)+1u);
                else if(action==11)W(0x799adcu+offset)=(uint16_t)(W(0x799adcu+offset)+1u);
            }else W(0x799adcu+offset)=(uint16_t)(W(0x799adcu+offset)+1u);
        }else if(kind==0x47){
            W(0x799adcu+offset)=(uint16_t)(W(0x799adcu+offset)+1u);
            W(0x799adeu+offset)=(uint16_t)(W(0x799adeu+offset)+payload);
        }
    }
}
