#include "restore_types.h"
#define DAT_00d3b5c0 (*(int32_t *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00e38dc8 (*(char *)0x00e38dc8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern int FUN_0053e590();

void FUN_00425730(int param_1,int param_2,short *param_3,int param_4)

{
  byte bVar1;
  byte bVar2;
  uint uVar3;
  
  if (0 < param_4) {
    do {
      DAT_00d3b5e0 = *param_3 + param_1;
      DAT_00d3b5dc = param_3[1] + param_2;
      DAT_00d3b5c0 = (&DAT_00f55bb4)[param_3[5] * 2] + (int)param_3[6];
      if (((DAT_00e38dc9 != '\0') || (DAT_00e38dca != '\0')) ||
         (*(char *)((int)param_3 + 0x1b) != '\0')) {
        if ((char)param_3[0xc] == '\0') {
          DAT_00d3b5c0 = DAT_00d3b5c0 + 3;
        }
        else {
          bVar1 = *(byte *)((int)param_3 + 0x1b);
          if (bVar1 == 0) {
            if (*(char *)((int)param_3 + 0x19) == '\0') {
              if ((char)param_3[0xd] != '\0') {
                DAT_00d3b5c0 = DAT_00d3b5c0 + 1;
              }
            }
            else {
              if (*(char *)((int)param_3 + 0x19) == '\x14') {
                DAT_00e38dc8 = 1;
              }
              DAT_00d3b5c0 = DAT_00d3b5c0 + 2;
            }
          }
          else {
            bVar2 = bVar1 >> 2;
            uVar3 = bVar1 & 3;
            if (bVar2 == 0) {
              uVar3 = (uVar3 != 2) + 1;
              bVar2 = 0x10;
            }
            *(byte *)((int)param_3 + 0x1b) = (bVar2 + 0x3f) * '\x04' | (byte)uVar3;
            DAT_00d3b5c0 = DAT_00d3b5c0 + -1 + uVar3;
          }
        }
        FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,DAT_00d3b5dc);
      }
      param_3 = param_3 + 0x12;
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return;
}
