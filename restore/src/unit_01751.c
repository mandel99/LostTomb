#include "restore_types.h"
#define DAT_00e36c90 (*(undefined *)0x00e36c90u)
#define DAT_00e92e48 (*(undefined *)0x00e92e48u)
#define s_status_txt_005e0714 (*(char (*)[11])0x005e0714u)
extern undefined4 FUN_00562517();
extern undefined4 FUN_005647eb();
extern DWORD FUN_005662e3();
extern int FUN_005664d5();
extern uint FUN_00568eab();

void FUN_004ce860(undefined4 param_1)

{
  int iVar1;
  
  if (DAT_00e92e48 == 0) {
    iVar1 = FUN_00568eab(s_status_txt_005e0714,0x8301,0x180);
    if (iVar1 == -1) {
      return;
    }
  }
  else {
    iVar1 = FUN_00568eab(s_status_txt_005e0714,0x8101,0x180);
    if (iVar1 == -1) {
      return;
    }
    FUN_005662e3(iVar1,0,2);
  }
  FUN_005664d5(iVar1,&DAT_00e36c90,param_1);
  FUN_00562517(iVar1);
  FUN_005647eb(iVar1);
  DAT_00e92e48 = 1;
  return;
}
