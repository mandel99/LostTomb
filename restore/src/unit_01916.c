#include "restore_types.h"
#define DAT_00e9511c (*(undefined *)0x00e9511cu)
#define DAT_00e95120 (*(undefined *)0x00e95120u)
#define DAT_00e951e4 (*(undefined *)0x00e951e4u)
#define DAT_00e951ec (*(undefined *)0x00e951ecu)
#define DAT_00e951f8 (*(undefined4 *)0x00e951f8u)
#define DAT_00e95232 (*(undefined4 *)0x00e95232u)
#define DAT_00e95234 (*(undefined4 *)0x00e95234u)
#define DAT_00e95264 (*(undefined4 *)0x00e95264u)
extern void FUN_0040f820();

void FUN_004e1640(void)

{
  short sVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  int local_8;
  
  sVar1 = -1;
  if (-1 < DAT_00e951e4) {
    sVar1 = (&DAT_00e95234)[DAT_00e951e4 * 0x18];
  }
  local_8 = 1000;
  do {
    psVar4 = &DAT_00e95264;
    do {
      if (psVar4[-0x19] == 0) {
        sVar2 = psVar4[-1];
joined_r0x004e1693: ;
        if (sVar2 != 0) {
          FUN_0040f820(psVar4 + -0x1e,&DAT_00e951f8,0x30);
          FUN_0040f820(psVar4 + -6,psVar4 + -0x1e,0x30);
          FUN_0040f820(&DAT_00e951f8,psVar4 + -6,0x30);
        }
      }
      else if (psVar4[-0x18] < *psVar4) {
        sVar2 = psVar4[-1];
        goto joined_r0x004e1693;
      }
      psVar4 = psVar4 + 0x18;
    } while ((int)psVar4 < 0xea0db4);
    local_8 = local_8 + -1;
    if (local_8 == 0) {
      DAT_00e951ec = 0;
      psVar4 = &DAT_00e95232;
      DAT_00e9511c = 0;
      do {
        if (*psVar4 != 0) {
          DAT_00e951ec = DAT_00e951ec + 1;
        }
        psVar4 = psVar4 + 0x18;
      } while ((int)psVar4 < 0xea0db2);
      if (DAT_00e951ec < 0xb) {
        DAT_00e95120 = 0;
      }
      else {
        DAT_00e9511c = DAT_00e951ec + -10;
      }
      iVar3 = DAT_00e951e4;
      if (-1 < sVar1) {
        iVar3 = 0;
        psVar4 = &DAT_00e95234;
        while (*psVar4 != sVar1) {
          psVar4 = psVar4 + 0x18;
          iVar3 = iVar3 + 1;
          if (0xea0db3 < (int)psVar4) {
            return;
          }
        }
      }
      DAT_00e951e4 = iVar3;
      return;
    }
  } while( true );
}
