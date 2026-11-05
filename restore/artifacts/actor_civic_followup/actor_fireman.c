#include <stdint.h>

/* Byte-addressed volatile RAM preserves the native read/store phases. */
#define B(p) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(p))
#define W(p) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(p))
#define D(p) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(p))

extern void FUN_004b17d0(int32_t actor);
extern void FUN_004b1d50(int32_t actor);
extern void FUN_004b1df0(int32_t actor);
extern void FUN_004b1ef0(int32_t actor,int32_t steps);
extern void FUN_004b3250(int32_t actor);
extern int32_t FUN_004b3ff0(int32_t actor,int32_t steps);
extern void FUN_004b4220(int32_t actor,int32_t from_x,int32_t from_y,
                        int32_t to_x,int32_t to_y,int32_t mode);
extern uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t kind,int32_t distance);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern uint32_t FUN_0051cba0(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
/* The authenticated 3-byte leaf ignores the native caller's pushed 0x9d. */
extern uint32_t FUN_0047d350(void);

static uint32_t fireman_actor_offset(int32_t actor)
{return (uint32_t)actor*388u;}
static uint32_t fireman_building_offset(int32_t building)
{return (uint32_t)building*264u;}
static int32_t fireman_difference(int32_t a,int32_t b)
{
    if(a>b)return (int32_t)((uint32_t)a-(uint32_t)b);
    if(a<b)return (int32_t)((uint32_t)b-(uint32_t)a);
    return 0;
}

/* 00469a50..00469b80: prefer the closest unassigned fire. A fire already
   assigned to another actor is a fallback only at Chebyshev distance <=2. */
int32_t restore_fireman_find_nearest_fire(int32_t x,int32_t y)
{
    int32_t count,remaining,unassigned=0,reserved=0,reserved_distance=10000;
    uint32_t next=0x78dd90u;
    D(0x78d7f8u)=10000;
    count=(int32_t)D(0x78d7fcu);
    D(0x78d800u)=0;
    if(count<=0)return 0;
    remaining=count;
    D(0x78d800u)=(uint32_t)count;
    do {
        int32_t building=(int16_t)W(next),dx,dy,distance;
        uint32_t p;
        next+=2;
        p=fireman_building_offset(building);
        if(B(0x7999d8u+p)==1 && W(0x7999e8u+p)==99 &&
           B(0x799a95u+p)==0 && W(0x7999f8u+p)!=0) {
            int32_t bx=(int16_t)W(0x7999deu+p);
            int32_t by=(int16_t)W(0x7999e0u+p);
            dx=fireman_difference(x,bx);
            D(0xd3b4fcu)=(uint32_t)dx;
            dy=fireman_difference(y,by);
            D(0xd3b4f8u)=(uint32_t)dy;
            distance=dx>=dy?dx:dy;
            if(W(0x799a08u+p)!=0) {
                if(distance<reserved_distance) {
                    reserved_distance=distance;
                    reserved=building;
                }
            } else if(distance<(int32_t)D(0x78d7f8u)) {
                D(0x78d7f8u)=(uint32_t)distance;
                unassigned=building;
            }
        }
        remaining=(int32_t)((uint32_t)remaining-1u);
    }while(remaining!=0);
    if(unassigned==0 && reserved_distance<=2)return reserved;
    return unassigned;
}

/* 0048e5e0..0048e6e9: periodic fire assignment, full DWORD boolean result. */
uint32_t restore_fireman_try_assign_fire(int32_t actor)
{
    uint32_t a,p;
    uint8_t state,timer;
    int32_t x,y,fire;
    uint16_t target_x,target_y;
    if((int32_t)D(0x78d804u)<=0)return 0;
    a=fireman_actor_offset(actor);
    state=B(0xbcd630u+a);
    if(state==5 || state==4 || state==8 || state==9 ||
       state==12 || state==13 || state==14 || state==15)return 0;
    timer=(uint8_t)(B(0xbcd663u+a)+1u);
    B(0xbcd663u+a)=timer;
    if(timer<20)return 0;
    y=(int16_t)W(0xbcd60eu+a);
    x=(int16_t)W(0xbcd60cu+a);
    fire=restore_fireman_find_nearest_fire(x,y);
    if(fire<=0)return 0;
    if((int32_t)D(0x78d7f8u)>25)return 0;
    p=fireman_building_offset(fire);
    B(0xbcd663u+a)=0;
    B(0xbcd630u+a)=12;
    target_x=W(0x7999feu+p);
    target_y=W(0x799a00u+p);
    W(0xbcd61cu+a)=target_x;
    W(0xbcd61eu+a)=target_y;
    W(0xbcd656u+a)=(uint16_t)fire;
    FUN_004b3250(actor);
    W(0x799a08u+p)=(uint16_t)actor;
    return 1;
}

/* FIG_FIREMAN kind10, 0048ded0..0048e5d8. The DWORD return is derived
   from the explicit final EAX writers; it is not a C-void scratch read. */
uint32_t restore_actor_update_fireman(void)
{
    int32_t actor=(int32_t)D(0xd056f8u),home,speed,x,y;
    uint32_t a=fireman_actor_offset(actor),home_offset,result,heading;
    uint8_t kind=B(0xbcd602u+a),state,direction,cycle;
    uint16_t target_x,target_y;
    B(0xbcd66au+a)=1;
    B(0xbcd604u+a)=0;
    W(0xbcd63au+a)=640;
    home=(int16_t)W(0xbcd652u+a);
    speed=(int32_t)D(0x7090ecu+(uint32_t)kind*56u);
    home_offset=fireman_building_offset(home);
    if(B(0x7999d8u+home_offset)!=1 ||
       (int16_t)W(0x799a02u+home_offset)!=actor)B(0xbcd606u+a)=2;
    cycle=(uint8_t)(B(0xbcd5f9u+a)+1u);
    B(0xbcd5f9u+a)=cycle;
    if(cycle>=12)B(0xbcd5f9u+a)=0;
    (void)restore_fireman_try_assign_fire(actor);
    actor=(int32_t)D(0xd056f8u);
    a=fireman_actor_offset(actor);
    state=B(0xbcd630u+a);
    if(state==5) {
        FUN_004b17d0(actor);
        goto reload_and_render;
    }
    if(state==4) {
        FUN_004b1d50(actor);
        goto reload_and_render;
    }
    if(state==8) {
        B(0xbcd65du+a)=1;
        B(0xbcd5f9u+a)=0;
        W(0xbcd62eu+a)=(uint16_t)(W(0xbcd62eu+a)-1u);
        if((int16_t)W(0xbcd62eu+a)>0)goto render;
        y=(int16_t)W(0x7999e0u+home_offset);
        x=(int16_t)W(0x7999deu+home_offset);
        kind=B(0x7999dbu+home_offset);
        result=FUN_0047fe60(x,y,kind,2);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        if(result==0) {B(0xbcd606u+a)=2;goto render;}
        target_x=W(0x7999feu+home_offset);
        target_y=W(0x799a00u+home_offset);
        goto begin_pixel_return;
    }
    if(state==9) {
        B(0xbcd604u+a)=1;
        B(0xbcd65du+a)=1;
        result=(uint32_t)FUN_004b3ff0(actor,1);
        if(result!=1)goto reload_and_render;
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        if((int16_t)W(0xb4e6fcu+D(0xbcd618u+a)*2u)==home)goto remove_and_render;
        B(0xbcd630u+a)=10;
        FUN_004b1df0(actor);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        W(0xbcd63cu+a)=0;
        goto render;
    }
    if(state==10) {
        int16_t elapsed,limit;
        B(0xbcd65du+a)=0;
        W(0xbcd63cu+a)=(uint16_t)(W(0xbcd63cu+a)+1u);
        elapsed=(int16_t)W(0xbcd63cu+a);
        limit=(int16_t)W(0xbcd63au+a);
        if(elapsed>=limit) {
            y=(int16_t)W(0x7999e0u+home_offset);
            x=(int16_t)W(0x7999deu+home_offset);
            kind=B(0x7999dbu+home_offset);
            result=FUN_0047fe60(x,y,kind,2);
            actor=(int32_t)D(0xd056f8u);
            a=fireman_actor_offset(actor);
            if(result==0)B(0xbcd606u+a)=2;
            else {
                target_x=W(0xd3b4e4u);
                B(0xbcd630u+a)=11;
                W(0xbcd61cu+a)=target_x;
                target_y=W(0xd3b4e0u);
                W(0xbcd61eu+a)=target_y;
                FUN_004b3250(actor);
                actor=(int32_t)D(0xd056f8u);
            }
        }
        FUN_004b1ef0(actor,1);
        goto reload_and_render;
    }
    if(state==11 || state==12) {
        if(state==12)B(0xbcd66au+a)=0;
        (void)FUN_004a5da0(actor,speed);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==9 || direction==10)goto remove_and_render;
        if(direction!=8)goto render;
        if(state==11) {
            target_x=W(0x7999deu+home_offset);
            target_y=W(0x7999e0u+home_offset);
            goto begin_pixel_return;
        }
        B(0xbcd630u+a)=13;
        FUN_004b3250(actor);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        W(0xbcd63cu+a)=0;
        W(0xbcd62eu+a)=50;
        W(0xbcd66cu+a)=0;
        goto render;
    }
    if(state==13) {
        int32_t fire=(int16_t)W(0xbcd656u+a),bx,by,dx,dy,distance;
        uint32_t fire_offset=fireman_building_offset(fire);
        x=(int16_t)W(0xbcd60cu+a);
        y=(int16_t)W(0xbcd60eu+a);
        bx=(int16_t)W(0x7999deu+fire_offset);
        by=(int16_t)W(0x7999e0u+fire_offset);
        dx=fireman_difference(x,bx);
        D(0xd3b4fcu)=(uint32_t)dx;
        dy=fireman_difference(y,by);
        D(0xd3b4f8u)=(uint32_t)dy;
        distance=dx>=dy?dx:dy;
        if(B(0x7999d8u+fire_offset)!=1)W(0xbcd62eu+a)=1;
        else if(W(0x7999e8u+fire_offset)!=99)W(0xbcd62eu+a)=1;
        else if(distance>=2)W(0xbcd62eu+a)=1;
        else {
            W(0x799a22u+fire_offset)=32;
            (void)FUN_0047d350();
            actor=(int32_t)D(0xd056f8u);
        }
        bx=(int16_t)W(0x7999deu+fire_offset);
        a=fireman_actor_offset(actor);
        by=(int16_t)W(0x7999e0u+fire_offset);
        y=(int16_t)W(0xbcd60eu+a);
        x=(int16_t)W(0xbcd60cu+a);
        result=FUN_0051cba0(x,y,bx,by);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        direction=(uint8_t)result;
        B(0xbcd60bu+a)=direction;
        if((int8_t)direction>=8)B(0xbcd60bu+a)=0;
        W(0xbcd62eu+a)=(uint16_t)(W(0xbcd62eu+a)-1u);
        if((int16_t)W(0xbcd62eu+a)>0)goto render;
        if((int16_t)W(0xbcd66cu+a)<36)goto render;
        B(0xbcd663u+a)=20;
        result=restore_fireman_try_assign_fire(actor);
        if(result!=0)goto reload_and_render;
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        home=(int16_t)W(0xbcd652u+a);
        home_offset=fireman_building_offset(home);
        kind=B(0x7999dbu+home_offset);
        y=(int16_t)W(0x7999e0u+home_offset);
        x=(int16_t)W(0x7999deu+home_offset);
        result=FUN_0047fe60(x,y,kind,2);
        actor=(int32_t)D(0xd056f8u);
        a=fireman_actor_offset(actor);
        if(result==0)goto remove_and_render;
        target_x=W(0xd3b4e4u);
        B(0xbcd630u+a)=11;
        W(0xbcd61cu+a)=target_x;
        target_y=W(0xd3b4e0u);
        W(0xbcd61eu+a)=target_y;
        FUN_004b3250(actor);
        goto reload_and_render;
    }
    B(0xbcd630u+a)=8;
    goto render;

begin_pixel_return:
    B(0xbcd630u+a)=9;
    W(0xbcd61cu+a)=target_x;
    y=(int16_t)target_y;
    W(0xbcd61eu+a)=target_y;
    x=(int16_t)W(0xbcd61cu+a);
    {
        int32_t pixel_y=(int16_t)W(0xbcd644u+a);
        int32_t pixel_x=(int16_t)W(0xbcd642u+a);
        FUN_004b4220(actor,pixel_x,pixel_y,x*15,y*15,0);
    }
    actor=(int32_t)D(0xd056f8u);
    a=fireman_actor_offset(actor);
    W(0xbcd63cu+a)=0;
    goto render;
remove_and_render:
    B(0xbcd606u+a)=2;
    goto render;
reload_and_render:
    actor=(int32_t)D(0xd056f8u);
render:
    a=fireman_actor_offset(actor);
    state=B(0xbcd630u+a);
    if(state==13 || state==5)heading=(uint32_t)(int32_t)(int8_t)B(0xbcd60bu+a);
    else {
        direction=B(0xbcd609u+a);
        if((int8_t)direction>=8)heading=(uint32_t)(int32_t)(int8_t)B(0xbcd60au+a);
        else heading=(uint32_t)(int32_t)(int8_t)direction;
    }
    heading-=D(0xbca2e0u);
    D(0xe94e00u)=heading;
    if((int32_t)heading<0) {
        heading+=8;
        D(0xe94e00u)=heading;
    }
    if(state==13) {
        int16_t frame;
        W(0xbcd66cu+a)=(uint16_t)(W(0xbcd66cu+a)+1u);
        frame=(int16_t)W(0xbcd66cu+a);
        result=D(0xf55d2cu);
        D(0xbcd5fcu+a)=result+(frame>=36?0u:(uint32_t)(int32_t)frame*8u)+heading;
        return heading;
    }
    if(state==5) {
        (void)B(0xbcd662u+a); /* Both native branches load the same base. */
        result=D(0xf55d1cu);
        D(0xbcd5fcu+a)=result+heading;
        return heading;
    }
    if(state==4) {
        int32_t frame=(int16_t)W(0xbcd62eu+a)/2;
        uint8_t image=B(0x5d048cu+(uint32_t)frame);
        result=D(0xf55d24u);
        D(0xbcd5fcu+a)=result+image;
        return result;
    }
    result=D(0xf55d1cu);
    cycle=B(0xbcd5f9u+a);
    D(0xbcd5fcu+a)=result+(uint32_t)cycle*8u+heading;
    return heading;
}
