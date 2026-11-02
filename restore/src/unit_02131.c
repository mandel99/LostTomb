#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern uint FUN_004eea10();
extern void FUN_004ef450();
extern void FUN_004f25c0();
extern uint FUN_004f26a0();
extern bool FUN_004f28b0();
extern uint FUN_004f28e0();

int FUN_004f29e0(int param_1,char param_2)

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *local_8;
  int local_4;
  
  FUN_004ef450(param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]);
  FUN_004f25c0();
  local_4 = 0;
  if (0 < DAT_00eaaf58) {
    local_8 = &DAT_00eaac44;
    do {
      iVar5 = *local_8;
      cVar3 = FUN_004f28b0(iVar5);
      if (cVar3 == '\0') {
        iVar7 = (int)(short)(&DAT_00799a3c)[iVar5 * 0x84];
        if (iVar7 != 0) {
          cVar3 = FUN_004eea10(iVar5);
          if (cVar3 == '\0') {
            if ((&DAT_00799a3c)[iVar5 * 0x84] != 0) {
              (&DAT_00799a3c)[iVar5 * 0x84] = 0;
            }
          }
          else {
            iVar4 = iVar7 * 0x184;
            if ((((&DAT_00bcd602)[iVar4] == 'Q') && ((&DAT_00bcd630)[iVar4] == '\n')) ||
               (((&DAT_00bcd602)[iVar4] == 'P' && ((&DAT_00bcd630)[iVar4] == '\v')))) {
              bVar1 = false;
              iVar6 = (int)*(short *)(&DAT_00bcd660 + iVar4);
              bVar2 = false;
              if (iVar6 == 0) {
                if (*(short *)(&DAT_00bcd660 + iVar4) != 0) {
                  *(undefined2 *)(&DAT_00bcd660 + iVar4) = 0;
                }
              }
              else {
                cVar3 = FUN_004f26a0(iVar6,iVar7);
                if (cVar3 != '\0') {
                  bVar1 = true;
                  cVar3 = (&DAT_00bcd630)[iVar6 * 0x184];
                  if ((cVar3 == '\x06') || (cVar3 == '\a')) {
                    cVar3 = (&DAT_00bcd608)[iVar6 * 0x184];
                  }
                  if (((((cVar3 == '\x12') || (cVar3 == '\x13')) || (cVar3 == '\x14')) ||
                      ((cVar3 == '\x1d' || (cVar3 == '\x11')))) || (cVar3 == '\x1e')) {
                    bVar2 = true;
                  }
                }
              }
              iVar5 = FUN_004f28e0(iVar5);
              if (iVar5 != 0) {
                if (!bVar1) {
                  return iVar7;
                }
                if ((!bVar2) && (param_2 != '\0')) {
                  *(undefined2 *)(&DAT_00bcd660 + iVar7 * 0x184) = 0;
                  return iVar7;
                }
              }
            }
          }
        }
      }
      local_4 = local_4 + 1;
      local_8 = local_8 + 1;
    } while (local_4 < DAT_00eaaf58);
  }
  return 0;
}
