#include "drag_leaves.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static uint32_t cell_at(int32_t x,int32_t y,uint32_t save)
{
    uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    if(save)U(0xbc9768u)=cell;
    return cell;
}
static void set_flags(int32_t x,int32_t y,uint32_t test,uint32_t keep,uint32_t add)
{
    uint32_t cell=cell_at(x,y,1),flags=U(0xaf598cu+cell*4u);
    if(flags&test){U(0xbc9730u)=1;return;}
    U(0xbc972cu)=1;U(0xaf598cu+cell*4u)=(flags&keep)|add;
}
void FUN_00477960(int32_t x,int32_t y)
{
    uint32_t cell=cell_at(x,y,1),flags=U(0xaf598cu+cell*4u),v;
    if(!flags){U(0xbc9730u)=1;return;}
    B(0xae8e7cu+cell)=0;U(0xaf598cu+cell*4u)=flags&0x99caf680u;
    v=B(0xb285ccu+cell);W(0xb4e6fcu+cell*2u)=0;B(0xb285ccu+cell)=(uint8_t)(v&0xe0u);
    v=B(0xb67d1cu+cell);W(0xa9cc1cu+cell*2u)=0;B(0xadc36cu+cell)=0;
    B(0xb67d1cu+cell)=(uint8_t)((v&0xc0u)|0x40u);U(0xbc972cu)=1;
}
void FUN_004779f0(int32_t x,int32_t y){set_flags(x,y,1,0x99caf681u,1);}
void FUN_00477a40(int32_t x,int32_t y,uint32_t add)
{
    uint32_t cell=cell_at(x,y,1),flags=U(0xaf598cu+cell*4u);
    if((flags&0x20300002u)==add){U(0xbc9730u)=1;return;}
    U(0xbc972cu)=1;U(0xaf598cu+cell*4u)=((flags&0xdfcffffdu)|add)&0xb9faf682u;
}
void FUN_00477ab0(int32_t x,int32_t y){set_flags(x,y,0x2000000u,0x9bcbf680u,0x2000000u);}
void FUN_00477b10(int32_t x,int32_t y){set_flags(x,y,0x800u,0x99cafe80u,0x800u);}
void FUN_00477b70(int32_t x,int32_t y){set_flags(x,y,0x40000u,0x99cef680u,0x40000u);}
void FUN_00477bd0(int32_t x,int32_t y)
{
    uint32_t cell=cell_at(x,y,1),flags;
    if(B(0xab623cu+cell))return;
    flags=U(0xaf598cu+cell*4u);U(0xbc972cu)=1;U(0xaf598cu+cell*4u)=(flags&0x99cbf680u)|0x10000u;
}
void FUN_00477c20(int32_t x,int32_t y)
{
    uint32_t cell=cell_at(x,y,1),flags;
    if(B(0xab623cu+cell))return;
    flags=U(0xaf598cu+cell*4u);if(flags&4u){U(0xbc9730u)=1;return;}
    U(0xbc972cu)=1;U(0xaf598cu+cell*4u)=(flags&0x9dcaf784u)|4u;
}
void FUN_00477c80(int32_t x,int32_t y){set_flags(x,y,0x40u,0xd9cbf7c0u,0x40u);}
void FUN_00477ce0(int32_t x,int32_t y){set_flags(x,y,0x100u,0xd9cbf7c0u,0x100u);}
uint32_t FUN_00477d40(int32_t x,int32_t y)
{
    static const uint32_t address[8]={0xaf55fc,0xaf5600,0xaf5990,0xaf5d20,0xaf5d1c,0xaf5d18,0xaf5988,0xaf55f8};
    uint32_t cell=cell_at(x,y,0),flags=U(0xaf598cu+cell*4u),i;
    if(!(flags&4u)||!(flags&0x4000000u))return 0;
    for(i=0;i<8u;++i)if(!(U(address[i]+cell*4u)&0x4000000u))return 1;
    return 0;
}
void FUN_004793e0(int32_t x,int32_t y)
{
    uint32_t cell=cell_at(x,y,1),value=B(0xab623cu+cell),selected,flags,v;
    if(value>=5u)return;
    selected=U(0xbc96ccu);if(value!=selected)return;
    flags=U(0xaf598cu+cell*4u);if(flags&0x600u)return;
    v=B(0xb285ccu+cell);B(0xb285ccu+cell)=(uint8_t)(v&0xf0u);
    U(0xaf598cu+cell*4u)=flags&0xfbffff93u;B(0xab623cu+cell)=(uint8_t)(value+1u);
}
void FUN_00479460(int32_t x,int32_t y)
{
    uint32_t cell=cell_at(x,y,1),flags=U(0xaf598cu+cell*4u),v,w,value,selected;
    if(flags&0x400u){
        v=B(0xb285ccu+cell);U(0xaf598cu+cell*4u)=(flags&0xfffffbffu)|0x200u;
        w=B(0xb67d1cu+cell);B(0xb285ccu+cell)=(uint8_t)(v&0xf0u);B(0xb67d1cu+cell)=(uint8_t)(w|0x40u);
    }
    value=B(0xab623cu+cell);
    if(!value){flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=flags&0xfffffdffu;return;}
    selected=U(0xbc96ccu);
    if(value==selected){flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=flags&0xfffff9ffu;B(0xab623cu+cell)=(uint8_t)(value-1u);}
}
uint32_t FUN_0047ed60(uint32_t cell)
{
    if(!(B(0xaf55fcu+cell*4u)&4u))return 0;
    if(!(B(0xaf5990u+cell*4u)&4u))return 0;
    if(!(B(0xaf5d1cu+cell*4u)&4u))return 0;
    return (U(0xaf5988u+cell*4u)>>2)&1u;
}
void FUN_0040f920(void *buffer,int32_t bytes,uint8_t mask)
{
    uint32_t p=(uint32_t)(uintptr_t)buffer,pattern=(uint32_t)mask*0x01010101u,i;
    while(bytes>=64){for(i=0;i<16u;++i){U(p)&=pattern;p+=4u;}bytes-=64;}
    while(bytes>0){B(p)&=mask;++p;--bytes;}
}
uint32_t FUN_0046fde0(uint32_t tool)
{
    uint32_t high=(tool-5u)&0xffffff00u;
    switch(tool){case 5:case 8:case 9:case 90:case 100:case 101:case 102:case 103:case 104:case 105:case 196:case 224:return high|1u;default:return high;}
}
