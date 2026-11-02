#ifndef RESTORE_CITY_NUMERIC_CONFIRM_H
#define RESTORE_CITY_NUMERIC_CONFIRM_H
#include <stdint.h>
/* Native4-stack-byte/cdeclRET0, without incoming ECX. */
void restore_city_numeric_00444270(int32_t city_index);
void restore_city_numeric_004444e0(void);
/* Shared save/editor route preparation and reconstruction. */
void restore_city_numeric_free_samples(void);
void restore_city_numeric_rebuild_routes(void);
/* Native ECX receiver +3DWORDs/RET12, normalized to explicit C receiver.
   from/to address valid signed-WORD vertices; sample storage is a real,
   disjoint allocation covering all generated records. No bounds clamp.
   Inherited PC/RC and masked x87 exceptions; FPSW/traps are not claimed. */
int32_t restore_city_numeric_00445d10(uint32_t self,int32_t from,int32_t to,uint32_t first_sample);
/* ROUTECAPACITYREPAIR: the canonical rebuild reserves max(native capacity,
   required samples), using the genuine planner and ORIGINAL logical need as
   stop_after. Vertex/math/output order and generator early stop are unchanged.
   Accepted domain: stable count<=50, signed-WORD vertices and masked x87
   arithmetic. If planning fails outside that domain the original allocation
   policy remains; no invented return, clamp, padding or OOM guard is added.
   Repaired underallocations, heap addresses and invalid states are not claimed
   bit-identical to native; full file/UI fidelity remains qualified. */
#endif
