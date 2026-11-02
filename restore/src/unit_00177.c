#include "restore_types.h"
#include "startup_geometry.h"
#include "preview_footprint_seeds.h"
#include "placement_preview.h"
#include "monument_base_queries.h"
#include "campaign_parser.h"
#include "video_player.h"
#include "game_diagnostics.h"
#define DAT_006059fc (*(uint32_t *)0x006059fcu)
#define DAT_00605a00 (*(uint32_t *)0x00605a00u)
#define DAT_00605a04 (*(uint32_t *)0x00605a04u)
#define DAT_00605a08 (*(uint32_t *)0x00605a08u)
#define DAT_00605a0c (*(uint32_t *)0x00605a0cu)
#define DAT_00605a30 (*(uint32_t *)0x00605a30u)
#define DAT_00605a34 (*(uint32_t *)0x00605a34u)
#define DAT_00605a38 (*(uint32_t *)0x00605a38u)
#define DAT_00605a3c (*(uint32_t *)0x00605a3cu)
#define DAT_006de514 (*(char *)0x006de514u)
#define DAT_006de515 (*(char *)0x006de515u)
#define DAT_006fdb38 (*(undefined4 *)0x006fdb38u)
#define DAT_006fdc54 RESTORE_CURRENT_VIDEO
#define DAT_006fdcb8 (*(uint32_t *)0x006fdcb8u)
#define DAT_00d3b43d (*(undefined1 *)0x00d3b43du)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dd6 (*(undefined1 *)0x00e38dd6u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
#define DAT_00e38edc (*(uint32_t *)0x00e38edcu)
#define DAT_00e39128 (*(uint32_t *)0x00e39128u)
#define DAT_00e3912c (*(uint32_t *)0x00e3912cu)
#define DAT_00e39130 (*(uint32_t *)0x00e39130u)
#define DAT_00e39134 (*(uint32_t *)0x00e39134u)
#define DAT_00e39138 (*(uint32_t *)0x00e39138u)
#define DAT_00e3913c (*(uint32_t *)0x00e3913cu)
#define DAT_00e39140 (*(uint32_t *)0x00e39140u)
#define s_Binks_high_intro_big_bik_00578c70 (*(char (*)[25])0x00578c70u)
#define s_Binks_low_intro_bik_00578c8c (*(char (*)[20])0x00578c8cu)
#define s_ERR_Exited_Pharaoh__Could_not_fi_00578ebc (*(char (*)[58])0x00578ebcu)
#define s_ERR_Exited_Pharaoh__Could_not_fi_00578ef8 (*(char (*)[52])0x00578ef8u)
#define s_ERR_Exited_Pharaoh___Pharaoh_Tex_00578fe8 (*(char (*)[89])0x00578fe8u)
#define s_ERR_Exited_Pharaoh__bmp_files_ha_00578e4c (*(char (*)[110])0x00578e4cu)
#define s_ERR_Exited_Pharaoh__could_not_fi_00579068 (*(char (*)[57])0x00579068u)
#define s_ERR_Exited_Pharaoh__could_not_lo_00578cf0 (*(char (*)[49])0x00578cf0u)
#define s_ERR_Exited_Pharaoh__could_not_lo_00578d64 (*(char (*)[50])0x00578d64u)
#define s_ERR_Exited_Pharaoh__could_not_lo_00578da8 (*(char (*)[55])0x00578da8u)
#define s_ERR_Exited_Pharaoh__could_not_lo_00578de0 (*(char (*)[44])0x00578de0u)
#define s_ERR_Exited_Pharaoh__could_not_lo_00578e0c (*(char (*)[42])0x00578e0cu)
#define s_ERR_Exited_Pharaoh__could_not_tr_00578d24 (*(char (*)[61])0x00578d24u)
#define s_ERR_Exited_Pharaoh__couldn_t_sta_00578f7c (*(char (*)[48])0x00578f7cu)
#define s_ERR_Exited_Pharaoh__insufficient_00578f2c (*(char (*)[48])0x00578f2cu)
#define s_OK__Direct_draw_initialised__00578f5c (*(char (*)[29])0x00578f5cu)
#define s_OK__Direct_sound_initialised__00578cd0 (*(char (*)[30])0x00578cd0u)
#define s_OK__Game_text_loaded__00578fac (*(char (*)[22])0x00578facu)
#define s_OK__Launching_game_loop__00578cb4 (*(char (*)[25])0x00578cb4u)
#define s_OK__Pharaoh_setup__00578e38 (*(char (*)[19])0x00578e38u)
#define s_Pharaoh_MM_eng_00579058 (*(char (*)[15])0x00579058u)
#define s_Pharaoh_Text_eng_00579044 (*(char (*)[17])0x00579044u)
#define s_campaign_txt_00578d98 (*(char (*)[13])0x00578d98u)
#define s_x__d_y__d_w__d_h__d_00578ca0 (*(char (*)[20])0x00578ca0u)
extern undefined4 FUN_004143c0();
extern undefined4 FUN_00414420();
extern void FUN_00414750();
extern undefined4 FUN_004147f0();
extern void FUN_004149d0();
extern undefined4 FUN_004268c0();
extern undefined4 FUN_0044efa0();
extern void FUN_004ce6e0();
extern undefined4 FUN_004cec40();
extern void FUN_004cf8e0();
extern undefined4 FUN_004cf960();
extern void FUN_004cff50();
extern void FUN_004dee10();
extern void FUN_004e63b0();
extern void FUN_0051e930();
extern void FUN_0051f780();
extern void FUN_005394f0();
extern void FUN_00540b50();
extern undefined4 FUN_00540c90();
extern undefined4 FUN_00540ed0();
extern undefined4 FUN_00541160();
extern undefined4 FUN_00541310();
extern void FUN_00541d50();
extern undefined4 FUN_00561a36();

