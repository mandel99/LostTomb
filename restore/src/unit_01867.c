#include "refresh_menu_dispatch.h"
#include "mission_menu.h"
#include "sound_streams.h"
#include "video_player.h"
#define DAT_00579788 (*(undefined4 *)0x00579788u)
#define DAT_005797b0 (*(undefined4 *)0x005797b0u)
#define DAT_0057be30 (*(undefined4 *)0x0057be30u)
#define DAT_005c4358 (*(undefined4 *)0x005c4358u)
#define DAT_005c5af8 (*(undefined4 *)0x005c5af8u)
#define DAT_005c7b10 (*(undefined4 *)0x005c7b10u)
#define DAT_005c7b80 (*(undefined4 *)0x005c7b80u)
#define DAT_005c7bc8 (*(undefined4 *)0x005c7bc8u)
#define DAT_005c7c58 (*(undefined4 *)0x005c7c58u)
#define DAT_005c7ce8 (*(undefined4 *)0x005c7ce8u)
#define DAT_005c7d10 (*(undefined4 *)0x005c7d10u)
#define DAT_005c7d38 (*(undefined4 *)0x005c7d38u)
#define DAT_005c81b0 (*(undefined4 *)0x005c81b0u)
#define DAT_005c8c60 (*(undefined4 *)0x005c8c60u)
#define DAT_005c8d08 (*(undefined4 *)0x005c8d08u)
#define DAT_005c8d38 (*(undefined4 *)0x005c8d38u)
#define DAT_005c8eb8 (*(undefined4 *)0x005c8eb8u)
#define DAT_006035e0 (*(undefined4 *)0x006035e0u)
#define DAT_0060594c (*(volatile int32_t *)0x0060594cu)
#define DAT_00605950 (*(volatile int32_t *)0x00605950u)
#define DAT_00605954 (*(volatile int32_t *)0x00605954u)
#define DAT_00605970 (*(volatile int32_t *)0x00605970u)
#define DAT_00605974 (*(volatile int32_t *)0x00605974u)
#define DAT_0060598c (*(volatile int32_t *)0x0060598cu)
#define DAT_00605a38 (*(volatile int32_t *)0x00605a38u)
#define DAT_00605a3c (*(volatile int32_t *)0x00605a3cu)
#define DAT_00605a44 (*(volatile int32_t *)0x00605a44u)
#define DAT_006fdc54 RESTORE_CURRENT_VIDEO
#define DAT_006fdc9c (*(volatile int32_t *)0x006fdc9cu)
#define DAT_006fdca0 (*(volatile int32_t *)0x006fdca0u)
#define DAT_007027a4 (*(volatile uint8_t *)0x007027a4u)
#define DAT_007056c8 (*(volatile int32_t *)0x007056c8u)
#define DAT_0077591c (*(volatile int32_t *)0x0077591cu)
#define DAT_00775920 (*(volatile int32_t *)0x00775920u)
#define DAT_00775928 (*(volatile int32_t *)0x00775928u)
#define DAT_0077f748 (*(volatile uint8_t *)0x0077f748u)
#define DAT_007999e8 (*(volatile int16_t *)0x007999e8u)
#define DAT_00d3a6c4 (*(volatile int32_t *)0x00d3a6c4u)
#define DAT_00d3b180 (*(volatile int32_t *)0x00d3b180u)
#define DAT_00e38da4 (*(volatile int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(volatile int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(volatile int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(volatile int32_t *)0x00e38db0u)
#define DAT_00e38dc9 (*(volatile uint8_t *)0x00e38dc9u)
#define DAT_00e38dd9 (*(volatile uint8_t *)0x00e38dd9u)
#define DAT_00e38df8 (*(volatile uint8_t *)0x00e38df8u)
#define DAT_00e38e64 (*(volatile uint8_t *)0x00e38e64u)
#define DAT_00e38e68 (*(volatile int32_t *)0x00e38e68u)
#define DAT_00e3908c (*(volatile uint8_t *)0x00e3908cu)
#define DAT_00ea4096 (*(volatile uint8_t *)0x00ea4096u)
#define DAT_00eaac28 (*(volatile int32_t *)0x00eaac28u)
#define DAT_00eadcc0 (*(volatile int32_t *)0x00eadcc0u)
#define DAT_00eadd50 (*(volatile int32_t *)0x00eadd50u)
#define DAT_00eadd6c (*(volatile int32_t *)0x00eadd6cu)
#define DAT_00eadd84 (*(volatile int32_t *)0x00eadd84u)
#define DAT_00eadd8c (*(volatile int32_t *)0x00eadd8cu)
#define DAT_00eadd90 (*(volatile int32_t *)0x00eadd90u)
#define DAT_00f53dd8 (*(volatile int32_t *)0x00f53dd8u)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
#define DAT_00f53f00 (*(volatile int32_t *)0x00f53f00u)
#define DAT_00f53f08 (*(volatile int32_t *)0x00f53f08u)
#define DAT_00f53f0c (*(volatile int32_t *)0x00f53f0cu)
#define DAT_00f53f1c (*(volatile int32_t *)0x00f53f1cu)
#define DAT_00f53f20 (*(volatile int32_t *)0x00f53f20u)
#define DAT_00f53f64 (*(volatile int32_t *)0x00f53f64u)
#define DAT_00f55b88 (*(volatile int32_t *)0x00f55b88u)
#define PTR_DAT_005c7d60 (*(unsigned char * *)0x005c7d60u)
#define PTR_DAT_005c8d68 (*(unsigned char * *)0x005c8d68u)
#define PTR_DAT_005c8d98 (*(unsigned char * *)0x005c8d98u)
#define PTR_DAT_005c8df8 (*(unsigned char * *)0x005c8df8u)
#define PTR_DAT_005f7ee0 (*(unsigned char * *)0x005f7ee0u)
#define PTR_DAT_00603580 (*(unsigned char * *)0x00603580u)
#define PTR_DAT_006035b0 (*(unsigned char * *)0x006035b0u)
#define PTR_s_an_g03_wav_0060023c_4_005e01d0 (*(unsigned char * *)0x005e01d0u)
#define s_audio_music_setup_mp3_005761ac (*(char (*)[22])0x005761acu)
extern void FUN_004138d0();
extern void FUN_00414c40();
extern void FUN_004180c0();
extern void FUN_00418f70();
extern void FUN_00419140();
extern void FUN_00419db0();
extern uint8_t FUN_0041b8f0(uint32_t self);
extern uint FUN_0041bb30();
extern void FUN_0041be10();
extern void FUN_0041d210();
extern undefined ** FUN_0041e100();
extern undefined * FUN_0041e130();
extern undefined4 FUN_0041e140();
extern char FUN_0041e170();
extern void FUN_0041f220();
extern void FUN_00425430();
extern void FUN_00425640();
extern void FUN_004256a0();
extern void FUN_00425730();
extern int FUN_004261d0();
extern int restore_refresh_hit();
extern void FUN_0043e1a0();
extern void FUN_00448640();
extern void FUN_0044aaf0();
extern void FUN_004c5660();
extern void FUN_004c7340();
extern void FUN_004caf10();
extern void FUN_004cb5a0();
extern int FUN_004cde80();
extern int restore_refresh_hover();
extern void FUN_004dddc0();
extern void FUN_004e1320();
extern void FUN_004e1a50();
extern void FUN_004e61f0();
extern void FUN_004f7770();
extern void FUN_004f94a0();
extern undefined4 FUN_005013a0();
extern int FUN_005027b0();
extern void FUN_00515bf0();
extern void FUN_00516210();
extern undefined4 FUN_005165b0();
extern void FUN_00516720();
extern void FUN_00516bb0();
extern void FUN_0051ec90();
extern void FUN_0051ed30();
extern void FUN_0051efb0();
extern void FUN_0051f780();
extern void FUN_0051f790();
extern void FUN_0051fd60();
extern void FUN_00520000();
extern void FUN_005208f0();
extern void FUN_00520b90();
extern void FUN_00520d00();
extern void FUN_00521060();
extern void FUN_005215d0();
extern void FUN_005219c0();
extern void FUN_00522a60();
extern void FUN_00523bb0();
extern void FUN_00524840();
extern void FUN_00524e20();
extern void FUN_005253c0();
extern void FUN_00525720();
extern void FUN_005271f0();
extern void FUN_005292b0();
extern void FUN_00529530();
extern void FUN_0052b6d0();
extern void FUN_0052d4d0();
extern void FUN_0052d940();
extern void FUN_0052e6f0();
extern void FUN_0052ec50();
extern void FUN_0052f8b0();
extern void FUN_005305e0();
extern void FUN_00530780();
extern void FUN_005309b0();
extern void FUN_00530d70();
extern void FUN_00531190();
extern void FUN_00531480();
extern void FUN_00531be0();
extern void FUN_00531cc0();
extern void FUN_00531ef0();
extern void FUN_00532340();
extern void FUN_005325f0();
extern void FUN_00532920();
extern void FUN_00532fe0();
extern void FUN_00533470();
extern void FUN_00533e70();
extern void FUN_00534320();
extern void FUN_005345a0();
extern void FUN_00534610();
extern void FUN_00534df0();
extern void FUN_00535080();
extern void FUN_00536120();
extern void FUN_00537260();
extern void FUN_005374a0();
extern void FUN_00537a50();
extern void FUN_00537cf0();
extern undefined4 FUN_00538fa0();
extern void FUN_0055a0e0();
extern void restore_initial_dispatch_training();

