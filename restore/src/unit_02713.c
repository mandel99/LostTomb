#include "restore_types.h"
#include "sound_initialization.h"
#define DAT_00604794 (*(undefined4 *)0x00604794u)
#define DAT_00604798 (*(undefined4 *)0x00604798u)
#define DAT_0060479c (*(undefined4 *)0x0060479cu)
#define DAT_006047a8 (*(undefined4 *)0x006047a8u)
#define DAT_006047b8 (*(undefined4 *)0x006047b8u)
#define DAT_006047c8 (*(undefined4 *)0x006047c8u)
#define PTR_DAT_00604790 (*(unsigned char * *)0x00604790u)
#define s_EAX_environment_selection_006047cc (*(char (*)[26])0x006047ccu)
#define s_Intel_006047a0 (*(char (*)[6])0x006047a0u)
#define s_hardware_006047ac (*(char (*)[9])0x006047acu)
#define s_software_006047bc (*(char (*)[9])0x006047bcu)
extern void FUN_0055fa1c();
extern void FUN_0055fec8();
extern undefined4 FUN_00561a36();
extern uint * FUN_005632a0();

void FUN_0055f697(RestoreSound *self)

{
  int iVar1;
  uintptr_t in_ECX=(uintptr_t)self;
  undefined4 local_22c;
  undefined4 local_228;
  int local_224 [4];
  int local_214;
  undefined1 local_210 [100];
  undefined1 local_1ac [100];
  undefined1 local_148 [100];
  undefined1 local_e4 [100];
  undefined1 local_80 [100];
  int local_1c;
  undefined4 local_18 [4];
  undefined4 local_8;
  
  local_22c = 0;
  local_224[1] = 0;
  if (*(int *)(in_ECX + 0x200) == 0) {
    while (iVar1 = _AIL_enumerate_3D_providers_12(&local_22c,local_224 + 3,&local_228), iVar1 != 0)
    {
      local_214 = FUN_005632a0(local_228,&PTR_DAT_00604790);
      if (local_214 != 0) {
        local_18[0] = local_224[3];
        FUN_00561a36(local_210,&DAT_00604794,local_228);
      }
      local_214 = FUN_005632a0(local_228,&DAT_00604798);
      if (local_214 != 0) {
        local_18[1] = local_224[3];
        FUN_00561a36(local_1ac,&DAT_0060479c,local_228);
      }
      local_214 = FUN_005632a0(local_228,s_Intel_006047a0);
      if (local_214 != 0) {
        local_18[2] = local_224[3];
        FUN_00561a36(local_148,&DAT_006047a8,local_228);
      }
      local_214 = FUN_005632a0(local_228,s_hardware_006047ac);
      if (local_214 != 0) {
        local_18[3] = local_224[3];
        FUN_00561a36(local_e4,&DAT_006047b8,local_228);
      }
      local_214 = FUN_005632a0(local_228,s_software_006047bc);
      if (local_214 != 0) {
        local_8 = local_224[3];
        FUN_00561a36(local_80,&DAT_006047c8,local_228);
      }
    }
    *(undefined4 *)(in_ECX + 0x164) = 5;
    for (local_224[2] = 0; local_224[2] < 5; local_224[2] = local_224[2] + 1) {
      local_1c = _AIL_open_3D_provider_4(local_18[local_224[2]]);
      if (local_1c == 0) {
        *(int *)(in_ECX + 0x164) = local_224[2];
        *(undefined4 *)(in_ECX + 0x1e8) = local_18[local_224[2]];
        break;
      }
    }
    if (*(int *)(in_ECX + 0x164) != 5) {
      _AIL_3D_provider_attribute_12
                (*(undefined4 *)(in_ECX + 0x1e8),s_EAX_environment_selection_006047cc,local_224);
      if (local_224[0] != -1) {
        *(undefined4 *)(in_ECX + 0x1ec) = 1;
      }
      iVar1 = FUN_0055e14f(self,5);
      if (iVar1 != 0) {
        for (local_224[2] = 0; local_224[2] < *(int *)(in_ECX + 0x1e4);
            local_224[2] = local_224[2] + 1) {
          FUN_0055fa1c(local_224[2]);
        }
        for (local_224[2] = 0; local_224[2] < *(int *)(in_ECX + 0x1e4);
            local_224[2] = local_224[2] + 1) {
          *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + local_224[2] * 0xc) = 0xffffffff;
          *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 4 + local_224[2] * 0xc) = 0;
          *(undefined4 *)(*(int *)(in_ECX + 0x1f8) + 8 + local_224[2] * 0xc) = 0;
        }
        if (*(int *)(in_ECX + 0x1ec) != 0) {
          FUN_0055fec8(1);
        }
      }
    }
  }
  return;
}
