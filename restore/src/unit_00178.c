#include <windows.h>
#include <ddraw.h>
#include <stdint.h>
#include "startup_trace.h"
extern void FUN_004cff90(void);
extern uint32_t FUN_004cffe0(void);
extern void FUN_004d6b60(void);

uint32_t FUN_004143c0(void)
{
    static unsigned int last_mode=UINT32_MAX;
    unsigned int mode=*(unsigned char *)0x00e38ddau;
    if(mode!=last_mode) {
        last_mode=mode;
        restore_startup_trace("STARTUP frame mode=%u redraw=%u speed=%lu\n",mode,*(unsigned char *)0x00e38dcau,*(uint32_t *)0x00e38e6cu);
    }
    LPDIRECTDRAWSURFACE surface=*(LPDIRECTDRAWSURFACE *)0x00e39178u;
    int lost=surface && surface->lpVtbl->IsLost(surface)==DDERR_SURFACELOST;
    if(!lost) {
        surface=*(LPDIRECTDRAWSURFACE *)0x00e39174u;
        lost=surface && surface->lpVtbl->IsLost(surface)==DDERR_SURFACELOST;
    }
    if(lost && !FUN_004cffe0()) {
        FUN_004cff90();
        return 0;
    }
    if(*(unsigned char *)0x00d3b43du==1) return 0;
    FUN_004d6b60();
    return 1;
}
