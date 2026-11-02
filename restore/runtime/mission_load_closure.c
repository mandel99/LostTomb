#include "mission_load_closure.h"
#include "actor_predicate.h"
#include "actor_spawn.h"
#include "camera_bounds.h"
#include "city_prepare.h"
#include "ground.h"
#include "growth_update.h"
#include "lookup_sentinel.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_commit_fields.h"
#include "map_file_load.h"
#include "map_record_refresh.h"
#include "mission_loaders.h"
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

extern void FUN_0040f8f0(void *,uint32_t,uint8_t);
extern uint32_t FUN_0051aff0(int32_t);
extern int32_t FUN_004cde40(int32_t,int32_t);
extern void FUN_00478420(void);
extern void FUN_004877f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00444a80(void);
extern void FUN_004cc560(int32_t,uint32_t,uint32_t,uint32_t,uint8_t,uint32_t);
extern void FUN_004bb090(void);
extern void FUN_004bbc10(void);
extern void FUN_004bb3f0(void);
extern void FUN_004d5980(void);
extern void FUN_004dea50(void);
extern void FUN_0046d480(void);
extern void FUN_00517a20(void);

/* Complete 221-instruction 5190A0, composed with the real 51AFF0 query.
   As in the native code, the caller supplies valid mapped cell/list storage.
   Target discovery on the west edge precedes its distance write; the other
   three directions write the distance before returning the target index. */
uint32_t restore_mission_reach_005190a0(int32_t x,int32_t y,
                                     const uint32_t *targets,int32_t count)
{
    uint32_t remaining=(uint32_t)count,index=0xffffffffu;
    uint32_t cell,head,tail,found=0xffffffffu;
    FUN_0040f8f0((void *)0xeed760u,0x32c40u,0);
    if(count<=0)return 0;
    do{
        cell=*targets++;
        U(0xeed760u+cell*4u)=index--;
    }while(--remaining);
    cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    U(0xeae004u)=cell;
    U(0xeadfd4u)=U(0xeadfd4u)+1u;
    U(0xeed760u+cell*4u)=1;
    U(0xf203a0u)=U(0xeae004u);
    U(0xeae00cu)=0;
    U(0xeae008u)=1;
    head=0;tail=1;
    do{
        uint32_t current=U(0xf203a0u+head*4u);
        uint32_t distance=U(0xeed760u+current*4u)+1u;
        uint32_t direction;
        for(direction=0;direction<4u;++direction){
            int8_t terrain;
            uint32_t visited;
            switch(direction){
            case 0: cell=current-228u;break;
            case 1: cell=current+1u;break;
            case 2: cell=current+228u;break;
            default: cell=current-1u;break;
            }
            visited=U(0xeed760u+cell*4u);
            if((int32_t)visited>0)continue;
            if(FUN_0051aff0((int32_t)cell)==0){tail=U(0xeae008u);continue;}
            terrain=(int8_t)B(0xee0c50u+cell);
            if(terrain<0&&terrain!=-3&&
               !(terrain==-1&&(U(0xaf598cu+cell*4u)&0x1000u))){
                tail=U(0xeae008u);continue;
            }
            if(terrain>4||terrain==3){tail=U(0xeae008u);continue;}
            found=0xffffffffu-U(0xeed760u+cell*4u);
            if(direction!=3u)U(0xeed760u+cell*4u)=distance;
            if((int32_t)found>=0)return found+1u;
            if(direction==3u)U(0xeed760u+cell*4u)=distance;
            tail=U(0xeae008u);
            U(0xf203a0u+tail*4u)=cell;
            ++tail;
            U(0xeae008u)=tail;
            if((int32_t)tail>0xcb0f){tail=0;U(0xeae008u)=0;}
        }
        head=U(0xeae00cu)+1u;
        U(0xeae00cu)=head;
        if((int32_t)head>0xcb0f){head=0;U(0xeae00cu)=0;}
    }while(head!=tail);
    return (int32_t)found>=0?found+1u:0;
}

/* The two inline RNG sequences in 4860D0 have this exact store order.
   D3B374 is deliberately written by the caller after loading both bases. */
