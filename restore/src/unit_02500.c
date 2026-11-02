#include "restore_types.h"
#define DAT_00f54578 (*(undefined4 *)0x00f54578u)
#define s_audio_wavs_desert1_wav_005fb13c (*(char (*)[23])0x005fb13cu)
#define s_audio_wavs_desert2_wav_005fb124 (*(char (*)[23])0x005fb124u)
#define s_audio_wavs_desert3_wav_005fb10c (*(char (*)[23])0x005fb10cu)
#define s_audio_wavs_desert4_wav_005fb0f4 (*(char (*)[23])0x005fb0f4u)
#define s_audio_wavs_desert5_wav_005fb0dc (*(char (*)[23])0x005fb0dcu)
#define s_audio_wavs_desert6_wav_005fb0c4 (*(char (*)[23])0x005fb0c4u)
#define s_audio_wavs_desert7_wav_005fb0ac (*(char (*)[23])0x005fb0acu)
#define s_audio_wavs_desert_r_wav_005fb154 (*(char (*)[24])0x005fb154u)
#define s_audio_wavs_farm1_wav_005faefc (*(char (*)[21])0x005faefcu)
#define s_audio_wavs_farm2_wav_005faee4 (*(char (*)[21])0x005faee4u)
#define s_audio_wavs_farm3_wav_005faecc (*(char (*)[21])0x005faeccu)
#define s_audio_wavs_farm4_wav_005faeb4 (*(char (*)[21])0x005faeb4u)
#define s_audio_wavs_farm5_wav_005fae9c (*(char (*)[21])0x005fae9cu)
#define s_audio_wavs_farm6_wav_005fae84 (*(char (*)[21])0x005fae84u)
#define s_audio_wavs_farm7_wav_005fae6c (*(char (*)[21])0x005fae6cu)
#define s_audio_wavs_farm_r_wav_005faf14 (*(char (*)[22])0x005faf14u)
#define s_audio_wavs_flood1_wav_005fae3c (*(char (*)[22])0x005fae3cu)
#define s_audio_wavs_flood2_wav_005fae24 (*(char (*)[22])0x005fae24u)
#define s_audio_wavs_flood3_wav_005fae0c (*(char (*)[22])0x005fae0cu)
#define s_audio_wavs_flood4_wav_005fadf4 (*(char (*)[22])0x005fadf4u)
#define s_audio_wavs_flood5_wav_005faddc (*(char (*)[22])0x005faddcu)
#define s_audio_wavs_flood6_wav_005fadc4 (*(char (*)[22])0x005fadc4u)
#define s_audio_wavs_flood7_wav_005fadac (*(char (*)[22])0x005fadacu)
#define s_audio_wavs_flood_r_wav_005fae54 (*(char (*)[23])0x005fae54u)
#define s_audio_wavs_forest1_wav_005fad7c (*(char (*)[23])0x005fad7cu)
#define s_audio_wavs_forest2_wav_005fad64 (*(char (*)[23])0x005fad64u)
#define s_audio_wavs_forest3_wav_005fad4c (*(char (*)[23])0x005fad4cu)
#define s_audio_wavs_forest4_wav_005fad34 (*(char (*)[23])0x005fad34u)
#define s_audio_wavs_forest5_wav_005fad1c (*(char (*)[23])0x005fad1cu)
#define s_audio_wavs_forest6_wav_005fad04 (*(char (*)[23])0x005fad04u)
#define s_audio_wavs_forest7_wav_005facec (*(char (*)[23])0x005facecu)
#define s_audio_wavs_forest_r_wav_005fad94 (*(char (*)[24])0x005fad94u)
#define s_audio_wavs_rock1_wav_005fafbc (*(char (*)[21])0x005fafbcu)
#define s_audio_wavs_rock2_wav_005fafa4 (*(char (*)[21])0x005fafa4u)
#define s_audio_wavs_rock3_wav_005faf8c (*(char (*)[21])0x005faf8cu)
#define s_audio_wavs_rock4_wav_005faf74 (*(char (*)[21])0x005faf74u)
#define s_audio_wavs_rock5_wav_005faf5c (*(char (*)[21])0x005faf5cu)
#define s_audio_wavs_rock6_wav_005faf44 (*(char (*)[21])0x005faf44u)
#define s_audio_wavs_rock7_wav_005faf2c (*(char (*)[21])0x005faf2cu)
#define s_audio_wavs_rock_r_wav_005fafd4 (*(char (*)[22])0x005fafd4u)
#define s_audio_wavs_water1_wav_005fb07c (*(char (*)[22])0x005fb07cu)
#define s_audio_wavs_water2_wav_005fb064 (*(char (*)[22])0x005fb064u)
#define s_audio_wavs_water3_wav_005fb04c (*(char (*)[22])0x005fb04cu)
#define s_audio_wavs_water4_wav_005fb034 (*(char (*)[22])0x005fb034u)
#define s_audio_wavs_water5_wav_005fb01c (*(char (*)[22])0x005fb01cu)
#define s_audio_wavs_water6_wav_005fb004 (*(char (*)[22])0x005fb004u)
#define s_audio_wavs_water7_wav_005fafec (*(char (*)[22])0x005fafecu)
#define s_audio_wavs_water_r_wav_005fb094 (*(char (*)[23])0x005fb094u)
extern void FUN_0053bc80();

