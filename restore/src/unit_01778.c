#include "restore_types.h"
#include "video_player.h"
#define DAT_006fdc54 RESTORE_CURRENT_VIDEO
#define s_OK__Exited_Normally_005e074c (*(char (*)[22])0x005e074cu)
extern void FUN_004ce6e0();
extern void FUN_00561a2b();

void FUN_004cff50(void)

{
  int iVar1;
  
  iVar1 = DAT_006fdc54;
  if (DAT_006fdc54 != 0) {
    FUN_004133f0(RESTORE_CURRENT_VIDEO);
    FUN_00561a2b(iVar1);
    DAT_006fdc54 = 0;
  }
  CoUninitialize();
  FUN_004ce6e0(s_OK__Exited_Normally_005e074c,0,0);
  return;
}
