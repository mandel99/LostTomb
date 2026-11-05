#include <stdint.h>
#define B(p) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(p))
#define W(p) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(p))
#define D(p) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(p))
#define H(p) ((int32_t)(int16_t)W(p))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_004bd250(int32_t actor);
extern void FUN_004b17d0(int32_t actor),FUN_004b1d50(int32_t actor);
extern void FUN_004b53b0(int32_t actor),FUN_004b3250(int32_t actor);
extern void FUN_004b1700(int32_t actor),FUN_004b5420(int32_t actor);
extern void FUN_004a5da0(int32_t actor,uint32_t speed);
extern uint32_t FUN_004a35e0(int32_t actor);
extern int32_t FUN_004999e0(int32_t x,int32_t y,int32_t radius,int32_t actor);
extern int32_t FUN_00499b50(int32_t actor,int32_t radius);
extern uint8_t FUN_0051cd80(int32_t x,int32_t y,int32_t tx,int32_t ty);
extern int32_t FUN_004b45c0(int32_t owner,int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t kind);

static uint32_t soldier_offset(int32_t actor){return (uint32_t)actor*388u;}
static int32_t soldier_delta(int32_t a,int32_t b)
{return a>b?I((uint32_t)a-(uint32_t)b):(a<b?I((uint32_t)b-(uint32_t)a):0);}
static int32_t soldier_max(int32_t a,int32_t b){return a>=b?a:b;}

/* Identical WORD table/publication phases, with caller-selected centre fields.
   The ordinal is read afresh after the X store before selecting the Y entry. */
static void soldier_publish_formation_target(uint32_t a,uint32_t g,uint8_t kind,
                                            int32_t mode,uint32_t dst,uint32_t centre)
{
    uint32_t table=0x5cd6f8u,layout=(uint32_t)mode*16u,index;
    uint16_t value,base;
    if(kind==13 && mode==1){table=0x5cdd78u;layout=0;}
    else if(kind==13 && mode==2){table=0x5cddf8u;layout=0;}
    index=layout+B(0xbcd65au+a);
    value=W(table+index*8u);
    base=W(centre+g);
    W(dst+a)=(uint16_t)(value+base);
    index=layout+B(0xbcd65au+a);
    value=W(table+4u+index*8u);
    base=W(centre+2u+g);
    W(dst+2u+a)=(uint16_t)(value+base);
}
static void soldier_apply_target_jitter(uint32_t a)
{
    uint8_t index=B(0xbcd5f8u+a);
    int16_t adjustment;
    if(index!=0){
        adjustment=(int8_t)B(0x5ca506u+(uint32_t)index*2u);
        W(0xbcd61cu+a)=(uint16_t)(W(0xbcd61cu+a)+adjustment);
        index=B(0xbcd5f8u+a);
        adjustment=(int8_t)B(0x5ca507u+(uint32_t)index*2u);
        W(0xbcd61eu+a)=(uint16_t)(W(0xbcd61eu+a)+adjustment);
    }
}
static void soldier_publish_combat_target(uint32_t a,uint32_t target,int32_t actor,int32_t id)
{
    uint16_t value=W(0xbcd60cu+target);
    W(0xbcd61cu+a)=value;
    value=W(0xbcd60eu+target);
    W(0xbcd61eu+a)=value;
    W(0xbcd67au+a)=(uint16_t)id;
    W(0xbcd67cu+target)=(uint16_t)actor;
    value=W(0xbcd67eu+target);
    W(0xbcd680u+a)=value;
}
/* All five native projectile sites read targetY,X then ownY,X, load the
   owner ID last, call the ordinary child, then publish the formation cooldown. */
static void soldier_fire_projectile(uint32_t a,uint32_t target,uint32_t g)
{
    int32_t ty=H(0xbcd60eu+target),tx=H(0xbcd60cu+target);
    int32_t y=H(0xbcd60eu+a),x=H(0xbcd60cu+a);
    int32_t owner=I(D(0xd056f8u));
    (void)FUN_004b45c0(owner,x,y,tx,ty,60);
    W(0xbcba24u+g)=6;
}

/* Shared registry callback for kind11 Javelin thrower, kind12 Mounted
   auxiliary and kind13 Legionary. Physical scratch/flags are not this API. */
