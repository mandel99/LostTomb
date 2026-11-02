#include "refresh_wrappers.h"
#include "terrain_redraw.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004770f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475dc0(void);
extern void FUN_00477430(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475bf0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0051f780(void);
void FUN_00484ee0(void *table,int32_t count)
{
 uint32_t p=(uint32_t)(uintptr_t)table+14u,n=(uint32_t)count;
 if(count>0)do{B(p)=0;p+=16u;--n;}while(n);
}
void FUN_0047b570(void)
{
 uint32_t height=U(0xbca2d4u),width=U(0xbca2d0u);FUN_0047b590(0,0,(int32_t)(width-1u),(int32_t)(height-1u));
}
void FUN_0047bd30(void)
{
 uint32_t height=U(0xbca2d4u),width=U(0xbca2d0u);FUN_0047bd50(0,0,(int32_t)(width-1u),(int32_t)(height-1u));
}
/* Native51F780 is RET and consumes no argument. Its redundant outgoing slots
   are omitted; the two actual global loads at479510 remain explicit above it. */
void FUN_00478d40(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_004767d0(param_1,param_2,param_3,param_4,0,0);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_0047b590((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  FUN_0047bd50((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  FUN_00477500(param_1,param_2,param_3,param_4);
  return;
}

void FUN_00478e10(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00475bf0(param_1,param_2,param_3,param_4);
  return;
}

void FUN_00478ea0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_0047b590((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  FUN_0047bd50((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  return;
}

void FUN_00478f40(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_0051f780();
  return;
}

void FUN_00478fd0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  return;
}

void FUN_00479060(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477210();
  return;
}

void FUN_004790e0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_00477500(param_1,param_2,param_3,param_4);
  return;
}

void FUN_00479170(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  return;
}

void FUN_00479200(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_004767d0(param_1,param_2,param_3,param_4,0,0);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_0047b590((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  FUN_0047bd50((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u));
  return;
}

void FUN_004792b0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_0047b570();
  return;
}

void FUN_00479340(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_00475cc0((int32_t)((uint32_t)param_1-1u),(int32_t)((uint32_t)param_2-1u),(int32_t)((uint32_t)param_3+1u),(int32_t)((uint32_t)param_4+1u),1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_00477430(param_1,param_2,param_3,param_4);
  FUN_0047bd30();
  return;
}

void FUN_00479510(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{
  FUN_00484ee0((void *)(uintptr_t)0x005ca658u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005ca958u,0x30);
  FUN_00484ee0((void *)(uintptr_t)0x005cac58u,0x18);
  FUN_00484ee0((void *)(uintptr_t)0x005cbdc8u,0xe);
  FUN_00484ee0((void *)(uintptr_t)0x005cbea8u,0x11);
  (void)U(0xbca2d4u);(void)U(0xbca2d0u);FUN_0051f780();
  FUN_00475cc0(param_1,param_2,param_3,param_4,1);
  FUN_004776f0(param_1,param_2,param_3,param_4);
  FUN_004777c0(param_1,param_2,param_3,param_4);
  FUN_00475bf0(param_1,param_2,param_3,param_4);
  FUN_00477500(param_1,param_2,param_3,param_4);
  FUN_0051f780();
  FUN_004770f0(param_1,param_2,param_3,param_4);
  FUN_00475dc0();
  FUN_004767d0(param_1,param_2,param_3,param_4,0,0);
  return;
}
