#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005f8b54 (*(undefined4 *)0x005f8b54u)
#define DAT_005f8b68 (*(undefined4 *)0x005f8b68u)
#define DAT_006059e4 (*(int32_t *)0x006059e4u)
#define DAT_00705c1c (*(int32_t *)0x00705c1cu)
#define DAT_00bca2e0 (*(int32_t *)0x00bca2e0u)
#define DAT_00d3b460 (*(int32_t *)0x00d3b460u)
#define DAT_00d3b470 (*(int32_t *)0x00d3b470u)
#define DAT_00d3b480 (*(int32_t *)0x00d3b480u)
#define DAT_00d3b490 (*(int32_t *)0x00d3b490u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
#define DAT_00e38e40 (*(int32_t *)0x00e38e40u)
#define DAT_00e94e0c (*(int32_t *)0x00e94e0cu)
#define DAT_00e951d4 (*(int32_t *)0x00e951d4u)
#define DAT_00e951d8 (*(int32_t *)0x00e951d8u)
#define DAT_00ea181c (*(int32_t *)0x00ea181cu)
#define DAT_00ea5cd0 (*(undefined4 *)0x00ea5cd0u)
#define DAT_00ea5e04 (*(undefined4 *)0x00ea5e04u)
#define DAT_00ea5e0c (*(undefined4 *)0x00ea5e0cu)
#define DAT_00ea5f14 (*(undefined4 *)0x00ea5f14u)
#define DAT_00ea627c (*(undefined *)0x00ea627cu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f535bc (*(int32_t *)0x00f535bcu)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f55b9c (*(uint32_t *)0x00f55b9cu)
#define DAT_00f55ba0 (*(uint32_t *)0x00f55ba0u)
#define DAT_00f56f84 (*(uint32_t *)0x00f56f84u)
#define s_Auto_removed_man_mades_tiles_005f8a94 (*(char (*)[29])0x005f8a94u)
#define s_MAP_ROTATION_005f8b30 (*(char (*)[14])0x005f8b30u)
#define s_Pop_is_005f8bc0 (*(char (*)[8])0x005f8bc0u)
#define s_alert_messages_005f8ad0 (*(char (*)[16])0x005f8ad0u)
#define s_births_005f8b98 (*(char (*)[11])0x005f8b98u)
#define s_building_in_progress_005f8a04 (*(char (*)[22])0x005f8a04u)
#define s_deaths_005f8b8c (*(char (*)[11])0x005f8b8cu)
#define s_docks_list__005f8b58 (*(char (*)[14])0x005f8b58u)
#define s_in_houses_005f8b7c (*(char (*)[13])0x005f8b7cu)
#define s_last_alert_message_shown_005f8ab4 (*(char (*)[26])0x005f8ab4u)
#define s_map_style_005f8b24 (*(char (*)[11])0x005f8b24u)
#define s_mission_in_place_005f8af8 (*(char (*)[18])0x005f8af8u)
#define s_mouse_movement_005f8a2c (*(char (*)[16])0x005f8a2cu)
#define s_mouse_x_005f8a48 (*(char (*)[9])0x005f8a48u)
#define s_mouse_y_005f8a3c (*(char (*)[9])0x005f8a3cu)
#define s_natives_are_restless_005f8b0c (*(char (*)[22])0x005f8b0cu)
#define s_nof_docks__005f8b6c (*(char (*)[13])0x005f8b6cu)
#define s_side_panel_x1_005f8a1c (*(char (*)[15])0x005f8a1cu)
#define s_spare_cap_005f8ba4 (*(char (*)[14])0x005f8ba4u)
#define s_test_value1a_005f8a84 (*(char (*)[14])0x005f8a84u)
#define s_test_value2a_005f8a74 (*(char (*)[14])0x005f8a74u)
#define s_test_value3a_005f8a64 (*(char (*)[14])0x005f8a64u)
#define s_test_value4a_005f8a54 (*(char (*)[14])0x005f8a54u)
#define s_tot_cap_005f8bb4 (*(char (*)[12])0x005f8bb4u)
#define s_was_in_int_005f8aec (*(char (*)[12])0x005f8aecu)
#define s_was_in_set_005f8ae0 (*(char (*)[12])0x005f8ae0u)
#define s_wine_sources___005f8b40 (*(char (*)[17])0x005f8b40u)
extern int FUN_00458f80();
extern void FUN_004cc770();
extern void FUN_004d6860();

