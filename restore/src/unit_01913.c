#include "event_publisher.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define I(a) (*(volatile int32_t *)(uintptr_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(a))
extern uint32_t FUN_0047d350(int32_t);
extern uint32_t FUN_004ce3f0(const char *);
extern void FUN_004ca980(void);

/* Whole 4E1320: queued events are 48-byte records. Opening changes only
   the opened BYTE, preserving the adjacent month and signed template WORD. */
void FUN_004e1320(void)
{
    uint32_t queue,index;
    int32_t sequence,message;
    RestoreMessageEvent *event;
    RestoreMessageRecord *layout;
    uint8_t templated;
    if(B(0xe38dda)!=1)return;
    if(I(0xea0da8)>0){U(0xea0da8)-=1u;return;}
    for(queue=0;queue<20;++queue)if(U(0xe95080u+queue*4u)!=0)break;
    if(queue==20)return;
    sequence=I(0xe95080u+queue*4u);U(0xe95080u+queue*4u)=0;
    if(sequence<=0)return;
    /* The native exclusive end EA0D84 scans 999 of the 1000 slots. */
    for(index=0;index<999;++index){
        event=restore_event_at(index);
        if(event->message==0)return;
        if(event->sequence==sequence)break;
    }
    if(index==999)return;
    U(0xea0da8)=5;message=event->message;
    FUN_004e14b0((uint32_t)message);
    event->opened=1;
    I(0xd3b178)=event->year;
    U(0xd3b174)=event->month;
    I(0xd3b170)=event->argument0;I(0xd3b16c)=event->argument1;
    B(0xe3908c)=0;U(0xe951e4)=index;I(0xe951dc)=message;
    FUN_004ca980();
    layout=restore_event_layout((uint32_t)message);
    if(!FUN_004ce3f0((const char *)(uintptr_t)(U(0xe36c74)+layout->media)))
        FUN_0047d350(*(uint32_t *)((uint8_t *)layout+56)==1?7:6);
    else U(0xf54118)=1;
    templated=event->body_template>=0;
    U(0xe950dc)=1;
    FUN_004c5bf0(I(0xe951dc),0,templated);
    U(0xe950dc)=0;B(0xe38dc9)=1;
}
