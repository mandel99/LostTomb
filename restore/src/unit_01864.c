#include "restore_types.h"
#include "sound_streams.h"
#include "music_manager.h"
#include "startup_probe.h"
#include "game_diagnostics.h"
#define DAT_006059e8 (*(uint32_t *)0x006059e8u)
#define DAT_006fdc38 (*(uint32_t *)0x006fdc38u)
#define DAT_006fdc40 (*(uint32_t *)0x006fdc40u)
#define DAT_0078d79c (*(uint32_t *)0x0078d79cu)
#define DAT_0078d7a0 (*(uint32_t *)0x0078d7a0u)
#define DAT_0078d7a8 (*(uint32_t *)0x0078d7a8u)
#define DAT_00bca2c4 (*(uint32_t *)0x00bca2c4u)
#define DAT_00bcb9c0 (*(uint32_t *)0x00bcb9c0u)
#define DAT_00bcb9cc (*(uint16_t *)0x00bcb9ccu)
#define DAT_00bcb9f2 (*(uint16_t *)0x00bcb9f2u)
#define DAT_00bcd630 (*(uint8_t *)0x00bcd630u)
#define DAT_00d05700 (*(uint32_t *)0x00d05700u)
#define DAT_00d05704 (*(uint32_t *)0x00d05704u)
#define DAT_00d05708 (*(uint32_t *)0x00d05708u)
#define DAT_00d0570c (*(uint32_t *)0x00d0570cu)
#define DAT_00d3a6c0 (*(uint32_t *)0x00d3a6c0u)
#define DAT_00d3a6e4 (*(uint32_t *)0x00d3a6e4u)
#define DAT_00e38dcc (*(uint32_t *)0x00e38dccu)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e64 (*(char *)0x00e38e64u)
#define DAT_00e3908c (*(undefined1 *)0x00e3908cu)
#define DAT_00e390ec (*(uint32_t *)0x00e390ecu)
#define DAT_00e92bf0 (*(uint32_t *)0x00e92bf0u)
#define DAT_00e92e0c (*(uint32_t *)0x00e92e0cu)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea6210 (*(undefined4 *)0x00ea6210u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00f55b88 (*(uint32_t *)0x00f55b88u)
#define DAT_00f55b8c (*(uint32_t *)0x00f55b8cu)
#define DAT_00f55b90 (*(uint32_t *)0x00f55b90u)
#define DAT_00f55b94 (*(uint32_t *)0x00f55b94u)
#define _DAT_00e94dbc (*(undefined4 *)0x00e94dbcu)
#define s_audio_music_battle_mp3_005e1de0 (*(char (*)[23])0x005e1de0u)
extern void FUN_00405f80();
extern void FUN_0040f270();
extern void FUN_00416bc0();
extern void FUN_0041d560();
extern int FUN_004b5640();
extern undefined4 FUN_004c8750();
extern undefined4 FUN_004c9750();
extern undefined4 FUN_004ca930();
extern void FUN_004cabd0();
extern void FUN_004cf1f0();
extern void FUN_004cf210();
extern void FUN_004cf460();
extern void FUN_004d0280();
extern void FUN_004d5c30();
extern void FUN_004d6070();
extern undefined4 FUN_004d6aa0();
extern undefined4 FUN_004d6b00();
extern void FUN_004d7090();
extern void FUN_004d71c0();
extern void FUN_004d75d0();
extern void FUN_004d8310();
extern undefined4 FUN_004ddcb0();
extern void FUN_004e2720();
extern void FUN_0051f0a0();
extern void FUN_0051f680();
extern void FUN_00536620();
extern void FUN_0053acf0();
extern void FUN_0053b6f0();
extern void FUN_0053b960();
extern uint32_t FUN_005605a0(uint32_t argument);
extern uint FUN_00562348();

void FUN_004d6b60(void)

