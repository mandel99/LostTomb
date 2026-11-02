#include "restore_types.h"
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc96f4 (*(undefined *)0x00bc96f4u)
#define DAT_00bc96f8 (*(undefined *)0x00bc96f8u)
#define DAT_00bc96fc (*(undefined *)0x00bc96fcu)
#define DAT_00bc9700 (*(undefined *)0x00bc9700u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178b0();

void FUN_0047f530(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  undefined *puVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  int local_4;
  
  iVar2 = DAT_00bc9700;
  iVar1 = DAT_00bc96f8;
  iVar6 = 0;
  local_4 = 0;
  if (DAT_00bc96fc < 1) {
    DAT_00bc96f8 = 0;
    return;
  }
  if (DAT_00bc96f8 < 2) {
    DAT_00bc96f8 = 0;
    return;
  }
  if ((param_3 == 1) && (DAT_00bc96f8 < 5)) {
    DAT_00bc96f8 = 0;
    return;
  }
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar4 = DAT_00bc96f4 - DAT_00bca2e0;
  if (iVar4 < 0) {
    iVar4 = iVar4 + 8;
  }
  if (param_3 == 1) {
    if (DAT_00bc96f8 == 9) {
      local_4 = 4;
    }
    else if (DAT_00bc96f8 == 10) {
      local_4 = 4;
    }
    else if (DAT_00bc96f8 == 0xb) {
      local_4 = 5;
    }
    else if (DAT_00bc96f8 == 0xc) {
      local_4 = 5;
    }
    else if (DAT_00bc96f8 == 0xd) {
      local_4 = 6;
    }
    else if (DAT_00bc96f8 == 0xe) {
      local_4 = 6;
    }
    else if (DAT_00bc96f8 == 0xf) {
      local_4 = 7;
    }
    else {
      local_4 = (DAT_00bc96f8 != 0x10) + 7;
    }
  }
  DAT_00bc96f4 = iVar4;
  if (0 < DAT_00bc96f8) {
    puVar3 = &DAT_00adc36c + param_1;
    puVar5 = &DAT_00af598c + param_1;
    do {
      *puVar5 = *puVar5 | 0x40;
      if (param_3 == 0) {
        if (iVar6 == iVar1 + -1) {
          if (iVar4 == 4) {
            *puVar3 = 4;
          }
          else if (iVar4 == 6) {
            *puVar3 = 1;
          }
          else if (iVar4 == 0) {
            *puVar3 = 6;
          }
          else if (iVar4 == 2) {
            *puVar3 = 3;
          }
        }
        else if (iVar6 == 0) {
          if (iVar4 == 4) {
            *puVar3 = 6;
          }
          else if (iVar4 == 6) {
            *puVar3 = 3;
          }
          else if (iVar4 == 0) {
            *puVar3 = 4;
          }
          else if (iVar4 == 2) {
            *puVar3 = 1;
          }
        }
        else if (iVar4 == 4) {
          *puVar3 = 5;
        }
        else if (iVar4 == 6) {
          *puVar3 = 2;
        }
        else if (iVar4 == 0) {
          *puVar3 = 5;
        }
        else if (iVar4 == 2) {
          *puVar3 = 2;
        }
      }
      else if (iVar6 == iVar1 + -2) {
        *puVar3 = 0xd;
      }
      else if (iVar6 == 1) {
        *puVar3 = 0xd;
      }
      else if (iVar6 == iVar1 + -1) {
        if (iVar4 == 4) {
          *puVar3 = 7;
        }
        else if (iVar4 == 6) {
          *puVar3 = 8;
        }
        else if (iVar4 == 0) {
          *puVar3 = 9;
        }
        else if (iVar4 == 2) {
          *puVar3 = 10;
        }
      }
      else if (iVar6 == 0) {
        if (iVar4 == 4) {
          *puVar3 = 9;
        }
        else if (iVar4 == 6) {
          *puVar3 = 10;
        }
        else if (iVar4 == 0) {
          *puVar3 = 7;
        }
        else if (iVar4 == 2) {
          *puVar3 = 8;
        }
      }
      else if (iVar6 == local_4) {
        if (iVar4 == 4) {
          *puVar3 = 0xe;
        }
        else if (iVar4 == 6) {
          *puVar3 = 0xf;
        }
        else if (iVar4 == 0) {
          *puVar3 = 0xe;
        }
        else if (iVar4 == 2) {
          *puVar3 = 0xf;
        }
      }
      else if (iVar4 == 4) {
        *puVar3 = 0xb;
      }
      else if (iVar4 == 6) {
LAB_0047f7d6: ;
        *puVar3 = 0xc;
      }
      else if (iVar4 == 0) {
        *puVar3 = 0xb;
      }
      else if (iVar4 == 2) goto LAB_0047f7d6;
      puVar3 = puVar3 + iVar2;
      puVar5 = puVar5 + iVar2;
      iVar6 = iVar6 + 1;
    } while (iVar6 < iVar1);
  }
  FUN_005178b0();
  FUN_00517370();
  FUN_00517660();
  return;
}
