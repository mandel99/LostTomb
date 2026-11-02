#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define s_ERR_Could_not_add_event_005da174 (*(char (*)[24])0x005da174u)
extern void FUN_004489c0();
extern bool FUN_0044d680();
extern void FUN_004ce6e0();

int * FUN_00448d10(char param_1)

{
  char cVar1;
  char *pcVar2;
  int *in_ECX;
  int iVar3;
  uint *puVar4;
  
  if ((param_1 != '\0') && (0x77 < *in_ECX)) {
    return (int *)0x0;
  }
  iVar3 = 0;
  if (0 < *in_ECX) {
    puVar4 = (uint *)(in_ECX + 0xb);
    do {
      if (((((*puVar4 & 8) != 0) && ((*puVar4 >> 2 & 1) != 0)) &&
          (*(char *)((int)puVar4 + 10) == '\0')) && (cVar1 = FUN_0044d680(1), cVar1 == '\0')) {
        if (*(char *)((int)puVar4 + -0x26) != '\x02') {
LAB_00448dca: ;
          return in_ECX + iVar3 * 0x1f + 1;
        }
        pcVar2 = &DAT_00bcba9a;
        do {
          if (((pcVar2[-0x42] == '\x01') && (*pcVar2 == '\0')) &&
             ((pcVar2[0x27] == '\0' && (*(short *)(pcVar2 + 0x36) == iVar3)))) goto LAB_00448dba;
          pcVar2 = pcVar2 + 0x90;
        } while ((int)pcVar2 < 0xbcd62a);
        pcVar2 = &DAT_00bcd786;
        while (((pcVar2[4] == '\0' || (*pcVar2 != '\\')) || (*(int *)(pcVar2 + 0x172) != iVar3))) {
          pcVar2 = pcVar2 + 0x184;
          if (0xc8ad41 < (int)pcVar2) goto LAB_00448dca;
        }
      }
LAB_00448dba: ;
      iVar3 = iVar3 + 1;
      puVar4 = puVar4 + 0x1f;
    } while (iVar3 < *in_ECX);
  }
  if (*in_ECX < 0x8c) {
    iVar3 = *in_ECX + 1;
    *in_ECX = iVar3;
    FUN_004489c0();
    *(short *)(in_ECX + iVar3 * 0x1f + -0x1e) = (short)*in_ECX + -1;
    return in_ECX + iVar3 * 0x1f + -0x1e;
  }
  if (param_1 == '\0') {
    FUN_004ce6e0(s_ERR_Could_not_add_event_005da174,0,0);
  }
  return (int *)0x0;
}
