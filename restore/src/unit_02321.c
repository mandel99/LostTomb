#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00eae010 (*(undefined4 *)0x00eae010u)
#define DAT_00ee0b6c (*(undefined4 *)0x00ee0b6cu)
#define DAT_00ee0c4f (*(undefined4 *)0x00ee0c4fu)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define DAT_00ee0d34 (*(undefined4 *)0x00ee0d34u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();
extern undefined4 FUN_0051c390();
extern undefined4 FUN_0051c510();
extern undefined4 FUN_0051c630();

undefined4 FUN_0051b680(undefined4 param_1,int param_2,int param_3,int param_4)

{
  int *piVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  bool bVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  uVar3 = (&DAT_00af598c)[DAT_00eae004];
  if (((((((uVar3 & 0x12040011) != 0) || (*(char *)((int)&DAT_00ee0c50 + DAT_00eae004) == -1)) ||
        (((uVar3 & 0x100) != 0 && (iVar7 = FUN_0051c390(DAT_00eae004), iVar7 == 0)))) &&
       ((param_4 == 0 ||
        (((uVar3 & 0x100) == 0 &&
         (((uVar3 & 8) == 0 ||
          (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + DAT_00eae004 * 2) * 0x84] != 0x5a &&
           ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + DAT_00eae004 * 2) * 0x84] != 0x8a))))))))))
      || (*(char *)((int)&DAT_00ee0c50 + DAT_00eae004) == '\x02')) ||
     ((((((uVar3 & 0x100) != 0 && (param_4 == 0)) &&
        (iVar7 = FUN_0051c390(DAT_00eae004), iVar7 == 0)) ||
       ((*(char *)((int)&DAT_00ee0c50 + DAT_00eae004) == -5 &&
        ((param_4 != 0 || (iVar7 = FUN_0051c510(DAT_00eae004), iVar7 == 0)))))) ||
      (((uVar3 & 0x40) != 0 && ((param_4 != 0 && (iVar7 = FUN_0051c630(DAT_00eae004), iVar7 == 0))))
      )))) {
    return 0;
  }
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  FUN_0040f8f0(&DAT_00eae010,0xcb10,0);
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar4 = (&DAT_00f203a0)[DAT_00eae00c];
    iVar5 = (&DAT_00eed760)[iVar4];
    iVar7 = iVar4 + -0xe4;
    iVar8 = iVar5 + 1;
    piVar1 = &DAT_00eed760 + iVar7;
    if (*piVar1 == 0) {
      bVar6 = false;
      if ((((&DAT_00af598c)[iVar7] & 0x100) == 0) || (iVar9 = FUN_0051c390(iVar7), iVar9 == 0)) {
        cVar2 = (&DAT_00ee0b6c)[iVar4];
        if (cVar2 == -5) {
          if (param_4 == 0) {
            iVar9 = FUN_0051c510(iVar7);
            goto LAB_0051b868;
          }
        }
        else if ((((cVar2 != '\x02') && (cVar2 != -1)) &&
                 (((&DAT_00af598c)[iVar7] & 0x12040011) == 0)) &&
                ((((&DAT_00af598c)[iVar7] & 8) == 0 || ((cVar2 == -6 && (param_4 == 1))))))
        goto LAB_0051b8a3;
LAB_0051b89e: ;
        bVar6 = true;
      }
      else if (param_4 == 0) {
        iVar9 = FUN_0051c390(iVar7);
LAB_0051b868: ;
        if (iVar9 == 0) {
          *piVar1 = -1;
          goto LAB_0051b89e;
        }
      }
LAB_0051b8a3: ;
      if ((((*(byte *)(&DAT_00af598c + iVar7) & 0x40) == 0) || (param_4 == 0)) ||
         (iVar9 = FUN_0051c630(iVar7), iVar9 != 0)) {
        if (!bVar6) {
          *piVar1 = iVar8;
          if ((&DAT_00ee0b6c)[iVar4] == -2) {
            *piVar1 = iVar5 + 5;
          }
          (&DAT_00f203a0)[DAT_00eae008] = iVar7;
          DAT_00eae008 = DAT_00eae008 + 1;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
          }
        }
      }
      else {
        *piVar1 = -1;
      }
    }
    iVar7 = iVar4 + 1;
    if ((&DAT_00eed760)[iVar7] == 0) {
      bVar6 = false;
      if (((*(uint *)(&DAT_00af5990 + iVar4 * 4) & 0x100) == 0) ||
         (iVar9 = FUN_0051c390(iVar7), iVar9 == 0)) {
        cVar2 = *(char *)((int)&DAT_00ee0c50 + iVar4 + 1);
        if (cVar2 == -5) {
          if (param_4 == 0) {
            iVar9 = FUN_0051c510(iVar7);
            goto LAB_0051b95c;
          }
        }
        else if ((((cVar2 != '\x02') && (cVar2 != -1)) &&
                 ((*(uint *)(&DAT_00af5990 + iVar4 * 4) & 0x12040011) == 0)) &&
                (((*(uint *)(&DAT_00af5990 + iVar4 * 4) & 8) == 0 ||
                 ((cVar2 == -4 && (param_4 == 1)))))) goto LAB_0051b99c;
LAB_0051b997: ;
        bVar6 = true;
      }
      else if (param_4 == 0) {
        iVar9 = FUN_0051c390(iVar7);
LAB_0051b95c: ;
        if (iVar9 == 0) {
          (&DAT_00eed760)[iVar7] = 0xffffffff;
          goto LAB_0051b997;
        }
      }
LAB_0051b99c: ;
      if (((((&DAT_00af5990)[iVar4 * 4] & 0x40) == 0) || (param_4 == 0)) ||
         (iVar9 = FUN_0051c630(iVar7), iVar9 != 0)) {
        if (!bVar6) {
          (&DAT_00eed760)[iVar7] = iVar8;
          if (*(char *)((int)&DAT_00ee0c50 + iVar4 + 1) == -2) {
            (&DAT_00eed760)[iVar7] = iVar5 + 5;
          }
          (&DAT_00f203a0)[DAT_00eae008] = iVar7;
          DAT_00eae008 = DAT_00eae008 + 1;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
          }
        }
      }
      else {
        (&DAT_00eed760)[iVar7] = 0xffffffff;
      }
    }
    iVar7 = iVar4 + 0xe4;
    if ((&DAT_00eed760)[iVar7] == 0) {
      bVar6 = false;
      if (((*(uint *)(&DAT_00af5d1c + iVar4 * 4) & 0x100) == 0) ||
         (iVar9 = FUN_0051c390(iVar7), iVar9 == 0)) {
        cVar2 = (&DAT_00ee0d34)[iVar4];
        if (cVar2 == -5) {
          if (param_4 == 0) {
            iVar9 = FUN_0051c510(iVar7);
            goto LAB_0051ba67;
          }
        }
        else if ((((cVar2 != '\x02') && (cVar2 != -1)) &&
                 ((*(uint *)(&DAT_00af5d1c + iVar4 * 4) & 0x12040011) == 0)) &&
                (((*(uint *)(&DAT_00af5d1c + iVar4 * 4) & 8) == 0 ||
                 ((cVar2 == -4 && (param_4 == 1)))))) goto LAB_0051baa7;
LAB_0051baa2: ;
        bVar6 = true;
      }
      else if (param_4 == 0) {
        iVar9 = FUN_0051c390(iVar7);
LAB_0051ba67: ;
        if (iVar9 == 0) {
          (&DAT_00eed760)[iVar7] = 0xffffffff;
          goto LAB_0051baa2;
        }
      }
LAB_0051baa7: ;
      if (((((&DAT_00af5d1c)[iVar4 * 4] & 0x40) == 0) || (param_4 == 0)) ||
         (iVar9 = FUN_0051c630(iVar7), iVar9 != 0)) {
        if (!bVar6) {
          (&DAT_00eed760)[iVar7] = iVar8;
          if ((&DAT_00ee0d34)[iVar4] == -2) {
            (&DAT_00eed760)[iVar7] = iVar5 + 5;
          }
          (&DAT_00f203a0)[DAT_00eae008] = iVar7;
          DAT_00eae008 = DAT_00eae008 + 1;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
          }
        }
      }
      else {
        (&DAT_00eed760)[iVar7] = 0xffffffff;
      }
    }
    iVar7 = iVar4 * 4;
    if (*(int *)(&DAT_00eed75c + iVar4 * 4) == 0) {
      bVar6 = false;
      if ((*(uint *)(&DAT_00af5988 + iVar4 * 4) & 0x100) == 0) {
LAB_0051bb5e: ;
        cVar2 = (&DAT_00ee0c4f)[iVar4];
        if (cVar2 == -5) {
          if (param_4 == 0) {
            iVar9 = FUN_0051c510(iVar4 + -1);
            goto LAB_0051bb79;
          }
        }
        else if ((((cVar2 != '\x02') && (cVar2 != -1)) &&
                 ((*(uint *)(&DAT_00af5988 + iVar4 * 4) & 0x12040011) == 0)) &&
                (((*(uint *)(&DAT_00af5988 + iVar4 * 4) & 8) == 0 ||
                 ((cVar2 == -4 && (param_4 == 1)))))) goto LAB_0051bbb5;
LAB_0051bbb0: ;
        bVar6 = true;
      }
      else {
        iVar9 = FUN_0051c390(iVar4 + -1);
        if (iVar9 == 0) goto LAB_0051bb5e;
        if (param_4 == 0) {
          iVar9 = FUN_0051c390(iVar4 + -1);
LAB_0051bb79: ;
          if (iVar9 == 0) {
            *(undefined4 *)(&DAT_00eed75c + iVar7) = 0xffffffff;
            goto LAB_0051bbb0;
          }
        }
      }
LAB_0051bbb5: ;
      if (((((&DAT_00af5988)[iVar4 * 4] & 0x40) == 0) || (param_4 == 0)) ||
         (iVar9 = FUN_0051c630(iVar4 + -1), iVar9 != 0)) {
        if (!bVar6) {
          *(int *)(&DAT_00eed75c + iVar7) = iVar8;
          if ((&DAT_00ee0c4f)[iVar4] == -2) {
            *(int *)(&DAT_00eed75c + iVar7) = iVar5 + 5;
          }
          (&DAT_00f203a0)[DAT_00eae008] = iVar4 + -1;
          DAT_00eae008 = DAT_00eae008 + 1;
          if (0xcb0f < DAT_00eae008) {
            DAT_00eae008 = 0;
          }
        }
      }
      else {
        *(undefined4 *)(&DAT_00eed75c + iVar7) = 0xffffffff;
      }
    }
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
    if (DAT_00eae00c == DAT_00eae008) {
      return 1;
    }
  } while( true );
}
