#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd64e (*(undefined4 *)0x00bcd64eu)
#define DAT_00bcd650 (*(undefined4 *)0x00bcd650u)
extern void FUN_004b4180();
extern void FUN_004b4220();
extern void FUN_004b43a0();
extern void FUN_004b43e0();

undefined4 FUN_004b4420(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  DAT_00bcd642 = (short)param_1 * 0xf;
  iVar3 = 0;
  DAT_00bcd644 = (short)param_2 * 0xf;
  if (((&DAT_00af598c)[DAT_00bca2d8 + param_2 * 0xe4 + param_1] & 0xc000) != 0) {
    iVar3 = 6;
  }
  FUN_004b4220(0,param_1 * 0xf,param_2 * 0xf,param_3 * 0xf,param_4 * 0xf,0);
  iVar1 = 0;
  do {
    iVar2 = 0;
    do {
      if ((int)DAT_00bcd64a + (int)DAT_00bcd64c < 1) {
        return 1;
      }
      FUN_004b4180(0);
      if (DAT_00bcd650 == '\x02') {
        FUN_004b43a0();
        if (-1 < DAT_00bcd64e) {
          DAT_00bcd64a = DAT_00bcd64a + -1;
          FUN_004b43e0(0);
        }
      }
      else {
        FUN_004b43e0(0);
        if (-1 < DAT_00bcd64e) {
          DAT_00bcd64c = DAT_00bcd64c + -1;
          FUN_004b43a0(0);
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < 8);
    DAT_00bcd60c = (DAT_00bcd642 / 0xf + (DAT_00bcd642 >> 0xf)) -
                   (short)((longlong)(int)DAT_00bcd642 * 0x88888889 >> 0x3f);
    DAT_00bcd60e = (DAT_00bcd644 / 0xf + (DAT_00bcd644 >> 0xf)) -
                   (short)((longlong)(int)DAT_00bcd644 * 0x88888889 >> 0x3f);
    if (iVar3 == 0) {
      iVar2 = DAT_00bca2d8 + DAT_00bcd60c + DAT_00bcd60e * 0xe4;
      if (((&DAT_00af598c)[iVar2] & 0xc001) != 0) {
        return 0;
      }
      if ((((&DAT_00af598c)[iVar2] & 8) != 0) && (((&DAT_00b285cc)[iVar2] & 0xf) != 0)) {
        return 0;
      }
    }
    else {
      iVar3 = iVar3 + -1;
    }
    iVar1 = iVar1 + 1;
    if (999 < iVar1) {
      return 0;
    }
  } while( true );
}
