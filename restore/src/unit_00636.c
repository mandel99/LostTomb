#include "restore_types.h"
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de00 (*(undefined4 *)0x0077de00u)
#define DAT_0077de6d (*(undefined4 *)0x0077de6du)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
extern void FUN_0040f8f0();
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern bool FUN_00444950();
extern byte FUN_00444a00();

void FUN_00444720(void)

{
  char cVar1;
  byte bVar2;
  undefined2 uVar3;
  char cVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  int iVar8;
  int local_4;
  
  FUN_0040f8f0(&DAT_0077de00,0x1942,0);
  local_4 = 0;
  pcVar7 = &DAT_0077de6d;
  pcVar6 = &DAT_0077f748;
  do {
    if ((pcVar6[1] != '\0') && (*pcVar6 == '\x01')) {
      if (0x77f744 < (int)pcVar7) {
        return;
      }
      cVar4 = pcVar6[0x18];
      cVar1 = pcVar6[0x19];
      pcVar7[-3] = '\x01';
      pcVar7[-1] = cVar4;
      bVar2 = pcVar6[0x1a];
      *pcVar7 = cVar1;
      if (0x13 < bVar2) {
        pcVar6[0x1a] = '\x13';
      }
      cVar4 = pcVar6[0x1b];
      uVar3 = *(undefined2 *)(pcVar6 + 0x1c);
      pcVar7[1] = pcVar6[0x1a];
      pcVar7[2] = cVar4;
      *(undefined2 *)(pcVar7 + 0x4b) = uVar3;
      cVar4 = FUN_00444950(pcVar7[1]);
      pcVar7[0x55] = cVar4;
      iVar8 = 1;
      do {
        pcVar7[iVar8 + 0x27] = '\0';
        pcVar7[iVar8 + 3] = '\0';
        cVar4 = pcVar7[-1];
        if ((((cVar4 == '\x01') || (cVar4 == '\x03')) || (cVar4 == '\x05')) || (pcVar7[-1] == '\0'))
        {
          iVar5 = FUN_00443000(local_4,iVar8);
          if (iVar5 != 0) {
            pcVar7[iVar8 + 0x27] = '\x01';
          }
          iVar5 = FUN_00443040(local_4,iVar8);
          if (iVar5 != 0) {
            pcVar7[iVar8 + 3] = '\x01';
          }
        }
        iVar5 = FUN_00444a00(local_4,iVar8);
        if (iVar5 == 1) {
          iVar5 = (iVar8 + (uint)(byte)pcVar7[1] * 0x24) * 4;
          *(undefined4 *)(&DAT_0077646c + iVar5) = 0xf;
        }
        else if (iVar5 == 2) {
          iVar5 = (iVar8 + (uint)(byte)pcVar7[1] * 0x24) * 4;
          *(undefined4 *)(&DAT_0077646c + iVar5) = 0x19;
        }
        else if (iVar5 == 3) {
          iVar5 = (iVar8 + (uint)(byte)pcVar7[1] * 0x24) * 4;
          *(undefined4 *)(&DAT_0077646c + iVar5) = 0x28;
        }
        else {
          iVar5 = (iVar8 + (uint)(byte)pcVar7[1] * 0x24) * 4;
          *(undefined4 *)(&DAT_0077646c + iVar5) = 0;
        }
        iVar8 = iVar8 + 1;
        *(undefined4 *)(&DAT_0077592c + iVar5) = 0;
      } while (iVar8 < 0x24);
      pcVar7[0x4d] = '\n';
      pcVar7[0x4e] = '\0';
      pcVar7[0x4f] = '\x04';
      pcVar7[0x50] = '\0';
      pcVar7[0x51] = '\0';
      pcVar7[0x52] = '\0';
      pcVar7[0x57] = '\0';
      pcVar7[0x58] = '\0';
      pcVar7[0x59] = '\0';
      pcVar7[0x5a] = '\0';
      pcVar7[0x5b] = '\0';
      pcVar7[0x5c] = '\0';
      *(undefined2 *)(pcVar7 + 0x53) = (undefined2)local_4;
      pcVar7 = pcVar7 + 0x6a;
    }
    pcVar6 = pcVar6 + 0x62;
    local_4 = local_4 + 1;
  } while ((int)pcVar6 < 0x7843d8);
  return;
}
