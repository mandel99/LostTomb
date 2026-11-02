#include "restore_types.h"
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bcd5e8 (*(undefined4 *)0x00bcd5e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd63a (*(undefined4 *)0x00bcd63au)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd63e (*(undefined4 *)0x00bcd63eu)
#define DAT_00bcd63f (*(undefined4 *)0x00bcd63fu)
#define DAT_00bcd640 (*(undefined4 *)0x00bcd640u)
#define DAT_00bcd641 (*(undefined4 *)0x00bcd641u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern uint FUN_0047e340();
extern void FUN_004a5da0();
extern void FUN_004b21d0();
extern void FUN_004b3c60();
extern void FUN_004b3f00();
extern undefined4 FUN_004baa10();

void FUN_004b1ef0(int param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  char cVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  iVar6 = 0;
  iVar7 = param_1 * 0x184;
  if ((&DAT_00bcd63e)[iVar7] == '\0') {
    DAT_00bcd5e8 = 1;
    FUN_004a5da0(DAT_00d056f8,
                 *(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38));
    cVar3 = (&DAT_00bcd609)[iVar7];
    if ((cVar3 == '\t') || (cVar3 == '\n')) {
      (&DAT_00bcd63e)[iVar7] = 1;
    }
    else if (cVar3 == '\b') {
      (&DAT_00bcd63e)[iVar7] = 1;
      *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
    }
    if ((&DAT_00bcd63e)[iVar7] == '\0') {
      return;
    }
    (&DAT_00bcd641)[iVar7] = 100;
    (&DAT_00bcd609)[iVar7] = (&DAT_00bcd60a)[iVar7];
  }
  if (param_2 < 1) {
    return;
  }
  do {
    cVar3 = (&DAT_00bcd631)[iVar7];
    param_2 = param_2 + -1;
    (&DAT_00bcd631)[iVar7] = cVar3 + '\x01';
    if ('\x0e' < (char)(cVar3 + '\x01')) {
      (&DAT_00bcd631)[iVar7] = 0xf;
      uVar8 = (int)(char)(&DAT_00bcd60a)[iVar7] + 4U & 0x80000007;
      if ((int)uVar8 < 0) {
        uVar8 = (uVar8 - 1 | 0xfffffff8) + 1;
      }
      (&DAT_00bcd63f)[iVar7] = (&DAT_00bcd63f)[iVar7] + '\x01';
      iVar5 = FUN_004baa10(param_1,0);
      if (iVar5 != 0) {
        return;
      }
      iVar5 = FUN_0047e340(*(undefined4 *)(&DAT_00bcd618 + iVar7),param_1);
      if (iVar5 < 1) {
        *(undefined2 *)(&DAT_00bcd63c + DAT_00d056f8 * 0x184) =
             *(undefined2 *)(&DAT_00bcd63a + DAT_00d056f8 * 0x184);
        return;
      }
      if (iVar5 < 2) {
        cVar3 = (char)iVar6 << 1;
        (&DAT_00bcd609)[iVar7] = cVar3;
        iVar5 = (&DAT_00bc97c0)[cVar3];
        iVar2 = iVar6;
        while ((iVar6 = iVar2, iVar5 == 0 && (iVar6 = iVar2 + 1, iVar2 < 4))) {
          cVar3 = (char)iVar6 << 1;
          (&DAT_00bcd609)[iVar7] = cVar3;
          iVar5 = (&DAT_00bc97c0)[cVar3];
          iVar2 = iVar6;
        }
      }
      else if (iVar5 == 2) {
        iVar5 = iVar6;
        if ((&DAT_00bcd641)[iVar7] == -1) {
          FUN_004b21d0(param_1);
          uVar8 = 0xffffffff;
        }
        do {
          cVar3 = (&DAT_00bcd609)[iVar7];
          if (((&DAT_00bc97c0)[cVar3] != 0) && (iVar6 = iVar5, (int)cVar3 != uVar8)) break;
          (&DAT_00bcd609)[iVar7] = (&DAT_00bcd640)[iVar7] + cVar3;
          if ('\x06' < (char)((&DAT_00bcd640)[iVar7] + cVar3)) {
            (&DAT_00bcd609)[iVar7] = 0;
          }
          if ((char)(&DAT_00bcd609)[iVar7] < '\0') {
            (&DAT_00bcd609)[iVar7] = 6;
          }
          iVar6 = iVar5 + 1;
          bVar1 = iVar5 < 4;
          iVar5 = iVar6;
        } while (bVar1);
      }
      else {
        bVar4 = (&DAT_00acf85c)[*(int *)(&DAT_00bcd618 + iVar7)] + (&DAT_00bcd63f)[iVar7] & 6;
        (&DAT_00bcd609)[iVar7] = bVar4;
        if (((&DAT_00bc97c0)[(char)bVar4] == 0) || ((int)(char)bVar4 == uVar8)) {
          cVar3 = (&DAT_00bcd641)[iVar7];
          (&DAT_00bcd641)[iVar7] = cVar3 + -1;
          iVar5 = iVar6;
          if ((char)(cVar3 + -1) < '\x01') {
            FUN_004b21d0(param_1);
            uVar8 = 0xffffffff;
          }
          do {
            cVar3 = (&DAT_00bcd609)[iVar7];
            if (((&DAT_00bc97c0)[cVar3] != 0) && (iVar6 = iVar5, (int)cVar3 != uVar8)) break;
            (&DAT_00bcd609)[iVar7] = (&DAT_00bcd640)[iVar7] + cVar3;
            if ('\x06' < (char)((&DAT_00bcd640)[iVar7] + cVar3)) {
              (&DAT_00bcd609)[iVar7] = 0;
            }
            if ((char)(&DAT_00bcd609)[iVar7] < '\0') {
              (&DAT_00bcd609)[iVar7] = 6;
            }
            iVar6 = iVar5 + 1;
            bVar1 = iVar5 < 4;
            iVar5 = iVar6;
          } while (bVar1);
        }
      }
      *(short *)(&DAT_00bcd634 + iVar7) = *(short *)(&DAT_00bcd634 + iVar7) + 1;
      (&DAT_00bcd60a)[iVar7] = (&DAT_00bcd609)[iVar7];
      (&DAT_00bcd631)[iVar7] = 0;
    }
    FUN_004b3f00(param_1);
    if ((&DAT_00bcd631)[iVar7] == '\b') {
      FUN_004b3c60(param_1);
    }
    if (param_2 < 1) {
      return;
    }
  } while( true );
}
