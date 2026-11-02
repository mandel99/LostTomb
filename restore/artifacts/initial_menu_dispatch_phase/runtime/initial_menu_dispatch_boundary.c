#include "initial_menu_dispatch.h"
extern void FUN_0055ad30(void);
/* The original caller pushes a context DWORD. The existing reconstructed
   renderer exposes no parameter. Keep that native context at a named typed
   boundary rather than declaring the existing zero-argument C body wrongly.
   The renderer and any native use of this context remain separate obligations. */
void restore_initial_dispatch_training(uint32_t native_context)
{
    (void)native_context;
    FUN_0055ad30();
}
