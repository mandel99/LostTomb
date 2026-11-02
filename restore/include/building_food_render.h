#ifndef RESTORE_BUILDING_FOOD_RENDER_H
#define RESTORE_BUILDING_FOOD_RENDER_H
#include <stdint.h>
/* This is an explicit native-stack context, never the host compiler's frame.
   sp is the current native ESP. Whole-function entries expect their real
   return token at[sp] and arguments already present above it. Nonvolatiles
   are the actual incoming register values. The owned entry returns with
   sp+=4 and restores them from their
   native saved slots. ECX/EDX/flags are not declared return values here. */
typedef struct RestoreBuildingFoodFrame {
    uint32_t sp,ebx,ebp,esi,edi;
} RestoreBuildingFoodFrame;

/* Whole442280,130 instructions, cdecl4/RET0. Args atS+4,+8,+12,+16 are
   building,tile INDEX,x,y. Returns full native EAX, whose AL alone is set1.
   Caller must supply actual mapped stack prestate; no invented extra offsets.
   Eight own pairs occupyS-40h..S-1. Pair8(zero based) reads[return,outer-index]
   atS/S+4; subsequent pairs read overwritten arguments and genuine caller
   frames. Actual producer's eight distinct IDs, each at most8 icons, can read
   throughS+1BFh; no8/15/64 runtime clamp is introduced. Owned stack stores
   extend down toS-8Ch for the final sprite CALL's token. Actual typed callees
   use their own host stacks; their inactive deeper scratch is not synthesized.
   There is intentionally no global/default physical-stack wrapper. */
uint32_t restore_building_food_00442280(RestoreBuildingFoodFrame *frame);

/* Whole435E70,171 instructions, cdecl3/RET0 over the explicit parent frameP.
   Composes genuine food core atS=P-44 with return token436026. The actual
   higher frame atP and above remains caller supplied. No claim that an
   ordinary typed C call can reconstruct that prestate. Native incidental
   EAX/ECX/EDX/flags of this void parent are not exposed as outputs. */
void restore_building_status_00435e70_frame(RestoreBuildingFoodFrame *frame);

/* Exact call-site adapters, NOT replacements for the two whole parents.
   frame contains the actual LIVE nonvolatiles immediately before the first
   PUSH, with sp=P-112 for436B60 orP-200 for439F10. EAX/ECX inputs are their
   actual live values at that point; no physical host registers are inferred.
   The adapter performs the four native pushes, CALL/RET0 and ADDESP16, leaving
   sp at its incoming value. Full EAX result is returned, though these three
   caller continuations discard it. The rest of each parent frame is supplied
   by its whole parent producer, never initialized/padded by these adapters. */
uint32_t restore_building_food_call_004376eb(RestoreBuildingFoodFrame *,uint32_t eax,uint32_t ecx);
uint32_t restore_building_food_call_00437de5(RestoreBuildingFoodFrame *,uint32_t eax,uint32_t ecx);
uint32_t restore_building_food_call_0043b468(RestoreBuildingFoodFrame *,uint32_t eax);

/* Remaining caller obligations, not substituted implementations:
   436B60: childS=P-132, tokens4376F0/437DEA; 439F10: S=P-220,43B46D.
   Their whole native locals/NVs/argument mutations and inherited higher
   frames must be retained before binding them to this core. Existing typed
   C parent locals cannot stand in for these raw slots. */
#endif
