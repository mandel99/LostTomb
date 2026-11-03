#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "standalone_data.h"
#include "text_templates.h"
#include "flood_simulation.h"
#include "mission_schema.h"
#include "vegetation_farm_init.h"
#include "monument_constructor.h"
#include "actor_ring_queue.h"
#include "file_list.h"
#include "sprite_shutdown.h"
#ifdef RESTORE_GAME_LAUNCHER
#include "game_launcher.h"
#include "game_diagnostics.h"
#endif
#ifdef _DEBUG
#include <crtdbg.h>
#endif
extern unsigned int FUN_00413ed0(HINSTANCE instance);
int main(int argc,char **argv)
{
    int probe=argc==2&&!strcmp(argv[1],"--game-startup-probe");
#ifdef RESTORE_GAME_LAUNCHER
    if(argc==1)probe=1;
#endif
#ifdef _DEBUG
    int report;
    for(report=_CRT_WARN;report<=_CRT_ASSERT;++report){
        _CrtSetReportMode(report,_CRTDBG_MODE_FILE|_CRTDBG_MODE_DEBUG);
        _CrtSetReportFile(report,_CRTDBG_FILE_STDERR);
    }
#endif
    if(argc>1&&!probe) {fputs("Usage: restore_link_check [--game-startup-probe]\n",stderr);return 2;}
#ifdef RESTORE_GAME_LAUNCHER
    {
        int launch_code;
        if(restore_game_prepare_launch(&launch_code))return launch_code;
        restore_game_diagnostics_initialize();
    }
#endif
    if(!restore_data_open()) return 1;
    /* Register the arena owner first: later game destructors run before its
       pages become inaccessible. Every normal return below uses this order. */
    if(atexit(restore_data_close)!=0){restore_data_close();return 1;}
    /* Restored subset in the original576008..576078 CRT-table order. */
    (void)FUN_00402360();
    restore_monument_constructors_initialize();
    (void)FUN_0041e050();
    FUN_00486080();
    FUN_00489940();
    FUN_004bd310();
    FUN_004d1340();
    FUN_004d4a50();
    (void)FUN_004f64a0();
    (void)FUN_00516570();
    (void)FUN_0053da50();
    if(probe) {
        unsigned int result;
        puts("Explicit C game-startup probe: entering restored 00413ed0; use a bounded debugger and isolated asset directory.");
        fflush(stdout);
        restore_text_templates_initialize();
        FUN_004482f0();
        result=FUN_00413ed0(GetModuleHandleA(NULL));
#ifdef RESTORE_GAME_LAUNCHER
        restore_game_trace("game loop returned result=%u ready=%u\n",result,*(unsigned char *)0xe38dd6u);
#endif
        {
            unsigned int ready=*(unsigned char *)0x00e38dd6u;
            FUN_004cbd70((RestoreMessageCatalog *)(uintptr_t)0x784f20u);
            printf("Graphics state: game=%ux%u display=%ux%u fullscreen=%u depth=16 desktop-depth=%u.\n",
                   *(unsigned int *)0xe38db0u,*(unsigned int *)0xe38dacu,
                   *(unsigned int *)0xe38d9cu,*(unsigned int *)0xe38d98u,
                   *(unsigned int *)0xe38e5cu,*(unsigned int *)0xe38e58u);
            printf("Restored C startup returned %u; game-ready flag=%u. This alone does not establish playable behavior.\n",result,ready);
            if(!ready) return 3;
        }
        return (int)result;
    }
    puts("C-only whole-program link and data initialization check. This diagnostic does not start the game.");
    return 0;
}
