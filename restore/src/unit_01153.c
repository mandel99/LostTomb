#include "map_refresh_postpass4.h"
void FUN_0047eb60(int32_t direction,uint32_t *count)
{
    static const int32_t offsets[4][5]={{-228,-1,1,-229,-227},{1,-228,228,-227,229},{228,-1,1,227,229},{-1,-228,228,-229,227}};
    const int32_t *row=offsets[direction/2];
    uint32_t tile=P4_R(0xbc9768u,4),current=P4_R(p4_terrain(tile,0),4),neighbor=P4_R(p4_terrain(tile,row[0]),4),connected=0;
    if(!(neighbor&0x100u))return;
    if(current&0x40u){
        if(neighbor&0x8000040u)return;
        if(P4_R(p4_terrain(tile,row[1]),4)&0x8000040u)connected=1;
        else if(P4_R(p4_terrain(tile,row[2]),4)&0x8000040u)connected=1;
    }else{
        if(!(neighbor&0x8000040u))connected=1;
        else if(P4_R(p4_terrain(tile,row[3]),4)&0x8000040u)connected=1;
        else if(P4_R(p4_terrain(tile,row[4]),4)&0x8000040u)connected=1;
    }
    if(connected){P4_W(0xbc97c0u+(uint32_t)direction*4u,4,1);*count=*count+1u;}
}
