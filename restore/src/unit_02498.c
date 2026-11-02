#include "restore_types.h"
#include "sound_streams.h"
#define DAT_005f9058 (*(undefined *)0x005f9058u)
#define DAT_00e38ec0 (*(char *)0x00e38ec0u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55b50 (*(undefined4 *)0x00f55b50u)
#define DAT_00f55b80 (*(undefined *)0x00f55b80u)
#define DAT_00f55b84 (*(undefined *)0x00f55b84u)
#define DAT_00f55b88 (*(undefined *)0x00f55b88u)
#define _DAT_00f55b58 (*(undefined4 *)0x00f55b58u)
#define s_audio_ambient_desert_mp3_005facd0 (*(char (*)[25])0x005facd0u)
#define s_audio_ambient_floodplain_mp3_005fac94 (*(char (*)[29])0x005fac94u)
#define s_audio_ambient_housing1_mp3_005fac08 (*(char (*)[27])0x005fac08u)
#define s_audio_ambient_housing2_mp3_005fabec (*(char (*)[27])0x005fabecu)
#define s_audio_ambient_housing3_mp3_005fabd0 (*(char (*)[27])0x005fabd0u)
#define s_audio_ambient_housing4_mp3_005fabb4 (*(char (*)[27])0x005fabb4u)
#define s_audio_ambient_housing5_mp3_005fab98 (*(char (*)[27])0x005fab98u)
#define s_audio_ambient_industrial_mp3_005fac24 (*(char (*)[29])0x005fac24u)
#define s_audio_ambient_meadow_mp3_005facb4 (*(char (*)[25])0x005facb4u)
#define s_audio_ambient_monument_b_mp3_005fab78 (*(char (*)[29])0x005fab78u)
#define s_audio_ambient_monument_f_mp3_005fab58 (*(char (*)[29])0x005fab58u)
#define s_audio_ambient_rock_mp3_005fac7c (*(char (*)[23])0x005fac7cu)
#define s_audio_ambient_vegetation_mp3_005fac5c (*(char (*)[29])0x005fac5cu)
#define s_audio_ambient_water_mp3_005fac44 (*(char (*)[24])0x005fac44u)
extern void FUN_0053b790();

void FUN_0053b960(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  
  if ((DAT_00f55b88 != 0) && (DAT_00e38ec0 != '\0')) {
    iVar3 = 0;
    iVar4 = -1;
    iVar2 = 0;
    piVar1 = &DAT_00f55b50;
    do {
      if (iVar3 < *piVar1) {
        iVar3 = *piVar1;
        iVar4 = iVar2;
      }
      piVar1 = piVar1 + 1;
      iVar2 = iVar2 + 1;
    } while ((int)piVar1 < 0xf55b80);
    if ((iVar4 != DAT_00f55b80) || (iVar3 = FUN_0055f5a1(RESTORE_SOUND,2), iVar3 == 0)) {
      DAT_00f55b80 = iVar4;
      switch((int)(iVar4)) {
      case 1: ;
        iVar3 = *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
        if (iVar3 < 500) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_housing1_mp3_005fac08,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        if (iVar3 < 2000) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_housing2_mp3_005fabec,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        if (iVar3 < 5000) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_housing3_mp3_005fabd0,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        if (7999 < iVar3) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_housing5_mp3_005fab98,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_housing4_mp3_005fabb4,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 2: ;
        pcVar5 = s_audio_ambient_industrial_mp3_005fac24;
        if ((double)_DAT_00f55b58 <= (double)DAT_00f55b84 * 0.33) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_industrial_mp3_005fac24,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        break;
      case 3: ;
      case 7: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_meadow_mp3_005facb4,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 4: ;
        if (999 < DAT_005f9058) {
          FUN_0055f449(RESTORE_SOUND,s_audio_ambient_monument_f_mp3_005fab58,2,0);
          FUN_0055f639(RESTORE_SOUND,2,0);
          return;
        }
        pcVar5 = s_audio_ambient_monument_b_mp3_005fab78;
        break;
default: ;
        FUN_0055f561(RESTORE_SOUND,2);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 6: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_desert_mp3_005facd0,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 8: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_floodplain_mp3_005fac94,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 9: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_rock_mp3_005fac7c,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 10: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_water_mp3_005fac44,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      case 0xb: ;
        FUN_0055f449(RESTORE_SOUND,s_audio_ambient_vegetation_mp3_005fac5c,2,0);
        FUN_0055f639(RESTORE_SOUND,2,0);
        return;
      }
      FUN_0055f449(RESTORE_SOUND,pcVar5,2,0);
      FUN_0055f639(RESTORE_SOUND,2,0);
      return;
    }
    iVar3 = FUN_0055f5f4(RESTORE_SOUND,2);
    if (iVar3 < 100) {
      FUN_0053b790(2);
      return;
    }
  }
  return;
}
