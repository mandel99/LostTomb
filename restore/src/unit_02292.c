#include "restore_types.h"
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)

void FUN_00517690(int param_1,int param_2,int param_3,int param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  int iVar5;
  
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (DAT_00bca2d0 <= param_3) {
    param_3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  iVar2 = DAT_00bca2d0 - param_3;
  iVar3 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  for (; param_2 <= param_4; param_2 = param_2 + 1) {
    if (param_1 <= param_3) {
      pbVar4 = &DAT_00af5600 + iVar3 * 4;
      iVar5 = param_1;
      do {
        if ((*(uint *)(pbVar4 + 0x38c) & 4) == 0) {
          (&DAT_00ed4140)[iVar3] = 0xfc;
        }
        else if ((*(uint *)(pbVar4 + 0x38c) & 0x80000) == 0) {
          if ((pbVar4[-4] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((*pbVar4 & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[0x390] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[0x720] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[0x71c] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[0x718] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[0x388] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if ((pbVar4[-8] & 4) == 0) {
            (&DAT_00ed4140)[iVar3] = 0xff;
          }
          else if (param_2 < 1) {
            (&DAT_00ed4140)[iVar3] = 0xfe;
          }
          else if (iVar5 < 1) {
            (&DAT_00ed4140)[iVar3] = 0xfe;
          }
          else if (DAT_00bca2d4 + -2 < param_2) {
            (&DAT_00ed4140)[iVar3] = 0xfe;
          }
          else if (DAT_00bca2d0 + -2 < iVar5) {
            (&DAT_00ed4140)[iVar3] = 0xfe;
          }
          else {
            cVar1 = (&DAT_00adc36c)[iVar3];
            if (cVar1 == '\r') {
              (&DAT_00ed4140)[iVar3] = 0xff;
            }
            else if (cVar1 == '\x05') {
              (&DAT_00ed4140)[iVar3] = 0xfd;
            }
            else if (cVar1 == '\x02') {
              (&DAT_00ed4140)[iVar3] = 0xfd;
            }
            else {
              (&DAT_00ed4140)[iVar3] = 0;
            }
          }
        }
        else {
          (&DAT_00ed4140)[iVar3] = 0xff;
        }
        iVar5 = iVar5 + 1;
        iVar3 = iVar3 + 1;
        pbVar4 = pbVar4 + 4;
      } while (iVar5 <= param_3);
    }
    iVar3 = iVar3 + DAT_00bca2dc + iVar2 + -1 + param_1;
  }
  return;
}
