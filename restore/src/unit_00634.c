#include "restore_types.h"
#include "world_route_geometry.h"
#define DAT_00776fb0 (*(undefined4 *)0x00776fb0u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_007843d8 (*(undefined4 *)0x007843d8u)
#define DAT_00784f10 (*(undefined *)0x00784f10u)
#define PTR_s_Pharaoh2_emp_005d2a28 (*(unsigned char * *)0x005d2a28u)
extern void FUN_0040f860();
extern void FUN_00442660();
extern void FUN_004426c0();
extern void FUN_004444e0();
extern int FUN_00445d10();
extern int FUN_004ce480();
extern void FUN_004d19e0();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

void FUN_00444270(int param_1)

{
  byte bVar1;
  void *pvVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  undefined1 *puVar6;
  byte *pbVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  undefined1 auStack_34 [52];
  
  if ((param_1 < 0x28) && (-1 < param_1)) {
    FUN_0040f860(&DAT_007843d8 + param_1 * 0x20,&DAT_007843d8 + DAT_00784f10 * 0x20,0x20);
    puVar6 = &DAT_0077f748;
    iVar9 = 0x500;
    do {
      FUN_004ce480(PTR_s_Pharaoh2_emp_005d2a28,puVar6,0x4c,iVar9);
      iVar9 = iVar9 + 0x4c;
      puVar6 = puVar6 + 0x62;
    } while (iVar9 < 0x4060);
    FUN_004d19e0();
    piVar10 = &DAT_00776fb4;
    do {
      if (*piVar10 != 0) {
        FUN_00561a2b(*piVar10);
        *piVar10 = 0;
        piVar10[0x4d] = 0;
      }
      piVar10 = piVar10 + 0x51;
    } while ((int)piVar10 < 0x77aefc);
    FUN_004ce480(PTR_s_Pharaoh2_emp_005d2a28,&DAT_00776fb0,0x4c90,param_1 * 0x3f48 + 0xbfb80);
    DAT_007770f1 = 0;
    DAT_007770e4 = 0;
    pbVar7 = &DAT_007770f1;
    do {
      iVar9 = 0;
      if (((((*(int *)(pbVar7 + -0x13d) == 0) && (*(int *)(pbVar7 + -9) == 0)) &&
           (bVar1 = *pbVar7, bVar1 < 0x33)) && ((pbVar7[-1] < 3 && (pbVar7[-0x141] != 0)))) &&
         (pbVar7[-0x141] < 0x33)) {
        iVar8 = 1;
        if (1 < bVar1) {
          pbVar5 = pbVar7 + -0x139;
          do {
            param_1 = (int)*(short *)pbVar5 - (int)*(short *)(pbVar5 + 6);
            iVar3 = (int)*(short *)(pbVar5 + 6) - (int)*(short *)pbVar5;
            if (param_1 <= iVar3) {
              param_1 = iVar3;
            }
            iVar3 = (int)*(short *)(pbVar5 + 2) - (int)*(short *)(pbVar5 + 8);
            iVar4 = (int)*(short *)(pbVar5 + 8) - (int)*(short *)(pbVar5 + 2);
            if (iVar3 <= iVar4) {
              iVar3 = iVar4;
            }
            if (iVar3 < param_1) {
              iVar9 = iVar9 + 1 + param_1;
              auStack_34[iVar8] = 1;
            }
            else {
              iVar9 = iVar9 + 1 + iVar3;
              auStack_34[iVar8] = 0;
            }
            iVar8 = iVar8 + 1;
            pbVar5 = pbVar5 + 6;
          } while (iVar8 < (int)(uint)bVar1);
        }
        if ((*(int *)(pbVar7 + -9) < iVar9) && (iVar8 = iVar9 + 0x32, *(int *)(pbVar7 + -9) < iVar8)
           ) {
          if (*(int *)(pbVar7 + -0x13d) != 0) {
            FUN_00561a2b(*(int *)(pbVar7 + -0x13d));
            pbVar7[-0xffffffff0000013d] = 0;
            pbVar7[-0xffffffff0000013c] = 0;
            pbVar7[-0xffffffff0000013b] = 0;
            pbVar7[-0xffffffff0000013a] = 0;
          }
          pvVar2 = FUN_00561a1d(iVar8 * 6);
          *(void **)(pbVar7 + -0x13d) = pvVar2;
          *(int *)(pbVar7 + -9) = iVar8;
        }
        *(int *)(pbVar7 + -0xd) = iVar9;
        iVar8 = 0;
        iVar9 = 1;
        if (1 < *pbVar7) {
          do {
            if (*(int *)(pbVar7 + -0xd) < iVar8) goto LAB_00444482;
            iVar3 = FUN_00445d10(iVar9 + -1,iVar9,iVar8);
            iVar8 = iVar8 + iVar3;
            iVar9 = iVar9 + 1;
          } while (iVar9 < (int)(uint)*pbVar7);
        }
        *(int *)(pbVar7 + -0xd) = iVar8;
        FUN_00407f90((RestoreWorldRoute *)(pbVar7-0x141));
      }
      else {
        pbVar7[-0xffffffff0000000d] = 0;
        pbVar7[-0xffffffff0000000c] = 0;
        pbVar7[-0xffffffff0000000b] = 0;
        pbVar7[-0xffffffff0000000a] = 0;
        pbVar7[-0xffffffff0000013d] = 0;
        pbVar7[-0xffffffff0000013c] = 0;
        pbVar7[-0xffffffff0000013b] = 0;
        pbVar7[-0xffffffff0000013a] = 0;
        *pbVar7 = 0;
        pbVar7[-0xffffffff00000009] = 0;
        pbVar7[-0xffffffff00000008] = 0;
        pbVar7[-0xffffffff00000007] = 0;
        pbVar7[-0xffffffff00000006] = 0;
        pbVar7[-0x141] = 5;
        pbVar7[1] = 0;
        pbVar7[-0xffffffff00000005] = 0xff;
        pbVar7[-0xffffffff00000004] = 0xff;
        pbVar7[-0xffffffff00000003] = 0xff;
        pbVar7[-0xffffffff00000002] = 0xff;
        pbVar7[-1] = 1;
      }
LAB_00444482: ;
      pbVar7 = pbVar7 + 0x144;
    } while ((int)pbVar7 < 0x77b039);
    FUN_00442660();
    FUN_004426c0();
    FUN_004444e0();
  }
  return;
}
