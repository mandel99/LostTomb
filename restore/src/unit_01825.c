#include "restore_types.h"
#include "city_numeric_confirm.h"
#include "game_diagnostics.h"
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004d0a40(uint8_t value);
extern void FUN_004ce2d0(void);
extern void FUN_004771c0(void);
extern int32_t FUN_00568eab(const char *filename,int32_t flags,int32_t permissions);
extern uint32_t FUN_0055bc50(int32_t file,uint32_t source,uint32_t bytes);
extern int32_t FUN_005664d5(int32_t file,uint32_t source,uint32_t bytes);
extern int32_t FUN_005647eb(int32_t file);
extern void FUN_004e2970();

/* Whole 4D4E20: native save descriptors and ordered write/compression calls.
 * Route rebuilding shares the actual receiver-aware generator and endpoints
 * with city editing, including the existing sample-capacity repair. */
uint32_t FUN_004d4e20(const char *filename)
{
    uint32_t descriptor,bytes;
    int32_t file;
    restore_game_trace("save begin path=%s\n",filename);
    FUN_004d0a40(0);
    U(0xe945c0u)=0xa0;
    strcpy((char *)(uintptr_t)0x5e17c4u,(const char *)(uintptr_t)0x5e17ccu);
    strcpy((char *)(uintptr_t)0xea184cu,(const char *)(uintptr_t)0xe38e74u);
    FUN_004ce2d0();
    FUN_004771c0();
    restore_game_trace("save prepared history and terrain\n");
    B(0x7770f1u)=0;
    U(0x7770e4u)=0;
    restore_city_numeric_free_samples();
    file=FUN_00568eab(filename,0x8301,0x180);
    if(file==-1)return 1;
    memset((void *)(uintptr_t)0xe92e50u,0,0x1770u);
    memcpy((void *)(uintptr_t)0xe92e50u,(const void *)(uintptr_t)0x5e09a0u,0xe24u);
    U(0xe945c4u)=0xb5;
    for(descriptor=0x5e09acu;descriptor<0x5e17d0u;descriptor+=20u){
        bytes=U(descriptor)*U(descriptor-4u);
        if(U(descriptor-12u))FUN_0055bc50(file,U(descriptor-8u),bytes);
        else FUN_005664d5(file,U(descriptor-8u),bytes);
    }
    FUN_005647eb(file);
    restore_game_trace("save closed descriptors=181\n");
    FUN_004e2970(0xd4,0,0);
    restore_game_trace("save notification published\n");
    restore_city_numeric_rebuild_routes();
    restore_game_trace("save route rebuild complete\n");
    restore_game_trace_save_complete();
    return 0;
}
