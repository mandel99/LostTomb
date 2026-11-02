#include "menu_timers.h"
#include "restore_types.h"
extern void restore_selection_popup_input(void); /* Keep other1868legacy prototypes unchanged. */
extern void restore_display_confirmation_input(void);
extern void restore_menu_modal_input(void);
#include "mission_menu.h"
#include "load_save_menu.h"
#include "high_scores_menu.h"
#include "video_player.h"
#define DAT_005763d8 (*(undefined4 *)0x005763d8u)
#define DAT_00579788 (*(undefined4 *)0x00579788u)
#define DAT_005797b0 (*(undefined4 *)0x005797b0u)
#define DAT_0057bd88 (*(undefined4 *)0x0057bd88u)
#define DAT_0057be30 (*(undefined4 *)0x0057be30u)
#define DAT_0057be78 (*(undefined4 *)0x0057be78u)
#define DAT_005c2bf0 (*(undefined4 *)0x005c2bf0u)
#define DAT_005c2d08 (*(undefined4 *)0x005c2d08u)
#define DAT_005c3268 (*(undefined4 *)0x005c3268u)
#define DAT_005c3368 (*(undefined4 *)0x005c3368u)
#define DAT_005c33c0 (*(undefined4 *)0x005c33c0u)
#define DAT_005c3418 (*(undefined4 *)0x005c3418u)
#define DAT_005c3dc0 (*(undefined4 *)0x005c3dc0u)
#define DAT_005c3e68 (*(undefined4 *)0x005c3e68u)
#define DAT_005c3e88 (*(undefined4 *)0x005c3e88u)
#define DAT_005c3ec0 (*(undefined4 *)0x005c3ec0u)
#define DAT_005c4048 (*(undefined4 *)0x005c4048u)
#define DAT_005c4208 (*(undefined4 *)0x005c4208u)
#define DAT_005c4ab8 (*(undefined4 *)0x005c4ab8u)
#define DAT_005c4cd0 (*(undefined4 *)0x005c4cd0u)
#define DAT_005c4f00 (*(undefined4 *)0x005c4f00u)
#define DAT_005c5130 (*(undefined4 *)0x005c5130u)
#define DAT_005c5168 (*(undefined4 *)0x005c5168u)
#define DAT_005c5368 (*(undefined4 *)0x005c5368u)
#define DAT_005c5468 (*(undefined4 *)0x005c5468u)
#define DAT_005c5768 (*(undefined4 *)0x005c5768u)
#define DAT_005c5798 (*(undefined4 *)0x005c5798u)
#define DAT_005c5878 (*(undefined4 *)0x005c5878u)
#define DAT_005c59e8 (*(undefined4 *)0x005c59e8u)
#define DAT_005c5e20 (*(undefined4 *)0x005c5e20u)
#define DAT_005c5eb0 (*(undefined4 *)0x005c5eb0u)
#define DAT_005c6cb8 (*(undefined4 *)0x005c6cb8u)
#define DAT_005c6cd8 (*(undefined4 *)0x005c6cd8u)
#define DAT_005c6f08 (*(undefined4 *)0x005c6f08u)
#define DAT_005c7300 (*(undefined4 *)0x005c7300u)
#define DAT_005c7348 (*(undefined4 *)0x005c7348u)
#define DAT_005c75b0 (*(undefined4 *)0x005c75b0u)
#define DAT_005c79f0 (*(undefined4 *)0x005c79f0u)
#define DAT_005c7a80 (*(undefined4 *)0x005c7a80u)
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
#define DAT_005c8ee8 (*(undefined4 *)0x005c8ee8u)
#define DAT_005c91f0 (*(undefined4 *)0x005c91f0u)
#define DAT_005c9230 (*(undefined4 *)0x005c9230u)
#define DAT_005d2a18 (*(undefined *)0x005d2a18u)
#define DAT_005d2a1c (*(undefined *)0x005d2a1cu)
#define DAT_005d2a20 (*(undefined *)0x005d2a20u)
#define DAT_005d2a30 (*(undefined *)0x005d2a30u)
#define DAT_005d2e88 (*(undefined *)0x005d2e88u)
#define DAT_005d9a48 (*(undefined4 *)0x005d9a48u)
#define DAT_005d9a90 (*(undefined4 *)0x005d9a90u)
#define DAT_005d9e20 (*(undefined4 *)0x005d9e20u)
#define DAT_005e1db0 (*(undefined *)0x005e1db0u)
#define DAT_005f8320 (*(undefined4 *)0x005f8320u)
#define DAT_005f8910 (*(undefined4 *)0x005f8910u)
#define DAT_006032e8 (*(undefined4 *)0x006032e8u)
#define DAT_006035e0 (*(undefined4 *)0x006035e0u)
#define DAT_006057e8 (*(undefined *)0x006057e8u)
#define DAT_00605948 (*(undefined *)0x00605948u)
#define DAT_0060594c (*(undefined *)0x0060594cu)
#define DAT_00605954 (*(undefined *)0x00605954u)
#define DAT_00605958 (*(undefined1 *)0x00605958u)
#define DAT_00605970 (*(undefined *)0x00605970u)
#define DAT_00605974 (*(undefined *)0x00605974u)
#define DAT_00605980 (*(undefined1 *)0x00605980u)
#define DAT_00605984 (*(undefined *)0x00605984u)
#define DAT_0060598c (*(undefined *)0x0060598cu)
#define DAT_00605994 (*(undefined *)0x00605994u)
#define DAT_00605a38 (*(undefined *)0x00605a38u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00605a44 (*(undefined *)0x00605a44u)
#define DAT_00605a58 (*(undefined *)0x00605a58u)
#define DAT_006fdc54 RESTORE_CURRENT_VIDEO
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_006fdc98 (*(int32_t *)0x006fdc98u)
#define DAT_006fdc9c (*(undefined *)0x006fdc9cu)
#define DAT_006fdca0 (*(undefined *)0x006fdca0u)
#define DAT_007027a4 (*(undefined *)0x007027a4u)
#define DAT_007027b0 (*(int32_t *)0x007027b0u)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704dc8 (*(undefined4 *)0x00704dc8u)
#define DAT_0070569c (*(undefined *)0x0070569cu)
#define DAT_007056a8 (*(int32_t *)0x007056a8u)
#define DAT_007056b0 (*(int32_t *)0x007056b0u)
#define DAT_007056b8 (*(int32_t *)0x007056b8u)
#define DAT_007056c0 (*(undefined *)0x007056c0u)
#define DAT_007056c4 (*(int32_t *)0x007056c4u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_007056d0 (*(undefined *)0x007056d0u)
#define DAT_007056d8 (*(undefined *)0x007056d8u)
#define DAT_007056dc (*(undefined *)0x007056dcu)
#define DAT_007056e8 (*(undefined *)0x007056e8u)
#define DAT_0077590c (*(undefined *)0x0077590cu)
#define DAT_00775910 (*(undefined *)0x00775910u)
#define DAT_00775918 (*(undefined *)0x00775918u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00775920 (*(undefined *)0x00775920u)
#define DAT_00775924 (*(undefined *)0x00775924u)
#define DAT_00775928 (*(undefined *)0x00775928u)
#define DAT_00776fb0 (*(undefined4 *)0x00776fb0u)
#define DAT_00776fb4 (*(undefined4 *)0x00776fb4u)
#define DAT_00776fb8 (*(undefined4 *)0x00776fb8u)
#define DAT_00776fba (*(undefined4 *)0x00776fbau)
#define DAT_00776fbe (*(undefined4 *)0x00776fbeu)
#define DAT_007770e4 (*(undefined *)0x007770e4u)
#define DAT_007770e8 (*(undefined4 *)0x007770e8u)
#define DAT_007770ee (*(undefined4 *)0x007770eeu)
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_007770f2 (*(undefined4 *)0x007770f2u)
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)
#define DAT_0077f754 (*(undefined4 *)0x0077f754u)
#define DAT_0077f760 (*(undefined4 *)0x0077f760u)
#define DAT_00784f14 (*(undefined *)0x00784f14u)
#define DAT_00784f18 (*(undefined *)0x00784f18u)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00d05708 (*(undefined *)0x00d05708u)
#define DAT_00d0570c (*(undefined *)0x00d0570cu)
#define DAT_00d3b180 (*(undefined *)0x00d3b180u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00d3b1bc (*(undefined1 *)0x00d3b1bcu)
#define DAT_00d3b43d (*(undefined1 *)0x00d3b43du)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
#define DAT_00e20998 (*(int32_t *)0x00e20998u)
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dd9 (*(undefined1 *)0x00e38dd9u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38ddc (*(undefined *)0x00e38ddcu)
#define DAT_00e38df7 (*(char *)0x00e38df7u)
#define DAT_00e38df8 (*(undefined1 *)0x00e38df8u)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e03 (*(undefined1 *)0x00e38e03u)
#define DAT_00e38e04 (*(undefined1 *)0x00e38e04u)
#define DAT_00e38e14 (*(int32_t *)0x00e38e14u)
#define DAT_00e38e28 (*(int32_t *)0x00e38e28u)
#define DAT_00e38e2c (*(int32_t *)0x00e38e2cu)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ea9 (*(undefined1 *)0x00e38ea9u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea5fd8 (*(undefined4 *)0x00ea5fd8u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00ea623c (*(undefined4 *)0x00ea623cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f535c8 (*(undefined *)0x00f535c8u)
#define DAT_00f53db4 (*(undefined *)0x00f53db4u)
#define DAT_00f53db8 (*(undefined *)0x00f53db8u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f60 (*(undefined *)0x00f53f60u)
#define DAT_00f53f64 (*(undefined *)0x00f53f64u)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define DAT_00f54118 (*(undefined *)0x00f54118u)
#define DAT_010d6a0c (*(undefined4 *)0x010d6a0cu)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
#define PTR_BYTE_005c3da0 (*(unsigned char * *)0x005c3da0u)
#define PTR_DAT_005c2e20 (*(unsigned char * *)0x005c2e20u)
#define PTR_DAT_005c2eb0 (*(unsigned char * *)0x005c2eb0u)
#define PTR_DAT_005c3038 (*(unsigned char * *)0x005c3038u)
#define PTR_DAT_005c3070 (*(unsigned char * *)0x005c3070u)
#define PTR_DAT_005c3118 (*(unsigned char * *)0x005c3118u)
#define PTR_DAT_005c3348 (*(unsigned char * *)0x005c3348u)
#define PTR_DAT_005c3488 (*(unsigned char * *)0x005c3488u)
#define PTR_DAT_005c4bb8 (*(unsigned char * *)0x005c4bb8u)
#define PTR_DAT_005c55b8 (*(unsigned char * *)0x005c55b8u)
#define PTR_DAT_005c5680 (*(unsigned char * *)0x005c5680u)
#define PTR_DAT_005c5710 (*(unsigned char * *)0x005c5710u)
#define PTR_DAT_005c63d8 (*(unsigned char * *)0x005c63d8u)
#define PTR_DAT_005c65d0 (*(unsigned char * *)0x005c65d0u)
#define PTR_DAT_005c68a8 (*(unsigned char * *)0x005c68a8u)
#define PTR_DAT_005c7370 (*(unsigned char * *)0x005c7370u)
#define PTR_DAT_005c7760 (*(unsigned char * *)0x005c7760u)
#define PTR_DAT_005c7980 (*(unsigned char * *)0x005c7980u)
#define PTR_DAT_005c7d60 (*(unsigned char * *)0x005c7d60u)
#define PTR_DAT_005c8d68 (*(unsigned char * *)0x005c8d68u)
#define PTR_DAT_005c8d98 (*(unsigned char * *)0x005c8d98u)
#define PTR_DAT_005c8df8 (*(unsigned char * *)0x005c8df8u)
#define PTR_DAT_005e0408 (*(unsigned char * *)0x005e0408u)
#define PTR_DAT_005f7650 (*(unsigned char * *)0x005f7650u)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
#define PTR_DAT_005f7ee0 (*(unsigned char * *)0x005f7ee0u)
#define PTR_DAT_005f7f28 (*(unsigned char * *)0x005f7f28u)
#define PTR_DAT_006034e0 (*(unsigned char * *)0x006034e0u)
#define PTR_DAT_00603520 (*(unsigned char * *)0x00603520u)
#define PTR_DAT_00603540 (*(unsigned char * *)0x00603540u)
#define PTR_DAT_00603560 (*(unsigned char * *)0x00603560u)
#define PTR_DAT_00603580 (*(unsigned char * *)0x00603580u)
#define PTR_DAT_006035b0 (*(unsigned char * *)0x006035b0u)
#define PTR_LAB_005c3648 (*(unsigned char * *)0x005c3648u)
#define PTR_LAB_005c5cd0 (*(unsigned char * *)0x005c5cd0u)
#define PTR_s_an_g03_wav_0060023c_4_005e01d0 (*(unsigned char * *)0x005e01d0u)
#define PTR_s_lice_g02_wav_00600044_2_005c51a8 (*(unsigned char * *)0x005c51a8u)
#define PTR_s_lice_g02_wav_00600044_2_005c5288 (*(unsigned char * *)0x005c5288u)
#define _DAT_005d2a2c (*(undefined4 *)0x005d2a2cu)
#define _DAT_007028f4 (*(undefined4 *)0x007028f4u)
#define _DAT_00e94e14 (*(undefined4 *)0x00e94e14u)
#define s_smk_credits_smk_005e1df8 (*(char (*)[16])0x005e1df8u)
#define s_smk_intro_smk_005e1e08 (*(char (*)[14])0x005e1e08u)
#define s_smk_logo_smk_005e1e18 (*(char (*)[13])0x005e1e18u)
extern void FUN_00402720();
extern undefined4 FUN_004029d0();
extern void FUN_00402bb0();
extern void FUN_00402de0();
extern int FUN_00403e00();
extern bool FUN_00405ba0();
extern void FUN_00405c20();
extern void FUN_00405d90();
extern void FUN_00405e90();
extern void FUN_00405f80();
extern void FUN_00406190();
extern undefined4 FUN_004061e0();
extern void FUN_00406250();
extern void FUN_004072c0();
extern void FUN_00407620();
extern int FUN_00407870();
extern void FUN_00407a10();
extern undefined4 FUN_00407f90();
extern void FUN_00409520();
extern void FUN_004099b0();
extern undefined4 FUN_00409e40();
extern undefined4 FUN_0040b630();
extern undefined4 FUN_0040b6d0();
extern undefined4 FUN_0040b790();
extern void FUN_0040b830();
extern void FUN_0040ba40();
extern void FUN_0040c440();
extern void FUN_0040c570();
extern void FUN_00417b40();
extern void FUN_00418220();
extern void FUN_00418640();
extern void FUN_00418fe0();
extern void FUN_00419090();


extern uint8_t FUN_0041b8f0(uint32_t self);
extern uint FUN_0041bb30();
extern void FUN_0041d310();
extern undefined ** FUN_0041e100();
extern undefined * FUN_0041e120();
extern undefined * FUN_0041e130();
extern undefined4 FUN_0041e140();
extern char FUN_0041e170();
extern undefined ** FUN_0041e190();
extern int FUN_0041f210();
extern undefined4 FUN_0041f4d0();
extern void FUN_0041f650();
extern void FUN_004256a0();
extern undefined4 FUN_00425840();
extern int FUN_00425990();
extern void FUN_00425bc0();
extern int FUN_00425c40();
extern void FUN_00425e80();
extern undefined4 FUN_00425f20();
extern undefined4 FUN_00426030();
extern void FUN_00426090();
extern int FUN_00426100();
extern int FUN_004261d0();
extern int FUN_00426450();
extern void FUN_00426530();
extern void FUN_00442500();
extern void FUN_004426c0();
extern void FUN_00442730();
extern void FUN_00442a40();
extern void FUN_00442ef0();
extern void FUN_00442f80();
extern int FUN_00444920();
extern undefined4 FUN_004483f0();
extern void FUN_00448cf0();
extern void FUN_004c5a40();
extern void FUN_004c7770();
extern void FUN_004cb6a0();
extern void FUN_004cb970();
extern void FUN_004cc500();
extern void FUN_004cf1c0();
extern int FUN_004cf660();
extern int FUN_004d6740();
extern int FUN_004dd8d0();
extern undefined4 FUN_004dd8e0();
extern void FUN_004dd900();
extern int FUN_004dd910();
extern int FUN_004dd920();
extern undefined4 FUN_004dd930();
extern uint FUN_004ddbd0();
extern void FUN_004ddde0();
extern void FUN_004e1e10();
extern void FUN_004e2ab0();
extern undefined4 FUN_004ea880();
extern undefined ** FUN_004ea8d0();
extern undefined4 FUN_004f7e50();
extern void FUN_004f96e0();
extern undefined4 FUN_005165b0();
extern undefined4 FUN_005168e0();
extern void FUN_0051f780();
extern undefined4 FUN_00538dc0();
extern void FUN_00539170();
extern undefined4 FUN_0055bb90();
extern longlong FUN_0056133c();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

void FUN_004d8310(void)

{
  short sVar1;
  char cVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  void *pvVar7;
  uint uVar8;
  short *psVar9;
  DWORD DVar10;
  undefined4 *puVar11;
  float fVar12;
  int iVar13;
  int iVar14;
  byte bVar15;
  float fVar16;
  undefined2 uVar17;
  uint uVar18;
  char *pcVar19;
  short *psVar20;
  byte *pbVar21;
  int iVar22;
  int iVar23;
  undefined *puVar24;
  undefined **ppuVar25;
  char local_79;
  float local_78;
  /* fsqrt writes a float at esp+18; this stack slot also held a pointer
     in other branches, which confused the decompiler's local_74 type. */
  float segment_length;
  short *local_74;
  uint local_70;
  int local_6c;
  int local_68;
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  int local_54;
  float local_50;
  int local_4c;
  int local_48;
  int local_3c;
  undefined1 auStack_34 [52];
  
  iVar5 = DAT_00e38da8;
  DAT_00e20990 = 0;
  DAT_007056b8 = 0;
  DAT_007056b0 = 0;
  DAT_007056a8 = 0;
  DAT_00e38e2c = DAT_00e38e34;
  DAT_00e38e28 = DAT_00e38e30;
  if (DAT_00e38e14 == 2) {
    DAT_00e38e14 = 0;
    DAT_00e38dca = 1;
  }
  if (DAT_00e38dda == '\0') {
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_0057bd88,5,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_0057bd88,5,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_0057bd88,5);
    return;
  }
  if (DAT_00e38dda == '\x17') {
    iVar5 = FUN_0041bb30();
    if ((0 < iVar5) &&
       (iVar5 = FUN_00425990(DAT_00e38da8,DAT_00e38da4,&DAT_0057be30,iVar5), iVar5 != 0)) {
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_0057be78,0xb,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_0057be78,0xb,uVar4);
    iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_0057be78,0xb);
    if (iVar5 != 0) {
      return;
    }
    if (DAT_00e38df8 == '\0') {
      return;
    }
    if ((DAT_006fdc98 != 1) && (DAT_007027b0 != 0)) {
      return;
    }
    FUN_004cb6a0();
    return;
  }
  if (DAT_00e38dda == '\x04') {
    FUN_0041aa60();
    return;
  }
  if (DAT_00e38dda == '?') {
    FUN_00426530();
    return;
  }
  if (DAT_00e38dda == 'D') {
    DAT_00f54118 = 2;
    FUN_004cf1c0();
    FUN_00538dc0(s_smk_logo_smk_005e1e18,(DAT_00e38db0 + -500) / 2,(DAT_00e38dac + -0x124) / 2,1,
                 0x40);
    return;
  }
  if (DAT_00e38dda == '@') {
    DAT_00f54118 = 2;
    FUN_004cf1c0();
    FUN_00538dc0(s_smk_intro_smk_005e1e08,(DAT_00e38db0 + -500) / 2,(DAT_00e38dac + -0x124) / 2,1,
                 0x41);
    return;
  }
  if (DAT_00e38dda == 'A') {
    DAT_00f54118 = 2;
    FUN_004cf1c0();
    FUN_00538dc0(s_smk_credits_smk_005e1df8,(DAT_00e38db0 + -0x280) / 2,(DAT_00e38dac + -0x1e0) / 2,
                 1,0x3c);
    return;
  }
  if (DAT_00e38dda == 'B') {
    _DAT_007028f4 = 0;
    FUN_004c5a40();
    FUN_0041aa20(2,0x33,1000);
    return;
  }
  if (DAT_00e38dda == 'C') {
    if ((('\x01' < DAT_00e38ea8) || (DAT_00e38ebc != 0)) || (uVar4 = 1, DAT_00e38ea9 != '\0')) {
      uVar4 = 3;
    }
    uVar6 = FUN_00426450(DAT_00f53db8,DAT_00f53db4,&PTR_DAT_005c65d0,uVar4,DAT_00e38da8,DAT_00e38da4
                        );
    DAT_007056c8 = FUN_004d6740(DAT_00f53db8,DAT_00f53db4,&PTR_DAT_005c65d0,uVar4,uVar6);
    FUN_004261d0(DAT_00f53db8,DAT_00f53db4,&PTR_DAT_005c65d0,uVar4);
    return;
  }
  if (DAT_00e38dda == '\x01') {
    _DAT_00e94e14 = 0;
    if (DAT_00605a3c == 1) {
      DAT_007056a8 = FUN_00426100(&DAT_005c9230,4);
      iVar5 = FUN_00426030(&DAT_005c9230,4);
    }
    else {
      DAT_007056a8 = FUN_00426100(&DAT_005c91f0,4);
      iVar5 = FUN_00426030(&DAT_005c91f0,4);
    }
    if (iVar5 != 0) {
      return;
    }
    if (DAT_00605a3c == 0) {
      FUN_00417b40();
    }
    uVar4 = FUN_004cf660();
    FUN_004ddde0(uVar4);
    if (DAT_00605a3c == 1) {
      if (DAT_00605a38 < 1) {
        FUN_00425bc0(DAT_00f53f64,0x1e,&PTR_DAT_005c7760,0xf,1);
      }
      else {
        uVar4 = FUN_00426450(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005763d8,8,DAT_00e38da8,
                             DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005763d8,8,uVar4);
      }
      FUN_00406190();
      iVar5 = FUN_004061e0(0);
      if (iVar5 != 0) {
        return;
      }
      cVar2 = FUN_00425840();
      if (cVar2 != '\0') {
        return;
      }
      if (DAT_00605a38 == 0) {
        uVar6 = 0xf;
      }
      else {
        if (DAT_00e38df7 != '\0') {
          DAT_00605a38 = 0;
          DAT_00e38dca = 1;
          return;
        }
        FUN_004cc500(5);
        uVar6 = 2;
        uVar4 = FUN_00426450(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005763d8,10,DAT_00e38da8,
                             DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005763d8,10,uVar4);
        iVar5 = FUN_004261d0(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005763d8,10);
        if (iVar5 != 0) {
          return;
        }
        iVar5 = FUN_00425c40(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0x28,&DAT_005c8eb8,2);
        if (iVar5 != 0) {
          return;
        }
      }
      iVar5 = FUN_00425990(DAT_00f53f64,0x18,&PTR_DAT_005c7760,uVar6);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
      if (DAT_00605a38 == 1) {
        return;
      }
    }
    else if (DAT_00f53f78 == 0) {
      FUN_00406190();
      iVar5 = FUN_004061e0(0);
      if (iVar5 != 0) {
        return;
      }
      cVar2 = FUN_00425840();
      if (cVar2 != '\0') {
        return;
      }
      FUN_00406250();
      if ((DAT_0070569c != 0) && (DAT_00e38df8 != '\0')) {
        FUN_004072c0();
        return;
      }
      FUN_00425bc0(DAT_00f53f64,0x1e,&DAT_005c7300,2,10);
      iVar5 = FUN_00425990(DAT_00f53f64,0x1e,&DAT_005c7300,2);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
      FUN_00425bc0(DAT_00f53f64,0x1e,&PTR_DAT_005c7370,0x10,0x1e);
      iVar5 = FUN_004261d0(DAT_00f53f64,0x1e,&DAT_005c6cb8,1);
      if (iVar5 != 0) {
        return;
      }
      iVar5 = FUN_00425990(DAT_00f53f64,0x1e,&PTR_DAT_005c7370,0x10);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
      FUN_00425bc0(DAT_00f53f64,0x1e,&PTR_DAT_005c7980,2,0x32);
      iVar5 = FUN_00425990(DAT_00f53f64,0x1e,&PTR_DAT_005c7980,2);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
    }
    else if (DAT_00f53f78 == 1) {
      cVar2 = FUN_00425840();
      if (cVar2 != '\0') {
        return;
      }
      FUN_00406250();
      if ((DAT_0070569c != 0) && (DAT_00e38df8 != '\0')) {
        FUN_004072c0();
        return;
      }
      FUN_00425bc0(DAT_00f53f60,0x18,&DAT_005c7348,1,10);
      iVar5 = FUN_00425990(DAT_00f53f60,0x18,&DAT_005c7348,1);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
      FUN_00425bc0(DAT_00f53f60,0x18,&DAT_005c75b0,0xc,0x1e);
      iVar5 = FUN_00425990(DAT_00f53f60,0x18,&DAT_005c75b0,0xc);
      if (iVar5 != 0) {
        DAT_00e38dc9 = 1;
        return;
      }
    }
    FUN_00405f80();
    DAT_00605994 = 0;
    if (DAT_00bca2c4 == 0) {
      DAT_00605994 = 0;
      return;
    }
    if (DAT_00e38e03 != '\0') {
      iVar5 = FUN_0040b630();
      if (iVar5 != 0) {
        return;
      }
      iVar5 = FUN_0040b790();
      if (iVar5 != 0) {
        return;
      }
      iVar5 = FUN_0040b6d0();
      if (iVar5 != 0) {
        return;
      }
      FUN_00405c20();
      return;
    }
    if (DAT_00e38e00 != '\0') {
      FUN_00405d90();
      return;
    }
    if (DAT_00e38e04 != '\0') {
      FUN_00405e90();
      return;
    }
    if (DAT_00e38df8 == '\0') {
      DAT_00605994 = 0;
      return;
    }
    iVar5 = FUN_00405ba0();
    if (iVar5 != 0) {
      return;
    }
    FUN_004f7e50();
    return;
  }
  if (DAT_00e38dda == '\x12') {
    if (DAT_00605a3c == 1) {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      return;
    }
    if (DAT_00f53f78 != 0) {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      return;
    }
    FUN_004261d0(DAT_00f53f64,0x18,&DAT_005c6cb8,1);
    return;
  }
  if (DAT_00e38dda == '\x06') {
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    iVar5 = FUN_00403e00();
    if (iVar5 != 0) {
      return;
    }
    if ((DAT_00605a3c == 1) &&
       (iVar5 = FUN_00425990(DAT_00f53f64,0x1e,&PTR_DAT_005c7760,
                             (-(uint)(DAT_00605a38 != 0) & 0xfffffff3) + 0xf), iVar5 != 0)) {
      DAT_00e38dc9 = 1;
      return;
    }
    if (DAT_00f53f78 == 0) {
      iVar5 = FUN_004261d0(DAT_00f53f64,0x1e,&DAT_005c6cb8,1);
      if (iVar5 != 0) {
        return;
      }
      uVar6 = 0x10;
      ppuVar25 = &PTR_DAT_005c7370;
      uVar4 = 0x1e;
      iVar5 = DAT_00f53f64;
    }
    else {
      if (DAT_00f53f78 != 1) {
        return;
      }
      uVar6 = 0xc;
      ppuVar25 = (undefined **)&DAT_005c75b0;
      uVar4 = 0x18;
      iVar5 = DAT_00f53f60;
    }
    iVar5 = FUN_00425990(iVar5,uVar4,ppuVar25,uVar6);
    if (iVar5 == 0) {
      return;
    }
    DAT_00e38dc9 = 1;
    return;
  }
  if (DAT_00e38dda == '\b') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00f53f64 + -0xaa,DAT_007056e8 + 0x20,&DAT_005c2bf0,10,DAT_00e38da8,
                         DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00f53f64 + -0xaa,DAT_007056e8 + 0x20,&DAT_005c2bf0,10,uVar4);
    iVar5 = FUN_004261d0(DAT_00f53f64 + -0xaa,DAT_007056e8 + 0x20,&DAT_005c2bf0,10);
    if (iVar5 != 0) {
      return;
    }
    FUN_00409e40();
    if (DAT_007056dc == 0) {
      return;
    }
    uVar4 = FUN_00426450(DAT_00f53f64 + -0x15c,DAT_007056e8 + 0x20 + DAT_007056d0 * 0x18,
                         &DAT_005c2d08,DAT_007056d8,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c4 = FUN_004d6740(DAT_00f53f64 + -0x15c,DAT_007056e8 + 0x20 + DAT_007056d0 * 0x18,
                                &DAT_005c2d08,DAT_007056d8,uVar4);
    FUN_004261d0(DAT_00f53f64 + -0x15c,DAT_007056e8 + 0x20 + DAT_007056d0 * 0x18,&DAT_005c2d08,
                 DAT_007056d8);
    return;
  }
  if (DAT_00e38dda == '\x05') {
    FUN_004c7770();
    return;
  }
  if (DAT_00e38dda == '\x13') {
    if (DAT_00d3b180 != 0) {
      FUN_004c7770();
      return;
    }
    if (DAT_00e38df8 != '\0') {
      FUN_00539170();
    }
    if (DAT_00e38e04 == '\0') {
      return;
    }
    FUN_00539170();
    return;
  }
  if (DAT_00e38dda == 'J') {
    if (DAT_00d3b180 != 0) {
      FUN_004c7770();
      return;
    }
    if (DAT_006fdc54 == 0) {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      return;
    }
    if (DAT_00e38df8 != '\0') {
      FUN_00413420(RESTORE_CURRENT_VIDEO);
      return;
    }
    if (DAT_00e38e04 == '\0') {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      return;
    }
    FUN_00413420(RESTORE_CURRENT_VIDEO);
    return;
  }
  if (DAT_00e38dda == '\t') {
    FUN_004f96e0();
    return;
  }
  if (DAT_00e38dda == '\x03') {
    if (DAT_00605a3c == 1) {
      iVar5 = FUN_00425f20(&DAT_005c9230,4);
    }
    else {
      iVar5 = FUN_00425f20(&DAT_005c91f0,4);
    }
    if (iVar5 != 0) {
      return;
    }
    if (DAT_00e38df8 == '\0') {
      return;
    }
    FUN_00426090();
    return;
  }
  if (DAT_00e38dda == '\x11') {
    FUN_004e1e10();
    return;
  }
  if (DAT_00e38dda == '\x02') {
    uVar4 = FUN_004cf660();
    FUN_004ddde0(uVar4);
    return;
  }
  if (DAT_00e38dda == '\n') {
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2e20,5,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2e20,5,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2e20,5);
    return;
  }
  if (DAT_00e38dda == 'V') {
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2eb0,0xe,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2eb0,0xe,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c2eb0,0xe);
    return;
  }
  if (DAT_00e38dda == 'E') {
    if (DAT_00e38df8 == '\0') {
      FUN_00425c40(DAT_00e38da8 + 0x120,DAT_00e38da4 + 0x70,&DAT_005c8ee8,2);
      return;
    }
    DAT_00e38dda = 1;
    DAT_00e38dca = 1;
    if (*(int *)(&DAT_00ea6018 + DAT_00eaac28 * 0x49d8) <= DAT_00e39080) {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      DAT_00e38dca = 1;
      DAT_00e38dda = 1;
      return;
    }
    *(int *)(&DAT_00ea6018 + DAT_00eaac28 * 0x49d8) = DAT_00e39080;
    return;
  }
  if (DAT_00e38dda == '\v') {
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3070,6,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3070,6,uVar4);
    iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3070,6);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425c40(DAT_00e38da8 + 0xd0,DAT_00e38da4 + 0x3c,&PTR_DAT_005c8df8,8);
    return;
  }
  if (DAT_00e38dda == '\f') {
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3038,2,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3038,2,uVar4);
    iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3038,2);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425c40(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x28,&PTR_DAT_005c8d98,4);
    return;
  }
  if (DAT_00e38dda == '\r') {
    restore_load_save_input();
    return;
  }
  if (DAT_00e38dda == '\x10') {
    FUN_004cc500(4);
    if (DAT_00e20998 != 0) {
      DAT_00e20998 = 0;
      DAT_00d3b198 = 1;
      FUN_004099b0();
    }
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3118,0xc,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3118,0xc,uVar4);
    iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c3118,0xc);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425bc0(DAT_00e38da8,DAT_00e38da4,&DAT_005c7a80,4,10);
    puVar24 = &DAT_005c7a80;
    iVar5 = FUN_00425990(DAT_00e38da8,DAT_00e38da4,puVar24,4);
    if (iVar5 != 0) return;
    FUN_004029d0();
    return;
  }
  if (DAT_00e38dda == '\x16') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 0x25;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x2c,&DAT_005c3368,2,DAT_00e38da8,
                         DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x2c,&DAT_005c3368,2,uVar4);
    FUN_004261d0(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x2c,&DAT_005c3368,2);
    return;
  }
  if (DAT_00e38dda == '\x19') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4 + 0x50,&DAT_005c5e20,3,DAT_00e38da8,DAT_00e38da4)
    ;
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4 + 0x50,&DAT_005c5e20,3,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4 + 0x50,&DAT_005c5e20,3);
    return;
  }
  if (DAT_00e38dda == '\x18') {
    if (DAT_00e38df7 != '\0') {
      FUN_00448cf0();
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_005d9e20,0x1e,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_005d9e20,0x1e,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_005d9e20,0x1e);
    return;
  }
  if (DAT_00e38dda == '\x1b') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&DAT_005c5eb0,0x2f,DAT_00e38da8,
                         DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&DAT_005c5eb0,0x2f,uVar4);
    FUN_004261d0(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&DAT_005c5eb0,0x2f);
    return;
  }
  if (DAT_00e38dda == 'F') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_005c6cd8,0x14,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_005c6cd8,0x14,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_005c6cd8,0x14);
    return;
  }
  if (DAT_00e38dda == 'K') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 0x1c;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_005c6f08,0x13,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_005c6f08,0x13,uVar4);
    FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_005c6f08,0x13);
    return;
  }
  if (DAT_00e38dda == '\'') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a90,0xe,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a90,0xe,uVar4);
    iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a90,0xe);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425bc0(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a48,2,10);
    iVar5 = FUN_00425990(DAT_00e38da8,DAT_00e38da4,&DAT_005d9a48,2);
    if (iVar5 != 0) {
      return;
    }
    FUN_004483f0();
    return;
  }
  if (DAT_00e38dda == '\x1c') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c63d8,0x12,DAT_00e38da8
                         ,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c63d8,0x12,uVar4
                               );
    FUN_004261d0(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c63d8,0x12);
    return;
  }
  if (DAT_00e38dda == '%') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0xc,&PTR_LAB_005c5cd0,0xc,DAT_00e38da8,
                         DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0xc,&PTR_LAB_005c5cd0,0xc,uVar4);
    FUN_004261d0(DAT_00e38da8 + 0xc,DAT_00e38da4 + 0xc,&PTR_LAB_005c5cd0,0xc);
    return;
  }
  if (DAT_00e38dda == '$') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 0x15;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c3488,0x10,DAT_00e38da8
                         ,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c3488,0x10,uVar4
                               );
    FUN_004261d0(DAT_00e38da8 + 0x10,DAT_00e38da4 + 0x20,&PTR_DAT_005c3488,0x10);
    return;
  }
  if (DAT_00e38dda == 'N') {
    if (DAT_00e38df7 != '\0') {
      DAT_00e38dda = 0x15;
      DAT_00e38dca = 1;
      return;
    }
    uVar4 = FUN_00426450(DAT_00e38da8 + 0x10,DAT_00e38da4,&PTR_LAB_005c3648,0x45,DAT_00e38da8,
                         DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00e38da8 + 0x10,DAT_00e38da4,&PTR_LAB_005c3648,0x45,uVar4);
    FUN_004261d0(DAT_00e38da8 + 0x10,DAT_00e38da4,&PTR_LAB_005c3648,0x45);
    return;
  }
  if (DAT_00e38dda == 'G') {
    if (DAT_00e38df7 == '\0') {
      DAT_007056a8 = 0;
      DAT_007056b0 = 0;
      DAT_007056b8 = 0;
      DAT_00e20990 = 0;
      return;
    }
    DAT_00e38dda = 0x15;
    DAT_00e38dca = 1;
    return;
  }
  if (DAT_00e38dda == '\x14') {
    uVar4 = FUN_004cf660();
    FUN_00442730(uVar4);
    FUN_00425bc0(0x14,DAT_00e38dac + -0x2c,&DAT_005c7ce8,1,10);
    iVar5 = FUN_00425990(0x14,DAT_00e38dac + -0x2c,&DAT_005c7ce8,1);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425bc0(DAT_00e38db0 + -0x2c,DAT_00e38dac + -0x2c,&DAT_005c7d10,1,0xb);
    iVar5 = FUN_00425990(DAT_00e38db0 + -0x2c,DAT_00e38dac + -0x2c,&DAT_005c7d10,1);
    if (iVar5 != 0) {
      return;
    }
    FUN_00425bc0(0x18,DAT_00e38dac + -0x6e,&DAT_005c7d38,1,0xc);
    iVar5 = FUN_00425990(0x18,DAT_00e38dac + -0x6e,&DAT_005c7d38,1);
    if (iVar5 != 0) {
      return;
    }
    if ((((DAT_00e38e03 != '\0') && (DAT_005d2a18 <= DAT_00e38e34)) &&
        (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)) &&
       ((DAT_005d2a1c <= DAT_00e38e30 &&
        (DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c)))) {
      FUN_00407a10(&DAT_00784f14,&DAT_005d2e88);
      DAT_00e38dca = 1;
    }
    if (DAT_00e38df7 == '\0') {
      if ((DAT_00784f14 != 0) && ((&DAT_0077f748)[(DAT_00784f14 + -1) * 0x62] == '\x01')) {
        DAT_00784f18 = FUN_00444920(DAT_00784f14 + -1);
        cVar2 = (&DAT_0077de02)[DAT_00784f18 * 0x6a];
        if (((cVar2 == '\x01') || ((cVar2 == '\x03' || (cVar2 == '\x05')))) &&
           ((&DAT_0077de05)[DAT_00784f18 * 0x6a] == '\0')) {
          uVar4 = FUN_00426450((DAT_00e38db0 + -500) / 2,DAT_00e38dac + -0x69,&PTR_BYTE_005c3da0,1,
                               DAT_00e38da8,DAT_00e38da4);
          DAT_007056c4 = FUN_004d6740((DAT_00e38db0 + -500) / 2,DAT_00e38dac + -0x69,
                                      &PTR_BYTE_005c3da0,1,uVar4);
          iVar5 = FUN_004261d0((DAT_00e38db0 + -500) / 2,DAT_00e38dac + -0x69,&PTR_BYTE_005c3da0,1);
          if (iVar5 != 0) {
            return;
          }
        }
      }
    }
    else {
      DAT_00784f14 = 0;
      DAT_005d2e88 = 0xffffffff;
      DAT_00e38dca = 1;
    }
    if (DAT_00e38df8 == '\0') {
      return;
    }
    FUN_00409520();
    return;
  }
  if (DAT_00e38dda != '\x15') {
    if (DAT_00e38dda == '&') {
      FUN_00425bc0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c7d60,2,10);
      FUN_00425990(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005c7d60,2);
      return;
    }
    if (DAT_00e38dda == '\x0e') {
      if (DAT_00e38df7 != '\0') {
        DAT_00e38dda = DAT_00605980;
        DAT_00e38dca = 1;
        return;
      }
      uVar4 = FUN_00426450(DAT_00605974,DAT_00605970,&DAT_005c4208,0xc,DAT_00e38da8,DAT_00e38da4);
      DAT_007056c8 = FUN_004d6740(DAT_00605974,DAT_00605970,&DAT_005c4208,0xc,uVar4);
      FUN_004261d0(DAT_00605974,DAT_00605970,&DAT_005c4208,0xc);
      return;
    }
    if ((DAT_00e38dda == '\x0f') || (DAT_00e38dda == 'H')) {
      restore_selection_popup_input();
      return;
    }
    if (DAT_00e38dda == '\a') {
      iVar14 = DAT_00f53f08 + 0x1b0;
      FUN_00425bc0(DAT_00e38da8,iVar14,&DAT_005f8320,0xe,10);
      iVar5 = FUN_00425990(iVar5,iVar14,&DAT_005f8320,0xe);
      if (iVar5 != 0) {
        return;
      }
      FUN_00425bc0(DAT_00f53f0c,(DAT_00f53f00 + -2) * 0x10 + DAT_00f53f08,&DAT_005c7b80,1,0x19);
      iVar5 = FUN_00425990(DAT_00f53f0c,(DAT_00f53f00 + -2) * 0x10 + DAT_00f53f08,&DAT_005c7b80,1);
      if (iVar5 != 0) {
        return;
      }
      if (DAT_00e38df8 != '\0') {
        DAT_00e38dda = 1;
        DAT_00e38dca = 1;
        (&DAT_00ea623c)[DAT_00eaac28 * 0x49d8] = 0;
        return;
      }
      if (DAT_00605a44 == 1) {
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4ab8,9,DAT_00e38da8,DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4ab8,9,uVar4);
        iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4ab8,9);
        if (iVar5 != 0) {
          return;
        }
        FUN_00425e80(DAT_00f53f0c,DAT_00f53f08,&DAT_005c8d08,2,10);
        ppuVar25 = (undefined **)&DAT_005c8d08;
