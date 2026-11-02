#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b9e (*(undefined4 *)0x00799b9eu)
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)

void FUN_004539e0(void)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  char *pcVar7;
  uint uVar8;
  int *local_c;
  uint local_8;
  int local_4;
  
  if (0 < DAT_0078d790) {
    pcVar7 = &DAT_00799b9e;
    local_4 = DAT_0078d790;
    do {
      if (pcVar7[-0xbe] == '\x01') {
        iVar2 = *(int *)(pcVar7 + -0xb2);
        uVar6 = (uint)(byte)pcVar7[-0xbb];
        if (uVar6 == 1) {
          *pcVar7 = (&DAT_00ac2d4c)[iVar2];
        }
        else {
          iVar5 = -9999;
          if (uVar6 != 0) {
            local_c = &DAT_005cc8c0;
            piVar4 = local_c;
            uVar8 = uVar6;
            local_8 = uVar6;
            do {
              do {
                if (iVar5 < (char)(&DAT_00ac2d4c)[iVar2 + *piVar4]) {
                  iVar5 = (int)(char)(&DAT_00ac2d4c)[iVar2 + *piVar4];
                }
                uVar8 = uVar8 - 1;
                piVar4 = piVar4 + 2;
              } while (uVar8 != 0);
              piVar4 = local_c + 0xc;
              local_8 = local_8 - 1;
              uVar8 = uVar6;
              local_c = piVar4;
            } while (local_8 != 0);
          }
          *pcVar7 = (char)iVar5;
        }
        if (pcVar7[2] != '\0') {
          *pcVar7 = *pcVar7 + '\n';
        }
        bVar1 = (&DAT_00ab623c)[iVar2];
        if (bVar1 == 1) {
          cVar3 = *pcVar7 + '\n';
        }
        else if (bVar1 == 2) {
          cVar3 = *pcVar7 + '\f';
        }
        else if (bVar1 == 3) {
          cVar3 = *pcVar7 + '\x0e';
        }
        else if (bVar1 == 4) {
          cVar3 = *pcVar7 + '\x10';
        }
        else {
          if (bVar1 < 5) goto LAB_00453ac4;
          cVar3 = *pcVar7 + '\x12';
        }
        *pcVar7 = cVar3;
      }
LAB_00453ac4: ;
      pcVar7 = pcVar7 + 0x108;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
