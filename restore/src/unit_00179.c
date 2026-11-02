#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_0057923c (*(undefined4 *)0x0057923cu)
#define DAT_00579244 (*(undefined4 *)0x00579244u)
#define DAT_00bca2b8 (*(undefined4 *)0x00bca2b8u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38dac (*(uint32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(uint32_t *)0x00e38db0u)
#define DAT_00e38e94 (*(uint32_t *)0x00e38e94u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define _DAT_005e06d0 (*(undefined4 *)0x005e06d0u)
#define s_AbuSimbelDummy_005790f0 (*(char (*)[15])0x005790f0u)
#define s_CaesareumDummy_00579134 (*(char (*)[15])0x00579134u)
#define s_Egyptian_0057916c (*(char (*)[9])0x0057916cu)
#define s_Hyksos_00579208 (*(char (*)[7])0x00579208u)
#define s_LibExtra_00576284 (*(char (*)[9])0x00576284u)
#define s_LibMainDummy_0057922c (*(char (*)[13])0x0057922cu)
#define s_LtHouseDummy_00579124 (*(char (*)[13])0x00579124u)
#define s_MausCarryoverDummy_00579110 (*(char (*)[19])0x00579110u)
#define s_OK__Setup_loaded_DAT_files__005790a4 (*(char (*)[28])0x005790a4u)
#define s_Pharaoh_Fonts_00579178 (*(char (*)[14])0x00579178u)
#define s_Pharaoh_General_005791f8 (*(char (*)[16])0x005791f8u)
#define s_Pharaoh_Terrain_005791e8 (*(char (*)[16])0x005791e8u)
#define s_Pharaoh_Unloaded_00579218 (*(char (*)[17])0x00579218u)
#define s_Pharaohmap_inf_0057924c (*(char (*)[15])0x0057924cu)
#define s_PyramidDummy_00579144 (*(char (*)[13])0x00579144u)
#define s_SPHINX1A_005791dc (*(char (*)[9])0x005791dcu)
#define s_SPHINX1B_005791d0 (*(char (*)[9])0x005791d0u)
#define s_SPHINX1C_005791c4 (*(char (*)[9])0x005791c4u)
#define s_SPHINX1D_005791b8 (*(char (*)[9])0x005791b8u)
#define s_SPHINX_EXTRA_005791a8 (*(char (*)[13])0x005791a8u)
#define s_SprAmbient_00579188 (*(char (*)[11])0x00579188u)
#define s_SprMain2_00579160 (*(char (*)[9])0x00579160u)
#define s_SprMain_00579210 (*(char (*)[8])0x00579210u)
#define s_TEMPLE_NILE_0057919c (*(char (*)[12])0x0057919cu)
#define s_TombRamDummy_005790c0 (*(char (*)[13])0x005790c0u)
#define s_TombSetiDummy_005790d0 (*(char (*)[14])0x005790d0u)
#define s_TombThutDummy_00579100 (*(char (*)[14])0x00579100u)
#define s_TombTutDummy_005790e0 (*(char (*)[13])0x005790e0u)
#define s_empire_00579194 (*(char (*)[7])0x00579194u)
#define s_expansion_00579154 (*(char (*)[10])0x00579154u)
extern void FUN_004cc110();
extern void FUN_004ce6e0();
extern undefined4 FUN_004ceb60();
extern void FUN_004cf7b0();
extern void FUN_004d0280();
extern void FUN_00520af0();
extern void FUN_00520b20();
extern void * FUN_00561b68();

undefined4 FUN_00414420(void)
{
    uint32_t bytes;
    const char *names[30]={
        s_Pharaoh_Unloaded_00579218,s_SprMain_00579210,s_Hyksos_00579208,s_Pharaoh_General_005791f8,
        s_Pharaoh_Terrain_005791e8,s_SPHINX1A_005791dc,s_SPHINX1B_005791d0,s_SPHINX1C_005791c4,
        s_SPHINX1D_005791b8,s_SPHINX_EXTRA_005791a8,s_TEMPLE_NILE_0057919c,s_empire_00579194,
        s_SprAmbient_00579188,s_Pharaoh_Fonts_00579178,s_Egyptian_0057916c,s_SprMain2_00579160,
        s_expansion_00579154,s_PyramidDummy_00579144,s_CaesareumDummy_00579134,s_LtHouseDummy_00579124,
        s_LibMainDummy_0057922c,s_LibMainDummy_0057922c,s_LibMainDummy_0057922c,s_LibExtra_00576284,
        s_MausCarryoverDummy_00579110,s_TombThutDummy_00579100,s_AbuSimbelDummy_005790f0,
        s_TombTutDummy_005790e0,s_TombSetiDummy_005790d0,s_TombRamDummy_005790c0};
    FUN_004ceb60(s_Pharaohmap_inf_0057924c,&DAT_00bca2b8,0);
    DAT_00e38e94=16;_DAT_005e06d0=1;
    CreateDirectoryA((char *)&DAT_00579244,NULL);CreateDirectoryA((char *)&DAT_0057923c,NULL);
    bytes=DAT_00e38dac*DAT_00e38db0*2u;
    if((int32_t)bytes<=1200000)bytes=1200000;
    DAT_00e92e1c=(uint32_t)(uintptr_t)FUN_00561b68(bytes);
    restore_sprite_libraries_initialize(RESTORE_SPRITE_LIBRARIES);
    FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);
    FUN_00520b20(0);FUN_004cc110(11,7);FUN_004cf7b0(DAT_00e20768);FUN_004d0280(1);FUN_004cf7b0(DAT_00e20768);
    if(!DAT_00e92e1c)return 1;
    FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);
    FUN_00520b20(1);FUN_004cc110(11,8);FUN_004cf7b0(DAT_00e20768);FUN_004d0280(1);FUN_004cf7b0(DAT_00e20768);
    FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);
    FUN_00520b20(2);FUN_004cc110(11,9);FUN_004cf7b0(DAT_00e20768);FUN_004d0280(1);FUN_004cf7b0(DAT_00e20768);
    FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);
    FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);FUN_0053dd10(RESTORE_SPRITE_LIBRARIES,names);
    FUN_00520b20(3);FUN_004cc110(11,10);FUN_004cf7b0(DAT_00e20768);FUN_004d0280(1);FUN_004cf7b0(DAT_00e20768);
    FUN_0053dd40(RESTORE_SPRITE_LIBRARIES,names);
    FUN_00520b20(3);FUN_004cc110(11,11);FUN_004cf7b0(DAT_00e20768);FUN_004d0280(1);FUN_004cf7b0(DAT_00e20768);
    FUN_00520af0();FUN_004d0280(1);FUN_004ce6e0(s_OK__Setup_loaded_DAT_files__005790a4,0,0);
    return 0;
}