LAB_004dc479: ;
        FUN_00425c40(DAT_00f53f0c,DAT_00f53f08,ppuVar25,2);
        return;
      }
      if (DAT_00605a44 == 2) {
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4cd0,0x14,DAT_00e38da8,DAT_00e38da4)
        ;
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4cd0,0x14,uVar4);
        uVar4 = 0x14;
        ppuVar25 = (undefined **)&DAT_005c4cd0;
      }
      else {
        if (DAT_00605a44 == 3) {
          uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4f00,0x14,DAT_00e38da8,
                               DAT_00e38da4);
          DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4f00,0x14,uVar4);
          FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c4f00,0x14);
          return;
        }
        if (DAT_00605a44 == 4) {
          uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5368,8,DAT_00e38da8,DAT_00e38da4);
          DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5368,8,uVar4);
          FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5368,8);
          return;
        }
        if (DAT_00605a44 == 5) {
          uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005f7650,4,DAT_00e38da8,
                               DAT_00e38da4);
          DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005f7650,4,uVar4);
          uVar4 = 4;
          ppuVar25 = &PTR_DAT_005f7650;
        }
        else {
          if (DAT_00605a44 == 6) {
            iVar5 = DAT_00f53e6c;
            if (0xf < DAT_00f53e6c) {
              iVar5 = 0xf;
            }
            uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_006032e8,iVar5 + 3,DAT_00e38da8,
                                 DAT_00e38da4);
            DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_006032e8,iVar5 + 3,uVar4);
            iVar5 = DAT_00f53e6c;
            if (0xf < DAT_00f53e6c) {
              iVar5 = 0xf;
            }
            iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_006032e8,iVar5 + 3);
            if (iVar5 != 0) {
              return;
            }
            FUN_0055bb90();
            return;
          }
          if (DAT_00605a44 == 7) {
            uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5130,2,DAT_00e38da8,DAT_00e38da4
                                );
            DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5130,2,uVar4);
            FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5130,2);
            return;
          }
          if (DAT_00605a44 == 8) {
            return;
          }
          if (DAT_00605a44 == 9) {
            return;
          }
          if (DAT_00605a44 == 0xb) {
            uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5168,1,DAT_00e38da8,DAT_00e38da4
                                );
            DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5168,1,uVar4);
            iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5168,1);
            if (iVar5 != 0) {
              return;
            }
            uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c59e8,5,DAT_00e38da8,DAT_00e38da4
                                );
            FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c59e8,5,uVar4);
            uVar4 = 5;
            ppuVar25 = (undefined **)&DAT_005c59e8;
          }
          else {
            if (DAT_00605a44 == 10) {
              return;
            }
            if (DAT_00605a44 == 0xc) {
              FUN_00425e80(DAT_00f53f0c,DAT_00f53f08,&DAT_005c8d38,2,10);
              ppuVar25 = (undefined **)&DAT_005c8d38;
              goto LAB_004dc479;
            }
            if (DAT_00605a44 != 0xe) {
              if (DAT_00605a44 != 0xd) {
                return;
              }
              uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5878,0xc,DAT_00e38da8,
                                   DAT_00e38da4);
              DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5878,0xc,uVar4);
              FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5878,0xc);
              return;
            }
            uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5798,8,DAT_00e38da8,DAT_00e38da4
                                );
            DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5798,8,uVar4);
            uVar4 = 8;
            ppuVar25 = (undefined **)&DAT_005c5798;
          }
        }
      }
