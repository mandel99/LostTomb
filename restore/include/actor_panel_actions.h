#ifndef RESTORE_ACTOR_PANEL_ACTIONS_H
#define RESTORE_ACTOR_PANEL_ACTIONS_H
#include <stdint.h>
struct restore_actor_panel_frame;
typedef void (*restore_actor_panel_call)(uint32_t,uint32_t,struct restore_actor_panel_frame *);
struct restore_actor_panel_frame {uint32_t r[8],flags;restore_actor_panel_call boundary;};
void restore_actor_panel_C_0040a450(struct restore_actor_panel_frame *);
void restore_actor_panel_C_0040a560(struct restore_actor_panel_frame *);
void restore_actor_panel_C_0040a5c0(struct restore_actor_panel_frame *);
void restore_actor_panel_C_0040a600(struct restore_actor_panel_frame *);
void restore_actor_panel_C_004bc620(struct restore_actor_panel_frame *);
void restore_actor_panel_C_00521a60(struct restore_actor_panel_frame *);
void restore_actor_panel_C_00521aa0(struct restore_actor_panel_frame *);
void restore_actor_panel_C_004b8aa0(struct restore_actor_panel_frame *);
/* Complete native control flow with explicit caller-supplied stack/register
   prestate. In40A450, entrySP-4 is read even when44D520 returns without an
   output write. Preserve that slot across iterations. No gate proving its
   first-write initialization is established by this module. */
void restore_actor_panel_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
/* A550 convenience entry retains natural uninitialized scratch. Its first
   never-written output slot is not claimed bit-identical to an original
   caller stack; exact prestate composition must use the raw API above.
   A560/A5C0/A600 have no corresponding unwritten-output dependency. */
void restore_actor_panel_0040a450(void);
void restore_actor_panel_0040a550(void);
void restore_actor_panel_0040a560(void);
void restore_actor_panel_0040a5c0(void);
void restore_actor_panel_0040a600(void);
int restore_actor_panel_action(uint32_t token);
void restore_actor_panel_requests_00521a60(uint8_t *first,uint8_t *second);
#endif
