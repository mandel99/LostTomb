#include "population_updates.h"
#include "actor_create.h"
#include "actor_walking.h"
#include "menu_building_kind.h"
#include "sound_streams.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern void FUN_004b1d50(int32_t actor);

/* Only the pure31-step recurrence is shared. The three native callers
   publish history, seeds and samples in different orders around callbacks. */
static void advance(uint32_t *high,uint32_t *low)
{
    uint32_t i,a,b;
    for(i=0;i<31u;++i){
        a=((*high>>4)^*high)&1u;b=((*low>>4)^*low)&1u;
        *high=(*high>>1)|(a<<30);*low=(*low>>1)|(b<<30);
    }
}

/* Whole4E7700. Spawn nine figures per classified farm, using width for
   both entry coordinates as in the original. The building limit is live. */
void FUN_004e7700(void)
{
    uint32_t id,p,high,low,sample,index,i,width,remainder,actor,off,laborer;
    int32_t x,y;
    if(U(0x78d79cu)!=0xffffffffu)return;
    if((uint32_t)FUN_004becf0()!=U(0x78d7c0u)+2u&&
       (uint32_t)FUN_004becf0()!=U(0x78d7c0u)-10u)return;
    U(0x78d79cu)=2632;
    if(I(U(0x78d790u))>=1){
        id=1;p=0x799ae6u;
        do{
            if((uint8_t)FUN_00488030(id)){
                index=U(0xd3b1c4u);sample=U(0xd3b374u);high=U(0xd3b384u);low=U(0xd3b380u);
                for(i=0;i<9u;++i){
                    if(i)index=U(0xd3b1c4u);
                    U(0xd3b1c8u+index*4u)=sample;++index;U(0xd3b1c4u)=index;
                    if(I(index)>=100)U(0xd3b1c4u)=0;
                    advance(&high,&low);
                    U(0xd3b380u)=low;U(0xd3b374u)=high&0x7fu;U(0xd3b36cu)=low&7u;
                    width=U(0xbca2d0u);U(0xd3b368u)=low&0x7fu;U(0xd3b364u)=low&0x7fffu;
                    U(0xd3b384u)=high;U(0xd3b378u)=high&7u;
                    remainder=(uint32_t)((int32_t)(high&0x7fffu)/(I(width)/4));
                    /* Native consumes IDIV's remainder, not its quotient. */
                    remainder=(high&0x7fffu)-remainder*(uint32_t)(I(width)/4);
                    U(0xd3b370u)=high&0x7fffu;
                    y=I((uint32_t)(I(width*3u)/8)-remainder);
                    x=I((uint32_t)(I(width*7u)/8)-remainder);
                    actor=(uint32_t)FUN_004b4800((uint8_t)U(0xeaac28u),107,x,y,0);
                    W(p+40u)=(uint16_t)actor;off=actor*388u;
                    index=U(0xd3b1c4u);sample=U(0xd3b374u);U(0xd3b1c8u+index*4u)=sample;
                    B(0xbcd630u+off)=8;W(0xbcd654u+off)=(uint16_t)id;
                    ++index;U(0xd3b1c4u)=index;if(I(index)>=100)U(0xd3b1c4u)=0;
                    high=U(0xd3b384u);low=U(0xd3b380u);advance(&high,&low);
                    U(0xd3b378u)=high&7u;U(0xd3b370u)=high&0x7fffu;U(0xd3b364u)=low&0x7fffu;
                    U(0xd3b368u)=low&0x7fu;U(0xd3b36cu)=low&7u;sample=high&0x7fu;
                    remainder=(high&0x7fffu)%300u;
                    U(0xd3b380u)=low;U(0xd3b384u)=high;U(0xd3b374u)=sample;
                    W(0xbcd62eu+off)=(uint16_t)remainder;B(0xbcd65cu+off)=0;B(0xbcd66au+off)=11;
                    W(0xbcd61cu+off)=(uint16_t)(W(p)+i/3u);
                    W(0xbcd61eu+off)=(uint16_t)(W(p+2u)+i%3u);
                }
                if(H(p+58u)>0){
                    B(p+161u)=0;W(p+58u)=0;laborer=(uint32_t)H(p+174u);
                    if(laborer){
                        off=laborer*388u;
                        if(B(0xbcd606u+off)!=2&&B(0xbcd602u+off)==85){
                            B(0xbcd606u+off)=2;FUN_004b5420(I(laborer));
                        }
                        W(p+174u)=0;
                    }
                }
            }
            ++id;p+=264u;
        }while(I(id)<=I(U(0x78d790u)));
    }
    if(FUN_0055f5a1(RESTORE_SOUND,0))FUN_0055f561(RESTORE_SOUND,0);
    U(0xf55b94u)=1;
    if(FUN_0055f5a1(RESTORE_SOUND,2))FUN_0055f561(RESTORE_SOUND,2);
    (void)FUN_0055f449(RESTORE_SOUND,(const char *)(uintptr_t)0x5e4990u,2,1);
}

