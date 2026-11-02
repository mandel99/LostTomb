#include "restore_types.h"
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e39140 (*(undefined *)0x00e39140u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00ea1384 (*(undefined4 *)0x00ea1384u)
#define PTR_s_Tax_Sentiment_Model_VeryEasy_txt_00601930 (*(unsigned char * *)0x00601930u)
#define s_ERR_No_Figure_model_txt_file_00601c60 (*(char (*)[29])0x00601c60u)
#define s_ERR__Tax_sentiment_model_has_inc_00601cc8 (*(char (*)[69])0x00601cc8u)
#define s_ERR_no_spare_screen_for_model_00601ba8 (*(char (*)[30])0x00601ba8u)
#define s_OK__tax_sentiment_data_loaded_00601c80 (*(char (*)[31])0x00601c80u)
#define s__s_Would_you_still_like_to_conti_005da204 (*(char (*)[40])0x005da204u)
#define s_tax_sentiment_model_files_out_of_00601ca0 (*(char (*)[38])0x00601ca0u)
extern void FUN_0040f8f0();
extern int FUN_004cbe30();
extern int FUN_004cc070();
extern int FUN_004cc0e0();
extern void FUN_004ce6e0();
extern undefined4 FUN_004ceb60();
extern undefined4 FUN_004ced50();
extern undefined4 FUN_00561a36();

undefined4 FUN_00541160(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  int local_cc;
  undefined1 local_c8 [100];
  CHAR local_64 [100];
  
  if (DAT_00e92e1c == 0) {
    FUN_004ce6e0(s_ERR_no_spare_screen_for_model_00601ba8,0);
    return 0;
  }
  FUN_0040f8f0(DAT_00e92e1c,100000,0);
  iVar1 = FUN_004ceb60((&PTR_s_Tax_Sentiment_Model_VeryEasy_txt_00601930)[DAT_00e39080],DAT_00e92e1c
                       ,0);
  if (iVar1 == 0) {
    FUN_004ce6e0(s_ERR_No_Figure_model_txt_file_00601c60,0,0);
    return 0;
  }
  uVar2 = FUN_004ced50((&PTR_s_Tax_Sentiment_Model_VeryEasy_txt_00601930)[DAT_00e39080]);
  iVar5 = 0;
  iVar6 = 299;
  iVar1 = DAT_00e92e1c;
  do {
    iVar3 = FUN_004cbe30(iVar1,0x7b,uVar2);
    if (iVar3 == 0) break;
    iVar5 = iVar5 + 1;
    iVar1 = iVar1 + iVar3;
    iVar6 = iVar6 + -1;
  } while (0 < iVar6);
  if (iVar5 != 0x1a) {
    FUN_00561a36(local_c8,s_ERR__Tax_sentiment_model_has_inc_00601cc8,iVar5 + 1,0x1a);
    FUN_00561a36(local_64,s__s_Would_you_still_like_to_conti_005da204,local_c8);
    iVar1 = MessageBoxA(DAT_00e39140,local_64,s_tax_sentiment_model_files_out_of_00601ca0,4);
    if (iVar1 == 7) {
      FUN_004ce6e0(local_c8,0,0);
      return 0;
    }
  }
  local_cc = 299;
  puVar7 = &DAT_00ea1384;
  iVar1 = DAT_00e92e1c;
  do {
    if (0xea17fb < (int)puVar7) break;
    iVar5 = FUN_004cbe30(iVar1,0x7b,uVar2);
    iVar1 = iVar1 + iVar5;
    iVar5 = 0xb;
    do {
      iVar6 = FUN_004cc0e0(iVar1);
      iVar1 = iVar1 + iVar6;
      uVar4 = FUN_004cc070(iVar1);
      *puVar7 = uVar4;
      iVar6 = FUN_004cbe30(iVar1,0x2c,uVar2);
      iVar1 = iVar1 + iVar6;
      puVar7 = puVar7 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    local_cc = local_cc + -1;
  } while (0 < local_cc);
  FUN_004ce6e0(s_OK__tax_sentiment_data_loaded_00601c80,0,0);
  return 1;
}
