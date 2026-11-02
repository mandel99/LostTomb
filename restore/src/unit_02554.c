#include "restore_types.h"
#define DAT_00601de4 (*(undefined4 *)0x00601de4u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern int FUN_00420430();
extern int FUN_0045bb50();
extern uint FUN_00542110();

uint FUN_005421d0(int param_1,int param_2,char param_3)

{
  char cVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined3 uVar11;
  uint uVar8;
  uint uVar9;
  int iVar10;
  int iVar12;
  int iVar13;
  char local_3;
  char local_2;
  char local_1;
  
  iVar12 = param_2 * 0x184;
  iVar13 = param_1 * 0x108;
  uVar8 = *(uint *)(&DAT_00799a3c + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x84);
  uVar6 = CONCAT12((char)(uVar8 >> 1),CONCAT11((char)uVar8,(&DAT_00bcd602)[iVar12] == 'U')) &
          0x101ff;
  uVar9 = uVar8 >> 4 & 0xffffff01;
  uVar11 = (undefined3)(uVar9 >> 8);
  local_3 = (char)uVar6;
  local_2 = (char)(uVar6 >> 8);
  switch((&DAT_00601de4)[(short)(&DAT_007999ea)[param_1 * 0x84] * 0xd]) {
  case 0: ;
    uVar9 = FUN_00542110(param_1);
    if ((char)uVar9 != '\0') goto switchD_0054225d_default;
    iVar10 = FUN_00420430(param_1,param_2);
    uVar9 = 0;
    if (iVar10 == 0) goto switchD_0054225d_default;
    uVar7 = CONCAT31((int3)((uint)iVar10 >> 8),param_3);
    if (param_3 != '\0') {
      if ((&DAT_00bcd602)[iVar12] == 'O') {
        (&DAT_00bcd630)[iVar12] = 10;
      }
      else if ((&DAT_00bcd602)[iVar12] == 'Q') {
        (&DAT_00bcd630)[iVar12] = 0xb;
      }
      *(int *)(&DAT_00799a34 + iVar13) = param_2;
      uVar3 = (&DAT_00799a00)[param_1 * 0x84];
      uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
      *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
      *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
      *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
      *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
      *(undefined2 *)(&DAT_00bcd62e + iVar12) = 0;
      return 1;
    }
    goto LAB_00542a3d;
  case 1: ;
    uVar9 = CONCAT31(uVar11,local_2);
    if ((local_2 == '\0') || (uVar9 = (&DAT_00799a30)[param_1 * 0x42], uVar9 == 0)) {
      if ((((&DAT_00799a30)[param_1 * 0x42] == 0) &&
          (uVar9 = CONCAT31((int3)(uVar9 >> 8),local_3), local_3 != '\0')) &&
         (uVar9 = FUN_00542110(param_1), (char)uVar9 == '\0')) {
        uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
        if (param_3 != '\0') {
          (&DAT_00bcd630)[iVar12] = 0x18;
          *(int *)(&DAT_00799a34 + iVar13) = param_2;
          uVar3 = (&DAT_00799a00)[param_1 * 0x84];
          uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
          *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
          *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
          *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
          *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
        }
        return CONCAT31((int3)((uint)uVar7 >> 8),1);
      }
      goto switchD_0054225d_default;
    }
    if (uVar9 != 1) goto switchD_0054225d_default;
    if (local_3 == '\0') {
      uVar9 = FUN_00542110(param_1);
      if (((char)uVar9 != '\0') || ((&DAT_00bcd602)[iVar12] != 'Q')) goto switchD_0054225d_default;
      uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
      if (param_3 == '\0') goto LAB_00542a3d;
      (&DAT_00bcd630)[iVar12] = 9;
      *(int *)(&DAT_00799a34 + iVar13) = param_2;
      goto LAB_00542a0e;
    }
    uVar9 = FUN_00542110(param_1);
    if ((char)uVar9 == '\0') goto switchD_0054225d_default;
    iVar13 = *(int *)(&DAT_00799a34 + iVar13);
    uVar9 = iVar13 * 0x61;
    if (*(short *)(&DAT_00bcd660 + iVar13 * 0x184) != 0) {
      uVar9 = *(short *)(&DAT_00bcd660 + iVar13 * 0x184) * 0x184;
      if (((((&DAT_00bcd606)[uVar9] != '\0') && ((&DAT_00bcd602)[uVar9] == 'U')) &&
          (((cVar1 = (&DAT_00bcd630)[uVar9], cVar1 == '\x06' ||
            (((cVar1 == '\a' || (cVar1 == '\x0f')) || (cVar1 == '\x10')))) || (cVar1 == '\x17'))))
         && (*(short *)(&DAT_00bcd660 + uVar9) == iVar13)) goto switchD_0054225d_default;
      *(undefined2 *)(&DAT_00bcd660 + iVar13 * 0x184) = 0;
    }
    uVar11 = (undefined3)(uVar9 >> 8);
    if (param_3 == '\0') goto LAB_00542954;
    iVar10 = FUN_0045bb50(param_1,0x1e,1);
    uVar3 = DAT_00d3b4e0;
    if (iVar10 != 0) {
      (&DAT_00bcd672)[iVar12] = 0x1e;
      *(undefined2 *)(&DAT_00bcd66c + iVar12) = 0;
      (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar10;
      uVar4 = DAT_00d3b4e4;
      uVar7 = CONCAT22((short)((uint)iVar10 >> 0x10),DAT_00d3b4e4);
      *(short *)(&DAT_00bcd660 + iVar12) = (short)iVar13;
      (&DAT_00bcd630)[iVar12] = 0xf;
      *(undefined2 *)(&DAT_00bcd62e + iVar12) = 0;
      *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar4;
      *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
      *(undefined2 *)(&DAT_00bcd660 + iVar13 * 0x184) = (undefined2)param_2;
      return CONCAT31((int3)((uint)uVar7 >> 8),1);
    }
    break;
  case 2: ;
  case 3: ;
  case 4: ;
  case 5: ;
  case 6: ;
  case 7: ;
    if (((uVar8 >> 2 & 1) == 0) || (uVar9 = (&DAT_00799a30)[param_1 * 0x42], uVar9 == 0)) {
      if ((((&DAT_00799a30)[param_1 * 0x42] == 0) &&
          (uVar9 = CONCAT31((int3)(uVar9 >> 8),local_3), local_3 != '\0')) &&
         (uVar9 = FUN_00542110(param_1), (char)uVar9 == '\0')) {
        uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
        if (param_3 != '\0') {
          (&DAT_00bcd630)[iVar12] = 0x18;
          *(int *)(&DAT_00799a34 + iVar13) = param_2;
          uVar7 = CONCAT22((short)(uVar9 >> 0x10),(&DAT_007999fe)[param_1 * 0x84]);
          uVar3 = (&DAT_00799a00)[param_1 * 0x84];
          uVar5 = *(undefined4 *)(&DAT_007999e4 + iVar13);
          *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
          *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
          *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar5;
          *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
        }
        return CONCAT31((int3)((uint)uVar7 >> 8),1);
      }
      goto switchD_0054225d_default;
    }
    if (uVar9 != 1) goto switchD_0054225d_default;
    if (local_3 == '\0') {
      uVar9 = FUN_00542110(param_1);
      if (((char)uVar9 == '\0') && ((&DAT_00bcd602)[iVar12] == 'Q')) {
        uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
        if (param_3 != '\0') {
          (&DAT_00bcd630)[iVar12] = 9;
          *(int *)(&DAT_00799a34 + iVar13) = param_2;
          uVar3 = (&DAT_00799a00)[param_1 * 0x84];
          uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
          *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
          *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
          *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
          *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
          return CONCAT31((int3)((uint)uVar7 >> 8),1);
        }
        goto LAB_00542a3d;
      }
      goto switchD_0054225d_default;
    }
    uVar9 = FUN_00542110(param_1);
    if ((char)uVar9 == '\0') goto switchD_0054225d_default;
    uVar8 = *(uint *)(&DAT_00799a34 + iVar13);
    if (*(short *)(&DAT_00bcd660 + uVar8 * 0x184) != 0) {
      uVar9 = *(short *)(&DAT_00bcd660 + uVar8 * 0x184) * 0x184;
      if ((((&DAT_00bcd606)[uVar9] != '\0') && ((&DAT_00bcd602)[uVar9] == 'U')) &&
         ((((cVar1 = (&DAT_00bcd630)[uVar9], cVar1 == '\x06' ||
            (((cVar1 == '\a' || (cVar1 == '\x0f')) || (cVar1 == '\x10')))) || (cVar1 == '\x17')) &&
          (uVar9 = (uint)*(short *)(&DAT_00bcd660 + uVar9), uVar9 == uVar8))))
      goto switchD_0054225d_default;
      *(undefined2 *)(&DAT_00bcd660 + uVar8 * 0x184) = 0;
    }
    uVar11 = (undefined3)(uVar9 >> 8);
    if (param_3 == '\0') goto LAB_00542954;
    iVar13 = FUN_0045bb50(param_1,0x1e,1);
    uVar3 = DAT_00d3b4e4;
    if (iVar13 != 0) {
      (&DAT_00bcd672)[iVar12] = 0x1e;
      *(undefined2 *)(&DAT_00bcd66c + iVar12) = 0;
      (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar13;
      uVar4 = DAT_00d3b4e0;
      uVar7 = CONCAT22((short)((uint)iVar13 >> 0x10),DAT_00d3b4e0);
      *(short *)(&DAT_00bcd660 + iVar12) = (short)uVar8;
      (&DAT_00bcd630)[iVar12] = 0xf;
      *(undefined2 *)(&DAT_00bcd62e + iVar12) = 0;
      *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar3;
      *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar4;
      *(undefined2 *)(&DAT_00bcd660 + uVar8 * 0x184) = (undefined2)param_2;
      return CONCAT31((int3)((uint)uVar7 >> 8),1);
    }
    break;
  case 8: ;
  case 9: ;
  case 10: ;
  case 0xb: ;
    uVar9 = CONCAT31(uVar11,local_3);
    if ((local_3 != '\0') && (uVar8 = FUN_00542110(param_1), uVar9 = uVar8, (char)uVar8 == '\0')) {
      local_1 = (char)(uVar6 >> 0x10);
      uVar11 = (undefined3)(uVar8 >> 8);
      if (local_1 == '\0') {
        uVar9 = (&DAT_00799a30)[param_1 * 0x42];
        if (uVar9 == 0) {
          uVar8 = 0;
          if (param_3 != '\0') {
            (&DAT_00bcd630)[iVar12] = 0x18;
            *(int *)(&DAT_00799a34 + iVar13) = param_2;
            uVar2 = (&DAT_00799a00)[param_1 * 0x84];
            uVar8 = (uint)uVar2;
            uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
            *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
            *(ushort *)(&DAT_00bcd61e + iVar12) = uVar2;
            *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
            *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
          }
          return CONCAT31((int3)(uVar8 >> 8),1);
        }
      }
      else {
        uVar9 = CONCAT31(uVar11,local_2);
        if ((local_2 != '\0') && ((&DAT_00799a30)[param_1 * 0x42] == 1)) {
          uVar7 = CONCAT31(uVar11,param_3);
          if (param_3 != '\0') {
            (&DAT_00bcd630)[iVar12] = 0x18;
            *(int *)(&DAT_00799a34 + iVar13) = param_2;
            uVar3 = (&DAT_007999fe)[param_1 * 0x84];
            uVar4 = (&DAT_00799a00)[param_1 * 0x84];
            uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
            *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar3;
            *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar4;
            *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
            *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
            return CONCAT31((int3)(CONCAT22((short)(uVar8 >> 0x10),uVar3) >> 8),1);
          }
          goto LAB_00542a3d;
        }
      }
    }
default: ;
switchD_0054225d_default: ;
    return uVar9 & 0xffffff00;
  case 0xc: ;
    if ((((char)uVar9 == '\0') || ((uVar8 >> 3 & 1) == 0)) ||
       (uVar9 = (&DAT_00799a30)[param_1 * 0x42], uVar9 == 0)) {
      if ((((&DAT_00799a30)[param_1 * 0x42] != 0) ||
          (uVar9 = CONCAT31((int3)(uVar9 >> 8),local_3), local_3 == '\0')) ||
         (uVar9 = FUN_00542110(param_1), (char)uVar9 != '\0')) goto switchD_0054225d_default;
      uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
      if (param_3 == '\0') goto LAB_00542a3d;
      (&DAT_00bcd630)[iVar12] = 0x18;
      *(int *)(&DAT_00799a34 + iVar13) = param_2;
LAB_00542a0e: ;
      uVar3 = (&DAT_00799a00)[param_1 * 0x84];
      uVar7 = CONCAT22((short)((uint)uVar7 >> 0x10),uVar3);
      uVar5 = *(undefined4 *)(&DAT_007999e4 + iVar13);
      *(undefined2 *)(&DAT_00bcd61c + iVar12) = (&DAT_007999fe)[param_1 * 0x84];
      *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
      *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar5;
      *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
LAB_00542a3d: ;
      return CONCAT31((int3)((uint)uVar7 >> 8),1);
    }
    if (((uVar9 != 1) && (uVar9 != 2)) && (uVar9 != 3)) goto switchD_0054225d_default;
    if (local_3 == '\0') {
      uVar9 = FUN_00542110(param_1);
      if (((char)uVar9 == '\0') && ((&DAT_00bcd602)[iVar12] == 'Q')) {
        uVar7 = CONCAT31((int3)(uVar9 >> 8),param_3);
        if (param_3 != '\0') {
          (&DAT_00bcd630)[iVar12] = 9;
          *(int *)(&DAT_00799a34 + iVar13) = param_2;
          uVar3 = (&DAT_007999fe)[param_1 * 0x84];
          uVar4 = (&DAT_00799a00)[param_1 * 0x84];
          uVar7 = *(undefined4 *)(&DAT_007999e4 + iVar13);
          *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar3;
          *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar4;
          *(undefined4 *)(&DAT_00bcd620 + iVar12) = uVar7;
          *(undefined2 *)(&DAT_00bcd63c + iVar12) = 0;
          return CONCAT31((int3)(CONCAT22((short)(uVar9 >> 0x10),uVar3) >> 8),1);
        }
        goto LAB_00542a3d;
      }
      goto switchD_0054225d_default;
    }
    uVar9 = FUN_00542110(param_1);
    if ((char)uVar9 == '\0') goto switchD_0054225d_default;
    iVar13 = *(int *)(&DAT_00799a34 + iVar13);
    if (*(short *)(&DAT_00bcd660 + iVar13 * 0x184) != 0) {
      uVar9 = *(short *)(&DAT_00bcd660 + iVar13 * 0x184) * 0x184;
      if (((((&DAT_00bcd606)[uVar9] != '\0') && ((&DAT_00bcd602)[uVar9] == 'U')) &&
          ((cVar1 = (&DAT_00bcd630)[uVar9], cVar1 == '\x06' ||
           ((((cVar1 == '\a' || (cVar1 == '\x0f')) || (cVar1 == '\x10')) || (cVar1 == '\x17'))))))
         && (*(short *)(&DAT_00bcd660 + uVar9) == iVar13)) goto switchD_0054225d_default;
      *(undefined2 *)(&DAT_00bcd660 + iVar13 * 0x184) = 0;
    }
    uVar11 = (undefined3)(uVar9 >> 8);
    if (param_3 == '\0') goto LAB_00542954;
    iVar10 = FUN_0045bb50(param_1,0x1e,1);
    uVar4 = DAT_00d3b4e4;
    uVar3 = DAT_00d3b4e0;
    if (iVar10 != 0) {
      (&DAT_00bcd672)[iVar12] = 0x1e;
      *(undefined2 *)(&DAT_00bcd66c + iVar12) = 0;
      (&DAT_00bcd656)[param_2 * 0xc2] = (short)iVar10;
      *(short *)(&DAT_00bcd660 + iVar12) = (short)iVar13;
      (&DAT_00bcd630)[iVar12] = 0xf;
      *(undefined2 *)(&DAT_00bcd62e + iVar12) = 0;
      *(undefined2 *)(&DAT_00bcd61c + iVar12) = uVar4;
      *(undefined2 *)(&DAT_00bcd61e + iVar12) = uVar3;
      (&DAT_00bcd66a)[iVar12] = 3;
      *(undefined2 *)(&DAT_00bcd660 + iVar13 * 0x184) = (undefined2)param_2;
      return CONCAT31((int3)(CONCAT22((short)((uint)iVar10 >> 0x10),(undefined2)param_2) >> 8),1);
    }
  }
  uVar11 = 0;
  (&DAT_00bcd606)[iVar12] = 2;
LAB_00542954: ;
  return CONCAT31(uVar11,1);
}
