#include "restore_types.h"
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_009783c0 (*(undefined4 *)0x009783c0u)
#define DAT_009783d8 (*(undefined4 *)0x009783d8u)
#define s_unable_to_find_a_repair_queue_fo_005dbba0 (*(char (*)[50])0x005dbba0u)

void FUN_0046a280(int param_1)

{
  char *pcVar1;
  int iVar2;
  
  iVar2 = 0;
  pcVar1 = &DAT_009783c0;
  do {
    if (*pcVar1 == '\0') {
      *(char *)((int)&DAT_00799aa0 + param_1 * 0x108 + 3) = (char)iVar2 + '\x01';
      (&DAT_009783c0)[iVar2 * 0x1c] = 1;
      (&DAT_009783d8)[iVar2 * 7] = param_1;
      return;
    }
    pcVar1 = pcVar1 + 0x1c;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar1 < 0x9784d8);
  OutputDebugStringA(s_unable_to_find_a_repair_queue_fo_005dbba0);
  return;
}
