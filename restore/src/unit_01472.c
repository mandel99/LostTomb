#include "restore_types.h"
int FUN_004b6ff0(undefined2 param_1,int param_2,int param_3,undefined2 param_4,undefined2 param_5)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  
  iVar1 = 10;
  pcVar2 = (char *)0x00bcbf68u;
  do {
    if (*pcVar2 == '\0') {
      if (pcVar2[0x69] == '\0') {
        iVar3 = iVar1 * 0x90;
        *(undefined2 *)(0x00bcb9feu + (uint)iVar3) = param_4;
        *(undefined2 *)(0x00bcba00u + (uint)iVar3) = param_5;
        *(undefined1 *)(0x00bcb9c8u + (uint)iVar3) = 1;
        *(undefined1 *)(0x00bcba0au + (uint)iVar3) = 0;
        *(undefined1 *)(iVar3 + 0xbcb9c9) = 0;
        *(undefined2 *)(0x00bcb9ccu + (uint)iVar3) = param_1;
        *(undefined1 *)(0x00bcb9cau + (uint)iVar3) = (char)iVar1 + -10;
        if (param_2 != 10) {
          *(short *)(0x00bcb9f2u + (uint)iVar3) = (short)param_2;
          *(undefined2 *)(0x00bcb9f4u + (uint)iVar3) = 100;
          return iVar1;
        }
        if ((param_3 != 0) && (param_3 != 4)) {
          *(undefined2 *)(0x00bcb9f2u + (uint)iVar3) = 2;
          *(undefined2 *)(0x00bcb9f4u + (uint)iVar3) = 100;
          return iVar1;
        }
        *(undefined2 *)(0x00bcb9f2u + (uint)iVar3) = 1;
        *(undefined2 *)(0x00bcb9f4u + (uint)iVar3) = 100;
        return iVar1;
      }
      *pcVar2 = '\x01';
    }
    pcVar2 = pcVar2 + 0x90;
    iVar1 = iVar1 + 1;
    if (0xbcd5e7 < (int)pcVar2) {
      return 0;
    }
  } while( true );
}
