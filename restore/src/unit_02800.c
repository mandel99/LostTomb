#include "restore_types.h"
extern void FUN_00561bdc();
extern undefined4 FUN_00562c25();
extern undefined4 FUN_005647eb();
extern void FUN_0056489e();

undefined4 FUN_00561994(restore_legacy_FILE *param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0xffffffff;
  if ((param_1->_flag & 0x40U) == 0) {
    if ((param_1->_flag & 0x83U) != 0) {
      uVar2 = FUN_00562c25(param_1);
      FUN_0056489e(param_1);
      iVar1 = FUN_005647eb(param_1->_file);
      if (iVar1 < 0) {
        uVar2 = 0xffffffff;
      }
      else if (param_1->_tmpfname != (char *)0x0) {
        FUN_00561bdc(param_1->_tmpfname);
        param_1->_tmpfname = (char *)0x0;
      }
    }
  }
  else {
    uVar2 = 0xffffffff;
  }
  param_1->_flag = 0;
  return uVar2;
}
