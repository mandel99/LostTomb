#include "restore_types.h"
#define DAT_00bcbacb (*(undefined4 *)0x00bcbacbu)
extern void FUN_004b7c00();
extern void FUN_004b7c40();

void FUN_004b79f0(void)

{
  short sVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_00bcbacb;
  do {
    if ((pcVar2[-0x73] == '\x01') && (pcVar2[-10] == '\0')) {
      if (pcVar2[-0x31] == '\0') {
        if (((*(short *)(pcVar2 + -0x47) < 0x15) && (pcVar2[-0xd] == '\0')) && (*pcVar2 == '\0')) {
          FUN_004b7c00(10);
          FUN_004b7c40(0xfffffff6);
        }
        sVar1 = *(short *)(pcVar2 + -0x47);
        if (sVar1 < 0xb) {
          *pcVar2 = *pcVar2 + '\x01';
        }
        else {
joined_r0x004b7a92: ;
          if (sVar1 < 0x15) {
            pcVar2[-0xd] = pcVar2[-0xd] + '\x01';
          }
        }
      }
      else if ((pcVar2[-0x70] == '\0') && (*(short *)(pcVar2 + 0xf) < 0)) {
        if ((*(short *)(pcVar2 + -0x47) < 0x15) && ((pcVar2[-0xd] == '\0' && (*pcVar2 == '\0')))) {
          FUN_004b7c00(0xfffffff6);
          FUN_004b7c40(10);
        }
        sVar1 = *(short *)(pcVar2 + -0x47);
        if (10 < sVar1) goto joined_r0x004b7a92;
        *pcVar2 = *pcVar2 + '\x01';
      }
    }
    pcVar2 = pcVar2 + 0x90;
    if (0xbcd65a < (int)pcVar2) {
      return;
    }
  } while( true );
}
