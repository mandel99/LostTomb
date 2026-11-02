#include "tax_collector.h"
#include "actor_callbacks_human.h"
#include "actor_work.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4599A0: one BYTE per active record, including the high-water record.
   The entry count is cached; neighboring service bytes remain untouched. */
void FUN_004599a0(void)
{
    uint32_t count=U(0x78d790u),p=0x799b2eu;
    if(I(count)<1)return;
    do{
        if(B(p-78u)==1u&&B(p))--B(p);
        p+=264u;
    }while(--count);
}

/* Whole484A90: scan the native inclusive radius-two rectangle. Each occupied
   map cell counts, including repeated references to the same house. The two
   published coordinates are loop endpoints, not a selected building. */
int32_t FUN_00484a90(int32_t x,int32_t y)
{
    uint32_t left=(uint32_t)x-2u,top=(uint32_t)y-2u;
    uint32_t right=(uint32_t)x+2u,bottom=(uint32_t)y+2u;
    uint32_t width,height,tile,gap,rows,cells,scan,count=0,off,metric;
    int32_t building,level;
    U(0x9ab9e8u)=0;
    if(I(left)<0)left=0;
    if(I(top)<0)top=0;
    width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
    height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
    U(0xd3b4e0u)=top;
    tile=U(0xbca2d8u)+top*228u+left;gap=227u+left-right;
    U(0xbc9768u)=tile;
    if(I(top)>I(bottom))return 0;
    rows=bottom-top+1u;U(0xd3b4e0u)=bottom+1u;
    do{
        U(0xd3b4e4u)=left;
        if(I(left)<=I(right)){
            cells=right-left+1u;scan=tile;tile+=cells;
            U(0xd3b4e4u)=right+1u;
            do{
                building=H(0xb4e6fcu+scan*2u);
                if(building>0){
                    off=(uint32_t)building*264u;
                    if(B(0x7999ddu+off)&&H(0x7999f4u+off)>0){
                        level=H(0x7999eau+off);
                        metric=U(0x70ce18u+(uint32_t)level*88u);
                        if(I(metric)>I(U(0x9ab9e8u)))U(0x9ab9e8u)=metric;
                        B(0x799a26u+off)=50u;++count;
                    }
                }
                ++scan;
            }while(--cells);
        }
        tile+=gap;U(0xbc9768u)=tile;
    }while(--rows);
    return I(count);
}

static uint32_t actor_offset(void){return U(0xd056f8u)*388u;}
static uint32_t home_road(uint32_t home)
{
    return FUN_0047fe60(H(0x7999deu+home),H(0x7999e0u+home),B(0x7999dbu+home),2);
}
static void begin_trip(uint32_t actor,uint32_t off)
{
    FUN_004b4220(I(actor),H(0xbcd642u+off),H(0xbcd644u+off),
        H(0xbcd61cu+off)*15,H(0xbcd61eu+off)*15,0);
    W(0xbcd63cu+actor_offset())=0;
}

/* Whole48C2B0. Home and movement profile belong to the entry actor, while
   native continuations reload the global current actor after every callback.
   The signed direction adjustment adds eight once; it is not a modulo. */
void FUN_0048c2b0(void)
{
    uint32_t actor=U(0xd056f8u),off=actor*388u,home,route,direction,found;
    int32_t home_id;
    uint8_t action;
    B(0xbcd66au+off)=1;B(0xbcd604u+off)=0;W(0xbcd63au+off)=512;
    home_id=H(0xbcd652u+off);home=(uint32_t)home_id*264u;
    route=U(0x7090ecu+(uint32_t)B(0xbcd602u+off)*56u);
    if(B(0x7999d8u+home)!=1u||H(0x799a02u+home)!=I(actor))B(0xbcd606u+off)=2;
    ++B(0xbcd5f9u+off);if(B(0xbcd5f9u+off)>=12u)B(0xbcd5f9u+off)=0;
    action=B(0xbcd630u+off);
    switch(action){
    case 5:FUN_004b17d0(I(actor));break;
    case 4:FUN_004b1d50(I(actor));break;
    case 8:
        B(0xbcd65du+off)=1;B(0xbcd5f9u+off)=0;--W(0xbcd62eu+off);
        if(H(0xbcd62eu+off)>0)break;
        found=home_road(home);actor=U(0xd056f8u);off=actor*388u;
        if(!found){B(0xbcd606u+off)=2;break;}
        B(0xbcd630u+off)=9;W(0xbcd61cu+off)=W(0xd3b4e4u);W(0xbcd61eu+off)=W(0xd3b4e0u);
        begin_trip(actor,off);break;
    case 9:
        B(0xbcd604u+off)=1;B(0xbcd65du+off)=1;
        if(FUN_004b3ff0(I(actor),1)!=1)break;
        actor=U(0xd056f8u);off=actor*388u;
        if(H(0xb4e6fcu+U(0xbcd618u+off)*2u)==home_id){B(0xbcd606u+off)=2;break;}
        B(0xbcd630u+off)=10;FUN_004b1df0(I(actor));W(0xbcd63cu+actor_offset())=0;break;
    case 10:
        B(0xbcd65du+off)=0;++W(0xbcd63cu+off);
        if(H(0xbcd63cu+off)>=H(0xbcd63au+off)){
            found=home_road(home);actor=U(0xd056f8u);off=actor*388u;
            if(!found)B(0xbcd606u+off)=2;
            else{
                B(0xbcd630u+off)=11;
                W(0xbcd61cu+off)=W(0xd3b4e4u);W(0xbcd61eu+off)=W(0xd3b4e0u);
            }
        }
        FUN_004b1ef0(I(actor),1);break;
    case 11:
        FUN_004a5da0(I(actor),route);actor=U(0xd056f8u);off=actor*388u;
        direction=B(0xbcd609u+off);
        if(direction==9u||direction==10u)B(0xbcd606u+off)=2;
        else if(direction==8u){
            B(0xbcd630u+off)=9;
            W(0xbcd61cu+off)=W(0x7999deu+home);W(0xbcd61eu+off)=W(0x7999e0u+home);
            begin_trip(actor,off);
        }
        break;
    default:B(0xbcd630u+off)=8;break;
    }
    off=actor_offset();direction=(uint32_t)(int32_t)(int8_t)B(0xbcd609u+off)-U(0xbca2e0u);
    U(0xe94e00u)=direction;if(I(direction)<0){direction+=8u;U(0xe94e00u)=direction;}
    if(B(0xbcd630u+off)==4u)
        U(0xbcd5fcu+off)=U(0xf55e3cu)+B(0x5d048cu+(uint32_t)(H(0xbcd62eu+off)/2));
    else U(0xbcd5fcu+off)=U(0xf55e34u)+(uint32_t)B(0xbcd5f9u+off)*8u+direction;
}
