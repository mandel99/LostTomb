#ifndef RESTORE_BUILDING_WORLD_FRAMES_H
#define RESTORE_BUILDING_WORLD_FRAMES_H
#include "world_base_model.h"
/* Explicit owned raw entry: pass the actual register/flags state, including
   native ESP and its complete backing caller frames. No local stack factory,
   initialization, image limit, or synthesized caller frame is provided.
   Whole parents preserve their real local writes, saved registers, reused
   arrays, arguments and original return tokens. Unknown entries fail rather
   than execute old legacy C. This module does not establish initial OS-stack
   bytes, nor pixel/device or undefined-flags fidelity of typed deep callees.
   Typed void calls expose no incidental volatile-register result; native
   caller continuations must discard those outputs before using this API.
   Included higher producers are430D30/430E60/4DDDC0/4FA330. For the actual
   eight-ID/at-most64-icon producer domain, food reads can reach entry4DDDC0
   ESP+291 or entry4FA330 ESP+275. These are inherited caller bytes, never
   allocation sizes or permission to initialize/pad them. Existing typed main
   callers must not be globally redirected here without their real frames. */
void restore_building_world_raw(uint32_t entry,uint32_t registers[8],uint32_t *flags);
void restore_building_world_dispatch(uint32_t site,uint32_t target,struct restore_building_world_frame *);
#endif
