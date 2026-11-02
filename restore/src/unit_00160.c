#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00536820(uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cc540(void);
extern void FUN_00536a30(uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cc720(const void *,uint32_t,uint32_t,int32_t,uint32_t,uint16_t);
extern void FUN_00537140(uint32_t,uint32_t,uint32_t);
extern void FUN_00536c30(int32_t,uint32_t,uint32_t,uint32_t,uint32_t,int32_t);
extern void FUN_004ccb30(int32_t,int32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ccbb0(int32_t,int32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern int32_t FUN_0053e590(int32_t,int32_t,int32_t);

/* Whole4138D0. The origin is cached before any drawing; fonts, hover and
   settings are read afresh at their native call sites. DWORD identifiers and
   coordinates must not be truncated. Scenario selectors are signed WORDs;
   the independent toggle is BYTE784CBD, not a slice of BYTE784CBC. */
void FUN_004138d0(void)
{
    uint32_t x=U(0xe38da8u),y=U(0xe38da4u),right=x+212u,image;
    int32_t choice;
    FUN_00536820(x,y+28u,30,25);
    FUN_004cc540();
    FUN_00536a30(x+109u,y+40u,18,2);
    FUN_004cc720((const void *)(uintptr_t)0x784914u,x+112u,y+50u,280,U(0xf56f9cu),0);
    FUN_00537140(U(0xe20770u)+x+112u,y+50u,65535);
    FUN_00536c30(1,right,y+76u,250,30,U(0x7056c8u)==1u);
    FUN_004ccb30(44,88,right,y+85u,250,U(0xf56f8cu),0);
    FUN_004ccbb0(44,76,x+32u,y+125u,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+116u,250,30,U(0x7056c8u)==2u);
    FUN_004ccb30(44,(int32_t)(U(0x784cb0u)&255u)+77,right,y+125u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+156u,250,30,U(0x7056c8u)==3u);
    FUN_004ccb30(44,95,right,y+165u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,x+17u,y+196u,175,30,U(0x7056c8u)==9u);
    FUN_004ccb30(44,B(0x784cbdu)==1u?227:41,x+19u,y+205u,173,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+196u,250,30,U(0x7056c8u)==4u);
    FUN_004ccb30(37,H(0x7848fcu),right,y+205u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+236u,250,30,U(0x7056c8u)==5u);
    FUN_004ccb30(44,164,right,y+245u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+276u,250,30,U(0x7056c8u)==6u);
    FUN_004ccb30(44,44,right,y+285u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+316u,250,30,U(0x7056c8u)==7u);
    FUN_004ccb30(44,45,right,y+325u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,right,y+356u,250,30,U(0x7056c8u)==8u);
    FUN_004ccb30(44,178,right,y+365u,250,U(0xf56f8cu),0);
    FUN_00536c30(1,x+18u,y+238u,184,144,0);
    choice=H(0x784b5eu);
    if(choice<=18)image=(uint32_t)choice+U(0xf55c94u);
    else image=(uint32_t)choice-19u+U(0xf57314u);
    FUN_0053e590((int32_t)image,(int32_t)(x+20u),(int32_t)(y+240u));
}
