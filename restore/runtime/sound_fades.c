#include "sound_fades.h"
#include "sound_streams.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static uint32_t channel_handle(int32_t channel)
{return U(0xe92dd0u+(uint32_t)channel*4u);}
void restore_sound_fade_to_stop(int32_t channel)
{
    int32_t volume;
    if(!U(0xf55b88u)||!channel_handle(channel)||!FUN_0055f5a1(RESTORE_SOUND,channel))return;
    volume=(int32_t)FUN_0055f5f4(RESTORE_SOUND,channel);
    while(volume>0){
        volume=(int32_t)((uint32_t)volume-5u);
        FUN_0055f639(RESTORE_SOUND,channel,volume);
    }
    FUN_0055f561(RESTORE_SOUND,channel);
}
void restore_sound_fade_out_step(int32_t channel)
{
    if(U(0xf55b88u)&&channel_handle(channel)&&FUN_0055f5a1(RESTORE_SOUND,channel)){
        uint32_t next=FUN_0055f5f4(RESTORE_SOUND,channel)-5u;
        /* Original JNS tests SF of the wrapped subtraction, including INT_MIN. */
        if((int32_t)next>=0){FUN_0055f639(RESTORE_SOUND,channel,(int32_t)next);return;}
        FUN_0055f561(RESTORE_SOUND,channel);
    }
    if(channel==0)U(0xf55b8cu)=0;
    else if(channel==1)U(0xf55b90u)=0;
}
void restore_sound_fade_in_step(int32_t channel)
{
    uint32_t next;int32_t cap;
    if(!U(0xf55b88u)||!channel_handle(channel)||!FUN_0055f5a1(RESTORE_SOUND,channel))return;
    next=FUN_0055f5f4(RESTORE_SOUND,channel)+5u;
    if(channel==0||channel==1||channel==2){
        cap=(int32_t)U(channel==0?0xe38ec8u:channel==1?0xe38eccu:0xe38ed0u);
        if((int32_t)next>cap)next=(uint32_t)cap;
    }
    FUN_0055f639(RESTORE_SOUND,channel,(int32_t)next);
}
