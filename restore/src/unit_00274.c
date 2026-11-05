#include "restore_types.h"
#include "campaign_profile.h"
#include "campaign_unlock.h"
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e92e20 (*(undefined *)0x00e92e20u)
#define s_shot1_bmp_0057c768 (*(char (*)[10])0x0057c768u)
#define s_shot2_bmp_0057c774 (*(char (*)[10])0x0057c774u)


extern void FUN_0041d3d0();
extern undefined4 FUN_004ceed0();
extern undefined4 * FUN_00516a20();

undefined4 FUN_0041d110(int param_1,int param_2)

{
  ushort uVar1;
  undefined4 uVar2;
  char *pcVar3;
  
  uVar1 = GetAsyncKeyState(0x12);
  if (DAT_00e38dda != '\x01') {
    return 0;
  }
  if (param_1 != 0x100) {
    if (param_1 != 0x104) {
      return 0;
    }
    if (param_2 == 0x31) {
      pcVar3 = s_shot1_bmp_0057c768;
    }
    else {
      if (param_2 != 0x32) goto LAB_0041d166;
      pcVar3 = s_shot2_bmp_0057c774;
    }
    FUN_004ceed0(pcVar3);
  }
LAB_0041d166: ;
  switch((int)(param_2)) {
  case 0x43: ;
  case 99: ;
    if ((DAT_00e92e20 != 0) && ((uVar1 & 0xff00) != 0)) {
      FUN_0041d3d0();
      return 1;
    }
    break;
  case 0x46: ;
  case 0x66: ;
    if ((((DAT_00e92e20 != 0) && ((uVar1 & 0xff00) != 0)) && (DAT_00e38e74 != '\0')) &&
       (DAT_00e38dda != '6')) {
      restore_campaign_unlock_all(RESTORE_CAMPAIGN_INDEX);
      uVar2 = (undefined4)(uintptr_t)FUN_00516a20();
      FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,(const char *)(uintptr_t)uVar2);
      return 1;
    }
  }
  return 0;
}
