#include "restore_types.h"
#define DAT_007027b8 (*(char *)0x007027b8u)
#define DAT_007027b9 (*(char *)0x007027b9u)
#define DAT_007027be (*(char *)0x007027beu)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_0078d7c8 (*(undefined *)0x0078d7c8u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00e38ec2 (*(char *)0x00e38ec2u)
#define DAT_00e3908c (*(undefined1 *)0x00e3908cu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3fac (*(undefined4 *)0x00ea3facu)
#define DAT_00ea5bfc (*(undefined4 *)0x00ea5bfcu)
#define DAT_00ea5f8c (*(undefined4 *)0x00ea5f8cu)
#define DAT_00ea5f90 (*(undefined4 *)0x00ea5f90u)
#define DAT_00ea5f94 (*(undefined4 *)0x00ea5f94u)
#define DAT_00ea5fcc (*(undefined4 *)0x00ea5fccu)
#define DAT_00ea5fd0 (*(undefined4 *)0x00ea5fd0u)
#define DAT_00ea5fd4 (*(undefined4 *)0x00ea5fd4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define s_last_sav_005dbb94 (*(char (*)[9])0x005dbb94u)
extern void FUN_00417ae0();
extern void FUN_004448e0();
extern void FUN_004464f0();
extern void FUN_004474e0();
extern void FUN_0044ba90();
extern int FUN_00452160();
extern void FUN_00452200();
extern void FUN_00455530();
extern void FUN_004599d0();
extern void FUN_00459e50();
extern void FUN_0045a2e0();
extern void FUN_0045a350();
extern void FUN_0045a3f0();
extern void FUN_0045a450();
extern void FUN_0045a490();
extern void FUN_0045a4d0();
extern void FUN_0045a500();
extern void FUN_0045a530();
extern void FUN_0045a560();
extern void FUN_0045a5a0();
extern void FUN_0045a5d0();
extern void FUN_0045e420();
extern void FUN_0046e180();
extern void FUN_00475cc0();
extern void FUN_0047b570();
extern void FUN_004b7960();
extern void FUN_004bc7a0();
extern void FUN_004be640();
extern void FUN_004c5300();
extern undefined4 FUN_004d4e20();
extern void FUN_004e0cf0();
extern void FUN_004e1640();
extern void FUN_004e1850();
extern void FUN_004e7700();
extern void FUN_004e81d0();
extern void FUN_004e8630();
extern void FUN_004e8840();
extern void FUN_004e8db0();
extern void FUN_004e9700();
extern void FUN_004ea000();
extern void FUN_004ea740();
extern void FUN_00517370();
extern void FUN_0051f780();

void FUN_00452370(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = DAT_00eaac28;
  DAT_0078d7c8 = DAT_0078d7c8 + 1;
  DAT_0078d7c4 = DAT_0078d7c4 + 1;
  if (0xf < DAT_0078d7c4) {
    DAT_0078d7c4 = 0;
    iVar1 = DAT_00eaac28 * 0x49d8;
    *(undefined4 *)(&DAT_00ea5fcc + iVar1) = *(undefined4 *)(&DAT_00ea3fac + iVar1);
    *(undefined4 *)(&DAT_00ea3fac + iVar1) = 0;
    *(undefined4 *)(&DAT_00ea5fd0 + iVar1) = 0;
    *(undefined4 *)(&DAT_00ea5fd4 + iVar1) = 0;
    (&DAT_00ea5bfc)[iVar2 * 0x1276] = (&DAT_00ea5bfc)[iVar2 * 0x1276] + 1;
    FUN_004e81d0();
    FUN_004599d0();
    FUN_0045a2e0();
    FUN_0045a350();
    FUN_0045a3f0();
    FUN_00452160();
    FUN_004474e0();
    FUN_0045e420();
    FUN_004464f0();
    FUN_0044ba90();
    FUN_004ea740();
    FUN_004b7960();
    FUN_004e1850();
    FUN_0047b570();
    FUN_00475cc0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
    FUN_00517370();
    FUN_004e1640();
    DAT_0078d7c0 = DAT_0078d7c0 + 1;
    FUN_004e7700();
    FUN_004bc7a0();
    FUN_00452200();
    if (DAT_0078d7c0 < 0xc) {
      FUN_0046e180(0);
    }
    else {
      DAT_0078d7c0 = 0;
      FUN_0051f780();
      DAT_00e3908c = '\0';
      DAT_0078d7bc = DAT_0078d7bc + 1;
      DAT_00bc9694 = 0;
      FUN_004e8630();
      FUN_00459e50();
      FUN_0045a450();
      FUN_0045a490();
      FUN_0045a4d0();
      FUN_0045a500();
      FUN_0045a530();
      FUN_0045a560();
      FUN_0045a5a0();
      FUN_0045a5d0();
      FUN_004448e0();
      FUN_00455530();
      FUN_0046e180(1);
      FUN_004c5300();
    }
    if ((DAT_0078d7c0 == 0) || (DAT_0078d7c0 == 6)) {
      (&DAT_00ea5f8c)[DAT_00eaac28 * 0x1276] = 2;
    }
    FUN_004e9700();
    FUN_004ea000();
    if ((DAT_00e3908c == '\0') && (DAT_00e38ec2 == '\x01')) {
      FUN_004d4e20(s_last_sav_005dbb94);
    }
  }
  if ((DAT_0078d7c4 == 0) || (DAT_0078d7c4 == 8)) {
    FUN_004e8840();
    if (DAT_0078d7c4 == 8) {
      FUN_004be640();
    }
  }
  FUN_004e8db0();
  iVar2 = DAT_00eaac28;
  if ((DAT_007027b8 != '\0') && ((&DAT_00ea5f90)[DAT_00eaac28 * 0x1276] == 0)) {
    (&DAT_00ea5f90)[DAT_00eaac28 * 0x1276] = 1;
    DAT_00e950dc = 1;
  }
  if ((DAT_007027be != '\0') && ((&DAT_00ea5f94)[iVar2 * 0x1276] == 0)) {
    (&DAT_00ea5f94)[iVar2 * 0x1276] = 1;
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x13e,0,0,0,0);
    iVar2 = DAT_00eaac28;
  }
  if ((DAT_007027b9 == '\0') && (0x95 < *(int *)(&DAT_00ea1894 + iVar2 * 0x49d8))) {
    FUN_00417ae0(1);
  }
  return;
}
