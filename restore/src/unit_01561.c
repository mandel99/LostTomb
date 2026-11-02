#include "restore_types.h"
#include "flood_simulation.h"
#define DAT_00bcd7b4 (*(undefined4 *)0x00bcd7b4u)
extern void FUN_004be4a0();

void FUN_004be580(uintptr_t self)

{
  char *pcVar1;
  int iVar2;
  
  FUN_004bd580(self+0x32c40);
  iVar2 = 1;
  pcVar1 = (char *)0x00bcd7b4u;
  do {
    switch(pcVar1[-0x2e]) {
    case '\x14': ;
      if (*pcVar1 != '\n') goto switchD_004be5af_caseD_19;
      break;
    case '\x19': ;
switchD_004be5af_caseD_19: ;
      if (*pcVar1 != '\f') goto switchD_004be5af_caseD_4d;
      break;
    case 'M': ;
    case 'N': ;
    case '\\': ;
    case ']': ;
    case 'd': ;
    case 'e': ;
switchD_004be5af_caseD_4d: ;
      FUN_004be4a0(iVar2);
    }
    pcVar1 = pcVar1 + 0x184;
    iVar2 = iVar2 + 1;
    if (0xc8ad6f < (int)pcVar1) {
      return;
    }
  } while( true );
}