void restore_actor_update_formation_soldier(void)
{
    int32_t actor=I(D(0xd056f8u)),formation,mode,layout_mode,target=0;
    uint32_t a=soldier_offset(actor),g,player,speed,value,p,heading,turned=0;
    uint32_t tile=D(0xbcd618u+a);
    uint8_t kind,state,direction,cycle,timer,in_range=0;
    int32_t x,y,tx,ty,dx,dy,distance,formation_x=0,formation_y=0;
    uint16_t wx,wy,word;
    uint8_t close_defence=0;
    static const uint8_t shield_offsets[16]={22,238,27,253,23,17,0,11,235,17,231,255,233,239,0,244};
    value=D(0xaf598cu+tile*4u)&0x10004u;
    if(value==0x10004u){FUN_004bd250(actor);return;}
    player=D(0xeaac28u)*0x49d8u;
    formation=H(0xbcd658u+a);
    value=D(0xea5ee8u+player)+1u;
    B(0xbcd66au+a)=9;
    cycle=(uint8_t)(B(0xbcd5f9u+a)+1u);
    D(0xea5ee8u+player)=value;
    B(0xbcd5f9u+a)=cycle;
    if(cycle>=12)B(0xbcd5f9u+a)=0;
    D(0xbcd778u+a)=0;
    g=(uint32_t)formation*144u;
    if(B(0xbcb9c8u+g)!=1)B(0xbcd630u+a)=4;
    kind=B(0xbcd602u+a);
    word=W(0xbcba46u+g);
    speed=D(0x7090ecu+(uint32_t)kind*56u);
    if(word!=0){
        word=W(0xbcba48u+g);
        W(0xbcba48u+g)=(uint16_t)(word+1u);
        if((int16_t)word>2000)W(0xbcba46u+g)=0;
    }
    mode=H(0xbcb9f2u+g);
    layout_mode=mode;
    if(B(0xbcd65bu+a)!=0)layout_mode=7;
    else if(B(0xbcd630u+a)==9)layout_mode=7;
    soldier_publish_formation_target(a,g,kind,layout_mode,0xbcd628u,0xbcb9feu);
    state=B(0xbcd630u+a);
    if(state==5){FUN_004b17d0(actor);goto reload_finish;}
    if(state==4){FUN_004b1d50(actor);goto reload_finish;}
    if(state==18){W(0xbcd62eu+a)=0;B(0xbcd65bu+a)=0;goto finish;}
    if(state==8){
        FUN_004b53b0(actor);
        actor=I(D(0xd056f8u));a=soldier_offset(actor);
        W(0xbcd62eu+a)=0;B(0xbcd65bu+a)=1;B(0xbcd5f9u+a)=0;
        wx=W(0xbcd60cu+a);word=W(0xbcd628u+a);
        if(wx!=word)B(0xbcd630u+a)=9;
        else{wy=W(0xbcd60eu+a);word=W(0xbcd62au+a);if(wy!=word)B(0xbcd630u+a)=9;}
        goto finish;
    }
    if(state==9 || state==3){
        if(state==3 && D(0xbcba4cu+g)!=0)goto finish;
        W(0xbcd62eu+a)=0;B(0xbcd65bu+a)=1;
        wx=W(0xbcd628u+a);W(0xbcd61cu+a)=wx;
        goto initial_home_y;
    }
    if(state==10){
        B(0xbcd65bu+a)=1;
        wx=W(0xbcd624u+a);W(0xbcd61cu+a)=wx;
        wy=W(0xbcd626u+a);W(0xbcd61eu+a)=wy;
        FUN_004a5da0(actor,speed);
        actor=I(D(0xd056f8u));a=soldier_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==8){B(0xbcd606u+a)=2;goto finish;}
        goto direction_error;
    }
    if(state==11 || state==19){
        B(0xbcd65bu+a)=0;
        kind=B(0xbcd602u+a);
        soldier_publish_formation_target(a,g,kind,mode,0xbcd61cu,0xbcb9fau);
        soldier_apply_target_jitter(a);
        y=H(0xbcd61eu+a);value=D(0xbca2d8u);x=H(0xbcd61cu+a);
        D(0xbcd620u+a)=value+(uint32_t)x+(uint32_t)y*228u;
        value=speed;
        if(state==19){
            int32_t previous=I(D(0xbcd734u+a));
            x=H(0xbcd60cu+a);
            if(x!=previous)goto moved_in_formation;
            y=H(0xbcd60eu+a);
            if(y!=I(D(0xbcd738u+a))){
moved_in_formation:
                D(0xbcd734u+a)=(uint32_t)x;
                y=H(0xbcd60eu+a);D(0xbcd738u+a)=(uint32_t)y;
                D(0xbcd72cu+a)=D(0xbcd72cu+a)+1u;
            }
            distance=I(D(0xbcd72cu+a));
            if(distance>5 && distance<20)value=speed*2u;
        }
        FUN_004a5da0(actor,value);
        actor=I(D(0xd056f8u));a=soldier_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==8){
            if(state==19){B(0xbcd630u+a)=20;B(0xbcd5f9u+a)=0;W(0xbcb9f2u+g)=15;D(0xbcd72cu+a)=0;}
            else{word=W(0xbcb9f2u+g);B(0xbcd630u+a)=word==15?20:12;B(0xbcd5f9u+a)=0;}
            goto finish;
        }
        if(direction==9)goto reset_route;
        if(direction==10){
            timer=(uint8_t)(B(0xbcd5f8u+a)+1u);B(0xbcd5f8u+a)=timer;
            if(timer>168)B(0xbcd606u+a)=2;
            B(0xbcd5f9u+a)=0;goto finish;
        }
        if(state==19)W(0xbcb9f2u+g)=0;
        goto finish;
    }
    if(state==20){
        /* This state intentionally uses DWORD generic offsets, without the
           WORD/special-kind publication helper used by travel states. */
        uint32_t index;
        formation_x=H(0xbcb9fau+g);B(0xbcd65bu+a)=0;
        index=(uint32_t)mode*16u+B(0xbcd65au+a);
        formation_x=I((uint32_t)formation_x+D(0x5cd6f8u+index*8u));
        formation_y=H(0xbcb9fcu+g);
        value=D(0x5cd6fcu+index*8u);
        timer=B(0xbcd5f8u+a);
        formation_y=I((uint32_t)formation_y+value);
        if(timer!=0){
            dx=(int8_t)B(0x5ca506u+(uint32_t)timer*2u);
            dy=(int8_t)B(0x5ca507u+(uint32_t)timer*2u);
            formation_x=I((uint32_t)formation_x+(uint32_t)dx);
            formation_y=I((uint32_t)formation_y+(uint32_t)dy);
        }
        close_defence=1;
        goto choose_near_target;
    }
    if(state==12)goto hold_formation;
    if(state==14){B(0xbcd65bu+a)=0;close_defence=0;goto choose_near_target;}
    if(state==21)goto pursue_designated_group;
    if(state==13){
        B(0xbcd66au+a)=10;W(0xbcba10u+g)=1;B(0xbcd65bu+a)=1;
        FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));a=soldier_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==8){B(0xbcd630u+a)=23;goto finish;}
        goto direction_error;
    }
    if(state==15){
        wx=W(0xea3ff0u+player);wy=W(0xea3ff2u+player);B(0xbcd65bu+a)=0;
        W(0xbcd61cu+a)=wx;W(0xbcd61eu+a)=wy;
        FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));a=soldier_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==8 || direction==9 || direction==10){
            B(0xbcd630u+a)=17;FUN_004b3250(actor);
            actor=I(D(0xd056f8u));FUN_004b5420(actor);
            goto reload_finish;
        }
        goto finish;
    }
    if(state==17){B(0xbcd65du+a)=1;B(0xbcd65bu+a)=1;goto finish;}
    if(state==16){
        B(0xbcd65du+a)=0;W(0xbcd62eu+a)=0;B(0xbcd65bu+a)=1;
        wx=W(0xbcd628u+a);W(0xbcd61cu+a)=wx;
        goto initial_home_y;
    }
    if(state==22 || state==23){
        B(0xbcd66au+a)=10;W(0xbcd62eu+a)=0;B(0xbcd65bu+a)=1;
        wx=W(0xbcd628u+a);W(0xbcd61cu+a)=wx;
        wy=W(0xbcd62au+a);W(0xbcd61eu+a)=wy;
        value=D(0xbca2d8u);y=(int16_t)wy;x=H(0xbcd61cu+a);
        D(0xbcd620u+a)=value+(uint32_t)x+(uint32_t)y*228u;
        FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));a=soldier_offset(actor);
        direction=B(0xbcd609u+a);
        if(direction==8){B(0xbcd630u+a)=8;goto finish;}
        goto direction_error;
    }
    if(state==6){B(0xbcd66au+a)=10;D(0xbcd5fcu+a)=0;D(0xbcd778u+a)=0;goto finish;}
    if(state==7){
        B(0xbcd66au+a)=10;B(0xbcd5f9u+a)=0;
        W(0xbcd62eu+a)=(uint16_t)(W(0xbcd62eu+a)+1u);
        if(H(0xbcd62eu+a)<=100)goto finish;
        W(0xbcd62eu+a)=0;value=FUN_004a35e0(actor);
        if((uint8_t)value!=0)goto reload_finish;
        actor=I(D(0xd056f8u));a=soldier_offset(actor);B(0xbcd606u+a)=2;
        goto finish;
    }
    B(0xbcd630u+a)=8;goto finish;

