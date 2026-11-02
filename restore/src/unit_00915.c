#include "restore_types.h"
#define DAT_00799a8e (*(undefined4 *)0x00799a8eu)
#define DAT_00799b96 (*(undefined4 *)0x00799b96u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)

int FUN_00460ea0(uint param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  
  if (param_1 == 0x19) {
    iVar5 = 0x4c;
  }
  else {
    iVar5 = (param_1 != 0x4d) + 0xb5;
  }
  iVar2 = 1;
  psVar4 = &DAT_00799b96;
  do {
    if (((char)psVar4[-0x5b] != '\0') && (psVar4[-0x53] == iVar5)) {
      sVar1 = *psVar4;
      if (sVar1 < 1) {
        return iVar2;
      }
      iVar3 = sVar1 * 0x184;
      if ((&DAT_00bcd606)[iVar3] == '\0') {
LAB_00460f4b: ;
        *(undefined2 *)(&DAT_00799a8e + iVar2 * 0x108) = 0;
        return iVar2;
      }
      if (((byte)(&DAT_00bcd602)[iVar3] != param_1) && ((&DAT_00bcd602)[iVar3] != 0)) {
        *(undefined2 *)(&DAT_00799a8e + iVar2 * 0x108) = 0;
        return iVar2;
      }
      if ((short)(&DAT_00bcd652)[sVar1 * 0xc2] != iVar2) goto LAB_00460f4b;
    }
    psVar4 = psVar4 + 0x84;
    iVar2 = iVar2 + 1;
    if (0x89b78d < (int)psVar4) {
      return 0;
    }
  } while( true );
}
