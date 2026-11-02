#include "restore_types.h"
#define DAT_00a835fc (*(undefined4 *)0x00a835fcu)
#define DAT_00bcba80 (*(undefined4 *)0x00bcba80u)
extern void FUN_0040f8f0();
extern void FUN_004ba180();

void FUN_004ba090(void)

{
  byte *pbVar1;
  
  FUN_0040f8f0(&DAT_00a835fc,0xcb10,0);
  pbVar1 = &DAT_00bcba80;
  do {
    if ((pbVar1[-0x28] == 1) && (pbVar1[0x1a] != 0)) {
      if (*pbVar1 != 0) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),7,1);
      }
      if (3 < *pbVar1) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),6,1);
      }
      if (6 < *pbVar1) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),5,1);
      }
      if (9 < *pbVar1) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),4,1);
      }
      if (0xc < *pbVar1) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),3,1);
      }
      if (0xf < *pbVar1) {
        FUN_004ba180((int)*(short *)(pbVar1 + 6),(int)*(short *)(pbVar1 + 8),2,1);
      }
    }
    pbVar1 = pbVar1 + 0x90;
  } while ((int)pbVar1 < 0xbcbd51);
  return;
}