initial_home_y:
    wy=W(0xbcd62au+a);value=D(0xbca2d8u);W(0xbcd61eu+a)=wy;
    y=(int16_t)wy;x=H(0xbcd61cu+a);
    D(0xbcd620u+a)=value+(uint32_t)x+(uint32_t)y*228u;
    FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));a=soldier_offset(actor);
    direction=B(0xbcd609u+a);
    if(direction==8){B(0xbcd630u+a)=8;goto finish;}
direction_error:
    if(direction==9)goto reset_route;
    if(direction==10)B(0xbcd606u+a)=2;
    goto finish;

choose_near_target:
    target=H(0xbcd67au+a);p=soldier_offset(target);
    if(B(0xbcd606u+p)!=1){target=0;W(0xbcd67au+a)=0;}
    p=soldier_offset(target);
    if(B(0xbcd630u+p)==4){W(0xbcd67au+a)=0;target=0;}
    if(close_defence){
        kind=B(0xbcd602u+soldier_offset(target));
        if(kind==11 || kind==12 || kind==13){target=0;W(0xbcd67au+a)=0;}
        x=H(0xbcd60cu+a);y=H(0xbcd60eu+a);
        dx=soldier_delta(x,formation_x);D(0xd3b4fcu)=(uint32_t)dx;
        dy=soldier_delta(y,formation_y);D(0xd3b4f8u)=(uint32_t)dy;
        if(soldier_max(dx,dy)>=6)goto clear_target_and_hold;
        if(target<=0){
            kind=B(0xbcd602u+a);
            if(kind!=11 && kind!=13){
                /* Here both coordinate reads dominate this call directly. */
                target=FUN_004999e0(x,y,6,actor);actor=I(D(0xd056f8u));
                goto publish_new_target;
            }
            goto search_ranged_target;
        }
        goto check_target_range;
    }
    if(target<=0){
        kind=B(0xbcd602u+a);
        if(kind!=11 && kind!=13){
            x=H(0xbcd60cu+a);y=H(0xbcd60eu+a);
            target=FUN_004999e0(x,y,20,actor);actor=I(D(0xd056f8u));
            goto publish_new_target;
        }
        goto search_ranged_target;
    }
    goto check_target_range;
