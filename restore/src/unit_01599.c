#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
#define PTR_LAB_004c4530 (*(unsigned char * *)0x004c4530u)
extern undefined4 FUN_005013a0();

void FUN_004c42b0(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  short *psVar5;
  int iVar6;
  int local_8;
  int local_4;
  
  local_8 = 0;
  psVar5 = &DAT_007999e8;
  iVar6 = DAT_00f53e6c;
  do {
    if (((char)psVar5[-8] == '\x01') &&
       (((sVar1 = *psVar5, 9 < sVar1 && (sVar1 < 0x1e)) || (sVar1 == 0x46)))) {
      if ((sVar1 < 10) || (0x1d < sVar1)) {
        local_4 = 0;
        if (0 < iVar6) {
          piVar4 = &DAT_00f53e70;
          do {
            iVar6 = *piVar4;
            switch((int)(iVar6)) {
            case 1: ;
              iVar3 = 0;
              break;
            case 2: ;
              iVar3 = 1;
              break;
            case 3: ;
              iVar3 = 2;
              break;
            case 4: ;
              iVar3 = 3;
              break;
            case 5: ;
              iVar3 = 4;
              break;
            case 6: ;
              iVar3 = 5;
              break;
            case 7: ;
              iVar3 = 6;
              break;
            case 8: ;
              iVar3 = 7;
              break;
default: ;
              iVar3 = -1;
              break;
            case 0xd: ;
              iVar3 = 9;
              break;
            case 0xf: ;
              iVar3 = 8;
              break;
            case 0x11: ;
              iVar3 = 10;
              break;
            case 0x13: ;
              iVar3 = 0xb;
            }
            iVar2 = FUN_005013a0(iVar6);
            if (iVar2 != 0) {
              if ((&DAT_00799a2e)[local_8 + iVar3] != 0) {
                if ((iVar6 < 1) || (8 < iVar6)) {
                  iVar6 = 400;
                }
                else {
                  iVar6 = (-(uint)(iVar6 != 1) & 0xffffff38) + 800;
                }
                iVar2 = (short)(&DAT_00799a2e)[local_8 + iVar3] * 2;
                if (iVar2 - iVar6 != 0 && iVar6 <= iVar2) {
                  iVar6 = iVar2;
                }
                if (19999 < iVar6) {
                  iVar6 = 20000;
                }
                (&DAT_00799a2e)[local_8 + iVar3] = (short)iVar6;
              }
            }
            local_4 = local_4 + 1;
            piVar4 = piVar4 + 1;
            iVar6 = DAT_00f53e6c;
          } while (local_4 < DAT_00f53e6c);
        }
      }
      else {
        iVar3 = 0;
        do {
          if ((&DAT_00799a2c)[local_8 + iVar3] != 0) {
            switch((int)(iVar3)) {
            case 0: ;
            case 1: ;
            case 2: ;
            case 3: ;
            case 4: ;
            case 5: ;
            case 6: ;
            case 7: ;
              (&DAT_00799a2c)[local_8 + iVar3] = psVar5[6] * 6;
              goto LAB_004c43d2;
            case 8: ;
              iVar2 = 0xf;
              break;
            case 9: ;
              iVar2 = 0xd;
              break;
            case 10: ;
              iVar2 = 0x11;
              break;
            case 0xb: ;
              iVar2 = 0x13;
              break;
default: ;
              iVar2 = -1;
            }
            if (iVar2 - 0xdU < 7) {
              (*(code *)(&PTR_LAB_004c4530)[iVar2 - 0xdU])();
              return;
            }
          }
LAB_004c43d2: ;
          iVar3 = iVar3 + 1;
        } while (iVar3 < 0xc);
      }
    }
    psVar5 = psVar5 + 0x84;
    local_8 = local_8 + 0x84;
    if (0x89b6e7 < (int)psVar5) {
      return;
    }
  } while( true );
}
