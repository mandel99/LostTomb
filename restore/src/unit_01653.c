#include "restore_types.h"
#include "sound_streams.h"
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00d3b188 (*(undefined1 *)0x00d3b188u)
#define DAT_00d3b18a (*(undefined1 *)0x00d3b18au)
#define DAT_00d3b18b (*(undefined1 *)0x00d3b18bu)
#define DAT_00e38e64 (*(char *)0x00e38e64u)
#define DAT_00f55b88 (*(int32_t *)0x00f55b88u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define PTR_DAT_005e0408 (*(unsigned char * *)0x005e0408u)
#define s_audio_music_setup_mp3_005761ac (*(char (*)[22])0x005761acu)
extern void FUN_004cd1a0();
extern void FUN_004cd240();

void FUN_004cb5a0(void)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  bool bVar4;
  undefined **ppuVar5;
  
  FUN_0055f561(RESTORE_SOUND,1);
  if (((DAT_00f55b88 != 0) && (iVar3 = FUN_0055f5a1(RESTORE_SOUND,0), iVar3 == 0)) && (DAT_00e38e64 != '\0')) {
    FUN_0055f449(RESTORE_SOUND,s_audio_music_setup_mp3_005761ac,0,0);
  }
  uVar2 = DAT_00f56f8c;
  iVar3 = 0;
  do {
    if (iVar3 == 0) {
      if (DAT_00d3b18b == '\0') {
        bVar4 = DAT_007056c8 == 1;
        FUN_004cd240(0x125,7,&PTR_DAT_005e0408,uVar2,0,4,7);
        ppuVar5 = &PTR_DAT_005e0408;
      }
      else {
LAB_004cb649: ;
        bVar4 = DAT_007056c8 == iVar3 + 1;
        ppuVar5 = &PTR_DAT_005e0408 + iVar3 * 7;
        FUN_004cd240(0x125,iVar3,ppuVar5,uVar2,0,4,7);
      }
      FUN_004cd1a0(0,ppuVar5,bVar4,0,0);
    }
    else {
      cVar1 = DAT_00d3b188;
      if (((iVar3 != 1) && (cVar1 = DAT_00d3b18a, iVar3 != 2)) || (cVar1 != '\0'))
      goto LAB_004cb649;
    }
    iVar3 = iVar3 + 1;
    if (4 < iVar3) {
      return;
    }
  } while( true );
}
