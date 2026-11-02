#include "restore_types.h"
extern void FUN_004ce6e0(char *message,int detail,int number);
undefined4 FUN_004cffe0(void)
{
    LPDIRECTDRAWSURFACE front=*(LPDIRECTDRAWSURFACE *)0x00e39178u;
    LPDIRECTDRAWSURFACE back;
    if(!front || front->lpVtbl->Restore(front)!=DD_OK) {
        FUN_004ce6e0((char *)0x005e0774u,0,0);
        return 0;
    }
    /* The front Restore may replace the back surface; load it afterwards. */
    back=*(LPDIRECTDRAWSURFACE *)0x00e39174u;
    if(!back || back->lpVtbl->Restore(back)!=DD_OK) {
        FUN_004ce6e0((char *)0x005e0798u,0,0);
        return 0;
    }
    return 1;
}