static uint32_t mission_random(void)
{
    uint32_t index=U(0xd3b1c4u),a,b,i;
    U(0xd3b1c8u+index*4u)=U(0xd3b374u);
    ++index;U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
    a=U(0xd3b384u);b=U(0xd3b380u);
    for(i=0;i<31u;++i){
        uint32_t high_feedback=((a>>4)^a)&1u;
        uint32_t low_feedback=((b>>4)^b)&1u;
        a>>=1;b>>=1;
        if(high_feedback)a|=0x40000000u;
        if(low_feedback)b|=0x40000000u;
    }
    U(0xd3b384u)=a;
    U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;
    U(0xd3b364u)=b&0x7fffu;
    U(0xd3b36cu)=b&7u;
    U(0xd3b370u)=a&0x7fffu;
    U(0xd3b368u)=b&0x7fu;
    return a;
}

static void mission_check_buildings(uint32_t front,uint32_t back)
{
    uint32_t id=1,record=0x799af0u;
    if((int32_t)U(0x78d790u)<1)return;
    do{
        if(B(record-16u)==1u&&W(record+0x30u)!=0){
            uint16_t kind=W(record);
            if(kind==0x6cu&&!B(0x978d98u)){
                int32_t y=S16(record+0x18u),x=S16(record+0x16u);
                if(restore_mission_reach_005190a0(x,y,(const uint32_t *)0x96e6b4u,
                                                 (int32_t)front))B(0x978d98u)=1;
            }else if(kind==0xc3u&&!B(0x978d9au)){
                int32_t y=S16(record+0x18u),x=S16(record+0x16u);
                if(restore_mission_reach_005190a0(x,y,
                    (const uint32_t *)(uintptr_t)(0x96e6b4u+back*4u),
                    (int32_t)(3999u-back)))B(0x978d9au)=1;
            }
        }
        ++id;record+=264u;
    }while((int32_t)id<=(int32_t)U(0x78d790u));
}

/* Complete 361-instruction mission terrain/resource setup. All wrap and
   124/264-byte record and DWORD-cell addressing remain explicitly 32-bit. */
void FUN_004860d0(void)
{
    uint32_t offset=U(0xeaac28u)*0x49d8u;
    uint32_t enabled=W(0xea5ce8u+offset),factor=100,front=0,back=3999,row=0;
    B(0x978d98u)=0;B(0x978d99u)=0;B(0x978d9au)=0;B(0x978d9bu)=0;
    if(enabled&&FUN_004c5120((int32_t)U(0xea5f9cu+offset))==0){
        offset=U(0xeaac28u)*0x49d8u;
        if(FUN_004c52d0((int32_t)U(0xea5f9cu+offset),2u))factor=125;
    }
    if((int32_t)U(0xbca2d4u)<=0)return;
    do{
        uint32_t right=U(0xbc9f24u+row*4u);
        uint32_t left=U(0xbc9b94u+row*4u);
        uint32_t cell=U(0xbc9804u+row*4u);
        if((int32_t)right<(int32_t)left){
            uint32_t flags_address=0xaf598cu+cell*4u,remaining=left-right;
            do{
                uint32_t flags=U(flags_address);
                if(flags&1u){
                    if(B(0xa43eacu+cell)!=255u){
                        uint32_t a=mission_random();
                        uint32_t base1=U(0x978d8cu),base2=U(0x978d94u);
                        int32_t delta;
                        U(0xd3b374u)=a&0x7fu;
                        delta=FUN_004cde40((int32_t)((a&0x7fffu)%10u+base1+base2),
                                          (int32_t)factor);
                        B(0xa43eacu+cell)=(uint8_t)(B(0xa43eacu+cell)+(uint8_t)delta);
                        /* Native CMP AL,FF / JBE always takes the no-clamp edge. */
                        FUN_00486600(cell);
                    }else if(!B(0x978d98u)){
                        U(0x96e6b4u+front*4u)=cell;++front;
                    }
                    B(0x978d99u)=1;
                }else if(flags&0x40000u){
                    if(B(0xa43eacu+cell)!=255u){
                        uint32_t a=mission_random();
                        uint32_t base1=U(0x978d90u),base2=U(0x978d88u);
                        int32_t delta;
                        U(0xd3b374u)=a&0x7fu;
                        delta=FUN_004cde40((int32_t)((a&0x7fffu)%10u+base1+base2),
                                          (int32_t)factor);
                        B(0xa43eacu+cell)=(uint8_t)(B(0xa43eacu+cell)+(uint8_t)delta);
                        FUN_00486600(cell);
                    }else if(!B(0x978d9au)){
                        U(0x96e6b4u+back*4u)=cell;--back;
                    }
                    B(0x978d9bu)=1;
                }
                if(front==back){
                    mission_check_buildings(front,back);
                    front=0;back=3999;
                }
                ++cell;flags_address+=4u;
            }while(--remaining);
        }
        ++row;
    }while((int32_t)row<(int32_t)U(0xbca2d4u));
    if(front!=0u||(int32_t)back<3999)mission_check_buildings(front,back);
}