void FUN_0053bcf0(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  
  puVar2 = &DAT_00f54578;
  do {
    puVar3 = puVar2 + 0xc;
    puVar4 = puVar2 + -10;
    for (iVar1 = 10; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar4 = 0xffffffff;
      puVar4 = puVar4 + 1;
    }
    *puVar2 = 0xffffffff;
    puVar2[1] = 0;
    puVar2 = puVar3;
  } while ((int)puVar3 < 0xf54698);
  FUN_0053bc80(0,s_audio_wavs_desert_r_wav_005fb154,0);
  FUN_0053bc80(0,s_audio_wavs_desert1_wav_005fb13c,1);
  FUN_0053bc80(0,s_audio_wavs_desert2_wav_005fb124,1);
  FUN_0053bc80(0,s_audio_wavs_desert3_wav_005fb10c,1);
  FUN_0053bc80(0,s_audio_wavs_desert4_wav_005fb0f4,1);
  FUN_0053bc80(0,s_audio_wavs_desert5_wav_005fb0dc,1);
  FUN_0053bc80(0,s_audio_wavs_desert6_wav_005fb0c4,1);
  FUN_0053bc80(0,s_audio_wavs_desert7_wav_005fb0ac,1);
  FUN_0053bc80(1,s_audio_wavs_water_r_wav_005fb094,0);
  FUN_0053bc80(1,s_audio_wavs_water1_wav_005fb07c,1);
  FUN_0053bc80(1,s_audio_wavs_water2_wav_005fb064,1);
  FUN_0053bc80(1,s_audio_wavs_water3_wav_005fb04c,1);
  FUN_0053bc80(1,s_audio_wavs_water4_wav_005fb034,1);
  FUN_0053bc80(1,s_audio_wavs_water5_wav_005fb01c,1);
  FUN_0053bc80(1,s_audio_wavs_water6_wav_005fb004,1);
  FUN_0053bc80(1,s_audio_wavs_water7_wav_005fafec,1);
  FUN_0053bc80(2,s_audio_wavs_rock_r_wav_005fafd4,0);
  FUN_0053bc80(2,s_audio_wavs_rock1_wav_005fafbc,1);
  FUN_0053bc80(2,s_audio_wavs_rock2_wav_005fafa4,1);
  FUN_0053bc80(2,s_audio_wavs_rock3_wav_005faf8c,1);
  FUN_0053bc80(2,s_audio_wavs_rock4_wav_005faf74,1);
  FUN_0053bc80(2,s_audio_wavs_rock5_wav_005faf5c,1);
  FUN_0053bc80(2,s_audio_wavs_rock6_wav_005faf44,1);
  FUN_0053bc80(2,s_audio_wavs_rock7_wav_005faf2c,1);
  FUN_0053bc80(3,s_audio_wavs_farm_r_wav_005faf14,0);
  FUN_0053bc80(3,s_audio_wavs_farm1_wav_005faefc,1);
  FUN_0053bc80(3,s_audio_wavs_farm2_wav_005faee4,1);
  FUN_0053bc80(3,s_audio_wavs_farm3_wav_005faecc,1);
  FUN_0053bc80(3,s_audio_wavs_farm4_wav_005faeb4,1);
  FUN_0053bc80(3,s_audio_wavs_farm5_wav_005fae9c,1);
  FUN_0053bc80(3,s_audio_wavs_farm6_wav_005fae84,1);
  FUN_0053bc80(3,s_audio_wavs_farm7_wav_005fae6c,1);
  FUN_0053bc80(4,s_audio_wavs_flood_r_wav_005fae54,0);
  FUN_0053bc80(4,s_audio_wavs_flood1_wav_005fae3c,1);
  FUN_0053bc80(4,s_audio_wavs_flood2_wav_005fae24,1);
  FUN_0053bc80(4,s_audio_wavs_flood3_wav_005fae0c,1);
  FUN_0053bc80(4,s_audio_wavs_flood4_wav_005fadf4,1);
  FUN_0053bc80(4,s_audio_wavs_flood5_wav_005faddc,1);
  FUN_0053bc80(4,s_audio_wavs_flood6_wav_005fadc4,1);
  FUN_0053bc80(4,s_audio_wavs_flood7_wav_005fadac,1);
  FUN_0053bc80(5,s_audio_wavs_forest_r_wav_005fad94,0);
  FUN_0053bc80(5,s_audio_wavs_forest1_wav_005fad7c,1);
  FUN_0053bc80(5,s_audio_wavs_forest2_wav_005fad64,1);
  FUN_0053bc80(5,s_audio_wavs_forest3_wav_005fad4c,1);
  FUN_0053bc80(5,s_audio_wavs_forest4_wav_005fad34,1);
  FUN_0053bc80(5,s_audio_wavs_forest5_wav_005fad1c,1);
  FUN_0053bc80(5,s_audio_wavs_forest6_wav_005fad04,1);
  FUN_0053bc80(5,s_audio_wavs_forest7_wav_005facec,1);
  return;
}
