#include "restore_types.h"
#define DAT_0078d5f0 (*(undefined *)0x0078d5f0u)
#define DAT_0078d5f4 (*(undefined *)0x0078d5f4u)
#define DAT_0078d5f8 (*(undefined *)0x0078d5f8u)
#define DAT_0078d5fc (*(undefined *)0x0078d5fcu)
#define DAT_0078d600 (*(undefined *)0x0078d600u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b60 (*(undefined4 *)0x00799b60u)

void FUN_00454fa0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  char cVar6;
  int local_4;
  
  iVar4 = DAT_0078d600;
  iVar3 = DAT_0078d5fc;
  iVar2 = DAT_0078d5f8;
  if (0 < DAT_0078d790) {
    pcVar5 = &DAT_00799b60;
    local_4 = DAT_0078d790;
    do {
      iVar1 = DAT_0078d5f4;
      if ((pcVar5[-0x80] == '\x01') && (pcVar5[-0x7b] != '\0')) {
        pcVar5[1] = '\0';
        iVar1 = (iVar4 + iVar3 + iVar2 + iVar1 + DAT_0078d5f0) / 5;
        pcVar5[2] = '\0';
        pcVar5[3] = '\0';
        cVar6 = ((char)(iVar1 / 5) + (char)(iVar1 >> 0x1f)) -
                (char)((longlong)iVar1 * 0x66666667 >> 0x3f);
        *pcVar5 = cVar6;
        if (pcVar5[-0x14] != '\0') {
          *pcVar5 = cVar6 + '\n';
        }
        if (pcVar5[-0x13] != '\0') {
          *pcVar5 = *pcVar5 + '\x14';
        }
        if (pcVar5[-0x11] != '\0') {
          *pcVar5 = *pcVar5 + '\x1e';
        }
        if (pcVar5[-0xf] != '\0') {
          *pcVar5 = *pcVar5 + '(';
        }
        if (pcVar5[6] != '\0') {
          if (*pcVar5 < '=') {
            *pcVar5 = *pcVar5 + '(';
          }
          else {
            *pcVar5 = 'd';
          }
        }
        if ('d' < *pcVar5) {
          *pcVar5 = 'd';
        }
        if ((((pcVar5[-0xe] != '\0') || (pcVar5[-0xd] != '\0')) &&
            (pcVar5[1] = '\x01', pcVar5[-0xe] != '\0')) && (pcVar5[-0xd] != '\0')) {
          pcVar5[1] = '\x02';
        }
        if (pcVar5[-7] != '\0') {
          pcVar5[3] = pcVar5[3] + '\x01';
        }
        if (pcVar5[-6] != '\0') {
          pcVar5[3] = pcVar5[3] + '\x01';
        }
        if (pcVar5[-5] != '\0') {
          pcVar5[3] = pcVar5[3] + '\x01';
        }
        if (pcVar5[-4] != '\0') {
          pcVar5[3] = pcVar5[3] + '\x01';
        }
        if (pcVar5[-3] != '\0') {
          pcVar5[3] = pcVar5[3] + '\x01';
        }
        if (pcVar5[-0x12] != '\0') {
          pcVar5[2] = pcVar5[2] + '\x01';
        }
        if (pcVar5[-8] != '\0') {
          pcVar5[2] = pcVar5[2] + '\x01';
        }
      }
      pcVar5 = pcVar5 + 0x108;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
