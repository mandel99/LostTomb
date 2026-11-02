#include "road_variant.h"
#include "map_neighbors_phase.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static int32_t variant(uint32_t cell,uint32_t table,uint32_t modulus)
{
    uint32_t mode=U(0xbca2e0u),v=B(0xacf85cu+cell),base;
    base=U(table+(uint32_t)((int32_t)mode/2)*4u);
    return (int32_t)(base+v%modulus);
}
int32_t FUN_00475fd0(uint32_t cell)
{
    static const uint32_t delta[8]={0xffffff1c,0xffffff1d,1,229,228,227,0xffffffff,0xffffff1b};
    uint32_t snapshot[8],i,current,index,record,root,count=0,v,tmp;
    volatile uint32_t s3,s7,s1,s6,s2,s0,s4;
    int32_t type,link;
    for(i=0;i<16u;++i)U(0xbc97c0u+i*4u)=0;
    U(0xbc9768u)=cell;FUN_0047dbc0(0x20000002u);
    for(i=0;i<8u;++i)snapshot[i]=U(0xbc97c0u+i*4u);
    FUN_0047dbc0(0x80000u);
    for(i=0;i<8u;++i){v=U(0xbc97c0u+i*4u);U(0xbc97c0u+i*4u)=v?0u:snapshot[i];}
    current=U(0xbc9768u);
    for(i=0;i<8u;++i){
        index=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+(current+delta[i])*2u);
        if((int32_t)index<=0)continue;
        record=0x7999e8u+index*264u;type=(int16_t)W(record);
        if(type==230){U(0xbc97c0u+i*4u)=1;continue;}
        if(type!=229&&type!=234&&type!=235&&type!=236)continue;
        if(U(record+88u)==18u)continue;
        link=(int16_t)W(record+2u);root=0x7999e8u+(index-(uint32_t)link)*264u;
        if(U(root+72u)==1u)U(0xbc97c0u+i*4u)=1;
    }
    for(i=0;i<8u;++i)if(U(0xbc97c0u+i*4u)==1u)++count;
    if(count==8u)return (int32_t)(B(0xacf85cu+cell)%24u+48u);
    /* Native preloads all three, including values unused by some branches. */
    s3=U(0xbc97ccu);s7=U(0xbc97dcu);s1=U(0xbc97c4u);
    if(count==7u){
        if(!s1)return variant(cell,0x5dbcc8u,3);
        if(!s7)return variant(cell,0x5dbcd8u,3);
        tmp=U(0xbc97d4u);if(!tmp)return variant(cell,0x5dbce8u,3);
        if(!s3)return variant(cell,0x5dbcf8u,3);
    }else if(count==6u){
        if(!s7&&!s3)return variant(cell,0x5dbd88u,3);
        if(!s1){tmp=U(0xbc97d4u);if(!tmp)return variant(cell,0x5dbd98u,3);}
    }
    s6=U(0xbc97d8u);s2=U(0xbc97c8u);s0=U(0xbc97c0u);
    if(s6&&s7&&s0&&s1&&s2&&count<=7u)return variant(cell,0x5dbd08u,6);
    s4=U(0xbc97d0u);
    if(s4){tmp=U(0xbc97d4u);if(tmp&&s6){tmp=U(0xbc97dcu);if(tmp&&s0&&count<=7u)return variant(cell,0x5dbd18u,6);}}
    if(s2){tmp=U(0xbc97ccu);if(tmp&&s4){tmp=U(0xbc97d4u);if(tmp&&s6&&count<=7u)return variant(cell,0x5dbd28u,6);}}
    if(s0&&s1){
        if(s2){tmp=U(0xbc97ccu);if(tmp&&s4&&count<=7u)return variant(cell,0x5dbd38u,6);}
        if(s1&&s2)return variant(cell,0x5dbd48u,3);
    }
    if(s6){tmp=U(0xbc97dcu);if(tmp&&s0)return variant(cell,0x5dbd58u,3);}
    if(s4){tmp=U(0xbc97d4u);if(tmp&&s6)return variant(cell,0x5dbd68u,3);}
    if(s2){tmp=U(0xbc97ccu);if(tmp&&s4)return variant(cell,0x5dbd78u,3);}
    return -1;
}
