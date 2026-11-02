#include "city_ui_layout.h"
#include "city_ui_controllers.h"
#include <stdint.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004dddc0(void);
extern void FUN_004ddec0(void);
extern void FUN_004ddf20(void);
extern void FUN_004de080(void);
extern void FUN_004d71c0(int force);
extern void FUN_00425730(uint32_t x,uint32_t y,const void *records,uint32_t count);
extern void FUN_004cd9d0(uint32_t x,uint32_t y,uint32_t width,uint32_t height,uint32_t color);
extern int FUN_0053e590(uint32_t image,uint32_t x,uint32_t y);
void FUN_0051fd60(void)
{
    int32_t duration=I(0x00e39104u),offset;
    uint32_t step=U(0x00f53f7cu),increment,amount,image;
    increment=duration<=24?1u:duration<=60?2u:duration<=96?3u:4u;
    step+=increment;U(0x00f53f7cu)=step;
    /* Native stores the advanced step even when it exceeds47. */
    if((int32_t)step>=47){
        if(U(0x00f53f78u)==0){FUN_004ddec0();U(0x00f53f78u)=1;}
        else{FUN_004ddf20();U(0x00f53f78u)=0;}
        B(0x00e38ddau)=1;
        FUN_004de080();FUN_004d71c0(1);return;
    }
    offset=U(0x00f53f78u)==1?I(0x005f831cu-step*4u):I(0x005f8260u+step*4u);
    FUN_004dddc0();
    if(U(0x00e38db0u)==1024)U(0x00d3b558u)=1000;
    else if(U(0x00e38db0u)==800)U(0x00d3b558u)=760;
    B(0x00e38dc9u)=1;
    FUN_0053e590(U(0x00f567e4u)+1u,U(0x00f53f60u),30);
    FUN_00425730(U(0x00f53f60u),30,(const void *)0x005c75b0u,12);
    FUN_00425730(U(0x00f53f60u),30,(const void *)0x005c7348u,1);
    FUN_0053e590(U(0x00f567e4u),U(0x00f53f64u)+(uint32_t)offset,30);
    FUN_00425730(U(0x00f53f64u)+(uint32_t)offset,30,(const void *)0x005c7370u,16);
    FUN_00425730(U(0x00f53f64u)+(uint32_t)offset,30,(const void *)0x005c7300u,2);
    FUN_00425730(U(0x00f53f64u)+(uint32_t)offset,30,(const void *)0x005c7980u,2);
    FUN_0051ed30(1,U(0x00f53f64u)+(uint32_t)offset+11u);
    FUN_0051edd0(1,U(0x00f53f64u)+(uint32_t)offset+11u);
    amount=(uint32_t)(offset>8?8:offset)-(uint32_t)offset+145u;
    FUN_004cd9d0(U(0x00f53f64u)+(uint32_t)offset+8u,59,amount,111,0);
    if(U(0x00e38db0u)==1024){
        FUN_0053e590(U(0x00f567e4u)+5u,U(0x00f53f60u),480);
        image=U(0x00f567e4u)+4u;
    }else if(U(0x00e38db0u)==800){
        FUN_0053e590(U(0x00f567e4u)+7u,U(0x00f53f60u),480);
        image=U(0x00f567e4u)+6u;
    }else{U(0x00d3b558u)=U(0x00e38db0u);return;}
    FUN_0053e590(image,U(0x00f53f64u)+(uint32_t)offset,480);
    U(0x00d3b558u)=U(0x00e38db0u);
}
