#include "restore_types.h"
void FUN_004b7b50(int param_1,short param_2)

{
  short sVar1;
  int iVar2;
  
  if (param_1 != 0) {
    param_1 = param_1 * 0x90;
    sVar1 = *(short *)(0x00bcb9ccu + (uint)param_1);
    if (sVar1 == 0xd) {
      iVar2 = (-(uint)(*(short *)(0x00bcba10u + (uint)param_1) != 0) & 0x14) + 0x50;
    }
    else if (sVar1 == 0x39) {
      iVar2 = 100;
    }
    else if ((sVar1 == 0xc) || (sVar1 == 0xb)) {
      iVar2 = (-(uint)(*(short *)(0x00bcba10u + (uint)param_1) != 0) & 0x14) + 0x3c;
    }
    else {
      iVar2 = (-(uint)(*(undefined1 *)(0x00bcba32u + (uint)param_1) != '\r') & 0xfffffff6) + 0x50;
    }
    *(short *)(0x00bcb9f4u + (uint)param_1) = *(short *)(0x00bcb9f4u + (uint)param_1) + param_2;
    if (iVar2 < *(short *)(0x00bcb9f4u + (uint)param_1)) {
      *(short *)(0x00bcb9f4u + (uint)param_1) = (short)iVar2;
    }
    if (*(short *)(0x00bcb9f4u + (uint)param_1) < 0) {
      *(undefined2 *)(0x00bcb9f4u + (uint)param_1) = 0;
    }
  }
  return;
}
