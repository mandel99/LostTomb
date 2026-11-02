#include "restore_types.h"
#define DAT_007854a8 (*(undefined4 *)0x007854a8u)
#define DAT_00e39140 (*(undefined *)0x00e39140u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define s_ERR_No_auto_reason_phrases_file_005da260 (*(char (*)[32])0x005da260u)
#define s_ERR__reason_file_has_too_many_li_005da22c (*(char (*)[49])0x005da22cu)
#define s_ERR_no_spare_screen_for_reason_t_005da298 (*(char (*)[37])0x005da298u)
#define s_OK__reason_table_loaded_005da1cc (*(char (*)[25])0x005da1ccu)
#define s__s_Would_you_still_like_to_conti_005da204 (*(char (*)[40])0x005da204u)
#define s_auto_reason_phrases_txt_005da280 (*(char (*)[24])0x005da280u)
#define s_reason_phrase_data_invalid_005da1e8 (*(char (*)[27])0x005da1e8u)
extern void FUN_0040f8f0();
extern int FUN_004cbe30();
extern int FUN_004cc070();
extern int FUN_004cc0e0();
extern void FUN_004ce6e0();
extern undefined4 FUN_004ceb60();
extern undefined4 FUN_004ced50();
extern undefined4 FUN_00561a36();

undefined4 FUN_0044efa0(void)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined1 *puVar5;
  int iVar6;
  undefined1 local_c8 [100];
  CHAR local_64 [100];
  
  if (DAT_00e92e1c == 0) {
    FUN_004ce6e0(s_ERR_no_spare_screen_for_reason_t_005da298,0);
    return 0;
  }
  FUN_0040f8f0(DAT_00e92e1c,100000,0);
  iVar2 = FUN_004ceb60(s_auto_reason_phrases_txt_005da280,DAT_00e92e1c,0);
  if (iVar2 == 0) {
    FUN_004ce6e0(s_ERR_No_auto_reason_phrases_file_005da260,0,0);
    return 0;
  }
  uVar3 = FUN_004ced50(s_auto_reason_phrases_txt_005da280);
  iVar2 = DAT_00e92e1c;
  iVar6 = 0;
  iVar4 = FUN_004cbe30(DAT_00e92e1c,0x7b,uVar3);
  if (iVar4 != 0) {
    do {
      iVar6 = iVar6 + 1;
      if (0x36 < iVar6) goto LAB_0044f04f;
      iVar2 = iVar2 + iVar4;
      iVar4 = FUN_004cbe30(iVar2,0x7b,uVar3);
    } while (iVar4 != 0);
    if (iVar6 == 0x36) goto LAB_0044f0b2;
  }
LAB_0044f04f: ;
  FUN_00561a36(local_c8,s_ERR__reason_file_has_too_many_li_005da22c,0x36);
  FUN_00561a36(local_64,s__s_Would_you_still_like_to_conti_005da204,local_c8);
  iVar2 = MessageBoxA(DAT_00e39140,local_64,s_reason_phrase_data_invalid_005da1e8,4);
  if (iVar2 == 7) {
    FUN_004ce6e0(local_c8,0,0);
    return 0;
  }
LAB_0044f0b2: ;
  puVar5 = &DAT_007854a8;
  iVar2 = DAT_00e92e1c;
  do {
    iVar4 = FUN_004cbe30(iVar2,0x7b,uVar3);
    iVar2 = iVar2 + iVar4;
    iVar4 = 0;
    do {
      iVar6 = FUN_004cc0e0(iVar2);
      iVar2 = iVar2 + iVar6;
      uVar1 = FUN_004cc070(iVar2);
      puVar5[iVar4] = uVar1;
      iVar6 = FUN_004cbe30(iVar2,0x2c,uVar3);
      iVar2 = iVar2 + iVar6;
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0x24);
    puVar5 = puVar5 + 0x24;
  } while ((int)puVar5 < 0x785c40);
  FUN_004ce6e0(s_OK__reason_table_loaded_005da1cc,0,0);
  return 1;
}
