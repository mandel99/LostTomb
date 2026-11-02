#include "restore_types.h"
#define DAT_005d19c8 (*(undefined4 *)0x005d19c8u)
#define DAT_005d19ce (*(undefined4 *)0x005d19ceu)
#define DAT_005d19d0 (*(undefined4 *)0x005d19d0u)
#define DAT_005d19d2 (*(undefined4 *)0x005d19d2u)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00ea5fac (*(undefined4 *)0x00ea5facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00401540();
extern int FUN_0040d940();
extern int FUN_00417050();
extern int FUN_004df040();
extern int FUN_004e6710();
extern void FUN_004f1310();
extern int FUN_00542ad0();
extern undefined4 FUN_00545740();
extern undefined4 FUN_0054f470();
extern undefined4 FUN_00555e80();
extern undefined4 FUN_00558be0();

uint FUN_004f5050(undefined2 *param_1,char param_2)

{
  short sVar1;
  undefined2 *puVar2;
  int iVar3;
  undefined2 *puVar4;
  undefined3 uVar6;
  int iVar5;
  bool bVar7;
  bool bVar8;
  int local_78;
  short *local_74;
  undefined1 local_70 [52];
  int local_3c;
  
  local_78 = 0;
  local_74 = &DAT_00784de0;
  do {
    iVar3 = DAT_00eaac28 * 0x1276 + local_78;
    if ((&DAT_00ea5fa0)[iVar3] == 0) {
      iVar3 = *local_74 * 0x10;
      puVar4 = &DAT_005d19c8 + *local_74 * 8;
      if (((undefined2 *)(int)*(short *)(&DAT_005d19ce + iVar3) == param_1) ||
         ((undefined2 *)(int)*(short *)(&DAT_005d19d0 + iVar3) == param_1)) goto LAB_004f5232;
      puVar4 = (undefined2 *)(int)*(short *)(&DAT_005d19d2 + iVar3);
      puVar2 = puVar4;
joined_r0x004f5206: ;
      if (puVar2 == param_1) {
LAB_004f5232: ;
        return CONCAT31((int3)((uint)puVar4 >> 8),1);
      }
    }
    else {
      iVar5 = *(int *)(&DAT_00ea5fac + iVar3 * 4);
      puVar4 = (undefined2 *)0x0;
      if (iVar5 != 0) {
        iVar3 = (&DAT_00ea5fa0)[iVar3] * 0x10;
        switch((&DAT_007999e8)[iVar5 * 0x84]) {
        case 0xb7: ;
          FUN_004f1310(iVar5,local_70);
          iVar5 = local_3c;
          break;
default: ;
          iVar5 = FUN_004df040(iVar5,0);
          break;
        case 0xcf: ;
        case 0xd0: ;
          bVar7 = *(char *)((int)&DAT_00799a2c + iVar5 * 0x108 + 1) != 'd';
          uVar6 = (undefined3)((uint)iVar5 >> 8);
          goto LAB_004f51e1;
        case 0xd2: ;
          sVar1 = *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2);
          bVar8 = SBORROW2(sVar1,0x10);
          bVar7 = (short)(sVar1 + -0x10) < 0;
          goto LAB_004f51de;
        case 0xd5: ;
          iVar5 = FUN_00542ad0(iVar5,0);
          break;
        case 0xe1: ;
          iVar5 = FUN_0040d940(iVar5,0);
          break;
        case 0xe3: ;
          iVar5 = FUN_00417050(iVar5,0);
          break;
        case 0xe4: ;
          iVar5 = FUN_004e6710(iVar5,0);
          break;
        case 0xe5: ;
          iVar5 = FUN_00545740(iVar5,0);
          break;
        case 0xe6: ;
          iVar5 = (FUN_00401540(iVar5,0), restore_unresolved_return(0x00401540u));
          break;
        case 0xea: ;
          iVar5 = FUN_00558be0(iVar5,0);
          break;
        case 0xeb: ;
          iVar5 = FUN_00555e80(iVar5,0);
          break;
        case 0xec: ;
          iVar5 = FUN_0054f470(iVar5,0);
        }
        bVar8 = SBORROW4(iVar5,100);
        bVar7 = iVar5 + -100 < 0;
LAB_004f51de: ;
        bVar7 = bVar8 != bVar7;
        uVar6 = (undefined3)((uint)iVar5 >> 8);
LAB_004f51e1: ;
        if ((bVar7) || (puVar4 = (undefined2 *)CONCAT31(uVar6,param_2), param_2 != '\0')) {
          puVar4 = (undefined2 *)(int)*(short *)(&DAT_005d19ce + iVar3);
          if ((puVar4 == param_1) ||
             ((undefined2 *)(int)*(short *)(&DAT_005d19d0 + iVar3) == param_1)) goto LAB_004f5232;
          puVar2 = (undefined2 *)(int)*(short *)(&DAT_005d19d2 + iVar3);
          goto joined_r0x004f5206;
        }
      }
    }
    local_74 = local_74 + 1;
    local_78 = local_78 + 1;
    if (0x784de5 < (int)local_74) {
      return (uint)puVar4 & 0xffffff00;
    }
  } while( true );
}