search_ranged_target:
    timer=(uint8_t)(B(0xbcd663u+a)+1u);B(0xbcd663u+a)=timer;
    kind=B(0xbcd602u+a);
    if((int32_t)timer>I(D(0x7090e8u+(uint32_t)kind*56u))){
        B(0xbcd663u+a)=0;kind=B(0xbcd602u+a);
        value=D(0x7090e4u+(uint32_t)kind*56u)+(close_defence?6u:20u);
        target=FUN_00499b50(actor,I(value));actor=I(D(0xd056f8u));
        a=soldier_offset(actor);
        if(target!=0){
            p=soldier_offset(target);B(0xbcd662u+a)=1;
            ty=H(0xbcd60eu+p);tx=H(0xbcd60cu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
            direction=FUN_0051cd80(x,y,tx,ty);actor=I(D(0xd056f8u));
            B(0xbcd609u+soldier_offset(actor))=direction;
        }else B(0xbcd662u+a)=0;
    }
publish_new_target:
    a=soldier_offset(actor);
    if(target!=0)soldier_publish_combat_target(a,soldier_offset(target),actor,target);
    else if(close_defence){
        x=H(0xbcd60cu+a);
        if(formation_x!=x)B(0xbcd630u+a)=12;
        else{y=H(0xbcd60eu+a);if(formation_y!=y)B(0xbcd630u+a)=12;else B(0xbcd5f9u+a)=0;}
    }else{B(0xbcd630u+a)=12;B(0xbcd5f9u+a)=0;}
    FUN_004b3250(actor);actor=I(D(0xd056f8u));
check_target_range:
    a=soldier_offset(actor);kind=B(0xbcd602u+a);
    if(kind==11 || kind==13){
        y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);p=soldier_offset(target);
        ty=H(0xbcd60eu+p);tx=H(0xbcd60cu+p);
        dx=soldier_delta(x,tx);D(0xd3b4fcu)=(uint32_t)dx;
        dy=soldier_delta(y,ty);D(0xd3b4f8u)=(uint32_t)dy;
        in_range=(uint8_t)(soldier_max(dx,dy)<=I(D(0x7090e4u+(uint32_t)kind*56u)));
    }else in_range=0;
    if(target==0)goto finish;
    if(!in_range){FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));}
    a=soldier_offset(actor);direction=B(0xbcd609u+a);
    if(direction==9 || direction==10)goto clear_target_and_hold;
    if(!in_range)goto follow_current_target;
    p=soldier_offset(target);
    tx=H(0xbcd60cu+p);ty=H(0xbcd60eu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
    direction=FUN_0051cd80(x,y,tx,ty);B(0xbcd609u+a)=direction;
    if(B(0xbcd662u+a)==7)soldier_fire_projectile(a,p,g);
    goto advance_attack_and_reset;

clear_target_and_hold:
    B(0xbcd630u+a)=12;W(0xbcd67au+a)=0;B(0xbcd5f9u+a)=0;goto finish;

hold_formation:
    /* State12 ranged variants follow below; cached offsets intentionally
       survive some calls while the active-ID variable is refreshed. */
    B(0xbcd65bu+a)=0;B(0xbcd5f9u+a)=0;
    kind=B(0xbcd602u+a);
    soldier_publish_formation_target(a,g,kind,mode,0xbcd61cu,0xbcb9fau);
    soldier_apply_target_jitter(a);
    wx=W(0xbcd60cu+a);word=W(0xbcd61cu+a);
    if(wx==word){
        wy=W(0xbcd60eu+a);word=W(0xbcd61eu+a);
        if(wy==word){if((int16_t)mode==15)goto return_to_formation;goto hold_kind;}
    }
    if(H(0xbcba24u+g)>0)goto hold_kind;
    if(H(0xbcba18u+g)>0)goto hold_kind;
    if(H(0xbcba26u+g)>0)goto hold_kind;
return_to_formation:
    B(0xbcd630u+a)=11;B(0xbcd5f8u+a)=0;
hold_kind:
    kind=B(0xbcd602u+a);
    if(kind==11){
        if(B(0xbcd630u+a)==11)goto finish;
        timer=B(0xbcd663u+a);target=H(0xbcd67au+a);
        timer=(uint8_t)(timer+1u);B(0xbcd663u+a)=timer;
        kind=B(0xbcd602u+a);
        if((int32_t)timer>I(D(0x7090e8u+(uint32_t)kind*56u))){
            B(0xbcd663u+a)=0;kind=B(0xbcd602u+a);
            value=D(0x7090e4u+(uint32_t)kind*56u);
            target=FUN_00499b50(actor,I(value));actor=I(D(0xd056f8u));a=soldier_offset(actor);
            if(target==0){B(0xbcd662u+a)=0;W(0xbcd67au+a)=0;goto finish;}
            B(0xbcd662u+a)=1;p=soldier_offset(target);
            ty=H(0xbcd60eu+p);tx=H(0xbcd60cu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
            direction=FUN_0051cd80(x,y,tx,ty);actor=I(D(0xd056f8u));
            B(0xbcd609u+a)=direction; /* a is deliberately cached across this call. */
        }
        if(target==0)goto finish;
        W(0xbcd67au+a)=(uint16_t)target;p=soldier_offset(target);
        W(0xbcd67cu+p)=(uint16_t)actor;
        word=W(0xbcd67eu+p);W(0xbcd680u+a)=word;
        tx=H(0xbcd60cu+p);x=H(0xbcd60cu+a);y=H(0xbcd60eu+a);ty=H(0xbcd60eu+p);
        dx=soldier_delta(x,tx);D(0xd3b4fcu)=(uint32_t)dx;
        dy=soldier_delta(y,ty);dx=I(D(0xd3b4fcu));D(0xd3b4f8u)=(uint32_t)dy;
        kind=B(0xbcd602u+a);
        if(soldier_max(dx,dy)>I(D(0x7090e4u+(uint32_t)kind*56u)))goto finish;
        in_range=1;
        ty=H(0xbcd60eu+p); /* X and own X/Y remain cached for this aim. */
        direction=FUN_0051cd80(x,y,tx,ty);B(0xbcd609u+a)=direction;
        if(B(0xbcd662u+a)==7)soldier_fire_projectile(a,p,g);
        goto advance_attack_and_reset;
    }
    if(kind!=13)goto finish;
    if(B(0xbcd630u+a)==11)goto finish;
    timer=(uint8_t)(B(0xbcd663u+a)+1u);B(0xbcd663u+a)=timer;
    kind=B(0xbcd602u+a);
    if((int32_t)timer>I(D(0x7090e8u+(uint32_t)kind*56u))){
        B(0xbcd663u+a)=0;kind=B(0xbcd602u+a);
        value=D(0x7090e4u+(uint32_t)kind*56u);
        target=FUN_00499b50(actor,I(value));actor=I(D(0xd056f8u));a=soldier_offset(actor);
        W(0xbcd67au+a)=(uint16_t)target;
        if(target!=0){
            B(0xbcd662u+a)=1;p=soldier_offset(target);
            ty=H(0xbcd60eu+p);tx=H(0xbcd60cu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
            direction=FUN_0051cd80(x,y,tx,ty);actor=I(D(0xd056f8u));B(0xbcd609u+a)=direction;
        }else B(0xbcd662u+a)=0;
    }
    if(B(0xbcd662u+a)==0){FUN_004b1700(actor);goto reload_finish;}
    target=H(0xbcd67au+a);
    if(target!=0){
        p=soldier_offset(target);a=soldier_offset(actor);
        ty=H(0xbcd60eu+p);tx=H(0xbcd60cu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
        direction=FUN_0051cd80(x,y,tx,ty);B(0xbcd609u+a)=direction;
        if(B(0xbcd662u+a)==7)soldier_fire_projectile(a,p,g);
        actor=I(D(0xd056f8u));
    }
    a=soldier_offset(actor);
    timer=(uint8_t)(B(0xbcd662u+a)+1u);B(0xbcd662u+a)=timer;
    if(timer>100)B(0xbcd662u+a)=0;
    goto finish;

pursue_designated_group:
    B(0xbcd65bu+a)=0;target=H(0xbcd67au+a);p=soldier_offset(target);
    if(B(0xbcd606u+p)!=1){target=0;W(0xbcd67au+a)=0;}
    p=soldier_offset(target);
    if(B(0xbcd630u+p)==4){target=0;W(0xbcd67au+a)=0;}
    value=D(0xbcba54u+g);p=soldier_offset(target);
    formation=H(0xbcd658u+p);
    if(formation!=I(value)){W(0xbcd67au+a)=0;goto search_designated_group;}
    if(target>0)goto have_designated_target;
search_designated_group:
    if(B(0xbcd602u+a)==12){
        y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
        target=FUN_004999e0(x,y,100,actor);
    }else target=FUN_00499b50(actor,100);
    value=D(0xbcba54u+g);
    if(value==UINT32_MAX)target=0;
    actor=I(D(0xd056f8u));
    if(value!=UINT32_MAX && target!=0){
        a=soldier_offset(actor);p=soldier_offset(target);
        soldier_publish_combat_target(a,p,actor,target);
    }
have_designated_target:
    if(value==UINT32_MAX || target==0){
        D(0xbcba54u+g)=UINT32_MAX;a=soldier_offset(actor);
        B(0xbcd630u+a)=11;B(0xbcd5f9u+a)=0;goto finish;
    }
    a=soldier_offset(actor);kind=B(0xbcd602u+a);
    if(kind==11 || kind==13){
        p=soldier_offset(target);
        y=H(0xbcd60eu+a);tx=H(0xbcd60cu+p);ty=H(0xbcd60eu+p);x=H(0xbcd60cu+a);
        dx=soldier_delta(x,tx);D(0xd3b4fcu)=(uint32_t)dx;
        dy=soldier_delta(y,ty);D(0xd3b4f8u)=(uint32_t)dy;
        if(soldier_max(dx,dy)<=I(D(0x7090e4u+(uint32_t)kind*56u))){
            B(0xbcd5f9u+a)=0;
            tx=H(0xbcd60cu+p);ty=H(0xbcd60eu+p);in_range=1;
            y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
            direction=FUN_0051cd80(x,y,tx,ty);B(0xbcd609u+a)=direction;
            if(B(0xbcd662u+a)==7)soldier_fire_projectile(a,p,g);
            goto advance_attack_and_reset;
        }
    }
    in_range=0;FUN_004a5da0(actor,speed);actor=I(D(0xd056f8u));a=soldier_offset(actor);
    direction=B(0xbcd609u+a);
follow_current_target:
    if(direction==8){
        target=H(0xbcd67au+a);p=soldier_offset(target);
        wx=W(0xbcd60cu+p);W(0xbcd61cu+a)=wx;
        wy=W(0xbcd60eu+p);W(0xbcd61eu+a)=wy;
        goto reset_route;
    }
    goto finish;

advance_attack_and_reset:
    actor=I(D(0xd056f8u));a=soldier_offset(actor);
    timer=(uint8_t)(B(0xbcd662u+a)+1u);B(0xbcd662u+a)=timer;
    if(timer>100)B(0xbcd662u+a)=0;
reset_route:
    FUN_004b3250(actor);
reload_finish:
    actor=I(D(0xd056f8u));
finish:
    a=soldier_offset(actor);
    kind=B(0xbcd602u+a);
    if(kind==11){
        word=W(0xbcd67au+a);
        if(word!=0 && B(0xbcd630u+a)==12){
            p=soldier_offset((int16_t)word);
            tx=H(0xbcd60cu+p);ty=H(0xbcd60eu+p);y=H(0xbcd60eu+a);x=H(0xbcd60cu+a);
            direction=FUN_0051cd80(x,y,tx,ty);B(0xbcd609u+a)=direction;
        }
        direction=B(0xbcd609u+a);
        x=(int8_t)direction<8?(int8_t)direction:(int8_t)B(0xbcd60au+a);
        heading=(uint32_t)x-D(0xbca2e0u);D(0xe94e00u)=heading;
        turned=heading+2u;if(I(turned)>7)turned-=8u;
    }else{
        state=B(0xbcd630u+a);
        if(state==5)x=(int8_t)B(0xbcd60bu+a);
        else{
            word=W(0xbcba24u+g);
            if(word!=0){
                direction=B(0xbcd609u+a);if((int8_t)direction<8){x=(int8_t)direction;goto selected_heading;}
                direction=B(0xbcd60au+a);if((int8_t)direction<8){x=(int8_t)direction;goto selected_heading;}
            }
            if(state==12){
                direction=B(0xbcba33u+g);
                if(direction<8 && B(0xbcd662u+a)==0){x=direction;goto selected_heading;}
                direction=B(0xbcba38u+g);
                if(direction<8 && B(0xbcd662u+a)==0){x=direction;goto selected_heading;}
            }
            direction=B(0xbcd609u+a);
            x=(int8_t)direction<8?(int8_t)direction:(int8_t)B(0xbcd60au+a);
        }
selected_heading:
        heading=(uint32_t)x-D(0xbca2e0u);D(0xe94e00u)=heading;
    }
    if(I(heading)<0){heading+=8u;D(0xe94e00u)=heading;}
    if(I(heading)>=8){heading-=8u;D(0xe94e00u)=heading;}
    kind=B(0xbcd602u+a);
    if(kind==13){
        uint8_t palette,shield_x,shield_y;
        value=D(0xf56074u);cycle=B(0xbcd5f9u+a);
        shield_x=shield_offsets[heading*2u];
        D(0xbcd778u+a)=value+(uint32_t)cycle*8u+heading;
        shield_y=shield_offsets[heading*2u+1u];
        B(0xbcd664u+a)=shield_x;B(0xbcd665u+a)=shield_y;
        state=B(0xbcd630u+a);
        if(state==5){
            timer=B(0xbcd662u+a);palette=B(0x784cbdu);
            if(timer<12){value=D(palette?0xf56414u:0xf55f14u);D(0xbcd5fcu+a)=value+heading;}
            else{value=D(palette?0xf56414u:0xf55f14u);D(0xbcd5fcu+a)=value+((uint32_t)timer-12u)/2u*8u+heading;}
        }else if(state==12){
            palette=B(0x784cbdu);value=D(palette?0xf56404u:0xf55f04u);D(0xbcd5fcu+a)=value+heading;
        }else if(state==20){
            timer=B(0xbcd662u+a);
            if(timer!=0){
                palette=B(0x784cbdu);timer=B(0x5d058cu+((uint32_t)timer>>1));
                value=D(palette?0xf56414u:0xf55f14u);D(0xbcd5fcu+a)=value+(uint32_t)timer*8u+heading;
            }else{
                palette=B(0x784cbdu);value=D(palette?0xf56404u:0xf55f04u);cycle=B(0xbcd5f9u+a);
                D(0xbcd5fcu+a)=value+(uint32_t)cycle*8u+heading;
            }
        }else{
            palette=B(0x784cbdu);
            if(state==4){x=H(0xbcd62eu+a)/2;timer=B(0x5d048cu+(uint32_t)x);value=D(palette?0xf5640cu:0xf55f0cu);D(0xbcd5fcu+a)=value+timer;}
            else{value=D(palette?0xf56404u:0xf55f04u);cycle=B(0xbcd5f9u+a);D(0xbcd5fcu+a)=value+(uint32_t)cycle*8u+heading;}
        }
    }else if(kind==12){
        uint8_t palette;
        D(0xbcd778u+a)=0;state=B(0xbcd630u+a);
        if(state==5){
            timer=B(0xbcd662u+a);palette=B(0x784cbdu);value=D(palette?0xf563ccu:0xf55efcu);
            D(0xbcd5fcu+a)=value+(timer<12?0u:((uint32_t)timer-12u)/2u*8u)+heading;
        }else{
            palette=B(0x784cbdu);
            if(state==4){x=H(0xbcd62eu+a)/2;timer=B(0x5d048cu+(uint32_t)x);value=D(palette?0xf563c4u:0xf55ef4u);D(0xbcd5fcu+a)=value+timer;}
            else{value=D(palette?0xf563bcu:0xf55eecu);cycle=B(0xbcd5f9u+a);D(0xbcd5fcu+a)=value+(uint32_t)cycle*8u+heading;}
        }
    }else if(kind==11){
        uint8_t palette;
        D(0xbcd778u+a)=0;state=B(0xbcd630u+a);
        if(state==5){
            timer=B(0xbcd662u+a);palette=B(0x784cbdu);value=D(palette?0xf563acu:0xf55ee4u);
            D(0xbcd5fcu+a)=value+(timer<12?(uint32_t)timer*8u:0u)+turned;
        }else if(state==12){
            word=W(0xbcd67au+a);palette=B(0x784cbdu);
            if(word==0){value=D(palette?0xf5639cu:0xf55ed4u);cycle=B(0xbcd5f9u+a);D(0xbcd5fcu+a)=value+(uint32_t)cycle*8u+heading;}
            timer=B(0xbcd662u+a);value=D(palette?0xf563acu:0xf55ee4u);
            D(0xbcd5fcu+a)=value+(timer!=0 && timer<12?(uint32_t)timer*8u:88u)+turned;
        }else if(state==20 && in_range){
            timer=B(0xbcd662u+a);palette=B(0x784cbdu);value=D(palette?0xf563acu:0xf55ee4u);
            D(0xbcd5fcu+a)=value+(timer!=0 && timer<12?(uint32_t)timer*8u:88u)+turned;
        }else{
            palette=B(0x784cbdu);
            if(state==4){x=H(0xbcd62eu+a)/2;timer=B(0x5d048cu+(uint32_t)x);value=D(palette?0xf563a4u:0xf55edcu);D(0xbcd5fcu+a)=value+timer;}
            else{value=D(palette?0xf5639cu:0xf55ed4u);cycle=B(0xbcd5f9u+a);D(0xbcd5fcu+a)=value+(uint32_t)cycle*8u+heading;}
        }
    }
    if(B(0xbcd630u+a)==18){D(0xbcd5fcu+a)=0;D(0xbcd778u+a)=0;}
}