{
  char cVar1;
  int iVar2;
  BOOL BVar3;
  int iVar4;
  int pending_ticks;
  int trace_after_save;
  undefined4 uVar5;
  tagMSG tStack_1c;
  
  restore_game_progress(1);
  DAT_00e390ec = DAT_00e390ec + 1;
  DAT_00e92e0c = timeGetTime();
  FUN_0040f270();
  pending_ticks = FUN_004ddcb0();
  iVar4 = 0;
  if (0 < pending_ticks) {
    do {
      _DAT_00e94dbc = _DAT_00e94dbc + 1;
      restore_game_progress(2);
      FUN_004d7090();
      restore_game_progress(1);
      if (DAT_00e3908c == '\0') {
LAB_004d6c41: ;
        pending_ticks = 0;
      }
      else {
        BVar3 = PeekMessageA(&tStack_1c,(HWND)0x0,0x205,0x205,0);
        if (BVar3 != 0) {
          iVar2 = PeekMessageA(&tStack_1c,(HWND)0x0,0x200,0x209,1);
          while (iVar2 != 0) {
            iVar2 = PeekMessageA(&tStack_1c,(HWND)0x0,0x200,0x209,1);
          }
          iVar2 = PeekMessageA(&tStack_1c,(HWND)0x0,0x100,0x108,1);
          while (iVar2 != 0) {
            iVar2 = PeekMessageA(&tStack_1c,(HWND)0x0,0x100,0x108,1);
          }
          DAT_00e3908c = '\0';
          goto LAB_004d6c41;
        }
        if (DAT_00e3908c == '\0') goto LAB_004d6c41;
      }
      FUN_004d5c30();
      iVar4 = iVar4 + 1;
    } while (iVar4 < pending_ticks);
  }
  trace_after_save=restore_game_trace_post_save_frame();
  if(trace_after_save)restore_game_trace("post-save input begin\n");
  restore_game_progress(3);
  FUN_004cf210();
  restore_startup_probe_input();
  FUN_004d71c0(0);
  if(trace_after_save)restore_game_trace("post-save rendering begin\n");
  restore_game_progress(4);
  FUN_004d6070();
  FUN_004d75d0();
  if(trace_after_save)restore_game_trace("post-save rendering complete\n");
  restore_startup_probe_frame_complete();
  restore_game_progress(5);
  FUN_004d8310();
  if(trace_after_save)restore_game_trace("post-save input dispatch complete\n");
  restore_game_progress(6);
  FUN_004cf1f0();
  FUN_004c8750();
  FUN_004c9750();
  FUN_0051f0a0(0);
  FUN_0051f680(1);
  FUN_004e2720();
  if (DAT_00d3a6e4 != 0) {
    FUN_004cabd0();
    if (DAT_00d3a6c0 == 0x23) {
      FUN_00405f80();
    }
    FUN_004ca930();
  }
  FUN_00536620();
  if (DAT_006fdc40 == 0) {
LAB_004d6ce1: ;
    restore_game_progress(7);
    FUN_004d0280(0);
  }
  else {
    cVar1 = (char)FUN_005605a0(DAT_006fdc38);
    if (cVar1 != '\0') {
      FUN_00416bc0();
    }
    if (DAT_006fdc40 == 0) goto LAB_004d6ce1;
  }
  FUN_0041d560();
  iVar2 = DAT_00d05700;
  if (DAT_00e38dda == '\x1e') {
    iVar2 = FUN_004b5640(DAT_00bca2c4);
    if (iVar2 == 0) {
      switch(*(uint16_t *)(uintptr_t)(0x00bcb9f2u + DAT_00bcb9c0 * 144u)) {
      case 1: ;
        uVar5 = 0xb;
        break;
      case 2: ;
        uVar5 = 0xc;
        break;
      case 3: ;
        uVar5 = 0xd;
        break;
      case 4: ;
        uVar5 = 0xe;
        break;
default: ;
        uVar5 = 1;
        break;
      case 0xf: ;
        uVar5 = 0xf;
      }
    }
    else if (*(int16_t *)(uintptr_t)(0x00bcb9ccu + DAT_00bcb9c0 * 144u) == 0xc) {
      uVar5 = 8;
    }
    else {
LAB_004d6e7d: ;
      uVar5 = 9;
    }
    goto LAB_004d6ec7;
  }
  if (DAT_00e38dda == 'I') {
    uVar5 = 10;
    goto LAB_004d6ec7;
  }
  if (DAT_00e38dda == '\x01') {
    if (DAT_006059e8 == 9) {
      uVar5 = 2;
    }
    else {
LAB_004d6ec5: ;
      uVar5 = 0;
    }
    goto LAB_004d6ec7;
  }
  if (DAT_00e38dda != '\x1f') {
    if (DAT_00e38dda != ' ') goto LAB_004d6ec5;
    iVar4 = FUN_004b5640(DAT_00bca2c4);
    iVar2 = DAT_00d05704;
    if (iVar4 != 0) goto LAB_004d6e7d;
    if (DAT_00d05704 < 1) {
      FUN_004cf460(0);
    }
    cVar1 = (&DAT_00bcd630)[iVar2 * 0x184];
    if (cVar1 == '\n') {
      uVar5 = 0x11;
    }
    else {
      if (cVar1 != '\x14') {
        if (cVar1 == '\x17') goto switchD_004d6dd3_caseD_e;
        goto LAB_004d6ec5;
      }
      if (DAT_00d0570c == 0) goto LAB_004d6ecf;
      uVar5 = 7;
    }
    goto LAB_004d6ec7;
  }
  if (DAT_00d05700 < 1) {
    FUN_004cf460(0);
  }
  switch((&DAT_00bcd630)[iVar2 * 0x184]) {
  case 9: ;
    iVar4 = FUN_004d6aa0(iVar2);
    if (iVar4 == 0) goto switchD_004d6dd3_caseD_d;
    uVar5 = 5;
    break;
  case 10: ;
    uVar5 = 0x10;
    break;
  case 0xb: ;
    if (DAT_00d05708 != 0) goto LAB_004d6e0b;
    iVar4 = FUN_004d6aa0(iVar2);
    if (iVar4 == 0) {
      iVar2 = FUN_004d6b00(iVar2);
      if (iVar2 == 0) goto LAB_004d6ecf;
      uVar5 = 4;
    }
    else {
      uVar5 = 5;
    }
    break;
  case 0xc: ;
    iVar2 = FUN_004d6aa0(iVar2);
    if (iVar2 == 0) goto LAB_004d6e0b;
    uVar5 = 5;
    break;
  case 0xd: ;
switchD_004d6dd3_caseD_d: ;
    iVar2 = FUN_004d6b00(iVar2);
    if (iVar2 == 0) {
LAB_004d6e0b: ;
      uVar5 = 0x12;
    }
    else {
      uVar5 = 4;
    }
    break;
default: ;
switchD_004d6dd3_caseD_e: ;
    uVar5 = 3;
    break;
  case 0x14: ;
    if (DAT_00d05708 != 0) {
      uVar5 = 6;
      break;
    }
    goto LAB_004d6ecf;
  }
LAB_004d6ec7: ;
  FUN_004cf460(uVar5);
LAB_004d6ecf: ;
  restore_game_progress(8);
  if (((int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] < 0x10) &&
     (*(int32_t *)(uintptr_t)(0x00ea6210u + DAT_00eaac28 * 0x49d8u) < 5)) {
    if (DAT_00e92bf0 == 1) {
      DAT_00f55b8c = 1;
      DAT_00e92bf0 = 0;
    }
    iVar2 = FUN_0055f5a1(RESTORE_SOUND,0);
    if ((iVar2 == 0) && (DAT_00f55b94 == 0)) {
      if (DAT_00e38dda != '\x01') goto LAB_004d6f9f;
      FUN_00539820(RESTORE_MUSIC);
    }
  }
  else if ((DAT_00f55b88 != 0) && ((DAT_00e92bf0 == 0 && (DAT_00e38e64 != '\0')))) {
    FUN_0055f449(RESTORE_SOUND,s_audio_music_battle_mp3_005e1de0,0,0);
    DAT_00e92bf0 = 1;
  }
  if ((DAT_00e38dda == '\x01') &&
     (iVar2 = FUN_00562348(), (int)(iVar2 * 0x32 + (iVar2 * 0x32 >> 0x1f & 0x7fffU)) >> 0xf == 1)) {
    FUN_0053acf0();
  }
LAB_004d6f9f: ;
  if (((DAT_0078d79c == 0) && (DAT_0078d7a0 == 0)) && (DAT_0078d7a8 == 0)) {
    if (DAT_00e38dcc == 0) {
      if (((DAT_00e38dda == '\x01') || (DAT_00e38dda == '\t')) || (DAT_00e38dda == '\x05')) {
        FUN_0053b960();
      }
      else {
        iVar2 = FUN_0055f5a1(RESTORE_SOUND,2);
        if (iVar2 != 0) {
          FUN_0055f561(RESTORE_SOUND,2);
        }
      }
    }
    else {
      FUN_0053b6f0(2);
    }
  }
  if (DAT_00f55b8c != 0) {
    FUN_0053b6f0(0);
  }
  if (DAT_00f55b90 != 0) {
    FUN_0053b6f0(1);
  }
  restore_game_progress(0);
  return;
}
