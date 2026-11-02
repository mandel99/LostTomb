#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_00799a97 (*(undefined4 *)0x00799a97u)
#define DAT_00ba746c (*(undefined *)0x00ba746cu)
#define DAT_00ba7888 (*(undefined *)0x00ba7888u)
#define DAT_00bc95f0 (*(undefined *)0x00bc95f0u)
extern void FUN_0040f8f0();

void FUN_00485080(void)

{
  int iVar1;
  short *psVar2;
  
  if (0 < DAT_00bc95f0) {
    psVar2 = &DAT_00ba746c;
    do {
      if (*psVar2 != 0) {
        iVar1 = *psVar2 * 0x108;
        if ((&DAT_007999d8)[iVar1] == '\x06') {
          (&DAT_007999d8)[iVar1] = 1;
        }
        (&DAT_00799a97)[iVar1] = 0;
      }
      psVar2 = psVar2 + 1;
      DAT_00bc95f0 = DAT_00bc95f0 + -1;
    } while (DAT_00bc95f0 != 0);
  }
  DAT_00bc95f0 = 0;
  FUN_0040f8f0(&DAT_00ba7888,0x21d68,0);
  FUN_0040f8f0(&DAT_00ba746c,0x41a,0);
  return;
}