undefined4 FUN_00413ed0(undefined4 param_1)

{
  BOOL BVar1;
  int iVar2;
  undefined4 *puVar3;
  tagMSG *ptVar4;
  char *pcVar5;
  undefined1 local_10c [100];
  tagMSG local_a8 [4];
  undefined1 local_28 [8];
  uint local_20;
  volatile int exception_scope=0;

  /* MSVC must own the real SEH registration. Independent C locals are not
     the contiguous native scope/handler record and must not enter FS:[0]. */
  __try {
  local_28[0] = 0x44;
  puVar3 = &DAT_006fdb38;
  for (iVar2 = 0x44; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  ptVar4 = local_a8;
  for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
    ptVar4->hwnd = (HWND)0x0;
    ptVar4 = (tagMSG *)&ptVar4->message;
  }
  DAT_00e38dd6 = 0;
  iVar2 = FUN_004147f0();
  if (iVar2 == 0) {
    FUN_004ce6e0(s_ERR_Exited_Pharaoh__could_not_fi_00579068,0,0);
    return 0;
  }
  iVar2 = FUN_004cec40(s_Pharaoh_Text_eng_00579044,s_Pharaoh_MM_eng_00579058);
  if (iVar2 == 0) {
    FUN_004ce6e0(s_ERR_Exited_Pharaoh___Pharaoh_Tex_00578fe8,0,0);
    return 0;
  }
  FUN_004268c0(local_28);
  DAT_00e38e62 = local_28[0];
  FUN_004149d0();
  FUN_004ce6e0(s_OK__Game_text_loaded__00578fac,0,0);
  iVar2 = FUN_004cf960(param_1);
  if (iVar2 == 0) {
    FUN_004ce6e0(s_ERR_Exited_Pharaoh__couldn_t_sta_00578f7c,0,0);
    CoUninitialize();
    return 0;
  }
  exception_scope = 1;
  FUN_004ce6e0(s_OK__Direct_draw_initialised__00578f5c,0,0);
  local_20 = FUN_00414420();
  if (local_20 == 1) {
    pcVar5 = s_ERR_Exited_Pharaoh__insufficient_00578f2c;
  }
  else if (local_20 == 2) {
    pcVar5 = s_ERR_Exited_Pharaoh__Could_not_fi_00578ef8;
  }
  else if (local_20 == 3) {
    pcVar5 = s_ERR_Exited_Pharaoh__Could_not_fi_00578ebc;
  }
  else if (local_20 == 0xfffffffe) {
    pcVar5 = s_ERR_Exited_Pharaoh__bmp_files_ha_00578e4c;
  }
  else {
    FUN_004ce6e0(s_OK__Pharaoh_setup__00578e38,0,0);
    local_20 = FUN_00540c90();
    if (local_20 == 0) {
      pcVar5 = s_ERR_Exited_Pharaoh__could_not_lo_00578e0c;
      local_20 = 0;
    }
    else {
      local_20 = FUN_00540ed0();
      if (local_20 == 0) {
        pcVar5 = s_ERR_Exited_Pharaoh__could_not_lo_00578de0;
        local_20 = 0;
      }
      else {
        local_20 = FUN_00541160();
        if (local_20 == 0) {
          pcVar5 = s_ERR_Exited_Pharaoh__could_not_lo_00578da8;
          local_20 = 0;
        }
        else {
          local_20 = FUN_0041ae40(RESTORE_CAMPAIGN,s_campaign_txt_00578d98);
          local_20 = local_20 & 0xff;
          if (local_20 == 0) {
            pcVar5 = s_ERR_Exited_Pharaoh__could_not_lo_00578d64;
            local_20 = 0;
          }
          else {
            local_20 = FUN_00541310();
            if (local_20 == 0) {
              pcVar5 = s_ERR_Exited_Pharaoh__could_not_tr_00578d24;
              local_20 = 0;
            }
            else {
              local_20 = FUN_0044efa0();
              local_20 = local_20 & 0xff;
              if (local_20 != 0) {
                FUN_005394f0();
                FUN_0051f780();
                FUN_004ce6e0(s_OK__Direct_sound_initialised__00578cd0,0,0);
                DAT_00605a3c = 0;
                DAT_00605a38 = 0;
                DAT_00605a34 = 2;
                DAT_00605a30 = 0;
                DAT_006de515 = 0;
                DAT_006de514 = 0;
                DAT_00605a0c = 0;
                DAT_00605a08 = 0;
                DAT_00605a04 = 1;
                DAT_00605a00 = 0;
                DAT_006059fc = 0xffffffff;
                FUN_00540b50();
                restore_preview_footprint_seeds_initialize();
                FUN_004013f0((void *)0x6de650u);
                FUN_0040d7e0((void *)0x6fd5d8u);
                FUN_00416f60((void *)0x6fd368u);
                FUN_004e6590((void *)0x6de548u);
                FUN_005456a0((void *)0x6f9320u);
                FUN_00558b40((void *)0x6f5648u);
                FUN_00555de0((void *)0x6de9e0u);
                FUN_0054f3d0((void *)0x6e9188u);
                restore_placement_preview_initialize_classes();
                restore_monument_base_initialize();
                FUN_00541d50();
                FUN_004dee10();
                DAT_006fdcb8 = 0;
                FUN_0051e930();
                DAT_00e38dda = 0x44;
                DAT_00e38dca = 1;
                FUN_004cf8e0();
                DAT_00e38dd6 = 1;
                FUN_004ce6e0(s_OK__Launching_game_loop__00578cb4,0,0);
                FUN_00561a36(local_10c,s_x__d_y__d_w__d_h__d_00578ca0,DAT_00e39130,DAT_00e39134,
                             DAT_00e39138 - DAT_00e39130,DAT_00e3913c - DAT_00e39134);
                if (DAT_006fdc54 != 0) {
                  if (DAT_00e38edc < 0xe9) {
                    pcVar5 = s_Binks_low_intro_bik_00578c8c;
                  }
                  else {
                    pcVar5 = s_Binks_high_intro_big_bik_00578c70;
                  }
                  FUN_00413580(RESTORE_CURRENT_VIDEO,pcVar5,(DAT_00e38db0 + -0x1e0) / 2,(DAT_00e38dac + -0x11e) / 2);
                }
LAB_00414315: ;
                BVar1 = PeekMessageA(local_a8,(HWND)0x0,0,0,0);
                if (BVar1 == 0) {
                  if ((DAT_00e39128 == 0) && (DAT_00e3912c != 0)) {
                    FUN_004143c0();
                  }
                  else {
                    WaitMessage();
                  }
                }
                else {
                  BVar1 = GetMessageA(local_a8,(HWND)0x0,0,0);
                  if (BVar1 == 0) goto LAB_00414082;
                  TranslateMessage(local_a8);
                  DispatchMessageA(local_a8);
                }
                if (DAT_00d3b43d == '\x01') {
                  PostMessageA(DAT_00e39140,0x10,0,0);
                  DAT_00d3b43d = '\0';
                }
                goto LAB_00414315;
              }
              pcVar5 = s_ERR_Exited_Pharaoh__could_not_lo_00578cf0;
            }
          }
        }
      }
    }
  }
  FUN_004ce6e0(pcVar5,0,0);
LAB_00414082: ;
  FUN_004e63b0();
  FUN_00414750();
  FUN_004cff50();
  return local_a8[0].wParam;
  } __except(exception_scope?restore_game_startup_exception_filter(GetExceptionInformation()):EXCEPTION_CONTINUE_SEARCH) {
    return local_a8[0].wParam;
  }
}
