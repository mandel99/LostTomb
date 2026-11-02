#include "restore_types.h"
#define DAT_007083ac (*(undefined4 *)0x007083acu)
#define DAT_007090cc (*(undefined4 *)0x007090ccu)
#define DAT_00e39080 (*(uint32_t *)0x00e39080u)
#define DAT_00e39140 (*(HWND *)0x00e39140u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define PTR_s_Figure_model_VeryEasy_txt_0060191c (*(unsigned char * *)0x0060191cu)
#define s_ALL_ENEMIES_00601be8 (*(char (*)[12])0x00601be8u)
#define s_ALL_FIGURES_00601c54 (*(char (*)[12])0x00601c54u)
#define s_ERR_No_Figure_model_txt_file_00601c60 (*(char (*)[29])0x00601c60u)
#define s_ERR__Figure_model_has_incorrect_n_00601c14 (*(char (*)[62])0x00601c14u)
#define s_ERR_no_spare_screen_for_model_00601ba8 (*(char (*)[30])0x00601ba8u)
#define s_Figure_model_files_out_of_sync_00601bf4 (*(char (*)[31])0x00601bf4u)
#define s_OK__figure_model_data_loaded_00601bc8 (*(char (*)[30])0x00601bc8u)
#define s__s_Would_you_still_like_to_conti_005da204 (*(char (*)[40])0x005da204u)
extern void FUN_0040f8f0();
extern int FUN_004cbdd0();
extern int FUN_004cbe30();
extern int FUN_004cc070();
extern int FUN_004cc0e0();
extern void FUN_004ce6e0();
extern undefined4 FUN_004ceb60();
extern undefined4 FUN_004ced50();
extern undefined4 FUN_00561a36();

undefined4 FUN_00540ed0(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  int iVar7;
  int local_d4;
  int local_d0;
  int local_cc;
  undefined1 local_c8 [100];
  CHAR local_64 [100];
  
  if (DAT_00e92e1c == 0) {
    FUN_004ce6e0(s_ERR_no_spare_screen_for_model_00601ba8,0);
    return 0;
  }
  FUN_0040f8f0(DAT_00e92e1c,100000,0);
  iVar1 = FUN_004ceb60((&PTR_s_Figure_model_VeryEasy_txt_0060191c)[DAT_00e39080],DAT_00e92e1c,0);
  if (iVar1 == 0) {
    FUN_004ce6e0(s_ERR_No_Figure_model_txt_file_00601c60,0,0);
    return 0;
  }
  uVar2 = FUN_004ced50((&PTR_s_Figure_model_VeryEasy_txt_0060191c)[DAT_00e39080]);
  iVar1 = DAT_00e92e1c;
  iVar7 = 0;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_FIGURES_00601c54,uVar2);
  iVar1 = iVar1 + iVar3;
  iVar3 = 299;
  do {
    iVar4 = FUN_004cbe30(iVar1,0x7b,uVar2);
    if (iVar4 == 0) break;
    iVar7 = iVar7 + 1;
    iVar1 = iVar1 + iVar4;
    iVar3 = iVar3 + -1;
  } while (0 < iVar3);
  if (iVar7 != 0xaa) {
    FUN_00561a36(local_c8,s_ERR__Figure_model_has_incorrect_n_00601c14,iVar7 + 1,0xaa);
    FUN_00561a36(local_64,s__s_Would_you_still_like_to_conti_005da204,local_c8);
    iVar1 = MessageBoxA(DAT_00e39140,local_64,s_Figure_model_files_out_of_sync_00601bf4,4);
    if (iVar1 == 7) {
      FUN_004ce6e0(local_c8,0,0);
      return 0;
    }
  }
  iVar1 = DAT_00e92e1c;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_FIGURES_00601c54,uVar2);
  iVar1 = iVar1 + iVar3;
  local_d4 = 299;
  puVar6 = &DAT_007090cc;
  do {
    if (0x70a8db < (int)puVar6) break;
    iVar3 = FUN_004cbe30(iVar1,0x7b,uVar2);
    iVar1 = iVar1 + iVar3;
    iVar3 = 0xe;
    do {
      iVar7 = FUN_004cc0e0(iVar1);
      iVar1 = iVar1 + iVar7;
      uVar5 = FUN_004cc070(iVar1);
      *puVar6 = uVar5;
      iVar7 = FUN_004cbe30(iVar1,0x2c,uVar2);
      iVar1 = iVar1 + iVar7;
      puVar6 = puVar6 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    local_d4 = local_d4 + -1;
  } while (0 < local_d4);
  iVar1 = DAT_00e92e1c;
  local_d0 = 0;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_ENEMIES_00601be8,uVar2);
  iVar1 = iVar1 + iVar3;
  local_d4 = 299;
  local_cc = 0;
  do {
    if (0x3b < local_cc) break;
    iVar3 = FUN_004cbe30(iVar1,0x7b,uVar2);
    iVar1 = iVar1 + iVar3;
    iVar3 = 0xe;
    /* Native 5410C9..5410D5 uses a 56-byte enemy row. Scaling a
       DWORD pointer by 56 overwrites the following civilian model table. */
    puVar6 = (undefined4 *)(0x007083acu + (uint32_t)(local_cc + local_d0) * 56u);
    do {
      iVar7 = FUN_004cc0e0(iVar1);
      iVar1 = iVar1 + iVar7;
      uVar5 = FUN_004cc070(iVar1);
      *puVar6 = uVar5;
      iVar7 = FUN_004cbe30(iVar1,0x2c,uVar2);
      iVar1 = iVar1 + iVar7;
      puVar6 = puVar6 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    local_d0 = local_d0 + 1;
    if (4 < local_d0) {
      local_d0 = 0;
      local_cc = local_cc + 5;
    }
    local_d4 = local_d4 + -1;
  } while (0 < local_d4);
  FUN_004ce6e0(s_OK__figure_model_data_loaded_00601bc8,0,0);
  return 1;
}
