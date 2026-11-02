#include "actor_work.h"
#include "god_effects.h"
#include "actor_create.h"
#include "actor_predicate.h"
#include "command_migration.h"
#include "displaced_population.h"
#include "event_publisher.h"
#include "menu_prepare_integer.h"
#include "menu_resource_neighbors.h"
#include "population_updates.h"
#include "disease_updates.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

/* Whole 4E9400, 36 instructions. Its ring cursor is local, not stored back.
   Both cohort operations are WORD writes; all player arithmetic is DWORD. */
void FUN_004e9400(int32_t player,int32_t amount)
{
    uint32_t cursor=U(0xd3b1c4u),toggle=0,base=(uint32_t)player*0x49d8u,people=(uint32_t)amount;
    if(!people)return;
    do{
        uint32_t age=U(0xd3b1c8u+cursor*4u)&63u,address;
        if((int32_t)age>50)age-=30u;
        else if((int32_t)age<10&&toggle)age+=20u;
        address=0xea3e34u+base+age*2u;
        W(address)=(uint16_t)(W(address)+1u);
        ++cursor;if((int32_t)cursor>=100)cursor=0;
        --people;toggle=toggle!=1u;
    }while(people);
}

/* Whole 4E90F0, 37 instructions, composed with real canonical 4E9350. */
void FUN_004e90f0(int32_t player,int32_t amount)
{
    uint32_t base=(uint32_t)player*0x49d8u,i,total=0;
    U(0xea3f6cu+base)=(uint32_t)amount;
    if(amount<0)FUN_004e9350(player,(int32_t)(0u-(uint32_t)amount));
    else if(amount>0)FUN_004e9400(player,amount);
    U(0xea1894u+base)=0;
    for(i=0;i<100u;++i){
        int32_t people=S16(0xea3e34u+base+i*2u);
        total=U(0xea1894u+base)+(uint32_t)people;
        U(0xea1894u+base)=total;
    }
    if((int32_t)total>(int32_t)U(0xea5f0cu+base))U(0xea5f0cu+base)=total;
}

/* Reuse the complete native patrol body through its shared C entry. */
void restore_god_actor_patrol_004b1df0(int32_t actor)
{FUN_004b1df0(actor);}

/* Whole 42FCA0, 29 instructions. Count and maximum are signed DWORDs. */
int32_t FUN_0042fca0(void)
{
    uint32_t count=U(0x78d790u),id=1,best=0,record=0x799afcu;
    int32_t score=0;
    if((int32_t)count<1)return 0;
    do{
        if(B(record-28u)&&W(record)&&B(record-23u)){
            int32_t value=(int32_t)U(record+224u);
            if(value>score){score=value;best=id;}
        }
        ++id;record+=264u;
    }while((int32_t)id<=(int32_t)count);
    return (int32_t)best;
}

/* Whole 42FCF0, 119 instructions. Native defines AL only. Its two full RNG
   states are committed after the complete reservoir loop, not each record. */
uint8_t FUN_0042fcf0(uint32_t random_choice)
{
    uint32_t selected=(uint32_t)FUN_0042fca0();
    if((uint8_t)random_choice&&selected==0u&&(int32_t)U(0x78d790u)>=1){
        uint32_t index=U(0xd3b1c4u),previous=U(0xd3b374u);
        uint32_t low=U(0xd3b380u),high=U(0xd3b384u),record=0x799afcu,id=1,seen=0;
        do{
            if(B(record-28u)&&W(record)&&B(record-23u)){
                uint32_t i;
                ++seen;U(0xd3b1c8u+index*4u)=previous;
                ++index;U(0xd3b1c4u)=index;
                if((int32_t)index>=100)U(0xd3b1c4u)=0;
                for(i=0;i<31u;++i){
                    uint32_t a=((high>>4)^high)&1u,b=((low>>4)^low)&1u;
                    high>>=1;low>>=1;
                    if(a)high|=0x40000000u;if(b)low|=0x40000000u;
                }
                U(0xd3b378u)=high&7u;U(0xd3b370u)=high&0x7fffu;
                U(0xd3b364u)=low&0x7fffu;U(0xd3b368u)=low&0x7fu;
                previous=high&0x7fu;
                U(0xd3b36cu)=low&7u;U(0xd3b374u)=previous;
                if((int32_t)(high&0x7fffu)%(int32_t)seen==0)selected=id;
            }
            record+=264u;++id;
            if((int32_t)id>(int32_t)U(0x78d790u))break;
            index=U(0xd3b1c4u);
        }while(1);
        U(0xd3b384u)=high;U(0xd3b380u)=low;
    }
    if(!FUN_0042fe90((int32_t)selected))return 0;
    U(0x70e12cu)=selected;U(0x70e138u)=1;
    return 1;
}

