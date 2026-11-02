#include "sound_streams.h"
#include "sprite_render.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole41A850. Dimensions, offsets, mode and background image are DWORDs;
   mission index is signed BYTE. The51F780 calls are actual no-effect RETs. */
void FUN_0041a850(void)
{
    uint32_t width=U(0xe38db0u),height=U(0xe38dacu),mode,base,restricted;
    uint8_t classic;
    int32_t mission;
    U(0xd3b574u)=width;
    U(0xd3b56cu)=(uint32_t)((int32_t)(1024u-width)/2);
    U(0xd3b570u)=height;
    U(0xd3b568u)=(uint32_t)((int32_t)(768u-height)/2);
    mode=U(0x6fdcb0u);
    if(mode==1){(void)FUN_0053e590((int32_t)U(0xf55c44u),0,0);return;}
    if(mode!=0&&mode!=2)return;
    restricted=U(0xe38ebcu);
    if(mode==2){U(0xf55b8cu)=1;U(0xf55b94u)=1;}
    if(restricted){
        (void)FUN_0053e590((int32_t)(U(0xf55c44u)+(mode==0)),0,0);
        B(0xe38dcau)=1;return;
    }
    if(B(0xe38ea8u)!=127u){
        (void)FUN_0053e590((int32_t)(U(0xf55c44u)+(mode==0)),0,0);
        classic=B(0xe39088u);
        if(mode==0){U(0xf55b8cu)=1;U(0xf55b94u)=1;}
        mission=(int8_t)B(0xe38ea8u);
        base=mode==0?(classic?0x57a164u:0x579804u):
                     (classic?0x57b424u:0x57aac4u);
        (void)FUN_00539b20((char *)(uintptr_t)(base+(uint32_t)mission*40u));
    }
    B(0xe38dcau)=1;
}
