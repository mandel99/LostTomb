#include "restore_types.h"
#define DAT_0070ce0c (*(undefined4 *)0x0070ce0cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00978d80 (*(undefined *)0x00978d80u)
extern uint FUN_00469d50();
extern void FUN_00469e40();
extern longlong FUN_0056133c();

void FUN_00469b90(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  
  DAT_00978d80 = DAT_00978d80 + 1;
  if (9 < DAT_00978d80) {
    DAT_00978d80 = 0;
    iVar2 = FUN_0056133c();
    iVar6 = 1;
    iVar4 = 3;
    if (0 < DAT_0078d790) {
      pcVar5 = &DAT_00799ae5;
      do {
        if (iVar2 == 0) {
          return;
        }
        if ((((pcVar5[-5] == '\x01') && (*pcVar5 != '\0')) && (*(short *)(pcVar5 + 0x17) != 0)) &&
           (*(int *)(pcVar5 + 0xdf) == 1000)) {
          cVar1 = FUN_00469d50();
          if ((cVar1 == '\0') || (iVar4 == 0)) {
            uVar3 = 0;
          }
          else {
            uVar3 = 1;
          }
          FUN_00469e40(iVar6,uVar3);
          iVar2 = iVar2 + -1;
          *(undefined4 *)(pcVar5 + 0xdf) =
               *(undefined4 *)(&DAT_0070ce0c + *(short *)(pcVar5 + 0xd) * 0x58);
          if (0 < iVar4) {
            iVar4 = iVar4 + -1;
          }
        }
        iVar6 = iVar6 + 1;
        pcVar5 = pcVar5 + 0x108;
      } while (iVar6 <= DAT_0078d790);
    }
  }
  return;
}
