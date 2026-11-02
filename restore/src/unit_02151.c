#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define s_AbuSimbelDummy_005790f0 (*(char (*)[15])0x005790f0u)
#define s_AbuSimbel_005760b0 (*(char (*)[10])0x005760b0u)
#define s_BENT_PYRAMID_005f763c (*(char (*)[13])0x005f763cu)
#define s_Caesareum1_005f75a0 (*(char (*)[11])0x005f75a0u)
#define s_CaesareumDummy_00579134 (*(char (*)[15])0x00579134u)
#define s_LibExtraDummy_005f75f8 (*(char (*)[14])0x005f75f8u)
#define s_LibExtra_00576284 (*(char (*)[9])0x00576284u)
#define s_LibMainDummy_0057922c (*(char (*)[13])0x0057922cu)
#define s_LibMaina1_005762fc (*(char (*)[10])0x005762fcu)
#define s_LibMainb1_00576338 (*(char (*)[10])0x00576338u)
#define s_LibMainc1_005762b4 (*(char (*)[10])0x005762b4u)
#define s_LtHouse1_005e4934 (*(char (*)[9])0x005e4934u)
#define s_LtHouseDummy_00579124 (*(char (*)[13])0x00579124u)
#define s_MASTABA_005f7610 (*(char (*)[8])0x005f7610u)
#define s_MUDBRICK_PYRAMID_005f7628 (*(char (*)[17])0x005f7628u)
#define s_MausCarryoverDummy_00579110 (*(char (*)[19])0x00579110u)
#define s_MausCarryover_005f7590 (*(char (*)[14])0x005f7590u)
#define s_PYRAMID_005f7608 (*(char (*)[8])0x005f7608u)
#define s_PyramidDummy_00579144 (*(char (*)[13])0x00579144u)
#define s_STEPPED_PYRAMID_005f7618 (*(char (*)[16])0x005f7618u)
#define s_TombRamDummy_005790c0 (*(char (*)[13])0x005790c0u)
#define s_TombRam_005f75ac (*(char (*)[8])0x005f75acu)
#define s_TombSetiDummy_005790d0 (*(char (*)[14])0x005790d0u)
#define s_TombSetiFin_005f75c0 (*(char (*)[12])0x005f75c0u)
#define s_TombSeti_005f75b4 (*(char (*)[9])0x005f75b4u)
#define s_TombThutDummy_00579100 (*(char (*)[14])0x00579100u)
#define s_TombThutFin_005f75ec (*(char (*)[12])0x005f75ecu)
#define s_TombThut_005f75e0 (*(char (*)[9])0x005f75e0u)
#define s_TombTutDummy_005790e0 (*(char (*)[13])0x005790e0u)
#define s_TombTutFin_005f75cc (*(char (*)[11])0x005f75ccu)
#define s_TombTut_005f75d8 (*(char (*)[8])0x005f75d8u)
extern int FUN_004f44e0();

void FUN_004f4520(void)

{
  char *pcVar1;
  undefined4 uVar2;
  
  if ((byte)DAT_00784cbc < 2) {
    RESTORE_SLICE1(DAT_00784cbc,0) = 1;
    DAT_005e4ef4 = FUN_004f44e0();
    switch((int)(DAT_005e4ef4)) {
    case 1: ;
      pcVar1 = s_BENT_PYRAMID_005f763c;
      break;
    case 2: ;
      pcVar1 = s_MUDBRICK_PYRAMID_005f7628;
      break;
    case 3: ;
      pcVar1 = s_STEPPED_PYRAMID_005f7618;
      break;
default: ;
      pcVar1 = s_PYRAMID_005f7608;
      break;
    case 5: ;
      pcVar1 = s_MASTABA_005f7610;
    }
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,pcVar1,0x11);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_CaesareumDummy_00579134,0x12);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LtHouseDummy_00579124,0x13);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x14);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x15);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x16);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibExtraDummy_005f75f8,0x17);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MausCarryoverDummy_00579110,0x18);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombThutDummy_00579100,0x19);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombTutDummy_005790e0,0x1b);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombSetiDummy_005790d0,0x1c);
    uVar2 = 0x1d;
    pcVar1 = s_TombRamDummy_005790c0;
  }
  else {
    if ((byte)DAT_00784cbc == 2) {
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_PyramidDummy_00579144,0x11);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_CaesareumDummy_00579134,0x12);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LtHouseDummy_00579124,0x13);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x14);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x15);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x16);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibExtraDummy_005f75f8,0x17);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MausCarryoverDummy_00579110,0x18);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_AbuSimbelDummy_005790f0,0x1a);
      if ((DAT_00e38ea8 < '\'') || (')' < DAT_00e38ea8)) {
        pcVar1 = s_TombThut_005f75e0;
      }
      else {
        pcVar1 = s_TombThutFin_005f75ec;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,pcVar1,0x19);
      if ((DAT_00e38ea8 == '(') || (DAT_00e38ea8 == ')')) {
        pcVar1 = s_TombTutFin_005f75cc;
      }
      else {
        pcVar1 = s_TombTut_005f75d8;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,pcVar1,0x1b);
      if (DAT_00e38ea8 == ')') {
        pcVar1 = s_TombSetiFin_005f75c0;
      }
      else {
        pcVar1 = s_TombSeti_005f75b4;
      }
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,pcVar1,0x1c);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombRam_005f75ac,0x1d);
      return;
    }
    if ((byte)DAT_00784cbc == 4) {
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_AbuSimbel_005760b0,0x1a);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_PyramidDummy_00579144,0x11);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_CaesareumDummy_00579134,0x12);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LtHouseDummy_00579124,0x13);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x14);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x15);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainDummy_0057922c,0x16);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibExtraDummy_005f75f8,0x17);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MausCarryoverDummy_00579110,0x18);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombThutDummy_00579100,0x19);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombSetiDummy_005790d0,0x1c);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombTutDummy_005790e0,0x1b);
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombRamDummy_005790c0,0x1d);
      return;
    }
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_PyramidDummy_00579144,0x11);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombThutDummy_00579100,0x19);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombTutDummy_005790e0,0x1b);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombSetiDummy_005790d0,0x1c);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_TombRamDummy_005790c0,0x1d);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_Caesareum1_005f75a0,0x12);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LtHouse1_005e4934,0x13);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMaina1_005762fc,0x14);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainb1_00576338,0x15);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibMainc1_005762b4,0x16);
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_LibExtra_00576284,0x17);
    uVar2 = 0x18;
    pcVar1 = s_MausCarryover_005f7590;
  }
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,pcVar1,uVar2);
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_AbuSimbelDummy_005790f0,0x1a);
  return;
}
