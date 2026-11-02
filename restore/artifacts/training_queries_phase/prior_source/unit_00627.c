#include "restore_types.h"
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de06 (*(undefined4 *)0x0077de06u)
extern bool FUN_00447790();

undefined4 FUN_00443a20(int param_1)

{
  char cVar1;
  char *pcVar2;
  char *pcVar3;
  int iVar4;
  
  iVar4 = 0;
  pcVar3 = &DAT_0077de02;
  pcVar2 = &DAT_0077de06 + param_1;
  do {
    if (pcVar3[-2] != '\0') {
      cVar1 = *pcVar3;
      if ((((cVar1 == '\x01') || (cVar1 == '\x03')) || (cVar1 == '\x05')) && (*pcVar2 == '\x01')) {
        cVar1 = FUN_00447790(iVar4,param_1);
        if (cVar1 != '\0') {
          return 1;
        }
      }
    }
    pcVar3 = pcVar3 + 0x6a;
    iVar4 = iVar4 + 1;
    pcVar2 = pcVar2 + 0x6a;
  } while ((int)pcVar3 < 0x77f744);
  return 0;
}