LAB_004dcc8c: ;
      FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,ppuVar25,uVar4);
      return;
    }
    if (DAT_00e38dda != '(') {
      if (DAT_00e38dda == '*') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c4bb8,DAT_00605984 + 1,
                             DAT_00e38da8,DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c4bb8,DAT_00605984 + 1,
                                    uVar4);
        iVar5 = DAT_00605984 + 1;
        ppuVar25 = &PTR_DAT_005c4bb8;
LAB_004dcb43: ;
        FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,ppuVar25,iVar5);
        return;
      }
      if (DAT_00e38dda == '+') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        FUN_00425bc0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c7bc8,4,10);
        iVar5 = FUN_00425990(DAT_00f53f0c,DAT_00f53f08,&DAT_005c7bc8,4);
        if (iVar5 != 0) {
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_s_lice_g02_wav_00600044_2_005c51a8,8,
                             DAT_00e38da8,DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,
                                    &PTR_s_lice_g02_wav_00600044_2_005c51a8,8,uVar4);
        iVar5 = 8;
        ppuVar25 = &PTR_s_lice_g02_wav_00600044_2_005c51a8;
        goto LAB_004dcb43;
      }
      if (DAT_00e38dda == 'R') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 9;
          DAT_00e38dca = 1;
          return;
        }
        FUN_00425bc0(0x1e,0x1e,&DAT_005c7c58,4,10);
        iVar5 = FUN_00425990(0x1e,0x1e,&DAT_005c7c58,4);
        if (iVar5 != 0) {
          return;
        }
        uVar4 = FUN_00426450(0x1e,0x1e,&PTR_s_lice_g02_wav_00600044_2_005c5288,8,DAT_00e38da8,
                             DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(0x1e,0x1e,&PTR_s_lice_g02_wav_00600044_2_005c5288,8,uVar4);
        FUN_004261d0(0x1e,0x1e,&PTR_s_lice_g02_wav_00600044_2_005c5288,8);
        return;
      }
      if (DAT_00e38dda == ',') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5468,0xc,DAT_00e38da8,DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5468,0xc,uVar4);
        FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5468,0xc);
        return;
      }
      if (DAT_00e38dda == 'M') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 9;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5468,0xc,DAT_00e38da8,DAT_00e38da4);
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&DAT_005c5468,0xc,uVar4);
        iVar5 = 0xc;
        ppuVar25 = (undefined **)&DAT_005c5468;
        goto LAB_004dcb43;
      }
      if (DAT_00e38dda == '-') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c55b8,7,DAT_00e38da8,DAT_00e38da4
                            );
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c55b8,7,uVar4);
        iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c55b8,7);
        if (iVar5 != 0) {
          return;
        }
        FUN_00425e80(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c8d68,2,10);
        ppuVar25 = &PTR_DAT_005c8d68;
        goto LAB_004dc479;
      }
      if (DAT_00e38dda == '.') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c5680,5,DAT_00e38da8,DAT_00e38da4
                            );
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c5680,5,uVar4);
        uVar4 = 5;
        ppuVar25 = &PTR_DAT_005c5680;
        goto LAB_004dcc8c;
      }
      if (DAT_00e38dda == 'L') {
        if (DAT_00e38df8 != '\0') {
          DAT_00e38dda = 7;
          DAT_00e38dca = 1;
          return;
        }
        uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c5710,3,DAT_00e38da8,DAT_00e38da4
                            );
        DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c5710,3,uVar4);
        iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_005c5710,3);
        if (iVar5 != 0) {
          return;
        }
        FUN_00425e80(DAT_00f53f0c + 0x130,DAT_00f53f08 + 0xe9,&DAT_005c5768,2,10);
        FUN_00425c40(DAT_00f53f0c + 0x130,DAT_00f53f08 + 0xe9,&DAT_005c5768,2);
        FUN_004256a0(DAT_00f53f0c + 0x130,DAT_00f53f08 + 0xe9,&DAT_005c5768,2);
        return;
      }
      if (DAT_00e38dda == '3') {
        if (DAT_00e38dd9 != '\x01') {
          uVar4 = FUN_004ea880(DAT_00e38da8,DAT_00e38da4);
          uVar4 = FUN_004ea8d0(uVar4);
          uVar4 = FUN_00426450(DAT_00f53db8,DAT_00f53db4 + -0x70,uVar4);
          uVar4 = FUN_004ea880(uVar4);
          uVar4 = FUN_004ea8d0(uVar4);
          DAT_007056c8 = FUN_004d6740(DAT_00f53db8,DAT_00f53db4 + -0x70,uVar4);
          uVar4 = FUN_004ea880();
          uVar4 = FUN_004ea8d0(uVar4);
          FUN_004261d0(DAT_00f53db8,DAT_00f53db4 + -0x70,uVar4);
          return;
        }
        if (DAT_00e38df8 == '\0') {
          DAT_007056a8 = 0;
          DAT_007056b0 = 0;
          DAT_007056b8 = 0;
          DAT_00e20990 = 0;
          return;
        }
        FUN_0051f780(0);
        FUN_0051f780(1);
        FUN_00418640();
        return;
      }
      if (DAT_00e38dda == '5') {
        iVar5 = FUN_00425990(DAT_006fdca0 + 0x234,DAT_006fdc9c + 0x19a,&DAT_00579788,1);
        if (iVar5 != 0) {
          return;
        }
        iVar5 = FUN_00425990(DAT_006fdca0 + 0x6e,DAT_006fdc9c + 0x1a0,&DAT_005c81b0,2);
        if (iVar5 != 0) {
          return;
        }
        cVar2 = FUN_0041b8f0(0x006fdd08u);
        if (cVar2 == '\0') goto LAB_004dcf17;
        uVar4 = 1;
        iVar14 = DAT_006fdc9c + 0x19c;
        puVar24 = &DAT_005797b0;
        iVar5 = DAT_006fdca0 + 10;
      }
      else {
        if (DAT_00e38dda == '4') {
          if ((DAT_007027a4 != '\0') &&
             (iVar5 = FUN_00425990(DAT_006fdca0 + 0x244,DAT_006fdc9c + 0x19a,&DAT_00579788,1),
             iVar5 != 0)) {
            return;
          }
          FUN_00419090();
          return;
        }
        if (DAT_00e38dda != ';') {
          if (DAT_00e38dda == '=') {
            restore_mission_menu_input();
            return;
          }
          if (DAT_00e38dda == '/') {
            iVar5 = FUN_005168e0();
            if (iVar5 != 0) {
              return;
            }
            uVar4 = FUN_005165b0();
            iVar5 = FUN_00425990(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7ee0,uVar4);
            if (iVar5 != 0) {
              return;
            }
            uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7f28,0x10,DAT_00e38da8,
                                 DAT_00e38da4);
            DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7f28,0x10,uVar4);
            iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7f28,0x10);
            if (iVar5 != 0) {
              return;
            }
            if (DAT_00e38df8 == '\0') {
              return;
            }
            FUN_00402bb0();
            return;
          }
          if (DAT_00e38dda == '0') {
            uVar4 = FUN_00426450(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005e0408,5,DAT_00e38da8,
                                 DAT_00e38da4);
            DAT_007056c8 = FUN_004d6740(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005e0408,5,uVar4);
            iVar5 = FUN_004261d0(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005e0408,5);
            if (iVar5 != 0) {
              return;
            }
            if (DAT_00e38df8 == '\0') {
              return;
            }
            FUN_004cb970();
            return;
          }
          if (DAT_00e38dda == '>') {
            restore_display_confirmation_input();
            return;
          }
          if (DAT_00e38dda == '6') {
            FUN_004cc500(1);
            iVar5 = FUN_00425990(DAT_00e38da8 + 0x1d0,DAT_00e38da4 + 0xf9,&DAT_00579788,1);
            if (iVar5 != 0) {
              return;
            }
            if (DAT_00e20998 == 0) {
              return;
            }
            FUN_00418220();
            return;
          }
          if (DAT_00e38dda == 'Q') {
            if (DAT_00e38df8 == '\0') {
              DAT_007056a8 = 0;
              DAT_007056b0 = 0;
              DAT_007056b8 = 0;
              DAT_00e20990 = 0;
              return;
            }
            DAT_00e20998 = 0;
            DAT_00e38dda = 0x36;
            DAT_00e38dca = 1;
            return;
          }
          if (DAT_00e38dda != '<') {
            if (DAT_00e38dda == '7') {
              if ((DAT_00e38df8 == '\0') && (DAT_00e38e03 == '\0')) {
                DAT_007056a8 = 0;
                DAT_007056b0 = 0;
                DAT_007056b8 = 0;
                DAT_00e20990 = 0;
                return;
              }
              DAT_00e38dda = 0x38;
              DAT_00e38dca = 1;
              return;
            }
            if (DAT_00e38dda == '8') {
              if (DAT_00e38df8 != '\0') {
                DAT_00d3b43d = 1;
                DAT_00e38dca = 1;
                return;
              }
              if (DAT_00e38e03 == '\0') {
                DAT_007056a8 = 0;
                DAT_007056b0 = 0;
                DAT_007056b8 = 0;
                DAT_00e20990 = 0;
                return;
              }
              DAT_00d3b43d = 1;
              DAT_00e38dca = 1;
              return;
            }
            if (DAT_00e38dda == '9') {
              if (DAT_00e38df8 != '\0') {
                DAT_007056a8 = 0;
                DAT_007056b0 = 0;
                DAT_007056b8 = 0;
                DAT_00d3b43d = 1;
                DAT_00e20990 = 0;
                DAT_00e38dca = 1;
                return;
              }
              if (DAT_00e38e03 == '\0') {
                DAT_007056a8 = 0;
                DAT_007056b0 = 0;
                DAT_007056b8 = 0;
                DAT_00e20990 = 0;
                return;
              }
              DAT_007056a8 = 0;
              DAT_007056b0 = 0;
              DAT_007056b8 = 0;
              DAT_00d3b43d = 1;
              DAT_00e20990 = 0;
              DAT_00e38dca = 1;
              return;
            }
            if (DAT_00e38dda == ':') {
              if ((DAT_00e38df8 == '\0') && (DAT_00e38e03 == '\0')) {
                DAT_007056a8 = 0;
                DAT_007056b0 = 0;
                DAT_007056b8 = 0;
                DAT_00e20990 = 0;
                return;
              }
              DAT_00e38dda = 0;
              DAT_00e38dca = 1;
              return;
            }
            if (DAT_00e38dda == '\x1e') {
              FUN_00405f80();
              if (DAT_00f53f78 == 0) {
                FUN_00406190();
                iVar5 = FUN_004061e0(1);
                if (iVar5 != 0) {
                  return;
                }
              }
              uVar4 = FUN_004cf660();
              FUN_004ddde0(uVar4);
              if (DAT_00e38df8 == '\0') {
                FUN_00405f80();
                if (DAT_00e38e03 == '\0') {
                  return;
                }
                FUN_0040c440();
                return;
              }
            }
            else {
              if (DAT_00e38dda != 'I') {
                if (DAT_00e38dda == '\x1f') {
                  FUN_00405f80();
                  if (DAT_00f53f78 == 0) {
                    FUN_00406190();
                    iVar5 = FUN_004061e0(1);
                    if (iVar5 != 0) {
                      return;
                    }
                  }
                  uVar4 = FUN_004cf660();
                  FUN_004ddde0(uVar4);
                  if (DAT_00e38df8 == '\0') {
                    FUN_00405f80();
                    if (DAT_00e38e03 == '\0') {
                      return;
                    }
                    FUN_0040ba40();
                    return;
                  }
                  DAT_00e38dda = 1;
                  DAT_00d05708 = 0;
                  FUN_004e2ab0();
                  DAT_00e38dca = 1;
                  return;
                }
                if (DAT_00e38dda == ' ') {
                  FUN_00405f80();
                  if (DAT_00f53f78 == 0) {
                    FUN_00406190();
                    iVar5 = FUN_004061e0(1);
                    if (iVar5 != 0) {
                      return;
                    }
                  }
                  uVar4 = FUN_004cf660();
                  FUN_004ddde0(uVar4);
                  if (DAT_00e38df8 == '\0') {
                    FUN_00405f80();
                    if (DAT_00e38e03 == '\0') {
                      return;
                    }
                    FUN_0040b830();
                    return;
                  }
                  DAT_00e38dda = 1;
                  DAT_00d0570c = 0;
                  FUN_004e2ab0();
                  DAT_00e38dca = 1;
                  return;
                }
                if (DAT_00e38dda != 'O') {
                  if (DAT_00e38dda == 'P') {
                    if (DAT_00e38df8 == '\0') {
                      DAT_007056a8 = 0;
                      DAT_007056b0 = 0;
                      DAT_007056b8 = 0;
                      DAT_00e20990 = 0;
                      return;
                    }
                    DAT_00e38dda = 7;
                    DAT_00e38dca = 1;
                    return;
                  }
                  if (DAT_00e38dda == 'S') {
                    restore_high_scores_input();
                    return;
                  }
                  if (DAT_00e38dda == 'T') {
                    restore_menu_modal_input();
                    return;
                  }
                  if (DAT_00e38dda != 'U') {
                    DAT_007056a8 = 0;
                    DAT_007056b0 = 0;
                    DAT_007056b8 = 0;
                    DAT_00e20990 = 0;
                    return;
                  }
                  FUN_0041d310();
                  return;
                }
                if (DAT_00e38df8 != '\0') {
                  DAT_00e38dda = 7;
                  DAT_00e38dca = 1;
                  return;
                }
                iVar5 = *(int *)(&DAT_00ea5fd8 + DAT_00eaac28 * 0x49d8);
                if ((&DAT_006fdc88)[iVar5] == '\0') {
                  DAT_007056a8 = 0;
                  DAT_007056b0 = 0;
                  DAT_007056b8 = 0;
                  DAT_00e20990 = 0;
                  return;
                }
                FUN_00425bc0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x110,(&PTR_DAT_005f78f0)[iVar5],1,
                             iVar5 + 0x28);
                FUN_00425990(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x110,
                             (&PTR_DAT_005f78f0)[*(int *)(&DAT_00ea5fd8 + DAT_00eaac28 * 0x49d8)],1)
                ;
                return;
              }
              FUN_00405f80();
              if (DAT_00f53f78 == 0) {
                FUN_00406190();
                iVar5 = FUN_004061e0(1);
                if (iVar5 != 0) {
                  return;
                }
              }
              uVar4 = FUN_004cf660();
              FUN_004ddde0(uVar4);
              if (DAT_00e38df8 == '\0') {
                FUN_00405f80();
                if (DAT_00e38e03 == '\0') {
                  return;
                }
                FUN_0040c570();
                return;
              }
            }
            DAT_00e38dda = 1;
            FUN_004e2ab0();
            DAT_00e38dca = 1;
            return;
          }
          if ((DAT_00e38df8 == '\0') && (DAT_00e38e04 == '\0')) {
            DAT_007056a8 = 0;
            DAT_007056b0 = 0;
            DAT_007056b8 = 0;
            DAT_00e20990 = 0;
            return;
          }
          DAT_00e38dda = 0;
          DAT_00e38dca = 1;
          return;
        }
        iVar5 = FUN_00425990(DAT_006fdca0 + 0x234,DAT_006fdc9c + 0x19a,&DAT_00579788,1);
        if (iVar5 != 0) {
          return;
        }
        uVar4 = 2;
        iVar14 = DAT_006fdc9c + 0x1a0;
        puVar24 = &DAT_005c81b0;
        iVar5 = DAT_006fdca0 + 0x6e;
      }
      iVar5 = FUN_00425990(iVar5,iVar14,puVar24,uVar4);
      if (iVar5 != 0) {
        return;
      }
