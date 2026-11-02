#include "restore_types.h"
#include "sound_file_lookup.h"
#define DAT_005763bc (*(undefined4 *)0x005763bcu)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
#define s__c___s_005763b4 (*(char (*)[7])0x005763b4u)
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern undefined4 FUN_00561a36();

undefined4 FUN_00539c60(undefined1 *param_1)

{
  int iVar1;
  undefined4 uVar2;
  undefined1 local_104 [260];
  
  iVar1 = FUN_00561a0a(param_1,&DAT_005763bc);
  if (iVar1 == 0) {
    FUN_00561a36(local_104,s__c___s_005763b4,DAT_00e38e62,param_1);
    iVar1 = FUN_00561a0a(local_104,&DAT_005763bc);
    if (iVar1 == 0) {
      return 0xffffffff;
    }
    param_1 = local_104;
    uVar2 = 1;
  }
  else {
    uVar2 = 0;
  }
  uVar2 = FUN_0055dd3f(&RESTORE_SOUND->files,(const char *)param_1,(int)uVar2);
  FUN_00561994(iVar1);
  return uVar2;
}
