#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea5d10 (*(undefined4 *)0x00ea5d10u)
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
extern longlong FUN_0056133c();

int FUN_0041c380(void)

{
  int iVar1;
  int iVar2;
  undefined1 local_70 [112];
  
  iVar2 = 0;
  do {
    iVar1 = *(int *)(&DAT_00ea5fac + (DAT_00eaac28 * 0x1276 + iVar2) * 4);
    if (0 < iVar1) {
      switch((&DAT_007999e8)[iVar1 * 0x84]) {
      case 0xb7: ;
        FUN_004f1310(iVar1,local_70);
        break;
      case 0xcf: ;
      case 0xd0: ;
        break;
      case 0xd2: ;
        break;
      case 0xd5: ;
        FUN_00542ad0(iVar1,0);
        break;
      case 0xde: ;
        FUN_004df040(iVar1,0);
        break;
      case 0xe1: ;
        FUN_0040d940(iVar1,0);
        break;
      case 0xe3: ;
        FUN_00417050(iVar1,0);
        break;
      case 0xe4: ;
        FUN_004e6710(iVar1,0);
        break;
      case 0xe5: ;
        FUN_00545740(iVar1,0);
        break;
      case 0xe6: ;
        FUN_00401540(iVar1,0);
        break;
      case 0xea: ;
        FUN_00558be0(iVar1,0);
        break;
      case 0xeb: ;
        FUN_00555e80(iVar1,0);
        break;
      case 0xec: ;
        FUN_0054f470(iVar1,0);
      }
    }
    iVar1 = DAT_00eaac28;
    iVar2 = iVar2 + 1;
  } while (iVar2 < 3);
  iVar2 = FUN_0056133c();
  return iVar2 - *(int *)(&DAT_00ea5d10 + iVar1 * 0x24ec);
}
