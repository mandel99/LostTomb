#include "god_effects.h"
#include "actor_combat.h"
#include "event_publisher.h"
#include "god_frog_effect.h"
#include "sound_streams.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

extern int32_t FUN_004eada0(int32_t kind);
/* Defined by this phase's complete city/actor-resource helpers, never by a
   fallback alias to legacy 449/923/924/1418. */
void restore_god_city_00461950(int32_t amount);

static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static void mix31(uint32_t *a,uint32_t *b)
{
    uint32_t i,high=*a,low=*b;
    for(i=0;i<31u;++i){
        uint32_t x=((high>>4)^high)&1u,y=((low>>4)^low)&1u;
        high>>=1;low>>=1;
        if(x)high|=0x40000000u;
        if(y)low|=0x40000000u;
    }
    *a=high;*b=low;
}

/* Whole 4BD5E0, original ECX receiver + one DWORD, RET4. The explicit C
   receiver is private; no unreviewed global member callers are rebound. */
void restore_god_settings_004bd5e0(uint32_t self,int32_t amount)
{
    uint32_t value=U(self+28u)+(uint32_t)amount;
    U(self+28u)=value;
    if((int32_t)value<0)value=0;
    else if((int32_t)value>100){U(self+28u)=100;return;}
    U(self+28u)=value;
}

/* Whole 4E8E80, low argument byte, signed byte result and signed clamps. */
void restore_god_city_004e8e80(int32_t amount)
{
    uint32_t record;
    for(record=0x799ba2u;record<0x89b79au;record+=264u){
        if(B(record-194u)==1u&&B(record-189u)){
            uint8_t value=(uint8_t)(B(record)+(uint8_t)amount);
            B(record)=value;
            if((int8_t)value<0)B(record)=0;
            if((int8_t)B(record)>100)B(record)=100;
        }
    }
}

/* Whole 4B4710; actor and building strides are 388 and 264 bytes. */
void restore_god_city_004b4710(void)
{
    uint32_t record;
    for(record=0xbcd7d6u;record<0xc8ad92u;record+=388u){
        if(B(record-76u)==1u){
            uint8_t kind=B(record-80u);
            if(kind==25u||kind==77u||kind==78u){
                int32_t building=S16(record);
                W(record)=0;
                B(record-80u)=67;
                W(record-36u)=0;
                W(0x799a8cu+(uint32_t)building*264u)=0;
            }
        }
    }
}

void restore_god_city_004e6eb0(void)
{
    U(0x78d79cu)=0xffffffffu;
    restore_god_city_004e8e80(-15);
}

static void ambient(const char *filename)
{
    if(FUN_0055f5a1(RESTORE_SOUND,0))FUN_0055f561(RESTORE_SOUND,0);
    U(0xf55b94u)=1;
    if(FUN_0055f5a1(RESTORE_SOUND,2))FUN_0055f561(RESTORE_SOUND,2);
    (void)FUN_0055f449(RESTORE_SOUND,filename,2,1);
    restore_god_city_004e8e80(-15);
}

/* Complete 142-instruction hail effect. RNG state remains in locals across
   inactive actors, and is reloaded after the two real classification calls. */
void restore_god_city_004e6ed0(void)
{
    uint32_t previous=U(0xd3b374u),a=U(0xd3b384u),b=U(0xd3b380u),record;
    for(record=0xbcd786u;record<0xc8ad42u;record+=388u){
        if(B(record+4u)){
            uint32_t index=U(0xd3b1c4u),percent,limit;
            uint8_t state;
            U(0xd3b1c8u+index*4u)=previous;
            ++index;U(0xd3b1c4u)=index;
            if((int32_t)index>=100)U(0xd3b1c4u)=0;
            mix31(&a,&b);
            U(0xd3b378u)=a&7u;
            U(0xd3b370u)=a&0x7fffu;
            U(0xd3b364u)=b&0x7fffu;
            U(0xd3b368u)=b&0x7fu;
            U(0xd3b36cu)=b&7u;
            previous=a&0x7fu;percent=(a&0x7fffu)%100u;
            state=B(record+46u);
            U(0xd3b380u)=b;U(0xd3b384u)=a;U(0xd3b374u)=previous;
            if(state==8u)limit=25;
            else{
                uint8_t kind=B(record);
                if(kind==11u||kind==12u||kind==13u||(kind>=43u&&kind<=57u)||kind==99u)limit=50;
                else{
                    if((uint8_t)FUN_004ea9c0(kind))limit=50;
                    else limit=(uint8_t)FUN_004eada0(B(record))?50u:75u;
                    previous=U(0xd3b374u);a=U(0xd3b384u);b=U(0xd3b380u);
                }
            }
            if(percent<=limit)B(record+46u)=4;
        }
    }
    ambient((const char *)0x5c92f8u);
    U(0x78d7a0u)=408;
}

