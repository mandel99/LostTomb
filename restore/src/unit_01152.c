#include "map_refresh_postpass4.h"
void FUN_0047e870(void)
{
    uint32_t tile=P4_R(0xbc9768u,4),current=P4_R(p4_terrain(tile,0),4),i,north,east,south,record,a,b,road,count=0,connected;
    for(i=8;i!=0;--i)P4_W(0xbc97c0u+(i-1u)*4u,4,0);
    if(current&0x10000u){
        if(P4_R(p4_terrain(tile,-228),1)&4u)P4_W(0xbc97c0u,4,1);
        if(P4_R(p4_terrain(tile,1),1)&4u)P4_W(0xbc97c8u,4,1);
        if(P4_R(p4_terrain(tile,228),1)&4u)P4_W(0xbc97d0u,4,1);
        if(P4_R(p4_terrain(tile,-1),1)&4u)P4_W(0xbc97d8u,4,1);
    }
    north=P4_R(p4_terrain(tile,-228),4);road=current&0x40u;
    if(north&0x100u){
        connected=0;
        if(road){if(!(north&0x8000040u)){if(P4_R(p4_terrain(tile,-1),4)&0x8000040u)connected=1;else if(P4_R(p4_terrain(tile,1),4)&0x8000040u)connected=1;}}
        else if(!(north&0x8000040u))connected=1;
        else if(P4_R(p4_terrain(tile,-229),4)&0x8000040u)connected=1;
        else if(P4_R(p4_terrain(tile,-227),4)&0x8000040u)connected=1;
        if(connected){P4_W(0xbc97c0u,4,1);count=1;}
    }
    east=P4_R(p4_terrain(tile,1),4);
    if(east&0x100u){
        connected=0;
        if(road){if(!(east&0x8000040u)){if(north&0x8000040u)connected=1;else if(P4_R(p4_terrain(tile,228),4)&0x8000040u)connected=1;}}
        else if(!(east&0x8000040u))connected=1;
        else if(P4_R(p4_terrain(tile,-227),4)&0x8000040u)connected=1;
        else if(P4_R(p4_terrain(tile,229),4)&0x8000040u)connected=1;
        if(connected){P4_W(0xbc97c8u,4,1);++count;}
    }
    south=P4_R(p4_terrain(tile,228),4);
    if(south&0x100u){
        connected=0;
        if(road){if(!(south&0x8000040u)){if(P4_R(p4_terrain(tile,-1),4)&0x8000040u)connected=1;else if(east&0x8000040u)connected=1;}}
        else if(!(south&0x8000040u))connected=1;
        else if(P4_R(p4_terrain(tile,227),4)&0x8000040u)connected=1;
        else if(P4_R(p4_terrain(tile,229),4)&0x8000040u)connected=1;
        if(connected){P4_W(0xbc97d0u,4,1);++count;}
    }
    FUN_0047eb60(6,&count);
    tile=P4_R(0xbc9768u,4);
    a=P4_R(p4_terrain(tile,-228),4);
    if(a&8u){record=p4_record(tile,-228);if(P4_R(record,2)==0x5au){a=P4_R(p4_terrain(tile,228),4);b=P4_R(p4_terrain(tile,0),4);if(!((a&b)&0x40u)&&!(P4_R(record+154u,1)&1u))P4_W(0xbc97c0u,4,1);}}
    east=P4_R(p4_terrain(tile,1),4);
    if(east&8u){record=p4_record(tile,1);if(P4_R(record,2)==0x5au){a=P4_R(p4_terrain(tile,-1),4);b=P4_R(p4_terrain(tile,0),4);if(!((a&b)&0x40u)&&(P4_R(record+154u,1)&1u))P4_W(0xbc97c8u,4,1);}}
    if(P4_R(p4_terrain(tile,228),1)&8u){record=p4_record(tile,228);if(P4_R(record,2)==0x5au){a=P4_R(p4_terrain(tile,0),4);b=P4_R(p4_terrain(tile,-228),4);if(!((a&b)&0x40u)&&!(P4_R(record+154u,1)&1u))P4_W(0xbc97d0u,4,1);}}
    if(P4_R(p4_terrain(tile,-1),1)&8u){record=p4_record(tile,-1);if(P4_R(record,2)==0x5au){a=P4_R(p4_terrain(tile,0),4);if(!((east&a)&0x40u)&&(P4_R(record+154u,1)&1u))P4_W(0xbc97d8u,4,1);}}
}
