#include "restore_types.h"
#define DAT_005d19ca (*(undefined4 *)0x005d19cau)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern undefined4 FUN_00426b30();
extern int FUN_004f44e0();
extern int FUN_0053d930();

int FUN_004f4b70(int param_1)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0xb7: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      do {
        if (((psVar2[-1] == 1) && (iVar1 = FUN_004f44e0(), *psVar2 == iVar1)) &&
           (psVar2[1] == (ushort)*(byte *)(&DAT_00799a2c + param_1 * 0x84))) {
          return iVar3;
        }
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
      } while (iVar3 < iVar1);
    }
    break;
  case 0xcf: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (iVar1 < 1) {
      return -1;
    }
    psVar2 = &DAT_005d19ca;
    while (((psVar2[-1] != 2 || (*psVar2 != 2)) || (psVar2[1] != 0))) {
      iVar3 = iVar3 + 1;
      psVar2 = psVar2 + 8;
      iVar1 = FUN_00426b30();
      if (iVar1 <= iVar3) {
        return -1;
      }
    }
    return iVar3;
  case 0xd0: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while (((psVar2[-1] != 2 || (*psVar2 != 2)) || (psVar2[1] != 1))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xd2: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 1))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xd5: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 3))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xde: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      do {
        if (((psVar2[-1] == 2) && (*psVar2 == 4)) && (iVar1 = FUN_0053d930(), psVar2[1] == iVar1)) {
          return iVar3;
        }
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
      } while (iVar3 < iVar1);
    }
  case 0xe5: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 10))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xe1: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 6))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xe3: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 7))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xe4: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 5))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xe6: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 0xe))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xea: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 0xb))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xeb: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 0xc))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
    break;
  case 0xec: ;
    iVar3 = 0;
    iVar1 = FUN_00426b30();
    if (0 < iVar1) {
      psVar2 = &DAT_005d19ca;
      while ((psVar2[-1] != 2 || (*psVar2 != 0xd))) {
        iVar3 = iVar3 + 1;
        psVar2 = psVar2 + 8;
        iVar1 = FUN_00426b30();
        if (iVar1 <= iVar3) {
          return -1;
        }
      }
      return iVar3;
    }
  }
  return -1;
}