static void worker_counter(uint32_t actor,uint32_t counter,uint32_t image,uint32_t limit)
{
    uint32_t value=U(counter);
    W(0xbcd668u+actor)=(uint16_t)value;
    ++value;
    U(0xbcd690u+actor)=image;
    U(counter)=value;
    if((int32_t)value>=(int32_t)limit)U(counter)=0;
}

/* Whole 461B20, 224 instructions and all three native switch tables. */
static void spawn_worker(int32_t building)
{
    uint32_t record=(uint32_t)building*264u,actor,skin;
    int32_t x,y,kind;
    uint8_t orientation,player;
    y=S16(0x7999e0u+record);orientation=B(0x7999dbu+record);x=S16(0x7999deu+record);
    if(!FUN_00480180(x,y,orientation))return;
    y=(int32_t)U(0xd3b4e0u);x=(int32_t)U(0xd3b4e4u);player=B(0x7999d9u+record);
    actor=(uint32_t)FUN_004b4800(player,91,x,y,0);
    W(0x799a02u+record)=(uint16_t)actor;
    kind=S16(0x7999e8u+record);actor*=388u;
    B(0xbcd630u+actor)=8;
    W(0xbcd652u+actor)=(uint16_t)building;
    W(0xbcd62eu+actor)=0;
    switch(kind){
    case 30:case 34:skin=16;break;
    case 31:case 32:case 36:case 226:skin=15;break;
    case 33:case 35:skin=17;break;
    case 51:case 53:skin=29;break;
    case 60:case 61:case 62:case 63:case 64:case 65:case 66:case 67:
    case 68:case 69:case 151:case 156:skin=27;break;
    default:skin=40;break;
    }
    B(0xbcd603u+actor)=(uint8_t)skin;
    switch(skin){
    case 5:worker_counter(actor,0xd056e0u,0xfe,128);return;
    case 15:worker_counter(actor,0xd056bcu,0x107,32);return;
    case 16:worker_counter(actor,0xd056b8u,0x108,32);return;
    case 17:worker_counter(actor,0xd056b4u,0x109,16);return;
    case 27:{
        uint32_t category=FUN_004c5120(S16(0xbcd652u+actor));
        if(category>4u)return;
        worker_counter(actor,0xd056b0u-category*4u,0x10au+category,16);return;
    }
    case 29:case 30:worker_counter(actor,0xd0569cu,0x10f,32);return;
    case 40:worker_counter(actor,0xd056d4u,0x101,32);return;
    default:W(0xbcd668u+actor)=0xffffu;U(0xbcd690u+actor)=0xffffffffu;return;
    }
}

/* Whole 461950, 146 instructions. Its eight adjacent native local DWORDs
   are represented by two real four-element arrays, without out-of-bounds C. */
void restore_god_city_00461950(int32_t amount)
{
    uint32_t targets[4]={0},quota[4]={0},group,id=0,record=0x7999e8u;
    for(group=0;group<4u;++group){
        uint32_t list=0x5da554u+group*48u,kind;
        int32_t share=(int32_t)(U(0x5da544u+group*4u)*(uint32_t)amount)/100;
        if(group==0u)targets[group]=(uint32_t)share;
        else targets[group]+=(uint32_t)share;
        quota[group]=0;
        while((kind=U(list))!=0u){quota[group]+=U(0x70573cu+kind*4u);list+=4u;}
        if(group<3u)targets[group+1u]=0;
        if((int32_t)quota[group]>(int32_t)targets[group])quota[group]=targets[group];
        else if((int32_t)quota[group]<(int32_t)targets[group]&&group<3u)
            targets[group+1u]=targets[group]-quota[group];
    }
    if((int32_t)U(0x78d790u)<0)return;
    do{
        if(B(record-16u)==1u){
            uint32_t owner=FUN_00475b50(id)*264u;
            if((W(0x799a18u+owner)!=0||B(0x7999ddu+owner)!=0)&&
               (B(0x7999ddu+owner)==0||W(0x7999f4u+owner)!=0)){
                int done=1;
                for(group=0;group<4u;++group){
                    if(quota[group]){
                        uint32_t list=0x5da554u+group*48u,kind;
                        done=0;
                        while((kind=U(list))!=0u){
                            if((uint32_t)S16(record)==kind){--quota[group];spawn_worker((int32_t)id);}
                            if(!quota[group])break;
                            list+=4u;
                        }
                    }
                }
                if(done)return;
            }
        }
        ++id;record+=264u;
    }while((int32_t)id<=(int32_t)U(0x78d790u));
}
