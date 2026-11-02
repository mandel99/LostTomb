#include "replay_mission.h"
#include "family_menu.h"
#include <string.h>
/* Preserve the existing legacy fopen C binding's pointer-sized DWORD arguments.
   Its result is the original32-byte FILE wrapper, never a host CRT FILE. */
extern undefined4 *FUN_00561a0a(undefined4 path,undefined4 mode);
extern uint32_t FUN_00561994(restore_legacy_FILE *stream);
extern void FUN_004cb6a0(void);
extern uint32_t FUN_004d51b0(uint32_t path);
extern void FUN_005405b0(char *name,char kind);
void RM_PUBLIC(FUN_00418510)(void)
{
    uint32_t kind=RM_U(0xe38ebc);
    char filename[100];
    restore_legacy_FILE *stream;
    RM_PUT32(0x6059e8,0);
    if(kind==0){
        if(RM_B(0x7027b4)!=1){RM_PUT8(0x6fdcb4,0);FUN_004181e0();return;}
        strcpy(filename,FUN_005169c0());
        strcat(filename,(const char *)0x5761d0u);
        strcat(filename,(const char *)0x57bfc4u);
        stream=(restore_legacy_FILE *)FUN_00561a0a((uint32_t)(uintptr_t)filename,0x5763bcu);
        if(stream==NULL){FUN_004cb6a0();return;}
        (void)FUN_00561994(stream);
        RM_PUT32(0x605a3c,0);RM_PUT8(0x6de515,0);
        (void)FUN_004d51b0((uint32_t)(uintptr_t)filename);
    }
    else{RM_PUT8(0x7027b4,0);FUN_005405b0((char *)0xe37078u,0);}
    RM_PUT8(0xe38dda,1);RM_PUT8(0xe38dca,1);
}
