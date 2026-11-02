#include "restore_types.h"
#include "menu_input.h"
#define DAT_00d3b194 (*(int32_t *)0x00d3b194u)
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00e38df8 (*(undefined1 *)0x00e38df8u)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e03 (*(undefined1 *)0x00e38e03u)
#define DAT_00e38e28 (*(int32_t *)0x00e38e28u)
#define DAT_00e38e2c (*(int32_t *)0x00e38e2cu)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
extern void FUN_00425b50();
extern void FUN_00425b80();
extern undefined4 FUN_0047d350();

int FUN_00425990(int param_1,int param_2,short *param_3,int param_4)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  char cVar5;
  short *psVar6;
  int iVar7;
  
  iVar3 = param_4;
  psVar2 = param_3;
  FUN_00425b80(param_3,param_4);
  param_4 = 0;
  iVar4 = 0;
  psVar6 = param_3;
  if (0 < iVar3) {
    param_3 = (short *)iVar3;
    psVar6 = psVar2;
    do {
      if (((char)psVar6[0xd] != '\0') &&
         (cVar5 = (char)psVar6[0xd] + -1, *(char *)(psVar6 + 0xd) = cVar5, cVar5 == '\0')) {
        param_4 = 1;
      }
      if ((char)psVar6[0xc] != '\0') {
        if ((((*psVar6 + param_1 <= DAT_00e38e34) &&
             (DAT_00e38e34 < (int)psVar6[2] + *psVar6 + param_1)) &&
            (psVar6[1] + param_2 <= DAT_00e38e30)) &&
           (DAT_00e38e30 < (int)psVar6[3] + psVar6[1] + param_2)) {
          DAT_00e38e28 = DAT_00e38e30;
          DAT_00e38e2c = DAT_00e38e34;
          if ((char)psVar6[0xd] == '\0') {
            param_4 = 1;
          }
          *(undefined1 *)(psVar6 + 0xd) = 2;
        }
      }
      psVar6 = psVar6 + 0x12;
      param_3 = (short *)((int)param_3 + -1);
      iVar4 = param_4;
    } while (param_3 != (short *)0x0);
  }
  sVar1 = psVar6[4];
  if ((sVar1 == 2) || (sVar1 == 4)) {
    if ((DAT_00e38e03 == '\0') && (DAT_00e38df8 == '\0')) {
      return iVar4;
    }
  }
  else if (((sVar1 == 6) && (DAT_00e38e03 == '\0')) && (DAT_00e38e00 == '\0')) {
    return iVar4;
  }
  iVar7 = 0;
  psVar6 = psVar2;
  if (0 < iVar3) {
    while (((char)psVar6[0xc] == '\0' || ((char)psVar6[0xd] == '\0'))) {
      iVar7 = iVar7 + 1;
      psVar6 = psVar6 + 0x12;
      if (iVar3 <= iVar7) {
        return iVar4;
      }
    }
    if (psVar6[4] == 2) {
      FUN_00425b50(psVar2,iVar3);
    }
    DAT_00d3b198 = *(undefined4 *)(psVar6 + 0xe);
    DAT_00d3b194 = *(undefined4 *)(psVar6 + 0x10);
    if (DAT_00e38e03 != '\0') {
      FUN_0047d350(3);
      *(undefined1 *)((int)psVar6 + 0x19) = 0x14;
      *(undefined1 *)((int)psVar6 + 0x1b) = 0;
      restore_menu_callback(*(uint32_t *)(psVar6 + 8));
      return iVar7 + 1;
    }
    if (DAT_00e38df8 != '\0') {
      restore_menu_callback(*(uint32_t *)(psVar6 + 10));
      return iVar7 + 1;
    }
    if ((psVar6[4] == 6) && (DAT_00e38e00 != '\0')) {
      *(undefined1 *)((int)psVar6 + 0x19) = 0x14;
      restore_menu_callback(*(uint32_t *)(psVar6 + 8));
    }
    iVar4 = iVar7 + 1;
  }
  return iVar4;
}
