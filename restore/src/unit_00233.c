#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00ea5fac (*(undefined4 *)0x00ea5facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53db4 (*(undefined *)0x00f53db4u)
#define DAT_00f53db8 (*(undefined *)0x00f53db8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_00401540();
extern int FUN_0040d940();
extern int FUN_00417050();
extern void FUN_004ccbb0();
extern int FUN_004df040();
extern int FUN_004e6710();
extern void FUN_004f1310();
extern int FUN_00542ad0();
extern undefined4 FUN_00545740();
extern undefined4 FUN_0054f470();
extern undefined4 FUN_00555e80();
extern undefined4 FUN_00558be0();

int FUN_004192b0(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 local_70 [52];
  uint local_3c;
  
  iVar3 = 0;
  iVar5 = 0;
  iVar4 = 0xb4;
  do {
    iVar1 = *(int *)(&DAT_00ea5fac + (DAT_00eaac28 * 0x1276 + iVar5) * 4);
    if (0 < iVar1) {
      switch((&DAT_007999e8)[iVar1 * 0x84]) {
      case 0xb7: ;
        FUN_004f1310(iVar1,local_70);
        uVar2 = local_3c;
        break;
default: ;
        goto switchD_00419304_caseD_b8;
      case 0xcf: ;
      case 0xd0: ;
        uVar2 = (uint)*(byte *)((int)&DAT_00799a2c + iVar1 * 0x108 + 1);
        break;
      case 0xd2: ;
        iVar1 = *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2) * 100;
        uVar2 = (int)(iVar1 + (iVar1 >> 0x1f & 0xfU)) >> 4;
        break;
      case 0xd5: ;
        uVar2 = FUN_00542ad0(iVar1,0);
        break;
      case 0xde: ;
        uVar2 = FUN_004df040(iVar1,0);
        break;
      case 0xe1: ;
        uVar2 = FUN_0040d940(iVar1,0);
        break;
      case 0xe3: ;
        uVar2 = FUN_00417050(iVar1,0);
        break;
      case 0xe4: ;
        uVar2 = FUN_004e6710(iVar1,0);
        break;
      case 0xe5: ;
        uVar2 = FUN_00545740(iVar1,0);
        break;
      case 0xe6: ;
        uVar2 = (FUN_00401540(iVar1,0), restore_unresolved_return(0x00401540u));
        break;
      case 0xea: ;
        uVar2 = FUN_00558be0(iVar1,0);
        break;
      case 0xeb: ;
        uVar2 = FUN_00555e80(iVar1,0);
        break;
      case 0xec: ;
        uVar2 = FUN_0054f470(iVar1,0);
      }
      if (uVar2 == 100) {
        iVar4 = iVar4 + 0x14;
        iVar3 = iVar3 + 1;
        FUN_004ccbb0(0xc6,(&DAT_00ea5fa0)[DAT_00eaac28 * 0x1276 + iVar5],DAT_00f53db8 + 0x113,
                     DAT_00f53db4 + iVar4,DAT_00f56f8c,0);
      }
    }
switchD_00419304_caseD_b8: ;
    iVar5 = iVar5 + 1;
    if (2 < iVar5) {
      return iVar3;
    }
  } while( true );
}
