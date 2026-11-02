#include "building_graph.h"
#include "terrain_redraw.h"
#include "ground.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_004f3d70(uint32_t cell);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
uint32_t FUN_004f3f70(int32_t building,uint32_t cell,int32_t state,uint32_t action)
{
    uint32_t table,base,variants=1,mask=255,bits=0,orientation,i,delta,value,image;
    int32_t relative,x,y,neighbor;
    (void)building;
    switch((uint32_t)state-3u){
    case 0:base=U(0xf5736cu);table=0xeaaf60u;mask=0;break;
    case 1:
        if((uint8_t)action){
            delta=U(0xbca2d8u);U(0xb7482cu+cell*4u)=0;relative=(int32_t)(cell-delta);x=relative%228;y=relative/228;
            FUN_00475bf0(x,y,x,y);FUN_004767d0(x,y,x,y,0,1);FUN_00477500(x,y,x,y);
        }
        return (uint8_t)action;
    case 2:base=U(0xf5733cu);table=0x5f6d70u;variants=8;break;
    case 3:base=U(0xf57344u);table=0x5f7170u;mask=85;break;
    case 4:base=U(0xf5734cu);table=0x5f7170u;mask=85;break;
    case 5:base=U(0xf57344u);table=0x5f7170u;mask=85;break;
    case 6:base=U(0xf57354u);table=0x5f7170u;mask=85;break;
    case 7:base=U(0xf5735cu);table=0x5f7170u;mask=85;break;
    default:return 0;
    }
    orientation=U(0xbca2e0u);
    for(i=0;i<8u;++i){
        delta=U(0x5f7570u+((orientation+i)&7u)*4u);neighbor=FUN_004f3d70(cell+delta);
        if(state==5?(neighbor>=5||neighbor==0):neighbor>=0)bits|=1u<<i;
    }
    bits&=mask;value=U(table+bits*4u);image=value+base;U(0xb7482cu+cell*4u)=image;
    if(bits==255u&&variants>1u){value=B(0xacf85cu+cell);U(0xb7482cu+cell*4u)=image+value%variants;}
    return 1;
}
