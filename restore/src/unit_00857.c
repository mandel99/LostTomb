#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
extern undefined4 FUN_00459ef0();

undefined4 FUN_00459fa0(void)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  
  iVar2 = 1;
  if (0 < DAT_0078d790) {
    pcVar3 = &DAT_00799ae0;
    do {
      if (*pcVar3 == '\x01') {
        iVar1 = FUN_00459ef0(iVar2);
        if (iVar1 != 0) {
          return 1;
        }
      }
      iVar2 = iVar2 + 1;
      pcVar3 = pcVar3 + 0x108;
    } while (iVar2 <= DAT_0078d790);
  }
  return 0;
}
