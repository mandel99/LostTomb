#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_00ea3f78 (*(undefined4 *)0x00ea3f78u)
#define DAT_00ea3f7c (*(undefined4 *)0x00ea3f7cu)
#define DAT_00ea5eac (*(undefined4 *)0x00ea5eacu)
#define DAT_00ea5eb0 (*(undefined4 *)0x00ea5eb0u)

int FUN_00458f80(int param_1)

{
  char cVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  
  iVar6 = DAT_0078d790;
  iVar4 = param_1 * 0x49d8;
  *(undefined4 *)(&DAT_00ea3f78 + iVar4) = 0;
  *(undefined4 *)(&DAT_00ea3f7c + iVar4) = 0;
  iVar3 = 0;
  (&DAT_00ea5eac)[param_1 * 0x1276] = 0;
  (&DAT_00ea5eb0)[param_1 * 0x1276] = 0;
  if (0 < iVar6) {
    pcVar7 = &DAT_00799ae0;
    do {
      cVar1 = *pcVar7;
      if ((((cVar1 != '\0') && (cVar1 != '\x05')) && (cVar1 != '\x02')) &&
         ((cVar1 != '\x06' && (pcVar7[5] != '\0')))) {
        iVar5 = (int)*(short *)(pcVar7 + 0x1c);
        sVar2 = *(short *)(pcVar7 + 0x12);
        iVar3 = iVar3 + iVar5;
        if (sVar2 < 2) {
          (&DAT_00ea5eac)[param_1 * 0x1276] = (&DAT_00ea5eac)[param_1 * 0x1276] + iVar5;
        }
        if (sVar2 < 4) {
          *(int *)(&DAT_00ea3f78 + iVar4) = *(int *)(&DAT_00ea3f78 + iVar4) + iVar5;
        }
        if (9 < sVar2) {
          (&DAT_00ea5eb0)[param_1 * 0x1276] = (&DAT_00ea5eb0)[param_1 * 0x1276] + iVar5;
        }
        if (0xd < sVar2) {
          *(int *)(&DAT_00ea3f7c + iVar4) = *(int *)(&DAT_00ea3f7c + iVar4) + iVar5;
        }
      }
      pcVar7 = pcVar7 + 0x108;
      iVar6 = iVar6 + -1;
    } while (iVar6 != 0);
  }
  return iVar3;
}
