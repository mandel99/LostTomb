#include "restore_types.h"
#define DAT_007770f2 (*(undefined4 *)0x007770f2u)
#define DAT_0077de54 (*(undefined4 *)0x0077de54u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)

int FUN_00447430(int param_1,undefined4 param_2)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  pcVar1 = &DAT_0077f748;
  while (((pcVar1[1] == '\0' || (*pcVar1 != '\x01')) || (pcVar1[0x18] != '\0'))) {
    pcVar1 = pcVar1 + 0x62;
    iVar3 = iVar3 + 1;
    if (0x7843d7 < (int)pcVar1) {
      return -1;
    }
  }
  if (iVar3 < 0) {
    return -1;
  }
  iVar2 = 0;
  iVar4 = (int)*(short *)((int)&DAT_0077de54 + param_1 * 0x6a + 2);
  pcVar1 = &DAT_007770f2;
  while( true ) {
    if ((byte)pcVar1[-1] < 2) {
      *pcVar1 = '\0';
    }
    if (((*pcVar1 != '\0') &&
        ((*(short *)(pcVar1 + -6) == iVar4 || (*(short *)(pcVar1 + -4) == iVar4)))) &&
       ((*(short *)(pcVar1 + -6) == iVar3 || (*(short *)(pcVar1 + -4) == iVar3)))) break;
    pcVar1 = pcVar1 + 0x144;
    iVar2 = iVar2 + 1;
    if (0x77b039 < (int)pcVar1) {
      return -1;
    }
  }
  *(bool *)param_2 = *(short *)(iVar2 * 0x144 + 0x7770ec) == iVar3;
  return iVar2;
}