LAB_004dcf17: ;
      FUN_00418fe0();
      return;
    }
    DAT_007056c8 = 0;
    DAT_007056c4 = 0;
    DAT_007056c0 = 0;
    if (DAT_00e38df8 != '\0') {
      DAT_00e38dda = 7;
      DAT_00e38dca = 1;
      return;
    }
    FUN_00425bc0(DAT_00f53f0c,DAT_00f53f08,&DAT_006035e0,2,10);
    iVar5 = FUN_00425990(DAT_00f53f0c,DAT_00f53f08,&DAT_006035e0,2);
    if (iVar5 != 0) {
      return;
    }
    if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 2) {
      FUN_00425e80(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603580,2,10);
      iVar5 = FUN_00425c40(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603580,2);
    }
    else {
      if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) != 1)
      goto LAB_004dc680;
      FUN_00425e80(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006035b0,2,10);
      iVar5 = FUN_00425c40(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006035b0,2);
    }
    if (iVar5 != 0) {
      return;
    }
LAB_004dc680: ;
    uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006034e0,1,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006034e0,1,uVar4);
    iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_006034e0,1);
    if (iVar5 != 0) {
      return;
    }
    uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603520,1,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c4 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603520,1,uVar4);
    iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603520,1);
    if (iVar5 != 0) {
      return;
    }
    if (DAT_007056c4 == 0) {
      uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603540,1,DAT_00e38da8,DAT_00e38da4);
      DAT_007056c4 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603540,1,uVar4);
      iVar5 = FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603540,1);
      if (iVar5 != 0) {
        return;
      }
      if (DAT_007056c4 != 0) {
        DAT_007056c4 = DAT_007056c4 + 1;
      }
    }
    uVar4 = FUN_00426450(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603560,1,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c0 = FUN_004d6740(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603560,1,uVar4);
    FUN_004261d0(DAT_00f53f0c,DAT_00f53f08,&PTR_DAT_00603560,1);
    return;
  }
  uVar18 = (DAT_00775910 - DAT_005d2a18) + DAT_00e38e34;
  fVar16 = (float)((DAT_0077590c - DAT_005d2a1c) + DAT_00e38e30);
  local_4c = (int)*(short *)((int)DAT_005d2a30 * 0x144 + 0x7770ec);
  local_48 = (int)*(short *)(&DAT_007770ee + (int)DAT_005d2a30 * 0x144);
  local_70 = uVar18;
  local_64 = fVar16;
  FUN_00442500();
  FUN_004426c0();
  uVar4 = FUN_004cf660();
  FUN_00442730(uVar4);
  uVar4 = FUN_00426450(0xf,DAT_00e38dac + -0x74,&DAT_005c3268,8,DAT_00e38da8,DAT_00e38da4);
  DAT_007056c8 = FUN_004d6740(0xf,DAT_00e38dac + -0x74,&DAT_005c3268,8,uVar4);
  iVar5 = FUN_004261d0(0xf,DAT_00e38dac + -0x74,&DAT_005c3268,8);
  if (iVar5 != 0) {
    return;
  }
  if (DAT_007056c8 == 0) {
    uVar4 = FUN_00426450(0xdf,DAT_00e38dac + -0x72,&PTR_DAT_005c3348,1,DAT_00e38da8,DAT_00e38da4);
    DAT_007056c8 = FUN_004d6740(0xdf,DAT_00e38dac + -0x72,&PTR_DAT_005c3348,1,uVar4);
    iVar5 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&PTR_DAT_005c3348,1);
    if (iVar5 != 0) {
      return;
    }
    if (DAT_007056c8 == 1) {
      DAT_007056c8 = 10;
    }
  }
  psVar20 = DAT_005d2a30;
  uVar17 = (undefined2)uVar18;
  uVar3 = SUB42(fVar16,0);
  if (DAT_00775928 != 0xe) {
    if (DAT_00775928 == 0xf) {
      iVar5 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c3418,4);
      psVar20 = DAT_005d2a30;
      if (iVar5 != 0) {
        return;
      }
      iVar5 = (int)DAT_005d2a30 * 0x144;
      local_50 = (float)(int)*(short *)(&DAT_007770ee + iVar5);
      local_54 = (int)*(short *)(iVar5 + 0x7770ec);
      if (DAT_00e38e03 == '\0') {
        if (DAT_00e38e04 != '\0') {
          if (-1 < (int)DAT_005e1db0) {
            if (((DAT_005d2a18 <= DAT_00e38e34) && (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)
                ) && ((DAT_005d2a1c <= DAT_00e38e30 &&
                      ((DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c &&
                       ((int)DAT_005e1db0 < (int)(uint)(byte)(&DAT_007770f1)[iVar5])))))) {
              iVar22 = 1;
              iVar14 = DAT_005e1db0 * 6 + iVar5;
              *(undefined2 *)(&DAT_00776fb0 + iVar14 + 8) = uVar17;
              *(undefined2 *)(&DAT_00776fb0 + iVar14 + 10) = uVar3;
              iVar14 = 0;
              local_58 = (float)(uint)(byte)(&DAT_007770f1)[iVar5];
              if (1 < (byte)(&DAT_007770f1)[iVar5]) {
                psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
                do {
                  local_64 = (float)((int)*psVar9 - (int)psVar9[3]);
                  fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
                  if ((int)local_64 <= (int)fVar16) {
                    local_64 = fVar16;
                  }
                  fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
                  fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
                  if ((int)fVar16 <= (int)fVar12) {
                    fVar16 = fVar12;
                  }
                  if ((int)fVar16 < (int)local_64) {
                    auStack_34[iVar22] = 1;
                    fVar16 = local_64;
                  }
                  else {
                    auStack_34[iVar22] = 0;
                  }
                  iVar14 = (int)fVar16 + 1 + iVar14;
                  iVar22 = iVar22 + 1;
                  psVar9 = psVar9 + 3;
                } while (iVar22 < (int)local_58);
              }
              if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
                 (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
                if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
                  FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
                  (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
                }
                pvVar7 = FUN_00561a1d(iVar22 * 6);
                (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
                (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
              }
              bVar15 = (&DAT_007770f1)[iVar5];
              (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
              local_6c = 0;
              local_70 = 1;
              if (1 < bVar15) {
                psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
                do {
                  if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < local_6c) goto LAB_004daba1;
                  local_58 = (float)(int)psVar9[-1];
                  local_5c = (float)(int)*psVar9;
                  local_68 = (int)psVar9[2] - (int)local_58;
                  local_64 = (float)((int)psVar9[3] - (int)local_5c);
                  iVar14 = 0;
                  local_78 = restore_x87_sqrt((float)(int)local_64 * (float)(int)local_64 +
                                  (float)local_68 * (float)local_68);
                  if (local_78 != 0.0) {
                    if ((&DAT_00776fb0)[iVar5] == '\0') {
                      iVar14 = 0;
                    }
                    else {
                      local_64 = (float)(int)local_64 / local_78;
                      local_60 = (float)local_68;
                      local_68 = 0;
                      local_60 = local_60 / local_78;
                      if (0.0 <= local_78) {
                        local_58 = (float)(int)local_5c;
                        iVar22 = local_6c * 6;
                        do {
                          uVar3 = FUN_0056133c();
                          *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                          uVar3 = FUN_0056133c();
                          *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3
                          ;
                          *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) =
                               (char)local_70 + -1;
                          local_68 = local_68 + (uint)(byte)(&DAT_00776fb0)[iVar5];
                          iVar14 = iVar14 + 1;
                          iVar22 = iVar22 + 6;
                        } while ((float)local_68 <= local_78);
                      }
                    }
                  }
                  local_6c = local_6c + iVar14;
                  local_70 = local_70 + 1;
                  psVar9 = psVar9 + 3;
                } while ((int)local_70 < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
              }
              (&DAT_007770e4)[(int)psVar20 * 0x51] = local_6c;
              *(undefined2 *)(&DAT_007770ee + iVar5) = 0xffff;
              *(undefined2 *)(iVar5 + 0x7770ec) = 0xffff;
              bVar15 = (&DAT_007770f1)[iVar5];
              local_58 = (float)CONCAT31(RESTORE_SLICE3(local_58,1),bVar15);
              if (bVar15 != 0) {
                local_68 = 0;
                pcVar19 = &DAT_0077f748;
                do {
                  if ((pcVar19[1] != '\0') && (*pcVar19 == '\x01')) {
                    uVar8 = (uint)*(ushort *)(pcVar19 + 0xc);
                    uVar18 = uVar8;
                    if (DAT_0126517c <= (int)uVar8) {
                      uVar18 = 0;
                    }
                    if (DAT_0126517c <= (int)uVar8) {
                      uVar8 = 0;
                    }
                    iVar22 = (int)*(short *)(pcVar19 + 6);
                    iVar14 = (int)*(short *)(pcVar19 + 4);
                    local_3c = (short)(&DAT_010d6a0c)[uVar18 * 0x20] + iVar14;
                    sVar1 = (&DAT_010d6a0e)[uVar8 * 0x20];
                    if ((((iVar14 <= (short)(&DAT_00776fb8)[(int)psVar20 * 0xa2]) &&
                         ((short)(&DAT_00776fb8)[(int)psVar20 * 0xa2] <= local_3c)) &&
                        (iVar22 <= (short)(&DAT_00776fba)[(int)psVar20 * 0xa2])) &&
                       ((int)(short)(&DAT_00776fba)[(int)psVar20 * 0xa2] <= sVar1 + iVar22)) {
                      *(undefined2 *)(iVar5 + 0x7770ec) = (undefined2)local_68;
                    }
                    iVar23 = (int)*(short *)(iVar5 + 0x776fb2 + (uint)bVar15 * 6);
                    if (((iVar14 <= iVar23) && (iVar23 <= local_3c)) &&
                       ((iVar14 = (int)*(short *)((int)&DAT_00776fb4 + iVar5 + (uint)bVar15 * 6),
                        iVar22 <= iVar14 && (iVar14 <= sVar1 + iVar22)))) {
                      *(undefined2 *)(&DAT_007770ee + iVar5) = (undefined2)local_68;
                    }
                  }
                  pcVar19 = pcVar19 + 0x62;
                  local_68 = local_68 + 1;
                } while ((int)pcVar19 < 0x7843d8);
              }
            }
LAB_004daba1: ;
            _DAT_005d2a2c = 0;
          }
          DAT_005e1db0 = 0xffffffff;
          goto LAB_004db028;
        }
        if (DAT_00e38e00 == '\0') {
          if (((DAT_00e38df8 == '\0') || (iVar5 = FUN_004ddbd0(uVar18,fVar16,&local_79), iVar5 < 0))
             || (local_79 == '\0')) goto LAB_004db028;
          FUN_004dd930(iVar5);
          iVar5 = FUN_004dd8d0();
          if (0 < iVar5) goto LAB_004db028;
          FUN_004dd900(0);
          FUN_004dd8e0();
        }
        else {
          if ((int)DAT_005e1db0 < 0) goto LAB_004db028;
          if (((DAT_005d2a18 <= DAT_00e38e34) && (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2))
             && ((DAT_005d2a1c <= DAT_00e38e30 &&
                 ((DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c &&
                  ((int)DAT_005e1db0 < (int)(uint)(byte)(&DAT_007770f1)[iVar5])))))) {
            iVar22 = 1;
            iVar14 = DAT_005e1db0 * 6 + iVar5;
            *(undefined2 *)(&DAT_00776fb0 + iVar14 + 8) = uVar17;
            *(undefined2 *)(&DAT_00776fb0 + iVar14 + 10) = uVar3;
            iVar14 = 0;
            local_58 = (float)(uint)(byte)(&DAT_007770f1)[iVar5];
            if (1 < (byte)(&DAT_007770f1)[iVar5]) {
              psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
              do {
                local_64 = (float)((int)*psVar9 - (int)psVar9[3]);
                fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
                if ((int)local_64 <= (int)fVar16) {
                  local_64 = fVar16;
                }
                fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
                fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
                if ((int)fVar16 <= (int)fVar12) {
                  fVar16 = fVar12;
                }
                if ((int)fVar16 < (int)local_64) {
                  auStack_34[iVar22] = 1;
                  fVar16 = local_64;
                }
                else {
                  auStack_34[iVar22] = 0;
                }
                iVar14 = (int)fVar16 + 1 + iVar14;
                iVar22 = iVar22 + 1;
                psVar9 = psVar9 + 3;
              } while (iVar22 < (int)local_58);
            }
            if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
               (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
              if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
                FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
                (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
              }
              pvVar7 = FUN_00561a1d(iVar22 * 6);
              (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
              (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
            }
            bVar15 = (&DAT_007770f1)[iVar5];
            (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
            local_74 = (short *)0x0;
            local_6c = 1;
            if (1 < bVar15) {
              psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
              do {
                if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < (int)local_74) goto LAB_004db01e;
                local_58 = (float)(int)psVar9[-1];
                local_5c = (float)(int)*psVar9;
                local_68 = (int)psVar9[2] - (int)local_58;
                local_64 = (float)((int)psVar9[3] - (int)local_5c);
                iVar14 = 0;
                local_78 = restore_x87_sqrt((float)(int)local_64 * (float)(int)local_64 +
                                (float)local_68 * (float)local_68);
                if ((local_78 != 0.0) && ((&DAT_00776fb0)[iVar5] != '\0')) {
                  local_64 = (float)(int)local_64 / local_78;
                  local_60 = (float)local_68;
                  local_68 = 0;
                  local_60 = local_60 / local_78;
                  if (0.0 <= local_78) {
                    local_58 = (float)(int)local_5c;
                    iVar22 = (int)local_74 * 6;
                    do {
                      uVar3 = FUN_0056133c();
                      *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                      uVar3 = FUN_0056133c();
                      *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                      *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) =
                           (char)local_6c + -1;
                      local_68 = local_68 + (uint)(byte)(&DAT_00776fb0)[iVar5];
                      iVar14 = iVar14 + 1;
                      iVar22 = iVar22 + 6;
                    } while ((float)local_68 <= local_78);
                  }
                }
                local_74 = (short *)((int)local_74 + iVar14);
                local_6c = local_6c + 1;
                psVar9 = psVar9 + 3;
              } while (local_6c < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
            }
            *(undefined2 *)(&DAT_007770ee + iVar5) = 0xffff;
            *(undefined2 *)(iVar5 + 0x7770ec) = 0xffff;
            bVar15 = (&DAT_007770f1)[iVar5];
            (&DAT_007770e4)[(int)psVar20 * 0x51] = local_74;
            local_58 = (float)CONCAT31(RESTORE_SLICE3(local_58,1),bVar15);
            if (bVar15 != 0) {
              local_68 = 0;
              pcVar19 = &DAT_0077f748;
              do {
                if ((pcVar19[1] != '\0') && (*pcVar19 == '\x01')) {
                  uVar8 = (uint)*(ushort *)(pcVar19 + 0xc);
                  uVar18 = uVar8;
                  if (DAT_0126517c <= (int)uVar8) {
                    uVar18 = 0;
                  }
                  if (DAT_0126517c <= (int)uVar8) {
                    uVar8 = 0;
                  }
                  iVar22 = (int)*(short *)(pcVar19 + 6);
                  iVar14 = (int)*(short *)(pcVar19 + 4);
                  local_3c = (short)(&DAT_010d6a0c)[uVar18 * 0x20] + iVar14;
                  sVar1 = (&DAT_010d6a0e)[uVar8 * 0x20];
                  if ((((iVar14 <= (short)(&DAT_00776fb8)[(int)psVar20 * 0xa2]) &&
                       ((short)(&DAT_00776fb8)[(int)psVar20 * 0xa2] <= local_3c)) &&
                      (iVar22 <= (short)(&DAT_00776fba)[(int)psVar20 * 0xa2])) &&
                     ((int)(short)(&DAT_00776fba)[(int)psVar20 * 0xa2] <= sVar1 + iVar22)) {
                    *(undefined2 *)(iVar5 + 0x7770ec) = (undefined2)local_68;
                  }
                  iVar23 = (int)*(short *)(iVar5 + 0x776fb2 + (uint)bVar15 * 6);
                  if (((iVar14 <= iVar23) && (iVar23 <= local_3c)) &&
                     ((iVar14 = (int)*(short *)((int)&DAT_00776fb4 + iVar5 + (uint)bVar15 * 6),
                      iVar22 <= iVar14 && (iVar14 <= sVar1 + iVar22)))) {
                    *(undefined2 *)(&DAT_007770ee + iVar5) = (undefined2)local_68;
                  }
                }
                pcVar19 = pcVar19 + 0x62;
                local_68 = local_68 + 1;
              } while ((int)pcVar19 < 0x7843d8);
            }
          }
        }
      }
      else {
        local_6c = (int)fVar16 + -8;
        local_70 = uVar18 - 8;
        local_78 = (float)(uVar18 + 8);
        local_68 = (int)fVar16 + 8;
        DAT_005e1db0 = 0;
        if ((byte)(&DAT_007770f1)[iVar5] != 0) {
          local_74 = &DAT_00776fba + (int)DAT_005d2a30 * 0xa2;
          do {
            if ((((int)local_70 <= (int)local_74[-1]) && ((int)local_74[-1] <= (int)local_78)) &&
               ((local_6c <= *local_74 && (*local_74 <= local_68)))) {
              local_79 = '\x01';
              goto LAB_004d9f52;
            }
            DAT_005e1db0 = DAT_005e1db0 + 1;
            local_74 = local_74 + 3;
          } while ((int)DAT_005e1db0 < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
        }
        iVar14 = 0;
        local_79 = '\0';
        if (0 < (int)(&DAT_007770e4)[(int)DAT_005d2a30 * 0x51]) {
          psVar9 = (short *)(&DAT_00776fb4)[(int)DAT_005d2a30 * 0x51];
          do {
            if (((((int)local_70 <= (int)*psVar9) && ((int)*psVar9 <= (int)local_78)) &&
                (local_6c <= psVar9[1])) && (psVar9[1] <= local_68)) {
              DAT_005e1db0 = (uint)*(byte *)((&DAT_00776fb4)[(int)DAT_005d2a30 * 0x51] + 4 +
                                            iVar14 * 6);
              goto LAB_004d9f52;
            }
            iVar14 = iVar14 + 1;
            psVar9 = psVar9 + 3;
          } while (iVar14 < (int)(&DAT_007770e4)[(int)DAT_005d2a30 * 0x51]);
        }
        DAT_005e1db0 = 0xffffffff;
LAB_004d9f52: ;
        if ((int)DAT_005e1db0 < 0) {
          local_74 = (short *)0x0;
          psVar20 = &DAT_00776fba;
          do {
            DAT_005e1db0 = 0;
            psVar9 = psVar20;
            if (*(char *)((int)psVar20 + 0x137) != '\0') {
              do {
                if ((((int)local_70 <= (int)psVar9[-1]) && ((int)psVar9[-1] <= (int)local_78)) &&
                   ((local_6c <= *psVar9 && (*psVar9 <= local_68)))) goto LAB_004da78d;
                DAT_005e1db0 = DAT_005e1db0 + 1;
                psVar9 = psVar9 + 3;
              } while ((int)DAT_005e1db0 < (int)(uint)*(byte *)((int)psVar20 + 0x137));
            }
            iVar5 = 0;
            if (0 < *(int *)(psVar20 + 0x95)) {
              psVar9 = *(short **)(psVar20 + -3);
              do {
                if (((((int)local_70 <= (int)*psVar9) && ((int)*psVar9 <= (int)local_78)) &&
                    (local_6c <= psVar9[1])) && (psVar9[1] <= local_68)) {
                  DAT_005e1db0 = (uint)*(byte *)(*(short **)(psVar20 + -3) + iVar5 * 3 + 2);
                  goto LAB_004da78d;
                }
                iVar5 = iVar5 + 1;
                psVar9 = psVar9 + 3;
              } while (iVar5 < *(int *)(psVar20 + 0x95));
            }
            DAT_005e1db0 = 0xffffffff;
LAB_004da78d: ;
            if (-1 < (int)DAT_005e1db0) {
              DAT_005e1db0 = 0xffffffff;
              DAT_005d2a30 = local_74;
              DAT_00e38dca = 1;
              break;
            }
            psVar20 = psVar20 + 0xa2;
            local_74 = (short *)((int)local_74 + 1);
          } while ((int)psVar20 < 0x77af02);
          goto LAB_004db028;
        }
        if (local_79 == '\0') {
          local_70 = DAT_005e1db0 + 1;
          if ((int)local_70 < 0) {
            local_70 = 0;
          }
          if ((byte)(&DAT_007770f1)[iVar5] < 0x32) {
            iVar14 = (byte)(&DAT_007770f1)[iVar5] - 1;
            if ((int)local_70 <= iVar14) {
              local_64 = (float)((iVar14 - local_70) + 1);
              puVar11 = (undefined4 *)(&DAT_00776fbe + (int)DAT_005d2a30 * 0xa2 + iVar14 * 3);
              do {
                *puVar11 = *(undefined4 *)((int)puVar11 + -6);
                *(undefined2 *)(puVar11 + 1) = *(undefined2 *)((int)puVar11 + -2);
                local_64 = (float)((int)local_64 + -1);
                puVar11 = (undefined4 *)((int)puVar11 + -6);
              } while (local_64 != 0.0);
            }
            iVar22 = 1;
            iVar14 = local_70 * 6 + iVar5;
            *(undefined2 *)(&DAT_00776fb0 + iVar14 + 8) = uVar17;
            *(undefined2 *)(&DAT_00776fb0 + iVar14 + 10) = uVar3;
            iVar14 = 0;
            bVar15 = (&DAT_007770f1)[iVar5] + 1;
            local_60 = (float)(uint)bVar15;
            (&DAT_007770f1)[iVar5] = bVar15;
            if (1 < bVar15) {
              psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
              do {
                local_64 = (float)((int)*psVar9 - (int)psVar9[3]);
                fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
                if ((int)local_64 <= (int)fVar16) {
                  local_64 = fVar16;
                }
                fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
                fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
                if ((int)fVar16 <= (int)fVar12) {
                  fVar16 = fVar12;
                }
                if ((int)fVar16 < (int)local_64) {
                  auStack_34[iVar22] = 1;
                  fVar16 = local_64;
                }
                else {
                  auStack_34[iVar22] = 0;
                }
                iVar14 = (int)fVar16 + 1 + iVar14;
                iVar22 = iVar22 + 1;
                psVar9 = psVar9 + 3;
              } while (iVar22 < (int)local_60);
            }
            if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
               (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
              if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
                FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
                (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
              }
              pvVar7 = FUN_00561a1d(iVar22 * 6);
              (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
              (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
            }
            bVar15 = (&DAT_007770f1)[iVar5];
            (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
            local_6c = 0;
            local_74 = (short *)0x1;
            if (1 < bVar15) {
              psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
              do {
                if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < local_6c) goto LAB_004da333;
                local_60 = (float)(int)psVar9[-1];
                local_5c = (float)(int)*psVar9;
                local_68 = (int)psVar9[2] - (int)local_60;
                local_64 = (float)((int)psVar9[3] - (int)local_5c);
                iVar14 = 0;
                local_78 = restore_x87_sqrt((float)(int)local_64 * (float)(int)local_64 +
                                (float)local_68 * (float)local_68);
                if (local_78 != 0.0) {
                  if ((&DAT_00776fb0)[iVar5] == '\0') {
                    iVar14 = 0;
                  }
                  else {
                    local_58 = (float)(int)local_64 / local_78;
                    local_64 = (float)local_68;
                    local_68 = 0;
                    local_64 = local_64 / local_78;
                    if (0.0 <= local_78) {
                      local_5c = (float)(int)local_5c;
                      iVar22 = local_6c * 6;
                      do {
                        uVar3 = FUN_0056133c();
                        *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                        uVar3 = FUN_0056133c();
                        *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                        *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) =
                             (char)local_74 + -1;
                        local_68 = local_68 + (uint)(byte)(&DAT_00776fb0)[iVar5];
                        iVar14 = iVar14 + 1;
                        iVar22 = iVar22 + 6;
                      } while ((float)local_68 <= local_78);
                    }
                  }
                }
                local_6c = local_6c + iVar14;
                local_74 = (short *)((int)local_74 + 1);
                psVar9 = psVar9 + 3;
              } while ((int)local_74 < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
            }
            (&DAT_007770e4)[(int)psVar20 * 0x51] = local_6c;
            *(undefined2 *)(&DAT_007770ee + iVar5) = 0xffff;
            *(undefined2 *)(iVar5 + 0x7770ec) = 0xffff;
            bVar15 = (&DAT_007770f1)[iVar5];
            local_58 = (float)CONCAT31(RESTORE_SLICE3(local_58,1),bVar15);
            if (bVar15 != 0) {
              local_74 = (short *)0x0;
              pcVar19 = &DAT_0077f748;
              do {
                if ((pcVar19[1] != '\0') && (*pcVar19 == '\x01')) {
                  uVar8 = (uint)*(ushort *)(pcVar19 + 0xc);
                  uVar18 = uVar8;
                  if (DAT_0126517c <= (int)uVar8) {
                    uVar18 = 0;
                  }
                  if (DAT_0126517c <= (int)uVar8) {
                    uVar8 = 0;
                  }
                  iVar22 = (int)*(short *)(pcVar19 + 6);
                  iVar14 = (int)*(short *)(pcVar19 + 4);
                  local_3c = (short)(&DAT_010d6a0c)[uVar18 * 0x20] + iVar14;
                  sVar1 = (&DAT_010d6a0e)[uVar8 * 0x20];
                  if ((((iVar14 <= (short)(&DAT_00776fb8)[(int)psVar20 * 0xa2]) &&
                       ((short)(&DAT_00776fb8)[(int)psVar20 * 0xa2] <= local_3c)) &&
                      (iVar22 <= (short)(&DAT_00776fba)[(int)psVar20 * 0xa2])) &&
                     ((int)(short)(&DAT_00776fba)[(int)psVar20 * 0xa2] <= sVar1 + iVar22)) {
                    *(undefined2 *)(iVar5 + 0x7770ec) = RESTORE_SLICE2(local_74,0);
                  }
                  iVar23 = (int)*(short *)(iVar5 + 0x776fb2 + (uint)bVar15 * 6);
                  if (((iVar14 <= iVar23) && (iVar23 <= local_3c)) &&
                     ((iVar14 = (int)*(short *)((int)&DAT_00776fb4 + iVar5 + (uint)bVar15 * 6),
                      iVar22 <= iVar14 && (iVar14 <= sVar1 + iVar22)))) {
                    *(undefined2 *)(&DAT_007770ee + iVar5) = RESTORE_SLICE2(local_74,0);
                  }
                }
                pcVar19 = pcVar19 + 0x62;
                local_74 = (short *)((int)local_74 + 1);
              } while ((int)pcVar19 < 0x7843d8);
            }
            DAT_005e1db0 = local_70;
          }
          else {
LAB_004da333: ;
            DAT_005e1db0 = 0xffffffff;
          }
        }
        else if (((DAT_005d2a18 <= DAT_00e38e34) &&
                 (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)) &&
                ((DAT_005d2a1c <= DAT_00e38e30 &&
                 ((DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c &&
                  ((int)DAT_005e1db0 < (int)(uint)(byte)(&DAT_007770f1)[iVar5])))))) {
          iVar22 = 1;
          iVar14 = DAT_005e1db0 * 6 + iVar5;
          *(undefined2 *)(&DAT_00776fb0 + iVar14 + 8) = uVar17;
          *(undefined2 *)(&DAT_00776fb0 + iVar14 + 10) = uVar3;
          iVar14 = 0;
          local_58 = (float)(uint)(byte)(&DAT_007770f1)[iVar5];
          if (1 < (byte)(&DAT_007770f1)[iVar5]) {
            psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
            do {
              local_64 = (float)((int)*psVar9 - (int)psVar9[3]);
              fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
              if ((int)local_64 <= (int)fVar16) {
                local_64 = fVar16;
              }
              fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
              fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
              if ((int)fVar16 <= (int)fVar12) {
                fVar16 = fVar12;
              }
              if ((int)fVar16 < (int)local_64) {
                auStack_34[iVar22] = 1;
                fVar16 = local_64;
              }
              else {
                auStack_34[iVar22] = 0;
              }
              iVar14 = (int)fVar16 + 1 + iVar14;
              iVar22 = iVar22 + 1;
              psVar9 = psVar9 + 3;
            } while (iVar22 < (int)local_58);
          }
          if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
             (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
            if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
              FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
              (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
            }
            pvVar7 = FUN_00561a1d(iVar22 * 6);
            (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
            (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
          }
          bVar15 = (&DAT_007770f1)[iVar5];
          (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
          local_6c = 0;
          local_70 = 1;
          if (1 < bVar15) {
            psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
            do {
              if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < local_6c) goto LAB_004db01e;
              local_58 = (float)(int)psVar9[-1];
              local_5c = (float)(int)*psVar9;
              local_68 = (int)psVar9[2] - (int)local_58;
              local_64 = (float)((int)psVar9[3] - (int)local_5c);
              iVar14 = 0;
              local_78 = restore_x87_sqrt((float)(int)local_64 * (float)(int)local_64 +
                              (float)local_68 * (float)local_68);
              if (local_78 != 0.0) {
                if ((&DAT_00776fb0)[iVar5] == '\0') {
                  iVar14 = 0;
                }
                else {
                  local_64 = (float)(int)local_64 / local_78;
                  local_60 = (float)local_68;
                  local_68 = 0;
                  local_60 = local_60 / local_78;
                  if (0.0 <= local_78) {
                    local_58 = (float)(int)local_5c;
                    iVar22 = local_6c * 6;
                    do {
                      uVar3 = FUN_0056133c();
                      *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                      uVar3 = FUN_0056133c();
                      *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                      *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) =
                           (char)local_70 + -1;
                      local_68 = local_68 + (uint)(byte)(&DAT_00776fb0)[iVar5];
                      iVar14 = iVar14 + 1;
                      iVar22 = iVar22 + 6;
                    } while ((float)local_68 <= local_78);
                  }
                }
              }
              local_6c = local_6c + iVar14;
              local_70 = local_70 + 1;
              psVar9 = psVar9 + 3;
            } while ((int)local_70 < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
          }
          (&DAT_007770e4)[(int)psVar20 * 0x51] = local_6c;
          *(undefined2 *)(&DAT_007770ee + iVar5) = 0xffff;
          *(undefined2 *)(iVar5 + 0x7770ec) = 0xffff;
          bVar15 = (&DAT_007770f1)[iVar5];
          local_58 = (float)CONCAT31(RESTORE_SLICE3(local_58,1),bVar15);
          if (bVar15 != 0) {
            local_70 = 0;
            pcVar19 = &DAT_0077f748;
            do {
              if ((pcVar19[1] != '\0') && (*pcVar19 == '\x01')) {
                uVar8 = (uint)*(ushort *)(pcVar19 + 0xc);
                uVar18 = uVar8;
                if (DAT_0126517c <= (int)uVar8) {
                  uVar18 = 0;
                }
                if (DAT_0126517c <= (int)uVar8) {
                  uVar8 = 0;
                }
                iVar22 = (int)*(short *)(pcVar19 + 6);
                iVar14 = (int)*(short *)(pcVar19 + 4);
                local_3c = (short)(&DAT_010d6a0c)[uVar18 * 0x20] + iVar14;
                sVar1 = (&DAT_010d6a0e)[uVar8 * 0x20];
                if ((((iVar14 <= (short)(&DAT_00776fb8)[(int)psVar20 * 0xa2]) &&
                     ((short)(&DAT_00776fb8)[(int)psVar20 * 0xa2] <= local_3c)) &&
                    (iVar22 <= (short)(&DAT_00776fba)[(int)psVar20 * 0xa2])) &&
                   ((int)(short)(&DAT_00776fba)[(int)psVar20 * 0xa2] <= sVar1 + iVar22)) {
                  *(undefined2 *)(iVar5 + 0x7770ec) = (undefined2)local_70;
                }
                iVar23 = (int)*(short *)(iVar5 + 0x776fb2 + (uint)bVar15 * 6);
                if (((iVar14 <= iVar23) && (iVar23 <= local_3c)) &&
                   ((iVar14 = (int)*(short *)((int)&DAT_00776fb4 + iVar5 + (uint)bVar15 * 6),
                    iVar22 <= iVar14 && (iVar14 <= sVar1 + iVar22)))) {
                  *(undefined2 *)(&DAT_007770ee + iVar5) = (undefined2)local_70;
                }
              }
              pcVar19 = pcVar19 + 0x62;
              local_70 = local_70 + 1;
            } while ((int)pcVar19 < 0x7843d8);
          }
        }
      }
LAB_004db01e: ;
      _DAT_005d2a2c = 0;
LAB_004db028: ;
      iVar5 = FUN_004dd910();
      if (local_54 != iVar5) {
        DAT_00e38dca = 1;
        return;
      }
      iVar5 = FUN_004dd920();
      if (local_50 == (float)iVar5) {
        return;
      }
      DAT_00e38dca = 1;
      return;
    }
    if (DAT_00775928 == 1) {
      if ((DAT_00775920 == 0) &&
         (iVar5 = FUN_00425c40(0xdf,DAT_00e38dac + -0x72,&DAT_005c8c60,5), iVar5 != 0)) {
        return;
      }
      iVar5 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c33c0,3);
      if (iVar5 != 0) {
        return;
      }
      FUN_00407620(DAT_00775918,0);
      iVar5 = FUN_00407870();
      if (iVar5 == 0) {
        return;
      }
      DAT_00775928 = 2;
      goto LAB_004db185;
    }
    if (DAT_00775928 != 2) {
      if (DAT_00775928 != 4) {
        return;
      }
      FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&PTR_DAT_005c3348,1);
      return;
    }
    if (0 < DAT_00e38ddc) {
      DAT_00e38ddc = DAT_00e38ddc + -1;
      return;
    }
    if (((&DAT_0077f748)[DAT_0077591c * 0x62] == '\0') &&
       (iVar5 = FUN_00425c40(0xdf,DAT_00e38dac + -0x72,&DAT_005c8c60,5), iVar5 != 0)) {
      return;
    }
    iVar5 = DAT_0077591c;
    iVar14 = DAT_0077591c * 0x62;
    cVar2 = (&DAT_0077f748)[iVar14];
    if (cVar2 == '\x01') {
      iVar22 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c3dc0,6);
      if (iVar22 != 0) {
        return;
      }
      if ((&DAT_0077f760)[iVar14] == '\0') {
        iVar14 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c3ec0,0xe);
      }
      else {
        iVar14 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c4048,0x10);
      }
      if (iVar14 != 0) {
        return;
      }
      FUN_00442a40(iVar5);
      FUN_00442ef0();
      FUN_00442f80();
    }
    else {
      if (cVar2 == '\x03') {
        iVar5 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c3e88,2);
      }
      else {
        if (cVar2 != '\x02') goto LAB_004db2f6;
        iVar5 = FUN_004261d0(0xdf,DAT_00e38dac + -0x72,&DAT_005c3e68,1);
      }
      if (iVar5 != 0) {
        return;
      }
    }
LAB_004db2f6: ;
    FUN_00407620(DAT_0077591c,1);
    iVar5 = FUN_00407870();
    if (iVar5 == 0) {
      return;
    }
LAB_004db185: ;
    DAT_00775924 = (uint)(ushort)(&DAT_0077f754)[(iVar5 + -1) * 0x31];
    DAT_0077591c = iVar5 + -1;
    return;
  }
  if (DAT_00e38e04 == '\0') {
    if (DAT_00e38df8 != '\0') {
      DAT_00775928 = 0xf;
      goto LAB_004dbe16;
    }
    iVar14 = (int)DAT_005d2a30 * 0x144;
    pbVar21 = &DAT_00776fb0 + iVar14;
    iVar5 = (byte)(&DAT_007770f1)[iVar14] - 1;
    if ((((DAT_005d2a18 <= DAT_00e38e34) && (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)) &&
        (DAT_005d2a1c <= DAT_00e38e30)) &&
       (((DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c && (-1 < iVar5)) &&
        (iVar5 < (int)(uint)(byte)(&DAT_007770f1)[iVar14])))) {
      iVar22 = 1;
      *(undefined2 *)(pbVar21 + iVar5 * 6 + 8) = uVar17;
      *(undefined2 *)(pbVar21 + iVar5 * 6 + 10) = uVar3;
      iVar5 = 0;
      local_50 = (float)(uint)(byte)(&DAT_007770f1)[iVar14];
      if (1 < (byte)(&DAT_007770f1)[iVar14]) {
        psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
        do {
          local_60 = (float)((int)*psVar9 - (int)psVar9[3]);
          fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
          if ((int)local_60 <= (int)fVar16) {
            local_60 = fVar16;
          }
          fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
          fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
          if ((int)fVar16 <= (int)fVar12) {
            fVar16 = fVar12;
          }
          if ((int)fVar16 < (int)local_60) {
            auStack_34[iVar22] = 1;
            fVar16 = local_60;
          }
          else {
            auStack_34[iVar22] = 0;
          }
          iVar5 = (int)fVar16 + 1U + iVar5;
          iVar22 = iVar22 + 1;
          psVar9 = psVar9 + 3;
        } while (iVar22 < (int)local_50);
      }
      if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar5) &&
         (iVar22 = iVar5 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
        if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
          FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
          (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
        }
        pvVar7 = FUN_00561a1d(iVar22 * 6);
        (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
        (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
      }
      bVar15 = (&DAT_007770f1)[iVar14];
      (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar5;
      local_78 = 0.0;
      local_68 = 1;
      if (1 < bVar15) {
        psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
        do {
          if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < (int)local_78) goto LAB_004dbe0c;
          local_50 = (float)(int)psVar9[-1];
          local_54 = (int)*psVar9;
          local_64 = (float)((int)psVar9[2] - (int)local_50);
          local_60 = (float)(psVar9[3] - local_54);
          iVar5 = 0;
          segment_length = restore_x87_sqrt((float)(int)local_60 * (float)(int)local_60 +
                                   (float)(int)local_64 * (float)(int)local_64);
          if ((segment_length != 0.0) && (*pbVar21 != 0)) {
            local_5c = (float)(int)local_60 / segment_length;
            local_58 = (float)(int)local_64;
            local_64 = 0.0;
            local_58 = local_58 / segment_length;
            if (0.0 <= segment_length) {
              local_50 = (float)local_54;
              iVar22 = (int)local_78 * 6;
              do {
                uVar3 = FUN_0056133c();
                *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                uVar3 = FUN_0056133c();
                *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = (char)local_68 + -1;
                local_64 = (float)((int)local_64 + (uint)*pbVar21);
                iVar5 = iVar5 + 1;
                iVar22 = iVar22 + 6;
              } while ((float)(int)local_64 <= segment_length);
            }
          }
          local_78 = (float)((int)local_78 + iVar5);
          local_68 = local_68 + 1;
          psVar9 = psVar9 + 3;
        } while (local_68 < (int)(uint)(byte)(&DAT_007770f1)[iVar14]);
      }
      goto LAB_004dbdff;
    }
  }
  else {
    iVar5 = (int)DAT_005d2a30 * 0x144;
    iVar14 = (byte)(&DAT_007770f1)[iVar5] - 1;
    if (iVar14 < 0) {
      if (DAT_00e38e34 < DAT_005d2a18) {
        return;
      }
      if (DAT_00e38d9c + DAT_005d2a18 * -2 <= DAT_00e38e34) {
        return;
      }
      if (DAT_00e38e30 < DAT_005d2a1c) {
        return;
      }
      if ((DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c <= DAT_00e38e30) {
        return;
      }
      (&DAT_007770f2)[iVar5] = 1;
      psVar20 = DAT_005d2a30;
      iVar5 = (int)DAT_005d2a30 * 0x144;
      uVar18 = (uint)(byte)(&DAT_007770f1)[iVar5];
      pbVar21 = &DAT_00776fb0 + iVar5;
      if ((byte)(&DAT_007770f1)[iVar5] < 0x32) {
        iVar14 = (byte)(&DAT_007770f1)[iVar5] - 1;
        if ((int)uVar18 <= iVar14) {
          iVar22 = (iVar14 - uVar18) + 1;
          puVar11 = (undefined4 *)(&DAT_00776fbe + (int)DAT_005d2a30 * 0xa2 + iVar14 * 3);
          do {
            iVar22 = iVar22 + -1;
            *puVar11 = *(undefined4 *)((int)puVar11 + -6);
            *(undefined2 *)(puVar11 + 1) = *(undefined2 *)((int)puVar11 + -2);
            puVar11 = (undefined4 *)((int)puVar11 + -6);
          } while (iVar22 != 0);
        }
        iVar22 = 1;
        iVar14 = 0;
        *(undefined2 *)(pbVar21 + uVar18 * 6 + 8) = (undefined2)local_70;
        *(undefined2 *)(pbVar21 + uVar18 * 6 + 10) = RESTORE_SLICE2(local_64,0);
        bVar15 = (&DAT_007770f1)[iVar5] + 1;
        local_50 = (float)(uint)bVar15;
        (&DAT_007770f1)[iVar5] = bVar15;
        if (1 < bVar15) {
          psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
          do {
            local_68 = (int)*psVar9 - (int)psVar9[3];
            iVar23 = (int)psVar9[3] - (int)*psVar9;
            if (local_68 <= iVar23) {
              local_68 = iVar23;
            }
            iVar23 = (int)psVar9[1] - (int)psVar9[4];
            iVar13 = (int)psVar9[4] - (int)psVar9[1];
            if (iVar23 <= iVar13) {
              iVar23 = iVar13;
            }
            if (iVar23 < local_68) {
              auStack_34[iVar22] = 1;
              iVar23 = local_68;
            }
            else {
              auStack_34[iVar22] = 0;
            }
            iVar14 = iVar23 + 1 + iVar14;
            iVar22 = iVar22 + 1;
            psVar9 = psVar9 + 3;
          } while (iVar22 < (int)local_50);
        }
        if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
           (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
          if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
            FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
            (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
          }
          pvVar7 = FUN_00561a1d(iVar22 * 6);
          (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
          (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
        }
        bVar15 = (&DAT_007770f1)[iVar5];
        (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
        local_78 = 0.0;
        local_6c = 1;
        if (1 < bVar15) {
          psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
          do {
            if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < (int)local_78) goto LAB_004db899;
            local_50 = (float)(int)psVar9[-1];
            local_54 = (int)*psVar9;
            local_60 = (float)((int)psVar9[2] - (int)local_50);
            local_68 = psVar9[3] - local_54;
            iVar14 = 0;
            segment_length = restore_x87_sqrt((float)local_68 * (float)local_68 +
                                     (float)(int)local_60 * (float)(int)local_60);
            if ((segment_length != 0.0) && (*pbVar21 != 0)) {
              local_5c = (float)local_68;
              local_68 = 0;
              local_5c = local_5c / segment_length;
              local_58 = (float)(int)local_60 / segment_length;
              if (0.0 <= segment_length) {
                local_50 = (float)local_54;
                iVar22 = (int)local_78 * 6;
                do {
                  uVar3 = FUN_0056133c();
                  *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                  uVar3 = FUN_0056133c();
                  *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                  *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = (char)local_6c + -1
                  ;
                  local_68 = local_68 + (uint)*pbVar21;
                  iVar14 = iVar14 + 1;
                  iVar22 = iVar22 + 6;
                } while ((float)local_68 <= segment_length);
              }
            }
            local_78 = (float)((int)local_78 + iVar14);
            local_6c = local_6c + 1;
            psVar9 = psVar9 + 3;
          } while (local_6c < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
        }
LAB_004db88c: ;
        *(float *)(pbVar21 + 0x134) = local_78;
        FUN_00407f90();
      }
    }
    else {
      pbVar21 = &DAT_00776fb0 + iVar5;
      if (((((DAT_005d2a18 <= DAT_00e38e34) && (DAT_00e38e34 < DAT_00e38d9c + DAT_005d2a18 * -2)) &&
           (DAT_005d2a1c <= DAT_00e38e30)) &&
          ((DAT_00e38e30 < (DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c && (-1 < iVar14)))) &&
         (iVar14 < (int)(uint)(byte)(&DAT_007770f1)[iVar5])) {
        iVar23 = 1;
        iVar22 = 0;
        *(undefined2 *)(pbVar21 + iVar14 * 6 + 8) = (undefined2)local_70;
        *(undefined2 *)(pbVar21 + iVar14 * 6 + 10) = RESTORE_SLICE2(local_64,0);
        local_50 = (float)(uint)(byte)(&DAT_007770f1)[iVar5];
        if (1 < (byte)(&DAT_007770f1)[iVar5]) {
          psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
          do {
            local_60 = (float)((int)*psVar9 - (int)psVar9[3]);
            fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
            if ((int)local_60 <= (int)fVar16) {
              local_60 = fVar16;
            }
            fVar16 = (float)((int)psVar9[1] - (int)psVar9[4]);
            fVar12 = (float)((int)psVar9[4] - (int)psVar9[1]);
            if ((int)fVar16 <= (int)fVar12) {
              fVar16 = fVar12;
            }
            if ((int)fVar16 < (int)local_60) {
              auStack_34[iVar23] = 1;
              fVar16 = local_60;
            }
            else {
              auStack_34[iVar23] = 0;
            }
            iVar22 = (int)fVar16 + 1U + iVar22;
            iVar23 = iVar23 + 1;
            psVar9 = psVar9 + 3;
          } while (iVar23 < (int)local_50);
        }
        if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22) &&
           (iVar14 = iVar22 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14)) {
          if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
            FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
            (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
          }
          pvVar7 = FUN_00561a1d(iVar14 * 6);
          (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
          (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar14;
        }
        bVar15 = (&DAT_007770f1)[iVar5];
        (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar22;
        local_78 = 0.0;
        local_6c = 1;
        if (1 < bVar15) {
          psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
          do {
            if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < (int)local_78) goto LAB_004db899;
            local_50 = (float)(int)psVar9[-1];
            local_54 = (int)*psVar9;
            local_68 = (int)psVar9[2] - (int)local_50;
            local_60 = (float)(psVar9[3] - local_54);
            iVar14 = 0;
            segment_length = restore_x87_sqrt((float)(int)local_60 * (float)(int)local_60 +
                                     (float)local_68 * (float)local_68);
            if ((segment_length != 0.0) && (*pbVar21 != 0)) {
              local_5c = (float)(int)local_60 / segment_length;
              local_58 = (float)local_68;
              local_68 = 0;
              local_58 = local_58 / segment_length;
              if (0.0 <= segment_length) {
                local_50 = (float)local_54;
                iVar22 = (int)local_78 * 6;
                do {
                  uVar3 = FUN_0056133c();
                  *(undefined2 *)(iVar22 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                  uVar3 = FUN_0056133c();
                  *(undefined2 *)(iVar22 + 2 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = uVar3;
                  *(char *)(iVar22 + 4 + (&DAT_00776fb4)[(int)psVar20 * 0x51]) = (char)local_6c + -1
                  ;
                  local_68 = local_68 + (uint)*pbVar21;
                  iVar14 = iVar14 + 1;
                  iVar22 = iVar22 + 6;
                } while ((float)local_68 <= segment_length);
              }
            }
            local_78 = (float)((int)local_78 + iVar14);
            local_6c = local_6c + 1;
            psVar9 = psVar9 + 3;
          } while (local_6c < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
        }
        goto LAB_004db88c;
      }
    }
LAB_004db899: ;
    psVar20 = DAT_005d2a30;
    iVar5 = (int)DAT_005d2a30 * 0x144;
    uVar18 = (uint)(byte)(&DAT_007770f1)[iVar5];
    pbVar21 = &DAT_00776fb0 + iVar5;
    if ((byte)(&DAT_007770f1)[iVar5] < 0x32) {
      iVar14 = (byte)(&DAT_007770f1)[iVar5] - 1;
      if ((int)uVar18 <= iVar14) {
        iVar22 = (iVar14 - uVar18) + 1;
        puVar11 = (undefined4 *)(&DAT_00776fbe + (int)DAT_005d2a30 * 0xa2 + iVar14 * 3);
        do {
          iVar22 = iVar22 + -1;
          *puVar11 = *(undefined4 *)((int)puVar11 + -6);
          *(undefined2 *)(puVar11 + 1) = *(undefined2 *)((int)puVar11 + -2);
          puVar11 = (undefined4 *)((int)puVar11 + -6);
        } while (iVar22 != 0);
      }
      iVar22 = 1;
      *(undefined2 *)(pbVar21 + uVar18 * 6 + 8) = (undefined2)local_70;
      *(undefined2 *)(pbVar21 + uVar18 * 6 + 10) = RESTORE_SLICE2(local_64,0);
      bVar15 = (&DAT_007770f1)[iVar5] + 1;
      local_50 = (float)(uint)bVar15;
      (&DAT_007770f1)[iVar5] = bVar15;
      iVar14 = 0;
      if (1 < bVar15) {
        psVar9 = &DAT_00776fb8 + (int)psVar20 * 0xa2;
        do {
          local_60 = (float)((int)*psVar9 - (int)psVar9[3]);
          fVar16 = (float)((int)psVar9[3] - (int)*psVar9);
          if ((int)local_60 <= (int)fVar16) {
            local_60 = fVar16;
          }
          iVar23 = (int)psVar9[1] - (int)psVar9[4];
          iVar13 = (int)psVar9[4] - (int)psVar9[1];
          if (iVar23 <= iVar13) {
            iVar23 = iVar13;
          }
          if (iVar23 < (int)local_60) {
            iVar14 = iVar14 + 1 + (int)local_60;
            auStack_34[iVar22] = 1;
          }
          else {
            iVar14 = iVar14 + 1 + iVar23;
            auStack_34[iVar22] = 0;
          }
          iVar22 = iVar22 + 1;
          psVar9 = psVar9 + 3;
        } while (iVar22 < (int)local_50);
      }
      if (((int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar14) &&
         (iVar22 = iVar14 + 0x32, (int)(&DAT_007770e8)[(int)psVar20 * 0x51] < iVar22)) {
        if ((&DAT_00776fb4)[(int)psVar20 * 0x51] != 0) {
          FUN_00561a2b((&DAT_00776fb4)[(int)psVar20 * 0x51]);
          (&DAT_00776fb4)[(int)psVar20 * 0x51] = 0;
        }
        pvVar7 = FUN_00561a1d(iVar22 * 6);
        (&DAT_00776fb4)[(int)psVar20 * 0x51] = pvVar7;
        (&DAT_007770e8)[(int)psVar20 * 0x51] = iVar22;
      }
      bVar15 = (&DAT_007770f1)[iVar5];
      (&DAT_007770e4)[(int)psVar20 * 0x51] = iVar14;
      local_78 = 0.0;
      local_68 = 1;
      if (1 < bVar15) {
        psVar9 = &DAT_00776fba + (int)psVar20 * 0xa2;
        do {
          if ((int)(&DAT_007770e4)[(int)psVar20 * 0x51] < (int)local_78) goto LAB_004dbe0c;
          local_50 = (float)(int)psVar9[-1];
          local_54 = (int)*psVar9;
          local_64 = (float)((int)psVar9[2] - (int)local_50);
          local_60 = (float)(psVar9[3] - local_54);
          iVar14 = 0;
          segment_length = restore_x87_sqrt((float)(int)local_60 * (float)(int)local_60 +
                                   (float)(int)local_64 * (float)(int)local_64);
          if ((segment_length != 0.0) && (*pbVar21 != 0)) {
            local_5c = (float)(int)local_60 / segment_length;
            local_58 = (float)(int)local_64;
            local_64 = 0.0;
            local_58 = local_58 / segment_length;
            if (0.0 <= segment_length) {
              local_50 = (float)local_54;
              iVar22 = (int)local_78 * 6;
              do {
                uVar3 = FUN_0056133c();
                *(undefined2 *)((&DAT_00776fb4)[(int)psVar20 * 0x51] + iVar22) = uVar3;
                uVar3 = FUN_0056133c();
                iVar22 = iVar22 + 6;
                *(undefined2 *)((&DAT_00776fb4)[(int)psVar20 * 0x51] + -4 + iVar22) = uVar3;
                *(char *)((&DAT_00776fb4)[(int)psVar20 * 0x51] + -2 + iVar22) = (char)local_68 + -1;
                local_64 = (float)((int)local_64 + (uint)*pbVar21);
                iVar14 = iVar14 + 1;
              } while ((float)(int)local_64 <= segment_length);
            }
          }
          local_78 = (float)((int)local_78 + iVar14);
          local_68 = local_68 + 1;
          psVar9 = psVar9 + 3;
        } while (local_68 < (int)(uint)(byte)(&DAT_007770f1)[iVar5]);
      }
LAB_004dbdff: ;
      *(float *)(pbVar21 + 0x134) = local_78;
      FUN_00407f90();
    }
  }
LAB_004dbe0c: ;
  _DAT_005d2a2c = 0;
LAB_004dbe16: ;
  if (local_4c != *(short *)((int)DAT_005d2a30 * 0x144 + 0x7770ec)) {
    DAT_00e38dca = 1;
    return;
  }
  if (local_48 == *(short *)(&DAT_007770ee + (int)DAT_005d2a30 * 0x144)) {
    return;
  }
  DAT_00e38dca = 1;
  return;
}
