#ifndef RESTORE_ACTOR_PANEL_NATIVE_CONTEXT_H
#define RESTORE_ACTOR_PANEL_NATIVE_CONTEXT_H
#include <stdint.h>
/* Compose the already-translated whole 40A450 with its native entrySP-4
   prestate. 40A550 is a tail JMP to the same body. This argument is an
   explicit emulated stack value, not an added original-game parameter.

   In the known 4D8310 mode7/table5C4CD0 or5C4F00 call paths, the last
   writer is425B8C PUSH ESI from the preceding425990(5C7B80,1):5C7B80.
   If the subsequent4D6740 actually executes its E94D98==2 draw branch,
   4D6850 replaces it with wrapped(panelX+signedWORD hoveredButton.left),
   using that call's nonzero hit index, not an assumed callback row.
   536C30 does not overwrite that second stack argument.

   Supply the proven caller value; do not substitute ambient host scratch.
   Other callers, including externally changed layout/debug state, require
   their own native context. No default seed or unknown-token fallback. */
void restore_actor_panel_with_native_outslot(uint32_t native_entry_minus4);

/* Scoped metadata only, never a replacement stack or a default native value.
   Capture actual last writers in the authentic 4D8310 callback paths. */
typedef struct RestoreActorPanelNativeScope {
    uint32_t word,valid,hover,buttons;
} RestoreActorPanelNativeScope;
RestoreActorPanelNativeScope restore_actor_panel_native_enter(void);
void restore_actor_panel_native_leave(RestoreActorPanelNativeScope previous);
void restore_actor_panel_native_small(uint32_t site,uint32_t table,int32_t count);
uint32_t restore_actor_panel_native_hover_begin(uint32_t site,uint32_t table,uint32_t count);
void restore_actor_panel_native_hover_end(uint32_t previous);
void restore_actor_panel_native_highlight(uint32_t x);
uint32_t restore_actor_panel_native_buttons_begin(uint32_t site,uint32_t table,uint32_t count);
void restore_actor_panel_native_buttons_end(uint32_t previous);
int restore_actor_panel_native_action(uint32_t token);
#endif
