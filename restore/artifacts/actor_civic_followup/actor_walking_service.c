#include <stdint.h>

#define SB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define SW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define SD(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

typedef char service_patrol_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t size,int32_t radius);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
/* Native4BAA10 reads only actor; the caller's extra pushed zero is ignored. */
extern uint32_t FUN_004baa10(int32_t actor);
extern uint32_t FUN_0047e340(int32_t tile,uint32_t actor);
extern void FUN_004b21d0(int32_t actor);
extern void FUN_004b3f00(int32_t actor);
extern void FUN_004b3c60(int32_t actor);

static uint32_t service_patrol_offset(int32_t actor)
{return (uint32_t)actor*388u;}

/* 4B1DF0: choose a road near one of four offsets from the cached home.
   The previous building direction chooses the offset; its incremented BYTE
   is published before coordinates are read. The full query API supplies the
   success result's high16; the target-X WORD supplies its low16. */
uint32_t restore_actor_prepare_service_patrol(int32_t actor)
{
    uint32_t a=service_patrol_offset(actor),building,width,height,result;
    int32_t home=(int16_t)SW(0xbcd652u+a),x,y;
    uint8_t previous,next;
    uint16_t target_x,target_y;
    (void)SB(0xbcd602u+a); /* Native's otherwise unused kind read/CMP5. */
    SB(0xbcd631u+a)=15;
    building=(uint32_t)home*264u;
    SB(0xbcd63eu+a)=0;
    SB(0xbcd641u+a)=255;
    SB(0xbcd640u+a)=2;
    previous=SB(0x799a0cu+building);
    next=(uint8_t)(previous+2u);
    SB(0x799a0cu+building)=next;
    if(next>6u)SB(0x799a0cu+building)=0;
    x=(int16_t)SW(0x7999deu+building);
    y=(int16_t)SW(0x7999e0u+building);
    if(previous==0)y-=8;
    else if(previous==2)x+=8;
    else if(previous==4)y+=8;
    else if(previous==6)x-=8;
    if(x<0)x=0;
    if(y<0)y=0;
    width=SD(0xbca2d0u);
    if(x>=(int32_t)width)x=(int32_t)(width-1u);
    height=SD(0xbca2d4u);
    if(y>=(int32_t)height)y=(int32_t)(height-1u);
    result=FUN_0047fe60(x,y,1,6);
    if(result!=0) {
        target_x=SW(0xd3b4e4u);
        target_y=SW(0xd3b4e0u);
        SW(0xbcd61cu+a)=target_x;
        SW(0xbcd61eu+a)=target_y;
        return (result&0xffff0000u)|(uint32_t)target_x;
    }
    SB(0xbcd63eu+a)=1;
    return 0;
}

static uint32_t service_patrol_permission(uint8_t direction)
{
    /* This is fixed DWORD-addressed RAM, not a bounded C array. The native
       signed-BYTE index can read before/after the usual direction entries. */
    return SD(0xbc97c0u+(uint32_t)(int32_t)(int8_t)direction*4u);
}

/* The two native direction-rotation loops have the same read/store phases.
   attempts belongs to the entire invocation and is never reset per step. */
static void service_patrol_rotate(uint32_t a,int32_t reverse,uint32_t *attempts)
{
    for(;;) {
        uint8_t direction=SB(0xbcd609u+a),next;
        uint32_t available=service_patrol_permission(direction),previous;
        if(available!=0 && (int32_t)(int8_t)direction!=reverse)return;
        next=(uint8_t)(SB(0xbcd640u+a)+direction);
        SB(0xbcd609u+a)=next;
        if((int8_t)next>6)SB(0xbcd609u+a)=0;
        if((int8_t)SB(0xbcd609u+a)<0)SB(0xbcd609u+a)=6;
        previous=*attempts;
        *attempts=previous+1u;
        if((int32_t)previous>=4)return;
    }
}

/* 4B1EF0: logical void patrol advancement. Native incidental EAX can depend
   on incoming EAX or a void movement child's high24; retain the full old RAW
   implementation instead of inventing a full-result C provider. */
