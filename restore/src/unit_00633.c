#include "restore_types.h"
#include "world_route_geometry.h"
#define LAB_0056b65b (*(undefined1 *)0x0056b65bu)
#define DAT_00776fb0 (*(undefined4 *)0x00776fb0u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_007843d8 (*(undefined4 *)0x007843d8u)
#define DAT_00784f10 (*(undefined *)0x00784f10u)
#define PTR_DAT_005d2ec0 (*(unsigned char * *)0x005d2ec0u)
#define PTR_s_Pharaoh2_emp_005d2a28 (*(unsigned char * *)0x005d2a28u)
#define PTR_s_Pharaoh_emp_005d2a24 (*(unsigned char * *)0x005d2a24u)
extern void FUN_0040f8f0();
extern void FUN_00442660();
extern void FUN_004426c0();
extern int FUN_00445d10();
extern int FUN_004ce480();
extern undefined4 FUN_004ce980();
extern void FUN_004d19e0();
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern int FUN_0056b1b6();
extern undefined4 FUN_0056b22e();

void FUN_00443ea0(int param_1)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  void *pvVar5;
  int iVar6;
  byte *pbVar7;
  undefined1 *puVar8;
  byte *pbVar9;
  undefined *puVar10;
  int iVar11;
  int *piVar12;
  int local_18c;
  undefined1 auStack_184 [52];
  undefined1 local_150 [4];
  int local_14c;
  undefined4 local_1c;
  undefined4 local_18;
  undefined2 local_14;
  undefined2 local_12;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0056b65b;
  local_c = ExceptionList;
  puVar10 = PTR_s_Pharaoh2_emp_005d2a28;
  if (param_1 == 0) {
    puVar10 = PTR_s_Pharaoh_emp_005d2a24;
  }
  ExceptionList = &local_c;
  iVar2 = FUN_00561a0a(puVar10,&PTR_DAT_005d2ec0);
  if (iVar2 != 0) {
    uVar3 = FUN_0056b22e(iVar2);
    iVar4 = FUN_0056b1b6(uVar3);
    FUN_00561994(iVar2);
    if (((iVar4 != 0x185790) && (iVar4 != 0x1d2860)) && (iVar4 < 0x185790)) {
      iVar2 = -((iVar4 + -0x500) / 0x4c90);
      iVar11 = iVar2 + 0x28;
      iVar4 = (iVar4 + -0x500) % 0x4c90;
      FUN_0040f8f0(&DAT_0077f748,0x4c90,0);
      if (iVar4 != 0) {
        FUN_004ce980(puVar10,&DAT_0077f748,0x4c90 - iVar4);
        iVar11 = iVar2 + 0x27;
      }
      if (0 < iVar11) {
        do {
          FUN_004ce980(puVar10,&DAT_0077f748,0x4c90);
          iVar11 = iVar11 + -1;
        } while (iVar11 != 0);
      }
      local_1c = 0;
      local_14c = 0;
      local_f = 0;
      local_18 = 0;
      local_150[0] = 5;
      local_e = 0;
      local_14 = 0xffff;
      local_12 = 0xffff;
      local_10 = 1;
      local_4 = 0;
      iVar2 = 2000;
      do {
        FUN_004ce980(puVar10,local_150,0x144);
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      local_4 = 0xffffffff;
      if (local_14c != 0) {
        FUN_00561a2b(local_14c);
      }
    }
  }
  FUN_004ce480(puVar10,&DAT_007843d8,0x500,0);
  FUN_0040f8f0(&DAT_0077f748,0x4c90,0);
  puVar8 = &DAT_0077f748;
  iVar2 = 0x500;
  do {
    FUN_004ce480(PTR_s_Pharaoh2_emp_005d2a28,puVar8,0x4c,iVar2);
    iVar2 = iVar2 + 0x4c;
    puVar8 = puVar8 + 0x62;
  } while (iVar2 < 0x4060);
  FUN_004d19e0();
  piVar12 = &DAT_00776fb4;
  do {
    if (*piVar12 != 0) {
      FUN_00561a2b(*piVar12);
      *piVar12 = 0;
      piVar12[0x4d] = 0;
    }
    piVar12 = piVar12 + 0x51;
  } while ((int)piVar12 < 0x77aefc);
  FUN_004ce480(puVar10,&DAT_00776fb0,0x4c90,DAT_00784f10 * 0x3f48 + 0xbfb80);
  DAT_007770f1 = 0;
  DAT_007770e4 = 0;
  pbVar9 = &DAT_007770f1;
  do {
    if ((((*(int *)(pbVar9 + -0x13d) == 0) && (*(int *)(pbVar9 + -9) == 0)) &&
        ((bVar1 = *pbVar9, bVar1 < 0x33 && ((pbVar9[-1] < 3 && (pbVar9[-0x141] != 0)))))) &&
       (pbVar9[-0x141] < 0x33)) {
      iVar4 = 1;
      iVar2 = 0;
      if (1 < bVar1) {
        pbVar7 = pbVar9 + -0x139;
        do {
          local_18c = (int)*(short *)pbVar7 - (int)*(short *)(pbVar7 + 6);
          iVar11 = (int)*(short *)(pbVar7 + 6) - (int)*(short *)pbVar7;
          if (local_18c <= iVar11) {
            local_18c = iVar11;
          }
          iVar11 = (int)*(short *)(pbVar7 + 2) - (int)*(short *)(pbVar7 + 8);
          iVar6 = (int)*(short *)(pbVar7 + 8) - (int)*(short *)(pbVar7 + 2);
          if (iVar11 <= iVar6) {
            iVar11 = iVar6;
          }
          if (iVar11 < local_18c) {
            iVar2 = iVar2 + 1 + local_18c;
            auStack_184[iVar4] = 1;
          }
          else {
            iVar2 = iVar2 + 1 + iVar11;
            auStack_184[iVar4] = 0;
          }
          iVar4 = iVar4 + 1;
          pbVar7 = pbVar7 + 6;
        } while (iVar4 < (int)(uint)bVar1);
      }
      if ((*(int *)(pbVar9 + -9) < iVar2) && (iVar4 = iVar2 + 0x32, *(int *)(pbVar9 + -9) < iVar4))
      {
        if (*(int *)(pbVar9 + -0x13d) != 0) {
          FUN_00561a2b(*(int *)(pbVar9 + -0x13d));
          pbVar9[-0xffffffff0000013d] = 0;
          pbVar9[-0xffffffff0000013c] = 0;
          pbVar9[-0xffffffff0000013b] = 0;
          pbVar9[-0xffffffff0000013a] = 0;
        }
        pvVar5 = FUN_00561a1d(iVar4 * 6);
        *(void **)(pbVar9 + -0x13d) = pvVar5;
        *(int *)(pbVar9 + -9) = iVar4;
      }
      *(int *)(pbVar9 + -0xd) = iVar2;
      iVar4 = 0;
      iVar2 = 1;
      if (1 < *pbVar9) {
        do {
          if (*(int *)(pbVar9 + -0xd) < iVar4) goto LAB_00444204;
          iVar11 = FUN_00445d10(iVar2 + -1,iVar2,iVar4);
          iVar4 = iVar4 + iVar11;
          iVar2 = iVar2 + 1;
        } while (iVar2 < (int)(uint)*pbVar9);
      }
      *(int *)(pbVar9 + -0xd) = iVar4;
      FUN_00407f90((RestoreWorldRoute *)(pbVar9-0x141));
    }
    else {
      pbVar9[-0xffffffff0000000d] = 0;
      pbVar9[-0xffffffff0000000c] = 0;
      pbVar9[-0xffffffff0000000b] = 0;
      pbVar9[-0xffffffff0000000a] = 0;
      pbVar9[-0xffffffff0000013d] = 0;
      pbVar9[-0xffffffff0000013c] = 0;
      pbVar9[-0xffffffff0000013b] = 0;
      pbVar9[-0xffffffff0000013a] = 0;
      *pbVar9 = 0;
      pbVar9[-0xffffffff00000009] = 0;
      pbVar9[-0xffffffff00000008] = 0;
      pbVar9[-0xffffffff00000007] = 0;
      pbVar9[-0xffffffff00000006] = 0;
      pbVar9[-0x141] = 5;
      pbVar9[1] = 0;
      pbVar9[-0xffffffff00000005] = 0xff;
      pbVar9[-0xffffffff00000004] = 0xff;
      pbVar9[-0xffffffff00000003] = 0xff;
      pbVar9[-0xffffffff00000002] = 0xff;
      pbVar9[-1] = 1;
    }
LAB_00444204: ;
    pbVar9 = pbVar9 + 0x144;
    if (0x77b038 < (int)pbVar9) {
      FUN_00442660();
      FUN_004426c0();
      ExceptionList = local_c;
      return;
    }
  } while( true );
}
