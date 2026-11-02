#include "restore_types.h"
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ed405c (*(undefined4 *)0x00ed405cu)
#define DAT_00ed413f (*(undefined4 *)0x00ed413fu)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
#define DAT_00ed4141 (*(undefined4 *)0x00ed4141u)
#define DAT_00ed4224 (*(undefined4 *)0x00ed4224u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();

undefined4 FUN_0051de70(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_180 [96];
  
  iVar8 = 0;
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  param_3 = DAT_00bca2d8 + param_4 * 0xe4 + param_3;
  if ((&DAT_00ed4140)[DAT_00eae004] == -1) {
    return 0;
  }
  iVar7 = 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar8 = iVar8 + 1;
    if (50000 < iVar8) {
      return 1;
    }
    iVar4 = (&DAT_00f203a0)[DAT_00eae00c];
    iVar2 = iVar4 * 4;
    iVar1 = iVar4 + -0xe4;
    iVar5 = (&DAT_00eed760)[iVar4] + 1;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if ((iVar1 == DAT_00eae004) || (iVar1 == param_3)) {
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      else {
        if ((((&DAT_00ed405c)[iVar4] == -1) || (((&DAT_00af598c)[iVar1] & 0x10000) != 0)) ||
           (((&DAT_00af598c)[iVar1] & 0x40) != 0)) goto LAB_0051e13a;
        local_180[0x30] = 0xffffff1c;
        local_180[0x31] = 0xffffff1d;
        local_180[0x32] = 1;
        local_180[0x33] = 0xe5;
        local_180[0x34] = 0xe4;
        local_180[0x35] = 0xe3;
        local_180[0x36] = 0xffffffff;
        local_180[0x37] = 0xffffff1b;
        local_180[0x38] = 0xfffffe38;
        local_180[0x39] = 0xfffffe39;
        local_180[0x3a] = 0xfffffe3a;
        local_180[0x3b] = 0xffffff1e;
        local_180[0x3c] = 2;
        local_180[0x3d] = 0xe6;
        local_180[0x3e] = 0x1ca;
        local_180[0x3f] = 0x1c9;
        local_180[0x40] = 0x1c8;
        local_180[0x41] = 0x1c7;
        local_180[0x42] = 0x1c6;
        local_180[0x43] = 0xe2;
        local_180[0x44] = 0xfffffffe;
        local_180[0x45] = 0xffffff1a;
        local_180[0x46] = 0xfffffe36;
        local_180[0x47] = 0xfffffe37;
        iVar9 = 0;
        piVar6 = local_180 + 0x30;
        do {
          iVar3 = iVar4 + -0xe4 + *piVar6;
          if (((&DAT_00af598c)[iVar3] & 0x40) != 0) goto LAB_0051e13a;
          if ((iVar3 == DAT_00eae004) || (iVar3 == param_3)) break;
          if (((&DAT_00ed4140)[iVar3] == -1) || (((&DAT_00af598c)[iVar3] & 0x10000) != 0))
          goto LAB_0051e13a;
          iVar9 = iVar9 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar9 < 8);
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      iVar7 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar7;
      if (0xcb0f < iVar7) {
        iVar7 = 0;
        DAT_00eae008 = 0;
      }
    }
LAB_0051e13a: ;
    iVar1 = iVar4 + 1;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if ((iVar1 == DAT_00eae004) || (iVar1 == param_3)) {
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      else {
        if ((((&DAT_00ed4141)[iVar4] == -1) ||
            ((*(uint *)(&DAT_00af5990 + iVar4 * 4) & 0x10000) != 0)) ||
           ((*(uint *)(&DAT_00af5990 + iVar4 * 4) & 0x40) != 0)) goto LAB_0051e2e7;
        local_180[0] = -0xe4;
        local_180[1] = 0xffffff1d;
        local_180[2] = 1;
        local_180[3] = 0xe5;
        local_180[4] = 0xe4;
        local_180[5] = 0xe3;
        local_180[6] = 0xffffffff;
        local_180[7] = 0xffffff1b;
        local_180[8] = 0xfffffe38;
        local_180[9] = 0xfffffe39;
        local_180[10] = 0xfffffe3a;
        local_180[0xb] = 0xffffff1e;
        local_180[0xc] = 2;
        local_180[0xd] = 0xe6;
        local_180[0xe] = 0x1ca;
        local_180[0xf] = 0x1c9;
        local_180[0x10] = 0x1c8;
        local_180[0x11] = 0x1c7;
        local_180[0x12] = 0x1c6;
        local_180[0x13] = 0xe2;
        local_180[0x14] = 0xfffffffe;
        local_180[0x15] = 0xffffff1a;
        local_180[0x16] = 0xfffffe36;
        local_180[0x17] = 0xfffffe37;
        iVar9 = 0;
        piVar6 = local_180;
        do {
          iVar3 = *piVar6 + 1 + iVar4;
          if (((&DAT_00af598c)[iVar3] & 0x40) != 0) goto LAB_0051e2e7;
          if ((iVar3 == DAT_00eae004) || (iVar3 == param_3)) break;
          if (((&DAT_00ed4140)[iVar3] == -1) || (((&DAT_00af598c)[iVar3] & 0x10000) != 0))
          goto LAB_0051e2e7;
          iVar9 = iVar9 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar9 < 8);
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      iVar7 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar7;
      if (0xcb0f < iVar7) {
        iVar7 = 0;
        DAT_00eae008 = 0;
      }
    }
LAB_0051e2e7: ;
    iVar1 = iVar4 + 0xe4;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if ((iVar1 == DAT_00eae004) || (iVar1 == param_3)) {
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      else {
        if ((((&DAT_00ed4224)[iVar4] == -1) ||
            ((*(uint *)(&DAT_00af5d1c + iVar4 * 4) & 0x10000) != 0)) ||
           ((*(uint *)(&DAT_00af5d1c + iVar4 * 4) & 0x40) != 0)) goto LAB_0051e4e5;
        local_180[0x18] = 0xffffff1c;
        local_180[0x19] = 0xffffff1d;
        local_180[0x1a] = 1;
        local_180[0x1b] = 0xe5;
        local_180[0x1c] = 0xe4;
        local_180[0x1d] = 0xe3;
        local_180[0x1e] = 0xffffffff;
        local_180[0x1f] = 0xffffff1b;
        local_180[0x20] = 0xfffffe38;
        local_180[0x21] = 0xfffffe39;
        local_180[0x22] = 0xfffffe3a;
        local_180[0x23] = 0xffffff1e;
        local_180[0x24] = 2;
        local_180[0x25] = 0xe6;
        local_180[0x26] = 0x1ca;
        local_180[0x27] = 0x1c9;
        local_180[0x28] = 0x1c8;
        local_180[0x29] = 0x1c7;
        local_180[0x2a] = 0x1c6;
        local_180[0x2b] = 0xe2;
        local_180[0x2c] = 0xfffffffe;
        local_180[0x2d] = 0xffffff1a;
        local_180[0x2e] = 0xfffffe36;
        local_180[0x2f] = 0xfffffe37;
        iVar9 = 0;
        piVar6 = local_180 + 0x18;
        do {
          iVar3 = *piVar6 + 0xe4 + iVar4;
          if (((&DAT_00af598c)[iVar3] & 0x40) != 0) goto LAB_0051e4e5;
          if ((iVar3 == DAT_00eae004) || (iVar3 == param_3)) break;
          if (((&DAT_00ed4140)[iVar3] == -1) || (((&DAT_00af598c)[iVar3] & 0x10000) != 0))
          goto LAB_0051e4e5;
          iVar9 = iVar9 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar9 < 8);
        (&DAT_00eed760)[iVar1] = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      iVar7 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar7;
      if (0xcb0f < iVar7) {
        iVar7 = 0;
        DAT_00eae008 = 0;
      }
    }
LAB_0051e4e5: ;
    if (*(int *)(&DAT_00eed75c + iVar2) == 0) {
      iVar1 = iVar4 + -1;
      if ((iVar1 == DAT_00eae004) || (iVar1 == param_3)) {
        *(int *)(&DAT_00eed75c + iVar2) = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      else {
        if ((((&DAT_00ed413f)[iVar4] == -1) ||
            ((*(uint *)(&DAT_00af5988 + iVar4 * 4) & 0x10000) != 0)) ||
           ((*(uint *)(&DAT_00af5988 + iVar4 * 4) & 0x40) != 0)) goto LAB_0051e6db;
        local_180[0x48] = 0xffffff1c;
        local_180[0x49] = 0xffffff1d;
        local_180[0x4a] = 1;
        local_180[0x4b] = 0xe5;
        local_180[0x4c] = 0xe4;
        local_180[0x4d] = 0xe3;
        local_180[0x4e] = 0xffffffff;
        local_180[0x4f] = 0xffffff1b;
        local_180[0x50] = 0xfffffe38;
        local_180[0x51] = 0xfffffe39;
        local_180[0x52] = 0xfffffe3a;
        local_180[0x53] = 0xffffff1e;
        local_180[0x54] = 2;
        local_180[0x55] = 0xe6;
        local_180[0x56] = 0x1ca;
        local_180[0x57] = 0x1c9;
        local_180[0x58] = 0x1c8;
        local_180[0x59] = 0x1c7;
        local_180[0x5a] = 0x1c6;
        local_180[0x5b] = 0xe2;
        local_180[0x5c] = 0xfffffffe;
        local_180[0x5d] = 0xffffff1a;
        local_180[0x5e] = 0xfffffe36;
        local_180[0x5f] = 0xfffffe37;
        iVar9 = 0;
        piVar6 = local_180 + 0x48;
        do {
          iVar3 = *piVar6 + -1 + iVar4;
          if (((&DAT_00af598c)[iVar3] & 0x40) != 0) goto LAB_0051e6db;
          if ((iVar3 == DAT_00eae004) || (iVar3 == param_3)) break;
          if (((&DAT_00ed4140)[iVar3] == -1) || (((&DAT_00af598c)[iVar3] & 0x10000) != 0))
          goto LAB_0051e6db;
          iVar9 = iVar9 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar9 < 8);
        *(int *)(&DAT_00eed75c + iVar2) = iVar5;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      }
      iVar7 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar7;
      if (0xcb0f < iVar7) {
        iVar7 = 0;
        DAT_00eae008 = 0;
      }
    }
LAB_0051e6db: ;
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
    if (DAT_00eae00c == iVar7) {
      return 1;
    }
  } while( true );
}
