#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784b74 (*(undefined *)0x00784b74u)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern int FUN_004b4800();
extern uint FUN_00562348();

void FUN_004bbc10(void)

{
  short sVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  short *psVar8;
  int local_4;
  
  if (DAT_00605a3c == 0) {
    psVar8 = &DAT_00784b74;
    do {
      sVar1 = *psVar8;
      if (0 < sVar1) {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_4 = 0x1f;
        do {
          uVar4 = DAT_00d3b384 >> 4;
          uVar6 = DAT_00d3b384 & 1;
          uVar7 = DAT_00d3b380 >> 4;
          uVar5 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar4 & 1) != uVar6) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar7 & 1) != uVar5) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_4 = local_4 + -1;
        } while (local_4 != 0);
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        iVar3 = FUN_004b4800(1,0x5f,(int)sVar1,(int)psVar8[8],0);
        bVar2 = FUN_00562348();
        (&DAT_00bcd5f9)[iVar3 * 0x184] = bVar2 & 0x1f;
      }
      psVar8 = psVar8 + 1;
    } while ((int)psVar8 < 0x784b84);
  }
  return;
}