void FUN_00534610(void)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int local_4;
  
  if (DAT_00ea181c == 1) {
    puVar2 = &DAT_00ea627c;
    iVar3 = 0x5e;
    do {
      local_4 = 0xffff;
      DAT_00e20778 = 0;
      if (*(char *)((int)puVar2 + -0x32) == '\x01') {
        iVar1 = iVar3 + -0xe;
        DAT_00e20778 = 0;
        FUN_004cc770(s_Pop_is_005f8bc0,8,iVar1,DAT_00f56f84,0xffff);
        FUN_004d6860(puVar2[-4],0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004cc770(s_tot_cap_005f8bb4,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff);
        FUN_004d6860(*puVar2,0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004cc770(s_spare_cap_005f8ba4,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff);
        FUN_004d6860(puVar2[1],0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004cc770(s_births_005f8b98,8,iVar3,DAT_00f56f84,0xffff);
        FUN_004d6860(puVar2[0x9aa],0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar3,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004cc770(s_deaths_005f8b8c,DAT_00e20778 + 8,iVar3,DAT_00f56f84,0xffff);
        FUN_004d6860(puVar2[0x9ab],0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar3,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        iVar1 = FUN_00458f80(DAT_00eaac28);
        if (iVar1 != puVar2[-4]) {
          local_4 = 0xf800;
        }
        FUN_004cc770(s_in_houses_005f8b7c,DAT_00e20778 + 8,iVar3,DAT_00f56f84,local_4);
        FUN_004d6860(iVar1,0x40,&DAT_0057c048,DAT_00e20778 + 8,iVar3,DAT_00f56f84,local_4,
                     DAT_00e38da8,DAT_00e38da4);
        iVar1 = iVar3 + 0xe;
        DAT_00e20778 = 0;
        FUN_004cc770(s_nof_docks__005f8b6c,8,iVar1,DAT_00f56f84,0xffff);
        FUN_004d6860(DAT_00705c1c,0x40,&DAT_005f8b68,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004cc770(s_docks_list__005f8b58,DAT_00e20778 + 8,iVar1,DAT_00f56f84,0xffff);
        local_4 = 0;
        do {
          FUN_004d6860((int)*(short *)((int)&DAT_00ea5cd0 + (DAT_00eaac28 * 0x24ec + local_4) * 2),
                       0x40,&DAT_005f8b54,DAT_00e20778 + 4,iVar1,DAT_00f56f84,0xffff,DAT_00e38da8,
                       DAT_00e38da4);
          local_4 = local_4 + 1;
        } while (local_4 < 10);
        DAT_00e20778 = 0;
        FUN_004cc770(s_wine_sources___005f8b40,8,iVar3 + 0x1c,DAT_00f56f84,0xffff);
        FUN_004d6860((&DAT_00ea5f14)[DAT_00eaac28 * 0x1276],0x40,&DAT_005f8b68,DAT_00e20778 + 8,
                     iVar3 + 0x1c,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00bca2e0,0x40,s_MAP_ROTATION_005f8b30,8,iVar3 + 0x2a,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00e94e0c,0x40,s_map_style_005f8b24,8,iVar3 + 0x38,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860((&DAT_00ea5e04)[DAT_00eaac28 * 0x1276],0x40,s_natives_are_restless_005f8b0c,8,
                     iVar3 + 0x46,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860((&DAT_00ea5e0c)[DAT_00eaac28 * 0x1276],0x40,s_mission_in_place_005f8af8,
                     DAT_00e20778 + 0x6c,iVar3 + 0x46,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4)
        ;
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00f55ba0,0x40,s_was_in_int_005f8aec,8,iVar3 + 0x54,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00f55b9c,0x40,s_was_in_set_005f8ae0,DAT_00e20778 + 0x6c,iVar3 + 0x54,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00e951d8,0x40,s_alert_messages_005f8ad0,8,iVar3 + 0x62,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00e951d4,0x40,s_last_alert_message_shown_005f8ab4,DAT_00e20778 + 0x6c,
                     iVar3 + 0x62,DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00f535bc,0x40,s_Auto_removed_man_mades_tiles_005f8a94,8,iVar3 + 0x70,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00d3b490,0x40,s_test_value1a_005f8a84,8,iVar3 + 0x7e,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00d3b480,0x40,s_test_value2a_005f8a74,DAT_00e20778 + 0x6c,iVar3 + 0x7e,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00d3b470,0x40,s_test_value3a_005f8a64,8,iVar3 + 0x8c,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00d3b460,0x40,s_test_value4a_005f8a54,DAT_00e20778 + 0x6c,iVar3 + 0x8c,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00e38e34,0x40,s_mouse_x_005f8a48,8,iVar3 + 0x9a,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00e38e30,0x40,s_mouse_y_005f8a3c,DAT_00e20778 + 0x6c,iVar3 + 0x9a,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_00e38e40,0x40,s_mouse_movement_005f8a2c,8,iVar3 + 0xa8,DAT_00f56f84,0xffff,
                     DAT_00e38da8,DAT_00e38da4);
        FUN_004d6860(DAT_00f53f64,0x40,s_side_panel_x1_005f8a1c,DAT_00e20778 + 0x6c,iVar3 + 0xa8,
                     DAT_00f56f84,0xffff,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860(DAT_006059e4,0x40,s_building_in_progress_005f8a04,8,iVar3 + 0xb6,DAT_00f56f84,
                     0xffff,DAT_00e38da8,DAT_00e38da4);
      }
      iVar3 = iVar3 + 0x32;
      puVar2 = puVar2 + 0x1276;
    } while (iVar3 < 0x1bd);
  }
  return;
}
