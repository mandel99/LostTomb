#include "restore_types.h"
#define DAT_00578c68 (*(char *)0x00578c68u)
#define DAT_007027e0 (*(char *)0x007027e0u)
#define PTR_DAT_0057c3c8 (*(unsigned char * *)0x0057c3c8u)
extern char * FUN_00562240();
#define stack0x00000000 (restore_stack[32768])

int FUN_0041d050(char *param_1)

{
  unsigned char restore_stack[65536];
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  undefined **ppuVar6;
  bool bVar7;
  byte local_64 [99];
  undefined1 local_1;
  
  FUN_00562240((char *)local_64,param_1,100);
  pbVar2 = local_64;
  local_1 = 0;
  while (local_64[0] != 0) {
    *pbVar2 = local_64[0] ^ 3;
    pbVar5 = pbVar2 + 1;
    pbVar2 = pbVar2 + 1;
    local_64[0] = *pbVar5;
  }
  if (DAT_007027e0 == '\0') {
    return -1;
  }
  if (&stack0x00000000 == (undefined1 *)0x64) {
    return -1;
  }
  iVar4 = 0;
  ppuVar6 = &PTR_DAT_0057c3c8;
  do {
    if ((DAT_00578c68 == '\0') || (*(char *)(ppuVar6 + 2) != '\0')) {
      pbVar5 = *ppuVar6;
      pbVar2 = local_64;
      do {
        bVar1 = *pbVar2;
        bVar7 = bVar1 < *pbVar5;
        if (bVar1 != *pbVar5) {
LAB_0041d0e9: ;
          iVar3 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
          goto LAB_0041d0ee;
        }
        if (bVar1 == 0) break;
        bVar1 = pbVar2[1];
        bVar7 = bVar1 < pbVar5[1];
        if (bVar1 != pbVar5[1]) goto LAB_0041d0e9;
        pbVar2 = pbVar2 + 2;
        pbVar5 = pbVar5 + 2;
      } while (bVar1 != 0);
      iVar3 = 0;
LAB_0041d0ee: ;
      if (iVar3 == 0) {
        return iVar4;
      }
    }
    ppuVar6 = ppuVar6 + 3;
    iVar4 = iVar4 + 1;
    if (0x57c55f < (int)ppuVar6) {
      return iVar4;
    }
  } while( true );
}
