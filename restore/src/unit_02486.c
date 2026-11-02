#include "ambient_sound.h"
#include "sound_initialization.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define S(a) (*(int16_t *)(uintptr_t)(a))
extern uint32_t FUN_00562348(void);

/* IMUL keeps its low DWORD; CDQ/AND/ADD/SAR divide it toward zero. */
static int32_t random_scaled(int32_t count)
{
    int32_t product=(int32_t)(FUN_00562348()*(uint32_t)count);
    return product/32768;
}

int32_t restore_ambient_position(int32_t x, int32_t width)
{
    const float range=128.0f;
    int64_t converted;
    unsigned short previous,truncate;
    /* Preserve the extended-precision expression and __ftol control word
       locally. Other legacy no-argument x87 callers are unchanged. */
    __asm {
        fild width
        fdivr range
        fild x
        fmul st,st(1)
        fstcw previous
        mov ax,previous
        or ax,0c00h
        mov truncate,ax
        fldcw truncate
        fistp converted
        fldcw previous
        fstp st(0)
    }
    return (int32_t)converted;
}

void FUN_0053acf0(void)
{
    int32_t x,y,tile,building,sound,category,width;
    uint32_t terrain;
    uintptr_t record,table;
    if(!U(0x00f55b88u)||!B(0x00e38ec0u))return;
    width=I(U(0x00f53f78u)?0x00f53f60u:0x00f53f64u);
    x=random_scaled(width);
    y=random_scaled(I(0x00e38d98u));
    tile=FUN_00430960(x,y,0);
    if(tile>=0x0fff0000)return;
    building=S(0x00b4e6fcu+(uint32_t)tile*2u);
    terrain=U(0x00af598cu+(uint32_t)tile*4u);
    if(building){
        record=0x007999e8u+(uint32_t)building*264u;
        category=S(record);
        if(category==57){
            int16_t subtype=S(record+2u);
            table=subtype==13?0x00f54990u:subtype==11?0x00f549e0u:0x00f549f4u;
            sound=I(table+(uint32_t)random_scaled(I(table+16u))*4u);
        }else{
            table=0x00f54670u+(uint32_t)category*20u;
            sound=I(table+(uint32_t)random_scaled(I(table+16u))*4u);
        }
    }else{
        if(!terrain)return;
        if(terrain&1u)category=5;
        else if(terrain&2u)category=2;
        else if((terrain&4u)&&!(terrain&0x80000u))category=1;
        else if(terrain&0x800u)category=3;
        else if(terrain&0x10000u)category=4;
        else return;
        table=0x00f54550u+(uint32_t)category*48u;
        sound=I(table+(uint32_t)random_scaled(I(table+44u))*4u);
    }
    if(sound<=-1)return;
    /* Native reloads the selected width after choosing a sound. */
    width=I(U(0x00f53f78u)?0x00f53f60u:0x00f53f64u);
    I(0x00e92db4u)=restore_ambient_position(x,width);
    U(0x00e92db0u)=U(0x00e38ed0u);
    I(0x00e92dacu)=sound;
    U(0x00e92da8u)=0xffffffffu;
    U(0x00e92db8u)=1;
    FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)0x00e92da8u);
}
