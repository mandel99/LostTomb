#ifndef RESTORE_BUILDING_REFRESH_HELPERS_H
#define RESTORE_BUILDING_REFRESH_HELPERS_H
#include <stdint.h>
/* Explicit native register frame. Only 488E60/488C20 consume the incoming
 * member ECX (forwarded into the genuine query closure). Native RET cleanup:
 * 488E60=8,488C20=16,488690/488840=24,4894D0=4; all other owned bodies=0.
 * The in-place raw entry advances SP by 4+cleanup, exactly once.
 * All internal callbacks are genuine C providers. 4E57F0 returns AL=1;
 * its incidental upper EAX from void widget refresh is deliberately not an API.
 * Numeric values use accepted MRFloat/raw80: masked exceptions, PC24/53/64,
 * all RC modes. FPSW, exception traps and renderer/image fidelity are unproved. */
struct restore_building_refresh_frame;
typedef void (*restore_building_refresh_call)(uint32_t,uint32_t,struct restore_building_refresh_frame *);
struct restore_building_refresh_frame {uint32_t r[8],flags;restore_building_refresh_call boundary;};
void restore_building_refresh_dispatch(uint32_t site,uint32_t target,struct restore_building_refresh_frame *f);
/* In-place raw bridge for owned typed-frame callers. No physical ECX sampling. */
void restore_building_refresh_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
void restore_building_refresh_C_00488690(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_00488840(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_00488c20(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_00488e60(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004894d0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004df9d0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004dfbb0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004e57f0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004e5840(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004edf50(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004ee4a0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004ee6c0(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004ee780(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004f3c90(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004f4160(struct restore_building_refresh_frame *f);
void restore_building_refresh_C_004f5f10(struct restore_building_refresh_frame *f);
#endif
