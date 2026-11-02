#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a4c (*(undefined4 *)0x00799a4cu)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00799a75 (*(undefined4 *)0x00799a75u)
#define DAT_00ea41f8 (*(undefined4 *)0x00ea41f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_004d3da0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = DAT_00eaac28;
  iVar1 = DAT_005e4ef4;
  iVar4 = 1;
  do {
    iVar3 = iVar4 * 0x108;
    if ((&DAT_007999d8)[iVar3] == '\x01') {
      switch((&DAT_007999e8)[iVar4 * 0x84]) {
      case 0xb7: ;
        (&DAT_00799a58)[iVar3] = 0;
        if (iVar1 == 5) {
          iVar4 = iVar4 + -1 +
                  *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar4 * 0x84) * 4);
        }
        else {
          iVar4 = iVar4 + -1 +
                  *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar4 * 0x84) * 4);
        }
        break;
      case 0xcf: ;
      case 0xd0: ;
        (&DAT_00799a58)[iVar3] = 0;
        break;
      case 0xd2: ;
        (&DAT_00ea41f8)[iVar2 * 0x1276] = 0;
        break;
      case 0xd5: ;
        *(undefined1 *)(&DAT_00799a40 + iVar4 * 0x84) = 0;
        iVar4 = iVar4 + 99;
        break;
      case 0xde: ;
        (&DAT_00799a4c)[iVar3] = 0;
        iVar4 = iVar4 + 0x73;
        break;
      case 0xe1: ;
        *(undefined1 *)(&DAT_00799a40 + iVar4 * 0x84) = 0;
        iVar4 = iVar4 + 0x18;
        break;
      case 0xe3: ;
        *(undefined1 *)(&DAT_00799a40 + iVar4 * 0x84) = 0;
        iVar4 = iVar4 + 8;
        break;
      case 0xe4: ;
        *(undefined1 *)(&DAT_00799a40 + iVar4 * 0x84) = 0;
        break;
      case 0xe5: ;
        (&DAT_00799a75)[iVar3] = 0;
        iVar4 = iVar4 + 0xa6;
        break;
      case 0xe6: ;
        (&DAT_00799a38)[iVar3] = 0;
        iVar4 = iVar4 + 0xf;
        break;
      case 0xea: ;
        (&DAT_00799a75)[iVar3] = 0;
        iVar4 = iVar4 + 0x9d;
        break;
      case 0xeb: ;
        (&DAT_00799a75)[iVar3] = 0;
        iVar4 = iVar4 + 0x1b4;
        break;
      case 0xec: ;
        (&DAT_00799a75)[iVar3] = 0;
        iVar4 = iVar4 + 0x200;
      }
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 4000);
  return;
}