void FUN_004d75d0(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 uVar8;
  undefined **ppuVar9;
  uint8_t mode = *(volatile uint8_t *)0x00e38ddau;
  
  if (mode == '\0') {
    if (((DAT_00f55b88 != 0) && (iVar3 = FUN_0055f5a1(RESTORE_SOUND,0), iVar3 == 0)) && (DAT_00e38e64 != '\0')) {
      FUN_0055f449(RESTORE_SOUND,s_audio_music_setup_mp3_005761ac,0,0);
    }
    FUN_0051f780();
    FUN_004180c0();
    goto LAB_004d82ef;
  }
  if (mode == '\x17') {
    FUN_0041be10();
    iVar3 = FUN_0041bb30();
    if (iVar3 == 0) goto LAB_004d82ef;
    ppuVar9 = (undefined **)&DAT_0057be30;
  }
  else {
    if (mode == '\x04') goto LAB_004d82ef;
    if (mode == '?') {
      FUN_00425640();
      goto LAB_004d82ef;
    }
    if (mode == '\x01') {
      if (DAT_00605a3c == 0) {
        FUN_004dddc0();
        FUN_0051ec90();
        FUN_0051ed30(0,RD_ADD(DAT_00f53f64,0xb));
        FUN_0051efb0(0);
        FUN_0051f790();
        FUN_0043e1a0();
        FUN_00534610();
        FUN_00534df0();
        FUN_00535080();
        FUN_00536120();
        FUN_004e1320();
        FUN_004e61f0();
      }
      else {
        if (DAT_00605a38 == 0) {
          FUN_004dddc0();
        }
        else {
          FUN_004138d0();
          FUN_004256a0(RD_ADD(DAT_00e38da8,0xc),RD_ADD(DAT_00e38da4,0x28),&DAT_005c8eb8,2);
        }
        FUN_0051efb0(0);
        FUN_00533e70();
        FUN_00535080();
      }
      goto LAB_004d82ef;
    }
    if (mode == '\x03') {
      FUN_004dddc0();
      FUN_00425430();
      goto LAB_004d82ef;
    }
    if (mode == '\x11') {
      FUN_004e1a50();
      goto LAB_004d82ef;
    }
    if (mode == '\x06') {
      FUN_004dddc0();
      FUN_00520000();
      goto LAB_004d82ef;
    }
    if (mode == '\b') {
      FUN_004dddc0();
      FUN_005208f0();
      goto LAB_004d82ef;
    }
    if (mode == '\x05') {
      FUN_004c7340();
      goto LAB_004d82ef;
    }
    if (mode == '\x13') {
      FUN_00538fa0(0);
      if (DAT_00d3b180 != 0) {
        FUN_004c7340();
      }
      goto LAB_004d82ef;
    }
    if (mode == 'J') {
      if (DAT_006fdc54 != 0) {
        FUN_00413470(RESTORE_CURRENT_VIDEO);
      }
      goto LAB_004d82ef;
    }
    if (mode == '\x12') goto LAB_004d82ef;
    if (mode == '#') {
      if (DAT_00d3a6c4 == 1) {
        FUN_004dddc0();
      }
      FUN_004caf10();
      goto LAB_004d82ef;
    }
    if (mode == '\t') {
      DAT_00e38dc9 = 1;
      FUN_004f94a0();
      if (DAT_00eadcc0 == 0) {
        if (DAT_00eadd50 == 6) {
          FUN_00425730(DAT_00eadd90,RD_ADD(RD_ADD(RD_MUL(DAT_00eadd84,16),-40),DAT_00eadd8c),&DAT_005c7b10,2);
        }
        else if (DAT_00eadd50 == 5) {
          FUN_00425730(DAT_00eadd90,RD_ADD(RD_ADD(RD_MUL(DAT_00eadd84,16),-40),DAT_00eadd8c),&DAT_005c7b10,2);
        }
        else {
          FUN_00425730(DAT_00eadd90,RD_ADD(RD_ADD(RD_MUL(DAT_00eadd84,16),-40),DAT_00eadd8c),&DAT_005c7b10,2);
          if (DAT_00eadd50 == 2) {
            FUN_00515bf0();
          }
        }
      }
      else {
        sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
        if ((sVar1 == 0x48) || (sVar1 == 0xc1)) {
          iVar3 = FUN_005027b0();
        }
        else if ((sVar1 == 0x46) || (iVar3 = DAT_00f53dd8, sVar1 == 0xbe)) {
          iVar3 = 0;
          puVar7 = &DAT_00f53e70;
          do {
            iVar4 = FUN_005013a0(*puVar7);
            if (iVar4 != 0) {
              iVar3 = iVar3 + 1;
            }
            puVar7 = puVar7 + 1;
          } while ((int)puVar7 < 0xf53f00);
        }
        if (iVar3 == 0) {
          FUN_00425730(DAT_00eadd90,0x1b0,&DAT_005c7b10,2);
        }
        else {
          iVar3 = FUN_004cde80(RD_MUL(iVar3,4),3);
          FUN_00425730(DAT_00eadd90,RD_ADD(RD_MUL(RD_ADD(iVar3,7),16),-8),&DAT_005c7b10,2);
        }
      }
      goto LAB_004d82ef;
    }
    if (mode == '\x02') {
      FUN_0051fd60();
      goto LAB_004d82ef;
    }
    if (mode == '\n') {
      FUN_00531190();
      goto LAB_004d82ef;
    }
    if (mode == 'V') {
      FUN_00531480();
      goto LAB_004d82ef;
    }
    if (mode == 'E') {
      FUN_00531be0();
      goto LAB_004d82ef;
    }
    if (mode == '\v') {
      FUN_00531ef0();
      if (0x14 < DAT_00f53f1c) {
        FUN_004256a0(RD_ADD(DAT_00e38da8,0xd0),RD_ADD(DAT_00e38da4,0x3c),&PTR_DAT_005c8df8,8);
      }
      goto LAB_004d82ef;
    }
    if (mode == '\f') {
      FUN_00531cc0();
      if (0x14 < DAT_00f53f20) {
        FUN_004256a0(RD_ADD(DAT_00e38da8,0x80),RD_ADD(DAT_00e38da4,0x28),&PTR_DAT_005c8d98,4);
      }
      goto LAB_004d82ef;
    }
    if (mode == '\r') {
      FUN_00532340();
      goto LAB_004d82ef;
    }
    if (mode == '\x10') {
      FUN_005325f0();
      goto LAB_004d82ef;
    }
    if (mode == '\x16') {
      FUN_005305e0();
      goto LAB_004d82ef;
    }
    if (mode == '\x19') {
      FUN_00530780();
      goto LAB_004d82ef;
    }
    if (mode == '\x18') {
      FUN_0044aaf0();
      goto LAB_004d82ef;
    }
    if (mode == '\x1b') {
      FUN_00532fe0();
      goto LAB_004d82ef;
    }
    if (mode == '\'') {
      FUN_00448640();
      goto LAB_004d82ef;
    }
    if (mode == 'F') {
      FUN_00537260();
      goto LAB_004d82ef;
    }
    if (mode == 'K') {
      FUN_005374a0();
      goto LAB_004d82ef;
    }
    if (mode == '\x1c') {
      FUN_00533470();
      goto LAB_004d82ef;
    }
    if (mode == '%') {
      FUN_00532920();
      goto LAB_004d82ef;
    }
    if (mode == '$') {
      FUN_00534320();
      goto LAB_004d82ef;
    }
    if (mode == 'N') {
      FUN_00537cf0();
      goto LAB_004d82ef;
    }
    if (mode == '\x14') {
      DAT_00e38dc9 = 1;
      FUN_0052f8b0();
      FUN_0052d4d0();
      FUN_0052d940();
      FUN_00425730(0x14,RD_ADD(DAT_00e38dac,-0x2c),&DAT_005c7ce8,1);
      FUN_00425730(RD_ADD(DAT_00e38db0,-0x2c),RD_ADD(DAT_00e38dac,-0x2c),&DAT_005c7d10,1);
      FUN_00425730(0x18,RD_ADD(DAT_00e38dac,-0x6e),&DAT_005c7d38,1);
      goto LAB_004d82ef;
    }
    if (mode == '\x15') {
      FUN_0052f8b0();
      FUN_0052d940();
      FUN_0052e6f0();
      FUN_0052ec50();
      FUN_0051f780();
      if (DAT_00775928 == 1) {
        if (DAT_00775920 == 0) {
          FUN_004256a0(0xdf,RD_ADD(DAT_00e38dac,-0x72),&DAT_005c8c60,5);
        }
      }
      else if ((DAT_00775928 == 2) && ((&DAT_0077f748)[DAT_0077591c * 0x62] == '\0')) {
        FUN_004256a0(0xdf,RD_ADD(DAT_00e38dac,-0x72),&DAT_005c8c60,5);
      }
      goto LAB_004d82ef;
    }
    if (mode != '&') {
      if (mode == '\x0e') {
        FUN_005309b0(DAT_00605974,DAT_00605970);
      }
      else if ((mode == '\x0f') || (mode == 'H')) {
        {
        int32_t popup_rows = *(int32_t *)0x00605950u;
        int32_t popup_y = *(int32_t *)0x0060594cu;
        int32_t popup_x = *(int32_t *)0x00605954u;
        FUN_00530d70(popup_x,popup_y,popup_rows);
      }
      }
      else {
        if (mode == '\a') {
          DAT_00e38dc9 = 1;
          FUN_00521060();
          FUN_00425730(DAT_00f53f0c,RD_ADD(RD_MUL(RD_ADD(DAT_00f53f00,-2),16),DAT_00f53f08),&DAT_005c7b80,1);
          if (DAT_00605a44 == 1) {
            FUN_004256a0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c8d08,2);
            FUN_005215d0();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 2) {
            FUN_00522a60();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 3) {
            FUN_00523bb0();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 4) {
            FUN_00524840();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 5) {
            FUN_004f7770();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 6) {
            FUN_0055a0e0();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 == 7) {
            FUN_005271f0();
            goto LAB_004d82ef;
          }
          if (((DAT_00605a44 == 8) || (DAT_00605a44 == 9)) || (DAT_00605a44 == 10))
          goto LAB_004d82ef;
          if (DAT_00605a44 == 0xb) {
            FUN_005292b0();
            goto LAB_004d82ef;
          }
          if (DAT_00605a44 != 0xc) {
            if ((DAT_00605a44 != 0xd) && (DAT_00605a44 == 0xe)) {
              FUN_0052b6d0();
            }
            goto LAB_004d82ef;
          }
          ppuVar9 = (undefined **)&DAT_005c8d38;
        }
        else {
          if (mode == '(') {
            DAT_00e38dc9 = 1;
            restore_initial_dispatch_training(0);
            FUN_00425730(DAT_00f53f0c,DAT_00f53f08,&DAT_006035e0,2);
            if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 2) {
              FUN_004256a0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603580,2);
            }
            if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 1) {
              FUN_004256a0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006035b0,2);
            }
            goto LAB_004d82ef;
          }
          if (mode == '*') {
            FUN_005219c0();
            goto LAB_004d82ef;
          }
          if (mode == '+') {
            DAT_00e38dc9 = 1;
            FUN_00529530();
            FUN_00425730(DAT_00f53f0c,DAT_00f53f08,&DAT_005c7bc8,4);
            goto LAB_004d82ef;
          }
          if (mode == 'R') {
            DAT_00e38dc9 = 1;
            FUN_00516210();
            FUN_00425730(0x1e,0x1e,&DAT_005c7c58,4);
            goto LAB_004d82ef;
          }
          if ((mode == ',') || (mode == 'M')) {
            DAT_00e38dc9 = 1;
            FUN_00524e20();
            goto LAB_004d82ef;
          }
          if (mode != '-') {
            if (mode == '.') {
              DAT_00e38dc9 = 1;
              FUN_00525720();
            }
            else if (mode == 'L') {
              DAT_00e38dc9 = 1;
              FUN_00537a50();
            }
            else if (mode == 'C') {
              FUN_00419db0();
            }
            else if (mode == '3') {
              if (DAT_00e38dd9 != '\x01') {
                FUN_00419db0();
              }
            }
            else if (mode == '5') {
              DAT_00e38dc9 = 1;
              FUN_00418f70();
              FUN_00425730(RD_ADD(DAT_006fdca0,0x234),RD_ADD(DAT_006fdc9c,0x19a),&DAT_00579788,1);
              FUN_00425730(RD_ADD(DAT_006fdca0,0x6e),RD_ADD(DAT_006fdc9c,0x1a0),&DAT_005c81b0,2);
              cVar2 = FUN_0041b8f0(0x006fdd08u);
              if (cVar2 != '\0') {
                FUN_00425730(RD_ADD(DAT_006fdca0,10),RD_ADD(DAT_006fdc9c,0x19c),&DAT_005797b0,1);
              }
            }
            else if (mode == '4') {
              DAT_00e38dc9 = 1;
              if (DAT_007027a4 != '\0') {
                FUN_00425730(RD_ADD(DAT_006fdca0,0x244),RD_ADD(DAT_006fdc9c,0x19a),&DAT_00579788,1);
              }
              FUN_00419140();
            }
            else if (mode == ';') {
              DAT_00e38dc9 = 1;
              FUN_00418f70();
              FUN_00425730(RD_ADD(DAT_006fdca0,0x234),RD_ADD(DAT_006fdc9c,0x19a),&DAT_00579788,1);
              FUN_00425730(RD_ADD(DAT_006fdca0,0x6e),RD_ADD(DAT_006fdc9c,0x1a0),&DAT_005c81b0,2);
            }
            else if (mode == '=') {
                restore_mission_menu_buttons();
              }
            else if (mode == '/') {
              DAT_00e38dc9 = 1;
              uVar5 = FUN_005165b0();
              FUN_00425730(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7ee0,uVar5);
              FUN_00516720();
            }
            else if (mode == '0') {
              FUN_004cb5a0();
            }
            else if (mode == '>') {
              FUN_005345a0();
            }
            else if (mode == '6') {
              DAT_00e38dc9 = 1;
              FUN_00520b90();
              FUN_00425730(RD_ADD(DAT_00e38da8,0x1d0),RD_ADD(DAT_00e38da4,0xf9),&DAT_00579788,1);
              uVar5 = restore_refresh_hit(RD_ADD(DAT_00e38da8,0x160),RD_ADD(DAT_00e38da4,0xf6),&DAT_005c5af8,1,
                                   DAT_00e38da8,DAT_00e38da4);
              DAT_007056c8 = restore_refresh_hover(RD_ADD(DAT_00e38da8,0x160),RD_ADD(DAT_00e38da4,0xf6),&DAT_005c5af8,1,
                                          uVar5);
              iVar3 = FUN_004261d0(RD_ADD(DAT_00e38da8,0x160),RD_ADD(DAT_00e38da4,0xf6),&DAT_005c5af8,1);
              if (iVar3 != 0) {
                return;
              }
              if (DAT_00e38df8 != '\0') {
                DAT_00e38df8 = 0;
                FUN_00516bb0();
                return;
              }
            }
            else if (mode == 'Q') {
              if (DAT_00e38e68 == 1) {
                uVar5 = 0x6e;
                uVar8 = 0x1e;
              }
              else if (DAT_00e38e68 == 2) {
                uVar5 = 0xbe;
                uVar8 = 0x5a;
              }
              else {
                uVar5 = 0x12e;
                uVar8 = 0xae;
              }
              uVar6 = restore_refresh_hit(uVar5,uVar8,&DAT_005c4358,0x2b,DAT_00e38da8,DAT_00e38da4);
              DAT_007056c8 = restore_refresh_hover(uVar5,uVar8,&DAT_005c4358,0x2b,uVar6);
              iVar3 = FUN_004261d0(uVar5,uVar8,&DAT_005c4358,0x2b);
              if (iVar3 != 0) {
                return;
              }
              FUN_00520d00();
            }
            else if (mode == '\x1e') {
              FUN_004dddc0();
              FUN_0051efb0(0);
              FUN_0051f790();
            }
            else if (mode == 'I') {
              FUN_004dddc0();
              FUN_0051efb0(0);
              FUN_0051f790();
            }
            else if (mode == '\x1f') {
              FUN_004dddc0();
              FUN_0051efb0(0);
              FUN_0051f790();
            }
            else if (mode == ' ') {
              FUN_004dddc0();
              FUN_0051efb0(0);
              FUN_0051f790();
            }
            else if (mode == 'S') {
              FUN_004c5660();
              FUN_00425730(DAT_00e38da8,DAT_00e38da4,&PTR_s_an_g03_wav_0060023c_4_005e01d0,2);
            }
            else if (mode == 'U') {
              FUN_0041d210();
            }
            goto LAB_004d82ef;
          }
          DAT_00e38dc9 = 1;
          FUN_005253c0();
          ppuVar9 = &PTR_DAT_005c8d68;
        }
        FUN_004256a0(DAT_00f53f0c,DAT_00f53f08,ppuVar9,2);
      }
      goto LAB_004d82ef;
    }
    iVar3 = 2;
    ppuVar9 = &PTR_DAT_005c7d60;
  }
  DAT_00e38dc9 = 1;
  FUN_00425730(DAT_00e38da8,DAT_00e38da4,ppuVar9,iVar3);
LAB_004d82ef: ;
  DAT_00e38dc9 = 0;
  if (DAT_00e3908c != '\0') {
    FUN_00414c40();
  }
  FUN_0051f780();
  return;
}
