#include "restore_types.h"
#define DAT_01266710 (*(undefined *)0x01266710u)
#define DAT_01266740 (*(undefined4 *)0x01266740u)
#define DAT_01266840 (*(undefined4 *)0x01266840u)
#define DAT_01266944 (*(undefined *)0x01266944u)
extern BOOL FUN_00569335();
extern int FUN_0056947e();

void FUN_00569a70(void)

{
  byte *pbVar1;
  BOOL BVar2;
  uint uVar3;
  char cVar4;
  uint uVar5;
  uint uVar6;
  ushort *puVar7;
  undefined1 uVar8;
  BYTE *pBVar9;
  undefined4 *puVar10;
  ushort local_518 [256];
  undefined1 local_318 [256];
  undefined1 local_218 [256];
  undefined4 local_118 [64];
  _cpinfo local_18;
  
  BVar2 = GetCPInfo(DAT_01266710,&local_18);
  if (BVar2 == 1) {
    uVar3 = 0;
    do {
      *(char *)((int)local_118 + uVar3) = (char)uVar3;
      uVar3 = uVar3 + 1;
    } while (uVar3 < 0x100);
    RESTORE_SLICE1(local_118[0],0) = 0x20;
    if (local_18.LeadByte[0] != 0) {
      pBVar9 = local_18.LeadByte + 1;
      do {
        uVar3 = (uint)local_18.LeadByte[0];
        if (uVar3 <= *pBVar9) {
          uVar5 = (*pBVar9 - uVar3) + 1;
          puVar10 = (undefined4 *)((int)local_118 + uVar3);
          for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
            *puVar10 = 0x20202020;
            puVar10 = puVar10 + 1;
          }
          for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
            *(undefined1 *)puVar10 = 0x20;
            puVar10 = (undefined4 *)((int)puVar10 + 1);
          }
        }
        local_18.LeadByte[0] = pBVar9[1];
        pBVar9 = pBVar9 + 2;
      } while (local_18.LeadByte[0] != 0);
    }
    FUN_00569335(1,local_118,0x100,local_518,DAT_01266710,DAT_01266944,0);
    FUN_0056947e(DAT_01266944,0x100,local_118,0x100,local_218,0x100,DAT_01266710,0);
    FUN_0056947e(DAT_01266944,0x200,local_118,0x100,local_318,0x100,DAT_01266710,0);
    uVar3 = 0;
    puVar7 = local_518;
    do {
      if ((*puVar7 & 1) == 0) {
        if ((*puVar7 & 2) != 0) {
          pbVar1 = (byte *)((int)&DAT_01266840 + uVar3 + 1);
          *pbVar1 = *pbVar1 | 0x20;
          uVar8 = local_318[uVar3];
          goto LAB_00569b7c;
        }
        (&DAT_01266740)[uVar3] = 0;
      }
      else {
        pbVar1 = (byte *)((int)&DAT_01266840 + uVar3 + 1);
        *pbVar1 = *pbVar1 | 0x10;
        uVar8 = local_218[uVar3];
LAB_00569b7c: ;
        (&DAT_01266740)[uVar3] = uVar8;
      }
      uVar3 = uVar3 + 1;
      puVar7 = puVar7 + 1;
    } while (uVar3 < 0x100);
  }
  else {
    uVar3 = 0;
    do {
      if ((uVar3 < 0x41) || (0x5a < uVar3)) {
        if ((0x60 < uVar3) && (uVar3 < 0x7b)) {
          pbVar1 = (byte *)((int)&DAT_01266840 + uVar3 + 1);
          *pbVar1 = *pbVar1 | 0x20;
          cVar4 = (char)uVar3 + -0x20;
          goto LAB_00569bc6;
        }
        (&DAT_01266740)[uVar3] = 0;
      }
      else {
        pbVar1 = (byte *)((int)&DAT_01266840 + uVar3 + 1);
        *pbVar1 = *pbVar1 | 0x10;
        cVar4 = (char)uVar3 + ' ';
LAB_00569bc6: ;
        (&DAT_01266740)[uVar3] = cVar4;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < 0x100);
  }
  return;
}
