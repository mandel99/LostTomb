#include "restore_types.h"
#define PTR_DAT_00604c58 (*(unsigned char * *)0x00604c58u)
extern BOOL FUN_00569335();

uint FUN_00565f66(int param_1,uint param_2)

{
  int iVar1;
  undefined4 in_ECX;
  undefined4 uVar2;
  undefined4 local_8;
  
  if (param_1 + 1U < 0x101) {
    RESTORE_SLICE2(param_1,2) = *(ushort *)(PTR_DAT_00604c58 + param_1 * 2);
  }
  else {
    if ((((undefined4 *)&PTR_DAT_00604c58)[(param_1 >> 8 & 0xffU) * 2 + 1] & 0x80) == 0) {
      local_8 = CONCAT31((int3)((uint)in_ECX >> 8),(char)param_1) & 0xffff00ff;
      uVar2 = 1;
    }
    else {
      RESTORE_SLICE2(local_8,0) = CONCAT11((char)param_1,(char)((uint)param_1 >> 8));
      local_8 = CONCAT22((short)((uint)in_ECX >> 0x10),(undefined2)local_8) & 0xff00ffff;
      uVar2 = 2;
    }
    iVar1 = FUN_00569335(1,&local_8,uVar2,(int)&param_1 + 2,0,0,1);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return RESTORE_SLICE2(param_1,2) & param_2;
}
