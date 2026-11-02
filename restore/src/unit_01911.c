#include "event_publisher.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern uint32_t FUN_004e0f90(int32_t,const int16_t *);
extern void FUN_0053b090(int32_t),FUN_004ca980(void);
extern uint8_t FUN_004ce3f0(const char *);
extern uint32_t FUN_0047d350(int32_t);
void FUN_004e0cf0(int32_t message,uint32_t argument0,uint32_t argument1,const int16_t *parameters,uint8_t force)
{
    int32_t index,i;uint8_t templated;RestoreMessageEvent *event;RestoreMessageRecord *layout;
    U(0xe950dc)=force?1:FUN_004e0f90(message,parameters);
    if(B(0xe38dda)==30||B(0xe38dda)==73||B(0xe38dda)==31||B(0xe38dda)==32)B(0xe38dda)=1;
    index=FUN_004e1750();if(index==-1)return;
    FUN_0053b090(9);if(!U(0xe950dc))B(0x5c755f)=1;
    event=restore_event_at((uint32_t)index);
    event->message=(int16_t)message;event->opened=0;event->year=W(0x78d7bc);event->month=B(0x78d7c0);
    event->argument0=(int32_t)argument0;event->argument1=(int32_t)argument1;
    U(0xe951ec)+=1;event->sequence=(int16_t)U(0xe951f0);U(0xe951f0)+=1;I(0xe951e8)=index;
    if(!parameters) {
        event->body_template=-1;event->title_template=-1;event->caption_template=-1;templated=0;
    } else {
        uint8_t *out=(uint8_t *)event+16;const uint8_t *in=(const uint8_t *)parameters;uint32_t value;uint16_t last;
        /* REP MOVSD then MOVSW: preserve native forward-copy order and widths. */
        for(i=0;i<7;++i){memcpy(&value,in+i*4,4);memcpy(out+i*4,&value,4);}
        memcpy(&last,in+28,2);memcpy(out+28,&last,2);templated=*parameters>=0;
    }
    if(B(0xe38dda)==1){FUN_004e14b0((uint32_t)message);index=I(0xe951e8);}
    layout=restore_event_layout((uint32_t)message);
    if(layout->subtype==1||layout->subtype==7){U(0xe951d8)=1;B(0xe38dca)=1;}
    if(!U(0xe950dc)) {
        if(U(0xe950e0)==1)FUN_0047d350(*(uint32_t *)((uint8_t *)layout+56)==1?7:6);
    } else if(B(0xe38dda)==1) {
        const char *filename;
        event=restore_event_at((uint32_t)index);U(0xea0da8)=5;I(0xe951e4)=index;event->opened=1;
        U(0xd3b174)=event->month;I(0xd3b178)=event->year;I(0xd3b170)=event->argument0;I(0xd3b16c)=event->argument1;
        B(0xe3908c)=0;I(0xe951dc)=message;FUN_004ca980();
        filename=(const char *)(uintptr_t)(U(0xe36c74)+layout->media);
        if(!FUN_004ce3f0(filename))FUN_0047d350(*(uint32_t *)((uint8_t *)layout+56)==1?7:6);
        else U(0xf54118)=1;
        FUN_004c5bf0(I(0xe951dc),0,templated);
    } else {
        for(i=0;i<20;++i)if(!U(0xe95080u+(uint32_t)i*4u)) {
            I(0xe95080u+(uint32_t)i*4u)=restore_event_at((uint32_t)index)->sequence;break;
        }
    }
    U(0xe950e0)=1;U(0xe950dc)=0;B(0xe38dc9)=1;
}
