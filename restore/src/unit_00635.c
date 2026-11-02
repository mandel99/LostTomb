#include "restore_types.h"
#define DAT_00776fb0 (*(undefined4 *)0x00776fb0u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_007843d8 (*(undefined4 *)0x007843d8u)
#define DAT_00784f10 (*(undefined *)0x00784f10u)
#define PTR_s_Pharaoh2_emp_005d2a28 (*(unsigned char * *)0x005d2a28u)
extern undefined4 FUN_00407f90();
extern int FUN_00445d10();
extern undefined4 FUN_004ce920();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

void FUN_004444e0(void)

{
  byte bVar1;
  void *pvVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  byte *pbVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_3c;
  undefined1 auStack_34 [52];
  
  FUN_004ce920(PTR_s_Pharaoh2_emp_005d2a28,&DAT_007843d8,0x500,0);
  FUN_004ce920(PTR_s_Pharaoh2_emp_005d2a28,&DAT_0077f748,0x4c90,DAT_00784f10 * 0x4c90 + 0x500);
  DAT_007770f1 = 0;
  DAT_007770e4 = 0;
  piVar8 = &DAT_00776fb4;
  do {
    if (*piVar8 != 0) {
      FUN_00561a2b(*piVar8);
      *piVar8 = 0;
      piVar8[0x4d] = 0;
    }
    piVar8 = piVar8 + 0x51;
  } while ((int)piVar8 < 0x77aefc);
  FUN_004ce920(PTR_s_Pharaoh2_emp_005d2a28,&DAT_00776fb0,0x4c90,DAT_00784f10 * 0x3f48 + 0xbfb80);
  pbVar6 = &DAT_007770f1;
  do {
    iVar9 = 0;
    if (((((*(int *)(pbVar6 + -0x13d) == 0) && (*(int *)(pbVar6 + -9) == 0)) &&
         (bVar1 = *pbVar6, bVar1 < 0x33)) && ((pbVar6[-1] < 3 && (pbVar6[-0x141] != 0)))) &&
       (pbVar6[-0x141] < 0x33)) {
      iVar7 = 1;
      if (1 < bVar1) {
        pbVar5 = pbVar6 + -0x139;
        do {
          local_3c = (int)*(short *)pbVar5 - (int)*(short *)(pbVar5 + 6);
          iVar3 = (int)*(short *)(pbVar5 + 6) - (int)*(short *)pbVar5;
          if (local_3c <= iVar3) {
            local_3c = iVar3;
          }
          iVar3 = (int)*(short *)(pbVar5 + 2) - (int)*(short *)(pbVar5 + 8);
          iVar4 = (int)*(short *)(pbVar5 + 8) - (int)*(short *)(pbVar5 + 2);
          if (iVar3 <= iVar4) {
            iVar3 = iVar4;
          }
          if (iVar3 < local_3c) {
            iVar9 = iVar9 + 1 + local_3c;
            auStack_34[iVar7] = 1;
          }
          else {
            iVar9 = iVar9 + 1 + iVar3;
            auStack_34[iVar7] = 0;
          }
          iVar7 = iVar7 + 1;
          pbVar5 = pbVar5 + 6;
        } while (iVar7 < (int)(uint)bVar1);
      }
      if ((*(int *)(pbVar6 + -9) < iVar9) && (iVar7 = iVar9 + 0x32, *(int *)(pbVar6 + -9) < iVar7))
      {
        if (*(int *)(pbVar6 + -0x13d) != 0) {
          FUN_00561a2b(*(int *)(pbVar6 + -0x13d));
          pbVar6[-0xffffffff0000013d] = 0;
          pbVar6[-0xffffffff0000013c] = 0;
          pbVar6[-0xffffffff0000013b] = 0;
          pbVar6[-0xffffffff0000013a] = 0;
        }
        pvVar2 = FUN_00561a1d(iVar7 * 6);
        *(void **)(pbVar6 + -0x13d) = pvVar2;
        *(int *)(pbVar6 + -9) = iVar7;
      }
      *(int *)(pbVar6 + -0xd) = iVar9;
      iVar7 = 0;
      iVar9 = 1;
      if (1 < *pbVar6) {
        do {
          if (*(int *)(pbVar6 + -0xd) < iVar7) goto LAB_004446da;
          iVar3 = FUN_00445d10(iVar9 + -1,iVar9,iVar7);
          iVar7 = iVar7 + iVar3;
          iVar9 = iVar9 + 1;
        } while (iVar9 < (int)(uint)*pbVar6);
      }
      *(int *)(pbVar6 + -0xd) = iVar7;
      FUN_00407f90();
    }
    else {
      pbVar6[-0xffffffff0000000d] = 0;
      pbVar6[-0xffffffff0000000c] = 0;
      pbVar6[-0xffffffff0000000b] = 0;
      pbVar6[-0xffffffff0000000a] = 0;
      pbVar6[-0xffffffff0000013d] = 0;
      pbVar6[-0xffffffff0000013c] = 0;
      pbVar6[-0xffffffff0000013b] = 0;
      pbVar6[-0xffffffff0000013a] = 0;
      *pbVar6 = 0;
      pbVar6[-0xffffffff00000009] = 0;
      pbVar6[-0xffffffff00000008] = 0;
      pbVar6[-0xffffffff00000007] = 0;
      pbVar6[-0xffffffff00000006] = 0;
      pbVar6[-0x141] = 5;
      pbVar6[1] = 0;
      pbVar6[-0xffffffff00000005] = 0xff;
      pbVar6[-0xffffffff00000004] = 0xff;
      pbVar6[-0xffffffff00000003] = 0xff;
      pbVar6[-0xffffffff00000002] = 0xff;
      pbVar6[-1] = 1;
    }
LAB_004446da: ;
    pbVar6 = pbVar6 + 0x144;
    if (0x77b038 < (int)pbVar6) {
      return;
    }
  } while( true );
}