/* Complete CALL-free 4583F0: snapshot DWORD building count, WORD output. */
static void housing_list(void)
{
    uint32_t count=U(0x78d790u),id=1,record=0x799ae5u,used=0,address=0x78e178u;
    U(0x78d83cu)=0;
    if((int32_t)count<1)return;
    do{
        if(B(record-5u)==1u&&B(record)){
            W(address)=(uint16_t)id;++used;address+=2u;
        }
        ++id;record+=264u;
    }while((int32_t)id<=(int32_t)count);
    U(0x78d83cu)=used;
}

/* Complete 107-instruction frog effect. The 65-percent multiplication wraps
   in signed32 before division, exactly as the native SHL/ADD + magic IMUL. */
void restore_god_city_004e70b0(int32_t duration)
{
    uint32_t count,address,remaining,total=0,damaged=0,index;
    int32_t level=0,budget;
    U(0x78d7a8u)=(uint32_t)duration*816u;
    housing_list();
    count=U(0x78d83cu);U(0x78d840u)=0;
    if((int32_t)count>0){
        U(0x78d840u)=count;remaining=count;address=0x78e178u;
        do{
            uint32_t building=(uint32_t)S16(address),record=building*264u;
            int32_t kind;
            address+=2u;kind=S16(0x7999e8u+record);
            if(kind>level)level=kind;
            total+=(uint32_t)S16(0x7999f4u+record);
        }while(--remaining);
    }
    if(level>=10){
        budget=(int32_t)(total*65u)/100;
        do{
            if((int32_t)damaged>=budget)break;
            count=U(0x78d83cu);index=0;U(0x78d840u)=0;
            if((int32_t)count>0){
                do{
                    int32_t building=S16(0x78e178u+index*2u);
                    uint32_t record=(uint32_t)building*264u;
                    ++index;U(0x78d840u)=index;
                    if(S16(0x7999e8u+record)==level){
                        damaged+=(uint32_t)S16(0x7999f4u+record);
                        if((int32_t)damaged<=budget){
                            restore_god_effect_004e7300(building);
                            index=U(0x78d840u);
                        }
                    }
                }while((int32_t)index<(int32_t)U(0x78d83cu));
            }
            --level;
        }while(level>=10);
    }
    ambient((const char *)0x5e4940u);
}

/* Complete 159-instruction delayed blessing. Every post-call player reload
   and DWORD counter/WORD-vs-BYTE field width follows the original body. */
void restore_god_city_004ea000(void)
{
    uint32_t offset=player_offset(),value,type,god,address;
    value=U(0xea5d98u+offset);if(value)U(0xea5d98u+offset)=value-1u;
    value=U(0xea5d9cu+offset);if(value)U(0xea5d9cu+offset)=value-1u;
    if((int32_t)U(0xea5d68u+offset)<=0)return;
    value=U(0xea5d6cu+offset)-1u;U(0xea5d6cu+offset)=value;
    if((int32_t)value>0)return;
    if((int32_t)U(0xea5d98u+offset)<=0){
        type=U(0xea5d68u+offset);
        if(type==1u||type==2u||type==3u){
            restore_god_city_004e8e80(type==1u?6:type==2u?10:15);
            offset=player_offset();god=U(0xea5d70u+offset);
            address=0xea5c0au+offset+god;
            B(address)=(uint8_t)(B(address)+(uint8_t)type);
        }else if(type==4u){
            uint32_t i,player;
            restore_god_city_004e8e80(15);
            player=U(0xeaac28u);
            for(i=0;i<5u;++i)if(i!=4u){
                address=0xea5c0au+player*0x49d8u+i;
                B(address)=(uint8_t)(B(address)+3u);
            }
        }
    }else if((int32_t)U(0xea5d9cu+offset)<=0){
        type=U(0xea5d68u+offset);
        if(type==1u||type==2u||type==3u){
            god=U(0xea5d70u+offset);address=0xea5c0au+offset+god;
            B(address)=(uint8_t)(B(address)+(type==3u?5u:type));
            restore_god_city_004e8e80(type==1u?2:type==2u?3:5);
        }
    }
    value=U(0xeaac28u);U(0xe950dcu)=1;
    offset=value*0x49d8u;
    U(0xea5bfcu+offset)=1;
    god=U(0xea5d70u+offset);
    U(0xea5c50u+(value*0x1276u+god)*4u)=0;
    type=U(0xea5d68u+offset);
    if(type>=1u&&type<=3u)FUN_004e0cf0((int32_t)(0x88u+type),0,0,0,0);
    type=U(0xea5d68u+player_offset());
    restore_god_city_00461950((int32_t)(type*10u));
    offset=player_offset();U(0xea5d68u+offset)=0;U(0xea5d6cu+offset)=0;
}
