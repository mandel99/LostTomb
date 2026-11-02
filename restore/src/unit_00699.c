#include "restore_types.h"

bool FUN_00449440(int param_1)

{
  int in_ECX;
  
  switch(*(undefined1 *)(in_ECX + 0x60)) {
  case 0: ;
  case 6: ;
    if (((param_1 != 0x25) && (param_1 != 0x15)) && (param_1 != 0x1b)) {
switchD_004494a1_caseD_c: ;
      return true;
    }
    break;
  case 1: ;
  case 2: ;
    return param_1 == 0x25;
  case 3: ;
    switch((int)(param_1)) {
    case 1: ;
    case 2: ;
    case 3: ;
    case 4: ;
    case 5: ;
    case 6: ;
    case 7: ;
    case 8: ;
    case 0xf: ;
    case 0x11: ;
    case 0x13: ;
    case 0x24: ;
      goto switchD_004494a1_caseD_c;
    }
    break;
  case 4: ;
    switch((int)(param_1)) {
    case 0xc: ;
    case 0x14: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x24: ;
      goto switchD_004494a1_caseD_c;
    }
    break;
  case 5: ;
    if ((0 < param_1) && (param_1 < 9)) {
      return true;
    }
  }
  return false;
}
