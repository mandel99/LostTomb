#ifndef RESTORE_INVASION_MODEL_H
#define RESTORE_INVASION_MODEL_H
#include <stdint.h>
struct restore_invasion_frame;
typedef void (*restore_invasion_call)(uint32_t,uint32_t,struct restore_invasion_frame *);
struct restore_invasion_frame {uint32_t r[8],flags;restore_invasion_call boundary;};
void restore_invasion_C_00446a30(struct restore_invasion_frame *);
void restore_invasion_C_004467a0(struct restore_invasion_frame *);
void restore_invasion_C_00446650(struct restore_invasion_frame *);
void restore_invasion_C_004620c0(struct restore_invasion_frame *);
void restore_invasion_C_00469140(struct restore_invasion_frame *);
void restore_invasion_C_004b7510(struct restore_invasion_frame *);
void restore_invasion_C_00459ec0(struct restore_invasion_frame *);
void restore_invasion_C_004c03d0(struct restore_invasion_frame *);
void restore_invasion_C_004684e0(struct restore_invasion_frame *);
void restore_invasion_C_0048c0e0(struct restore_invasion_frame *);
void restore_invasion_C_00497b90(struct restore_invasion_frame *);
void restore_invasion_C_00480060(struct restore_invasion_frame *);
void restore_invasion_C_004800b0(struct restore_invasion_frame *);
#endif
