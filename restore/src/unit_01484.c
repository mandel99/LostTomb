#include "restore_types.h"
#define DAT_00bcbac1 (*(undefined4 *)0x00bcbac1u)
extern void FUN_004b7b50();

void FUN_004b7960(void)

{
  byte bVar1;
  char *pcVar2;
  int iVar3;
  undefined4 uVar4;
  
  iVar3 = 1;
  pcVar2 = &DAT_00bcbac1;
  do {
    if ((pcVar2[-0x69] == '\x01') && (*pcVar2 == '\0')) {
      if (pcVar2[-0x27] == '\0') {
        uVar4 = 0;
LAB_004b79c7: ;
        FUN_004b7b50(iVar3,uVar4);
      }
      else if (pcVar2[-0x66] == '\0') {
        if (*(short *)(pcVar2 + -0x19) == 0) {
          bVar1 = pcVar2[9] + 1;
          pcVar2[9] = bVar1;
          if (3 < bVar1) {
            if (100 < bVar1) {
              pcVar2[9] = 'd';
            }
            uVar4 = 0xfffffffb;
            goto LAB_004b79c7;
          }
        }
      }
      else {
        pcVar2[9] = '\0';
        pcVar2[10] = '\0';
        pcVar2[-3] = '\0';
        FUN_004b7b50(iVar3,5);
        if (*(short *)(pcVar2 + -0x3f) == 6) {
          *(undefined2 *)(pcVar2 + -0x3f) = *(undefined2 *)(pcVar2 + -7);
        }
      }
    }
    pcVar2 = pcVar2 + 0x90;
    iVar3 = iVar3 + 1;
    if (0xbcd650 < (int)pcVar2) {
      return;
    }
  } while( true );
}
