#include "restore_types.h"
#define DAT_00d3b16c (*(undefined *)0x00d3b16cu)
#define DAT_00d3b170 (*(undefined *)0x00d3b170u)
#define DAT_00d3b174 (*(undefined *)0x00d3b174u)
#define DAT_00d3b178 (*(undefined *)0x00d3b178u)
#define DAT_00e209f0 (*(undefined4 *)0x00e209f0u)
#define DAT_00e209f4 (*(undefined4 *)0x00e209f4u)
#define DAT_00e36c74 (*(uint32_t *)0x00e36c74u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e3908c (*(undefined1 *)0x00e3908cu)
#define DAT_00e95080 (*(undefined4 *)0x00e95080u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00e951dc (*(undefined *)0x00e951dcu)
#define DAT_00e951e4 (*(undefined *)0x00e951e4u)
#define DAT_00e95228 (*(undefined4 *)0x00e95228u)
#define DAT_00e9522c (*(undefined4 *)0x00e9522cu)
#define DAT_00e95230 (*(undefined4 *)0x00e95230u)
#define DAT_00e95232 (*(undefined4 *)0x00e95232u)
#define DAT_00e95234 (*(undefined4 *)0x00e95234u)
#define DAT_00e95236 (*(undefined4 *)0x00e95236u)
#define DAT_00e95237 (*(undefined4 *)0x00e95237u)
#define DAT_00e95238 (*(undefined4 *)0x00e95238u)
#define DAT_00ea0da8 (*(undefined *)0x00ea0da8u)
#define DAT_00f54118 (*(undefined *)0x00f54118u)
extern undefined4 FUN_0047d350();
extern void FUN_004c5bf0();
extern void FUN_004ca980();
extern bool FUN_004ce3f0();
extern void FUN_004e14b0();

void FUN_004e1320(void)

{
  int *piVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (DAT_00e38dda == '\x01') {
    if (0 < DAT_00ea0da8) {
      DAT_00ea0da8 = DAT_00ea0da8 + -1;
      return;
    }
    iVar3 = 0;
    piVar1 = &DAT_00e95080;
    while (*piVar1 == 0) {
      piVar1 = piVar1 + 1;
      iVar3 = iVar3 + 1;
      if (0xe950cf < (int)piVar1) {
        return;
      }
    }
    iVar4 = (&DAT_00e95080)[iVar3];
    (&DAT_00e95080)[iVar3] = 0;
    if (0 < iVar4) {
      iVar3 = 0;
      psVar2 = &DAT_00e95234;
      while (psVar2[-1] != 0) {
        if (*psVar2 == iVar4) {
          if (iVar3 < 0) {
            return;
          }
          iVar4 = iVar3 * 0x30;
          DAT_00ea0da8 = 5;
          iVar5 = (int)(short)(&DAT_00e95232)[iVar3 * 0x18];
          FUN_004e14b0(iVar5);
          (&DAT_00e95236)[iVar4] = 1;
          DAT_00d3b170 = *(undefined4 *)(&DAT_00e95228 + iVar4);
          DAT_00d3b178 = (int)*(short *)(&DAT_00e95230 + iVar4);
          DAT_00d3b174 = (uint)(byte)(&DAT_00e95237)[iVar4];
          DAT_00d3b16c = *(undefined4 *)(&DAT_00e9522c + iVar4);
          DAT_00e3908c = 0;
          DAT_00e951dc = iVar5;
          DAT_00e951e4 = iVar3;
          FUN_004ca980();
          iVar4 = FUN_004ce3f0(*(int *)(&DAT_00e209f4 + iVar5 * 0x50) + DAT_00e36c74);
          if (iVar4 == 0) {
            if (*(int *)(&DAT_00e209f0 + iVar5 * 0x50) == 1) {
              FUN_0047d350(7);
            }
            else {
              FUN_0047d350(6);
            }
          }
          else {
            DAT_00f54118 = 1;
          }
          DAT_00e950dc = 1;
          FUN_004c5bf0(DAT_00e951dc,0,-1 < (short)(&DAT_00e95238)[iVar3 * 0x18]);
          DAT_00e38dc9 = 1;
          DAT_00e950dc = 0;
          return;
        }
        psVar2 = psVar2 + 0x18;
        iVar3 = iVar3 + 1;
        if (0xea0d83 < (int)psVar2) {
          return;
        }
      }
    }
  }
  return;
}