void restore_actor_advance_service_patrol(int32_t actor,int32_t steps)
{
    uint32_t a=service_patrol_offset(actor),attempts=0;
    uint8_t direction,phase;
    if(SB(0xbcd63eu+a)==0) {
        uint32_t kind,speed;
        int32_t current;
        SD(0xbcd5e8u)=1;
        kind=SB(0xbcd602u+a);
        current=(int32_t)SD(0xd056f8u);
        speed=SD(0x7090ecu+kind*56u);
        FUN_004a5da0(current,speed);
        direction=SB(0xbcd609u+a);
        if(direction==9u || direction==10u)SB(0xbcd63eu+a)=1;
        else if(direction==8u) {
            SB(0xbcd63eu+a)=1;
            SW(0xbcd63cu+a)=0;
        }
        if(SB(0xbcd63eu+a)==0)return;
        SB(0xbcd641u+a)=100;
        direction=SB(0xbcd60au+a);
        SB(0xbcd609u+a)=direction;
    }
    if(steps<=0)return;
    do {
        phase=SB(0xbcd631u+a);
        steps=(int32_t)((uint32_t)steps-1u);
        phase=(uint8_t)(phase+1u);
        SB(0xbcd631u+a)=phase;
        if((int8_t)phase>=15) {
            int32_t reverse,roads;
            uint8_t counter;
            uint32_t tile;
            SB(0xbcd631u+a)=15;
            reverse=((int32_t)(int8_t)SB(0xbcd60au+a)+4)%8;
            counter=(uint8_t)(SB(0xbcd63fu+a)+1u);
            SB(0xbcd63fu+a)=counter;
            if(FUN_004baa10(actor)!=0)return;
            tile=SD(0xbcd618u+a);
            roads=(int32_t)FUN_0047e340((int32_t)tile,(uint32_t)actor);
            if(roads<=0) {
                uint32_t current_a=service_patrol_offset((int32_t)SD(0xd056f8u));
                uint16_t limit=SW(0xbcd63au+current_a);
                SW(0xbcd63cu+current_a)=limit;
                return;
            }
            if(roads==1) {
                uint32_t available;
                direction=(uint8_t)(attempts*2u);
                SB(0xbcd609u+a)=direction;
                available=service_patrol_permission(direction);
                while(available==0) {
                    uint32_t previous=attempts;
                    ++attempts;
                    if((int32_t)previous>=4)break;
                    direction=(uint8_t)(attempts*2u);
                    SB(0xbcd609u+a)=direction;
                    available=service_patrol_permission(direction);
                }
            } else if(roads==2) {
                if(SB(0xbcd641u+a)==255u) {
                    FUN_004b21d0(actor);
                    reverse=-1;
                }
                service_patrol_rotate(a,reverse,&attempts);
            } else {
                uint8_t terrain;
                uint32_t available;
                tile=SD(0xbcd618u+a);
                terrain=SB(0xacf85cu+tile);
                counter=SB(0xbcd63fu+a);
                direction=(uint8_t)((uint8_t)(terrain+counter)&6u);
                SB(0xbcd609u+a)=direction;
                available=service_patrol_permission(direction);
                if(available==0 || (int32_t)(int8_t)direction==reverse) {
                    counter=(uint8_t)(SB(0xbcd641u+a)-1u);
                    SB(0xbcd641u+a)=counter;
                    if((int8_t)counter<=0) {
                        FUN_004b21d0(actor);
                        reverse=-1;
                    }
                    service_patrol_rotate(a,reverse,&attempts);
                }
            }
            SW(0xbcd634u+a)=(uint16_t)(SW(0xbcd634u+a)+1u);
            direction=SB(0xbcd609u+a);
            SB(0xbcd60au+a)=direction;
            SB(0xbcd631u+a)=0;
        }
        FUN_004b3f00(actor);
        if(SB(0xbcd631u+a)==8u)FUN_004b3c60(actor);
    }while(steps>0);
}
