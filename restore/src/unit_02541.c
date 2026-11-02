#include "restore_types.h"
#define DAT_00705ea4 (*(undefined4 *)0x00705ea4u)
#define DAT_0070cdcc (*(undefined4 *)0x0070cdccu)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e39140 (*(undefined *)0x00e39140u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define PTR_s_Pharaoh_Model_VeryEasy_txt_00601908 (*(unsigned char * *)0x00601908u)
#define s_ALL_BUILDINGS_00601b78 (*(char (*)[14])0x00601b78u)
#define s_ALL_HOUSES_00601b08 (*(char (*)[11])0x00601b08u)
#define s_Building_model_files_out_of_sync_00601b14 (*(char (*)[33])0x00601b14u)
#define s_ERR_No_Pharaoh_model_txt_file_00601b88 (*(char (*)[30])0x00601b88u)
#define s_ERR__Building_model_has_incorrec_00601b38 (*(char (*)[64])0x00601b38u)
#define s_ERR_no_spare_screen_for_model_00601ba8 (*(char (*)[30])0x00601ba8u)
#define s_OK__model_loaded_00601af4 (*(char (*)[18])0x00601af4u)
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

undefined4 FUN_00540c90(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  int iVar7;
  int local_cc;
  undefined1 local_c8 [100];
  CHAR local_64 [100];
  
  if (DAT_00e92e1c == 0) {
    FUN_004ce6e0(s_ERR_no_spare_screen_for_model_00601ba8,0);
    return 0;
  }
  FUN_0040f8f0(DAT_00e92e1c,100000,0);
  iVar1 = FUN_004ceb60((&PTR_s_Pharaoh_Model_VeryEasy_txt_00601908)[DAT_00e39080],DAT_00e92e1c,0);
  if (iVar1 == 0) {
    FUN_004ce6e0(s_ERR_No_Pharaoh_model_txt_file_00601b88,0,0);
    return 0;
  }
  uVar2 = FUN_004ced50((&PTR_s_Pharaoh_Model_VeryEasy_txt_00601908)[DAT_00e39080]);
  iVar1 = DAT_00e92e1c;
  iVar7 = 0;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_BUILDINGS_00601b78,uVar2);
  iVar1 = iVar1 + iVar3;
  iVar3 = 299;
  do {
    iVar4 = FUN_004cbe30(iVar1,0x7b,uVar2);
    if (iVar4 == 0) break;
    iVar7 = iVar7 + 1;
    iVar1 = iVar1 + iVar4;
    iVar3 = iVar3 + -1;
  } while (0 < iVar3);
  if (iVar7 != 0x101) {
    FUN_00561a36(local_c8,s_ERR__Building_model_has_incorrec_00601b38,iVar7 + 1,0x101);
    FUN_00561a36(local_64,s__s_Would_you_still_like_to_conti_005da204,local_c8);
    iVar1 = MessageBoxA(DAT_00e39140,local_64,s_Building_model_files_out_of_sync_00601b14,4);
    if (iVar1 == 7) {
      FUN_004ce6e0(local_c8,0,0);
      return 0;
    }
  }
  iVar1 = DAT_00e92e1c;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_BUILDINGS_00601b78,uVar2);
  iVar1 = iVar1 + iVar3;
  local_cc = 299;
  puVar6 = &DAT_00705ea4;
  do {
    if (0x7083ab < (int)puVar6) break;
    iVar3 = FUN_004cbe30(iVar1,0x7b,uVar2);
    iVar1 = iVar1 + iVar3;
    iVar3 = 10;
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
    local_cc = local_cc + -1;
  } while (0 < local_cc);
  iVar1 = DAT_00e92e1c;
  iVar3 = FUN_004cbdd0(DAT_00e92e1c,s_ALL_HOUSES_00601b08,uVar2);
  iVar1 = iVar1 + iVar3;
  local_cc = 299;
  puVar6 = &DAT_0070cdcc;
  do {
    if (0x70d4ab < (int)puVar6) break;
    iVar3 = FUN_004cbe30(iVar1,0x7b,uVar2);
    iVar1 = iVar1 + iVar3;
    iVar3 = 0x16;
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
    local_cc = local_cc + -1;
  } while (0 < local_cc);
  FUN_004ce6e0(s_OK__model_loaded_00601af4,0,0);
  return 1;
}