/* Whole4B03B0. Incoming actor is the global selection. Native movement
   callbacks can change it, while the entry farm remains cached for state9. */
void FUN_004b03b0(void)
{
    uint32_t actor=U(0xd056f8u),off=actor*388u,kind,speed,building,p,timer,high,low,index,width,remainder;
    uint8_t frame,action,direction;int32_t progress;
    kind=B(0xbcd602u+off);B(0xbcd66au+off)=11;U(0xbcd778u+off)=0;
    building=(uint32_t)H(0xbcd654u+off);p=0x7999d8u+building*264u;
    speed=U(0x7090ecu+kind*56u);
    if(B(p)!=1){B(0xbcd606u+off)=2;return;}
    frame=(uint8_t)(B(0xbcd5f9u+off)+1u);B(0xbcd5f9u+off)=frame;
    if(frame>=6u)B(0xbcd5f9u+off)=0;
    action=B(0xbcd630u+off);
    if(action==4){FUN_004b1d50(I(actor));actor=U(0xd056f8u);}
    else if(action==8){
        B(0xbcd65du+off)=1;B(0xbcd5f9u+off)=0;W(0xbcd62eu+off)=(uint16_t)(W(0xbcd62eu+off)-1u);
        if(H(0xbcd62eu+off)<=0){
            FUN_004b2d20(I(actor));actor=U(0xd056f8u);off=actor*388u;
            if(H(0xbcd632u+off)<=0)B(0xbcd606u+off)=2;else B(0xbcd630u+off)=9;
        }
    }else if(action==9){
        B(0xbcd65du+off)=0;FUN_004a5da0(I(actor),speed);actor=U(0xd056f8u);off=actor*388u;
        direction=B(0xbcd609u+off);
        if(direction==9){
            B(0xbcd630u+off)=8;W(0xbcd62eu+off)=1;FUN_004b3250(I(actor));actor=U(0xd056f8u);
        }else if(direction==10){W(p+46u)=0;W(p+32u)=0;B(0xbcd606u+off)=2;}
        else if(direction==8){B(0xbcd630u+off)=10;W(0xbcd61cu+off)=W(p+6u);W(0xbcd61eu+off)=W(p+8u);}
    }else if(action==10){
        building=(uint32_t)H(0xbcd654u+off);p=0x799a2cu+building*264u;progress=H(p);timer=U(0x78d79cu);
        if(progress>I(timer-1000u))W(p)=(uint16_t)(timer-1000u);if(H(p)<0)W(p)=0;
        if(I(timer)<=1000){
            index=U(0xd3b1c4u);B(0xbcd630u+off)=11;U(0xd3b1c8u+index*4u)=U(0xd3b374u);
            ++index;U(0xd3b1c4u)=index;if(I(index)>=100)U(0xd3b1c4u)=0;
            high=U(0xd3b384u);low=U(0xd3b380u);advance(&high,&low);
            U(0xd3b384u)=high;U(0xd3b378u)=high&7u;U(0xd3b364u)=low&0x7fffu;
            U(0xd3b380u)=low;U(0xd3b374u)=high&0x7fu;width=U(0xbca2d0u);
            U(0xd3b36cu)=low&7u;U(0xd3b368u)=low&0x7fu;U(0xd3b370u)=high&0x7fffu;
            remainder=(uint32_t)((int32_t)(high&0x7fffu)%(I(width)/4));
            W(0xbcd61cu+off)=(uint16_t)((uint32_t)(I(width*3u)/8)-remainder+2u);
            W(0xbcd61eu+off)=(uint16_t)((uint32_t)(I(width*7u)/8)-remainder-2u);W(0xbcd63cu+off)=0;
        }
    }else if(action==11){
        B(0xbcd65du+off)=0;FUN_004a5da0(I(actor),speed);actor=U(0xd056f8u);off=actor*388u;
        direction=B(0xbcd609u+off);if(direction==9||direction==10||direction==8)B(0xbcd606u+off)=2;
    }else B(0xbcd630u+off)=8;
    off=actor*388u;
    U(0xbcd5fcu+off)=U(B(0xbcd630u+off)==10?0xf5714cu:0xf5713cu)+B(0xbcd5f9u+off);
}

/* Whole4BF920, native RET4. Public C is cdecl; the laborer frame adapter
   retains the original cleanup. Cache both references before classification. */
void FUN_004bf920(int32_t actor)
{
    uint32_t off=(uint32_t)actor*388u,destination=(uint32_t)H(0xbcd656u+off),source=(uint32_t)H(0xbcd652u+off);
    uint32_t p,period,kind;
    if(!(uint8_t)FUN_00488030(destination)){B(0x799a80u+source*264u)=32;return;}
    period=U(0x78d79cu);B(0xbcd606u+off)=2;if(period)return;
    B(0x799a7fu+source*264u)=32;p=destination*264u;
    B(0x799a7fu+p)=96;++B(0x799a7eu+p);W(0x799a8cu+p)=0;
    kind=(uint32_t)H(0x7999e8u+p);W(0x799a18u+p)=W(0x705eb8u+kind*40u);
}
