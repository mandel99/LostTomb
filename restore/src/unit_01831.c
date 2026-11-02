#include "restore_types.h"
#include "mission_loaders.h"
#include "game_diagnostics.h"
#define DAT_00e38ea8 (*(int8_t *)0x00e38ea8u)
#define DAT_00e39080 (*(int32_t *)0x00e39080u)
#define DAT_00ea5d10 (*(undefined4 *)0x00ea5d10u)
#define DAT_00eaac28 (*(int32_t *)0x00eaac28u)
#define s_mission1_pak_005e1b98 (*(char (*)[13])0x005e1b98u)
extern void FUN_004ce2d0();
extern int FUN_004ce480();
extern void FUN_004d52a0();
extern undefined4 FUN_005647eb();
extern DWORD FUN_005662e3();
extern uint FUN_00568eab();

uint32_t FUN_004d58b0(int param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  int local_4;
  
  uVar1 = DAT_00e39080;
  if (DAT_00e38ea8 == '\x7f') {
    return 0;
  }
  FUN_004ce2d0();
  iVar2 = FUN_004ce480(s_mission1_pak_005e1b98 + param_1 * 0x14,&local_4,4,(int)DAT_00e38ea8 * 4);
  if (iVar2 == 0) {
    return 0;
  }
  if (local_4 < 1) {
    return 0;
  }
  restore_game_trace("mission PAK load scene=%d pack=%d offset=%d\n",(int)DAT_00e38ea8,param_1,local_4);
  iVar2 = FUN_00568eab(s_mission1_pak_005e1b98 + param_1 * 0x14,0x8000);
  if (iVar2 == -1) {
    return 0;
  }
  FUN_005662e3(iVar2,local_4,0);
  FUN_004d41e0(iVar2);
  restore_game_trace("mission PAK loaded scene=%d year=%d version=%u\n",(int)DAT_00e38ea8,
      (int)*(int16_t *)(uintptr_t)0x7848d8u,*(uint32_t *)(uintptr_t)0xe945c0u);
  FUN_005647eb(iVar2);
  DAT_00e39080 = uVar1;
  *(undefined4 *)(uintptr_t)(0xea5d10u + DAT_00eaac28 * 0x49d8u) = 0;
  if (param_2 == 0) {
    FUN_004bd360(RESTORE_LOAD_SETTINGS);
    FUN_004d52a0();
  }
  return 1;
}
