#include "restore_types.h"
#include "sound_initialization.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38e63 (*(char *)0x00e38e63u)
#define DAT_00e38ec4 (*(undefined *)0x00e38ec4u)
#define DAT_00e92da8 (*(undefined *)0x00e92da8u)
#define DAT_00f53f60 (*(undefined *)0x00f53f60u)
#define DAT_00f53f64 (*(undefined *)0x00f53f64u)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define DAT_00f54120 (*(undefined4 *)0x00f54120u)
#define DAT_00f55b88 (*(undefined *)0x00f55b88u)
#define _DAT_00e92da8 (*(undefined4 *)0x00e92da8u)
#define _DAT_00e92dac (*(undefined4 *)0x00e92dacu)
#define _DAT_00e92db0 (*(undefined4 *)0x00e92db0u)
#define _DAT_00e92db4 (*(undefined4 *)0x00e92db4u)
#define _DAT_00e92db8 (*(undefined4 *)0x00e92db8u)
extern void FUN_004f0ba0();
extern longlong FUN_0056133c();

void FUN_0053b4a0(int param_1,int param_2,int param_3)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = param_2;
  iVar1 = DAT_00e38d98;
  if ((((DAT_00f55b88 != 0) && (param_2 < 0x21)) && (param_3 < 5)) && (DAT_00e38e63 != '\0')) {
    iVar4 = DAT_00f53f60;
    if (DAT_00f53f78 == 0) {
      iVar4 = DAT_00f53f64;
    }
    iVar5 = param_1 * 0x184;
    FUN_004f0ba0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                 (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],&param_2,&param_1);
    if (((param_2 < 0) || (param_1 < 0)) || ((iVar4 < param_2 || (iVar1 < param_1)))) {
      switch((&DAT_00bcd602)[iVar5]) {
      case 0xb: ;
      case 0xc: ;
      case 0xd: ;
      case 0x2b: ;
      case 0x2c: ;
      case 0x2d: ;
      case 0x36: ;
      case 0x37: ;
      case 0x38: ;
      case 0x4d: ;
      case 0x4e: ;
      case 0x5c: ;
      case 0x5d: ;
      case 99: ;
      case 100: ;
      case 0x65: ;
        bVar2 = true;
        break;
default: ;
        goto switchD_0053b55f_caseD_e;
      }
    }
    else {
      bVar2 = false;
    }
    iVar1 = (&DAT_00f54120)[iVar3 * 5 + param_3];
    if (-1 < iVar1) {
      _DAT_00e92db4 = FUN_0056133c();
      if (bVar2) {
        _DAT_00e92db0 = (int)(DAT_00e38ec4 + (DAT_00e38ec4 >> 0x1f & 3U)) >> 2;
      }
      else {
        _DAT_00e92db0 = DAT_00e38ec4;
      }
      _DAT_00e92da8 = 0xffffffff;
      _DAT_00e92db8 = 1;
      _DAT_00e92dac = iVar1;
      FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)0x00e92da8u);
    }
  }
switchD_0053b55f_caseD_e: ;
  return;
}
