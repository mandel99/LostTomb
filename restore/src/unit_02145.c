#include "restore_types.h"
#define DAT_005f6d70 (*(undefined4 *)0x005f6d70u)
#define DAT_005f7170 (*(undefined4 *)0x005f7170u)
#define DAT_005f7570 (*(undefined4 *)0x005f7570u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00eaaf60 (*(undefined4 *)0x00eaaf60u)
#define DAT_00f5733c (*(undefined *)0x00f5733cu)
#define DAT_00f57344 (*(undefined *)0x00f57344u)
#define DAT_00f5734c (*(undefined *)0x00f5734cu)
#define DAT_00f57354 (*(undefined *)0x00f57354u)
#define DAT_00f5735c (*(undefined *)0x00f5735cu)
#define DAT_00f5736c (*(undefined *)0x00f5736cu)
extern void FUN_00475bf0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern uint FUN_004f3d70();
#define _param_4 restore_param_4

char FUN_004f3f70(undefined4 param_1,int param_2,int param_3,unsigned int restore_param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int local_c;
  uint local_8;
  
  iVar3 = DAT_00bca2e0;
  iVar2 = DAT_00bca2d8;
  iVar5 = 0;
  uVar6 = 0;
  local_8 = 1;
  uVar4 = 0xff;
  switch(param_3 + -3) {
  case 0: ;
    _param_4 = &DAT_00eaaf60;
    local_c = DAT_00f5736c;
    local_8 = 1;
    uVar4 = 0;
    goto LAB_004f408a;
  case 1: ;
    if (RESTORE_SLICE1(restore_param_4,0) != '\0') {
      (&DAT_00b7482c)[param_2] = 0;
      iVar3 = (param_2 - iVar2) % 0xe4;
      iVar2 = (param_2 - iVar2) / 0xe4;
      FUN_00475bf0(iVar3,iVar2,iVar3,iVar2);
      FUN_004767d0(iVar3,iVar2,iVar3,iVar2,0,1);
      FUN_00477500(iVar3,iVar2,iVar3,iVar2);
    }
    return RESTORE_SLICE1(restore_param_4,0);
  case 2: ;
    _param_4 = &DAT_005f6d70;
    local_c = DAT_00f5733c;
    local_8 = 8;
    goto LAB_004f408a;
  case 3: ;
    local_c = DAT_00f57344;
    break;
  case 4: ;
    local_c = DAT_00f5734c;
    break;
  case 5: ;
    local_c = DAT_00f57344;
    break;
  case 6: ;
    local_c = DAT_00f57354;
    break;
  case 7: ;
    local_c = DAT_00f5735c;
    break;
default: ;
    return '\0';
  }
  _param_4 = &DAT_005f7170;
  uVar4 = 0x55;
LAB_004f408a: ;
  do {
    uVar1 = iVar5 + iVar3 & 7;
    if (param_3 == 5) {
      iVar2 = FUN_004f3d70(*(int *)(&DAT_005f7570 + uVar1 * 4) + param_2);
      if ((4 < iVar2) || (iVar2 == 0)) {
        uVar6 = uVar6 | 1 << ((byte)iVar5 & 0x1f);
      }
    }
    else {
      iVar2 = FUN_004f3d70(*(int *)(&DAT_005f7570 + uVar1 * 4) + param_2);
      if (-1 < iVar2) {
        uVar6 = uVar6 | 1 << ((byte)iVar5 & 0x1f);
      }
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 8);
  iVar2 = *(int *)(_param_4 + (uVar6 & uVar4) * 4);
  (&DAT_00b7482c)[param_2] = iVar2 + local_c;
  if (((uVar6 & uVar4) == 0xff) && (1 < local_8)) {
    (&DAT_00b7482c)[param_2] =
         (int)((ulonglong)(byte)(&DAT_00acf85c)[param_2] % (ulonglong)(longlong)(int)local_8) +
         iVar2 + local_c;
  }
  return '\x01';
}
