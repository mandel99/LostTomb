#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f2 (*(undefined4 *)0x007999f2u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a1e (*(undefined4 *)0x00799a1eu)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a5d (*(undefined4 *)0x00799a5du)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)
#define DAT_00799ac4 (*(undefined4 *)0x00799ac4u)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
#define DAT_00799ad8 (*(undefined4 *)0x00799ad8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65e (*(undefined4 *)0x00bcd65eu)
extern void FUN_00454190();
extern undefined4 FUN_00459240();
extern undefined4 FUN_00459ff0();
extern undefined4 FUN_00460fb0();
extern int FUN_00488030();
extern undefined4 FUN_00488050();

void FUN_00432920(int param_1)

{
  byte bVar1;
  short sVar2;
  ushort uVar3;
  int iVar4;
  char cVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  iVar4 = param_1;
  iVar9 = param_1 * 0x108;
  if ((&DAT_007999e8)[param_1 * 0x84] == 0x5c) {
    *(undefined4 *)(&DAT_00799ad8 + iVar9) = 0;
    return;
  }
  if (param_1 == 0) {
    return;
  }
  if (*(short *)(&DAT_00799a10 + iVar9) != 0) {
    return;
  }
  *(undefined4 *)(&DAT_00799ad8 + iVar9) = 0;
  if (800 < (short)(&DAT_00799a1e)[param_1 * 0x84]) {
    *(undefined4 *)(&DAT_00799ad8 + iVar9) = 1;
  }
  if (800 < *(short *)(&DAT_00799a20 + iVar9)) {
    *(uint *)(&DAT_00799ad8 + iVar9) = *(uint *)(&DAT_00799ad8 + iVar9) | 2;
  }
  if (*(char *)((int)&DAT_007999dc + iVar9 + 1) == '\0') {
    cVar5 = FUN_00488030(param_1);
    if ((cVar5 == '\0') && ((sVar2 = (&DAT_007999e8)[iVar4 * 0x84], sVar2 < 0x6a || (0x6d < sVar2)))
       ) {
      if ((sVar2 < 0x6e) || (0x72 < sVar2)) {
        switch((int)(sVar2)) {
        case 0x20: ;
        case 0x2f: ;
        case 0x33: ;
        case 0x35: ;
        case 0x4a: ;
        case 0x71: ;
        case 0xb1: ;
        case 0xb2: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
        case 0xc2: ;
        case 0xcb: ;
        case 0xcc: ;
        case 0xcd: ;
        case 0xe2: ;
        case 0xe7: ;
        case 0xe8: ;
        case 0xe9: ;
          break;
default: ;
          goto switchD_00432ace_caseD_21;
        }
      }
      iVar7 = FUN_00459ff0(iVar4);
      if (iVar7 == 0) {
        if ((((short)(&DAT_00799a02)[iVar4 * 0x84] != 0) &&
            (iVar7 = (short)(&DAT_00799a02)[iVar4 * 0x84] * 0x184, (&DAT_00bcd602)[iVar7] == '\x04')
            ) && (((&DAT_00bcd630)[iVar7] == '\b' && ((&DAT_00bcd65e)[iVar7] != '\0'))))
        goto LAB_00432baf;
        if ((short)(&DAT_00799a14)[iVar4 * 0x84] < 1) {
          if ((&DAT_007999e8)[iVar4 * 0x84] != 0x4a) {
LAB_00432b6c: ;
            *(uint *)(&DAT_00799ad8 + iVar9) = *(uint *)(&DAT_00799ad8 + iVar9) | 0x100;
            goto LAB_00432baf;
          }
          uVar3 = *(ushort *)(&DAT_00799a8c + iVar9);
          if ((uVar3 & 0xf000) == 0) {
            cVar5 = FUN_00460fb0(&param_1);
            if (cVar5 != '\0') goto LAB_00432b6c;
          }
          else if (((uVar3 & 0xf000) != 0x1000) || ((uVar3 & 0xfff) != 0x19)) goto LAB_00432b6c;
        }
      }
    }
    else if (((short)(&DAT_00799a02)[iVar4 * 0x84] != 0) &&
            (((iVar7 = (short)(&DAT_00799a02)[iVar4 * 0x84] * 0x184,
              (&DAT_00bcd602)[iVar7] == '\x04' && ((&DAT_00bcd630)[iVar7] == '\b')) &&
             ((&DAT_00bcd65e)[iVar7] != '\0')))) {
LAB_00432baf: ;
      *(undefined1 *)((int)&DAT_00799a9a + iVar9 + 1) = 1;
    }
switchD_00432ace_caseD_21: ;
    iVar7 = *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[iVar4 * 0x84] * 0x28);
    if (iVar7 == 0) {
      return;
    }
    iVar8 = FUN_00459240(iVar4);
    if (iVar8 == 0) {
      if (((short)(&DAT_007999f2)[iVar4 * 0x84] < 1) && (cVar5 = FUN_00488050(iVar4), cVar5 == '\0')
         ) {
        uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x1000;
      }
      else if ((&DAT_00799a18)[iVar4 * 0x84] == 0) {
        uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x200;
      }
      else {
        if (iVar7 <= (short)(&DAT_00799a18)[iVar4 * 0x84]) goto LAB_00432c37;
        uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x400;
      }
    }
    else {
      uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x800;
    }
  }
  else {
    if ((&DAT_007999d8)[iVar9] == '\0') goto LAB_00432c37;
    if ((&DAT_007999f4)[param_1 * 0x84] == 0) {
      uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 8;
LAB_004329e2: ;
      *(uint *)(&DAT_00799ad8 + iVar9) = uVar6;
    }
    else if (((&DAT_00799ac4)[param_1 * 0x42] != 0) && ((int)(&DAT_00799ac4)[param_1 * 0x42] < 3)) {
      uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x10;
      goto LAB_004329e2;
    }
    if ((&DAT_00799a5f)[iVar9] == '\x01') {
      uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x20;
LAB_00432a29: ;
      *(uint *)(&DAT_00799ad8 + iVar9) = uVar6;
    }
    else {
      if ((&DAT_00799a5f)[iVar9] == '\x02') {
        uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x2000;
        goto LAB_00432a29;
      }
      if (800 < *(int *)(&DAT_00799ad4 + iVar9)) {
        if ((&DAT_007999f4)[param_1 * 0x84] == 0) goto LAB_00432c37;
        uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 4;
        goto LAB_00432a29;
      }
    }
    if ((&DAT_007999f4)[param_1 * 0x84] == 0) goto LAB_00432c37;
    if (800 < (int)(&DAT_00799abc)[param_1 * 0x42]) {
      *(uint *)(&DAT_00799ad8 + iVar9) = *(uint *)(&DAT_00799ad8 + iVar9) | 0x40;
    }
    FUN_00454190(param_1);
    bVar1 = (&DAT_00799a5d)[iVar9];
    if (((0x1d < bVar1) && (bVar1 != 0x48)) && (bVar1 != 0x46)) goto LAB_00432c37;
    uVar6 = *(uint *)(&DAT_00799ad8 + iVar9) | 0x80;
  }
  *(uint *)(&DAT_00799ad8 + iVar9) = uVar6;
LAB_00432c37: ;
  if ((*(int *)(&DAT_00799ad8 + iVar9) != 0) && (*(char *)((int)&DAT_00799a9a + iVar9 + 1) == '\0'))
  {
    *(undefined1 *)((int)&DAT_00799a9a + iVar9 + 1) = 1;
  }
  return;
}
