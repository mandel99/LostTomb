#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0045b570();
extern int FUN_0045e360();
extern int FUN_004b4800();

undefined4
FUN_004968a0(undefined4 param_1,int param_2,int param_3,undefined4 param_4,int param_5,
            undefined2 param_6)

{
  int iVar1;
  int iVar2;
  undefined1 uVar3;
  
  switch((int)(param_1)) {
  case 1: ;
    uVar3 = 0;
    break;
  case 2: ;
    uVar3 = 1;
    break;
  case 3: ;
    uVar3 = 2;
    break;
  case 4: ;
    uVar3 = 3;
    break;
  case 5: ;
    uVar3 = 4;
    break;
  case 6: ;
    uVar3 = 5;
    break;
  case 7: ;
    uVar3 = 6;
    break;
  case 8: ;
    uVar3 = 7;
    break;
default: ;
    uVar3 = 0xff;
    break;
  case 0xd: ;
    uVar3 = 9;
    break;
  case 0xf: ;
    uVar3 = 8;
    break;
  case 0x11: ;
    uVar3 = 10;
    break;
  case 0x13: ;
    uVar3 = 0xb;
  }
  if (((&DAT_007999e8)[param_5 * 0x84] == 0x47) || ((&DAT_007999e8)[param_5 * 0x84] == 0xbf)) {
    FUN_0045e360(DAT_00eaac28,param_5,param_1,param_2 * 100);
  }
  else {
    FUN_0045b570(DAT_00eaac28,param_5,param_1,param_2 * 100);
  }
  iVar1 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
  *(undefined2 *)(&DAT_00bcd660 + iVar1 * 0x184) = param_6;
  (&DAT_00bcd672)[iVar1 * 0x184] = uVar3;
  (&DAT_00bcd652)[iVar1 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
  if (1 < param_2) {
    iVar2 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
    *(short *)(&DAT_00bcd660 + iVar2 * 0x184) = (short)iVar1;
    (&DAT_00bcd672)[iVar2 * 0x184] = uVar3;
    (&DAT_00bcd652)[iVar2 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
    if (2 < param_2) {
      iVar1 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
      *(short *)(&DAT_00bcd660 + iVar1 * 0x184) = (short)iVar2;
      (&DAT_00bcd672)[iVar1 * 0x184] = uVar3;
      (&DAT_00bcd652)[iVar1 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
      if (3 < param_2) {
        iVar2 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
        *(short *)(&DAT_00bcd660 + iVar2 * 0x184) = (short)iVar1;
        (&DAT_00bcd672)[iVar2 * 0x184] = uVar3;
        (&DAT_00bcd652)[iVar2 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
        if (4 < param_2) {
          iVar1 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
          *(short *)(&DAT_00bcd660 + iVar1 * 0x184) = (short)iVar2;
          (&DAT_00bcd672)[iVar1 * 0x184] = uVar3;
          (&DAT_00bcd652)[iVar1 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
          if (5 < param_2) {
            iVar2 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
            *(short *)(&DAT_00bcd660 + iVar2 * 0x184) = (short)iVar1;
            (&DAT_00bcd672)[iVar2 * 0x184] = uVar3;
            (&DAT_00bcd652)[iVar2 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
            if (6 < param_2) {
              iVar1 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                                   (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
              *(short *)(&DAT_00bcd660 + iVar1 * 0x184) = (short)iVar2;
              (&DAT_00bcd672)[iVar1 * 0x184] = uVar3;
              (&DAT_00bcd652)[iVar1 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
              if (7 < param_2) {
                iVar2 = FUN_004b4800(DAT_00eaac28,0x42,(int)(short)(&DAT_00bcd60c)[param_3 * 0xc2],
                                     (int)(short)(&DAT_00bcd60e)[param_3 * 0xc2],0);
                *(short *)(&DAT_00bcd660 + iVar2 * 0x184) = (short)iVar1;
                (&DAT_00bcd672)[iVar2 * 0x184] = uVar3;
                (&DAT_00bcd652)[iVar2 * 0xc2] = (&DAT_00bcd652)[param_3 * 0xc2];
              }
            }
          }
        }
      }
    }
  }
  return 1;
}
