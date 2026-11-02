#include "restore_types.h"
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5c14 (*(undefined4 *)0x00ea5c14u)
#define DAT_00ea5c78 (*(undefined4 *)0x00ea5c78u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

bool FUN_004e9f50(void)

{
  char cVar1;
  int iVar2;
  short *psVar3;
  char cVar4;
  int iVar5;
  
  iVar5 = 0;
  cVar4 = '\0';
  iVar2 = 0;
  psVar3 = &DAT_007848e2;
  do {
    if (*psVar3 != 0) {
      cVar1 = *(char *)((int)&DAT_00ea5c14 + iVar2 + DAT_00eaac28 * 0x49d8);
      if (cVar4 < cVar1) {
        iVar5 = iVar2 + 1;
        cVar4 = cVar1;
      }
    }
    psVar3 = psVar3 + 1;
    iVar2 = iVar2 + 1;
  } while ((int)psVar3 < 0x7848ec);
  if (0 < iVar5) {
    (&DAT_00ea5c78)[DAT_00eaac28 * 0x1276] = iVar5;
    return true;
  }
  cVar4 = '(';
  iVar2 = 0;
  psVar3 = &DAT_007848e2;
  do {
    if (*psVar3 != 0) {
      cVar1 = *(char *)((int)&DAT_00ea5c0a + iVar2 + DAT_00eaac28 * 0x49d8);
      if (cVar1 < cVar4) {
        iVar5 = iVar2 + 1;
        cVar4 = cVar1;
      }
    }
    psVar3 = psVar3 + 1;
    iVar2 = iVar2 + 1;
  } while ((int)psVar3 < 0x7848ec);
  (&DAT_00ea5c78)[DAT_00eaac28 * 0x1276] = iVar5;
  return 0 < iVar5;
}
