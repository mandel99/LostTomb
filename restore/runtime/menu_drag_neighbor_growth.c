#include "growth.h"
#include "map_cells_phase.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_00484f40(void);
static uint32_t bounded_growth(uint32_t cell,uint32_t value)
{
    uint32_t object=U(0xd05720u+cell*4u);
    if(object&&B(object+24u)==0)return 100;
    if(U(0xaf598cu+cell*4u)&0x40004u)return 100;
    return value<128u?value:0;
}
uint32_t FUN_00486f20(uint32_t cell)
{
    static const uint32_t delta[8]={0xffffff1c,0xffffff1d,1,229,228,227,0xffffffff,0xffffff1b};
    uint32_t n=cell-228u,flags,level,i,value;
    flags=B(0xaf598cu+n*4u);
    if((flags&4u)&&(B(0xaf5d1cu+cell*4u)&4u)){B(0xa509bcu+cell)=100;return 0;}
    if((B(0xaf5988u+cell*4u)&4u)&&(B(0xaf5990u+cell*4u)&4u)){B(0xa509bcu+cell)=100;return 0;}
    if(FUN_00487260((int32_t)cell)){B(0xa509bcu+cell)=100;return 0;}
    level=B(0xa509bcu+cell);
    if(level>90u||level<=40u){if(level==100u)B(0xa509bcu+cell)=100;return 0;}
    for(i=0;i<4u;++i){
        n=cell+delta[i];
        if((uint8_t)FUN_004872c0((int32_t)n,4u)||FUN_00487260((int32_t)n))value=100;
        else value=B(0xa509bcu+n);
        value=bounded_growth(n,value);U(0xbc97c0u+i*4u)=value==100u;
    }
    for(i=4;i<7u;++i){
        n=cell+delta[i];value=FUN_00487300((int32_t)n);value=bounded_growth(n,value);U(0xbc97c0u+i*4u)=value==100u;
    }
    value=FUN_00487390((int32_t)cell,-229);U(0xbc97dcu)=value==100u;
    return (uint32_t)FUN_00484f40();
}
