#include "restore_types.h"

/* Original import jump has DirectDrawCreate's three-argument stdcall ABI. */
HRESULT __stdcall FUN_0055c0e0(GUID *guid, LPDIRECTDRAW *drawing, IUnknown *outer)
{
    return DirectDrawCreate(guid, drawing, outer);
}