/* Complete 25-instruction update; dimensions are reloaded after each call. */
void FUN_00486e70(void)
{
    uint32_t right,bottom;
    if(!B(0xbca30cu))return;
    FUN_00478420();
    bottom=U(0xbca2d4u)-1u;right=U(0xbca2d0u)-1u;
    FUN_00487450(0,0,(int32_t)right,(int32_t)bottom);
    bottom=U(0xbca2d4u)-1u;right=U(0xbca2d0u)-1u;
    FUN_004877f0(0,0,(int32_t)right,(int32_t)bottom);
    B(0xbca30cu)=0;
}

/* Complete 33-instruction 98-byte-record update. The limit is a signed
   DWORD; the addition and the actual output stores are WORD operations. */
void FUN_00442660(void)
{
    uint32_t limit=U(0xf56a14u),record=0x77f748u,id=0;
    do{
        if(B(record+1u)){
            uint8_t kind=B(record);
            if(kind==1u){
                uint32_t value=FUN_004425c0(id);
                limit=U(0xf56a14u);
                W(record+12u)=(uint16_t)value;
            }else if(kind==0u){
                uint16_t value=(uint16_t)(W(record+14u)+W(0xf56a0cu));
                W(record+12u)=value;
                if((int32_t)(uint32_t)value>(int32_t)limit)W(record+12u)=(uint16_t)limit;
            }
        }
        record+=98u;++id;
    }while(record<0x7843d8u);
}

/* Complete 158-instruction map-to-city controller. Existing file, sprite,
   actor and redraw providers remain real calls, with no synthetic returns. */
void FUN_004de600(void)
{
    char path[100];
    uint32_t a,b,c,d,record;
    FUN_0046fa40();
    FUN_004ce3a0((char *)0xe37078u);
    FUN_004cbea0(0x576274u,(char *)0xe38aecu,3);
    FUN_004ce3c0((char *)0xe37078u);
    strcpy(path,(const char *)0x57626cu);
    strcat(path,(const char *)0xe37078u);
    FUN_004d5820(path,0);
    FUN_004ce3a0((char *)0xe37078u);
    FUN_00444a80();
    a=U(0x784904u);b=U(0x784908u);c=U(0x78490cu);
    U(0xbca2d0u)=a;d=U(0x784910u);
    U(0xbca2d4u)=b;U(0xbca2d8u)=c;U(0xbca2dcu)=d;
    FUN_0046f7b0();
    for(record=0x784be0u;record<0x784c00u;record+=2u){
        b=U(0xbca2ecu)+(uint32_t)S16(record);
        a=U(0xbca2e8u)+(uint32_t)S16(record-32u);
        if((uint8_t)FUN_0046f930((int32_t)a,(int32_t)b)==0){
            W(record-32u)=0xffffu;W(record)=0xffffu;
        }
        b=U(0xbca2ecu)+(uint32_t)S16(record);
        a=U(0xbca2e8u)+(uint32_t)S16(record-32u);
        if((uint8_t)FUN_0046f930((int32_t)a,(int32_t)b)==0){
            W(record-32u)=0xffffu;W(record)=0xffffu;
        }
    }
    FUN_004cc560(5,0x784914u,24,280,0,U(0xf56f84u));
    FUN_004cc560(6,0x784954u,500,400,0,U(0xf56f84u));
    /* Native pushes an additional 1 which the whole 430530 never reads. */
    FUN_00430530(U(0xe94e04u));
    if(U(0x605a3cu)==1u){
        FUN_004bb090();
        a=W(0x784c8cu);
        if(a!=0xffffu){
            b=W(0x784c8eu);
            if(b!=0xffffu&&W(0x784c90u)!=0xffffu&&W(0x784c92u)!=0xffffu)
                FUN_004bbd40((int16_t)a,(int16_t)b,1);
        }
    }
    FUN_004bbc10();FUN_004bb3f0();FUN_004d5980();FUN_004dea50();
    FUN_0046d480();FUN_004ddf90();FUN_004de080();FUN_00517a20();
    FUN_005178b0();FUN_00517370();FUN_00517660();FUN_005178e0();
}
