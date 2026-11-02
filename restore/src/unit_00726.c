#include "earthquake_event.h"
#include "simulation_tick.h"
#include "editor_event_list_actions.h"
#include "menu_event_actions.h"
#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

/* Whole44BE30, including all16 expansion choices. Message parameters are
   one native32-byte local; only bytes0..29 are defined by this call chain. */
uint8_t restore_earthquake_tick(uintptr_t record)
{
    static const uint8_t branch[16]={0,1,2,3,0,0,0,1,1,1,2,2,2,3,3,3};
    static const int8_t dx[16]={0,1,0,-1,0,-1,1,1,0,0,0,-1,1,-1,0,0};
    static const int8_t dy[16]={-1,0,1,0,-1,0,0,0,-1,1,1,0,0,0,-1,1};
    uint32_t self=(uint32_t)record,choice,slot,cell;
    int32_t x,y,limit,maximum;
    int16_t parameters[16];
    if(B(self+2u)!=3u)return 0;
    if(W(self+48u)==0){
        if(!restore_event_record_due_0044d680(self,0))return 0;
        B(self+50u)=1;U(self+40u)|=4u;W(self+48u)=1;
        W(self+58u)=0;W(self+62u)=0;
        FUN_0044bd50(S16(self+64u),S16(self+66u));
        x=S16(self+64u);y=S16(self+66u);
        cell=U(0xbca2d8u)+(uint32_t)x+(uint32_t)y*228u;
        U(0xe950dcu)=1;
        parameters[0]=216;parameters[1]=215;
        parameters[8]=(int16_t)restore_event_message_kind_0044f660(self);
        restore_event_message_parameters_0044e6f0(self,parameters);
        FUN_004e0cf0(261,0,cell,parameters,0);
        return 1;
    }
    if(W(self+48u)==2u)return 0;
    W(self+62u)=(uint16_t)(W(self+62u)+1u);
    if(S16(self+62u)<S16(self+60u))return 1;
    W(self+62u)=0;
    limit=S16(self+12u)*25;
    if(limit<25)limit=25;
    W(self+58u)=(uint16_t)(W(self+58u)+1u);
    if(S16(self+58u)>=limit){W(self+48u)=2;B(self+50u)=0;}
    choice=U(0xd3b374u)&15u;W(self+80u)=(uint16_t)choice;
    slot=(uint32_t)branch[choice]*4u;
    x=S16(self+64u+slot)+(int32_t)dx[choice];
    y=S16(self+66u+slot)+(int32_t)dy[choice];
    if(x<0)x=0;
    maximum=(int32_t)(U(0xbca2d0u)-1u);if(x>maximum)x=maximum;
    if(y<0)y=0;
    maximum=(int32_t)(U(0xbca2d4u)-1u);if(y>maximum)y=maximum;
    if(FUN_0044bd20(x,y)){
        W(self+64u+slot)=(uint16_t)x;W(self+66u+slot)=(uint16_t)y;
        FUN_0044bd50(x,y);
    }
    return 1;
}
unsigned char FUN_0044be30(uintptr_t record){return restore_earthquake_tick(record);}
