#ifndef RESTORE_TOOLTIP_CONTROLLER_H
#define RESTORE_TOOLTIP_CONTROLLER_H
#include <stdint.h>
struct restore_tooltip_frame;
typedef void (*restore_tooltip_call)(uint32_t,uint32_t,struct restore_tooltip_frame *);
struct restore_tooltip_frame {uint32_t r[8],flags;restore_tooltip_call boundary;};
void restore_tooltip_dispatch(uint32_t,uint32_t,struct restore_tooltip_frame *);
void restore_tooltip_C_004c8750(struct restore_tooltip_frame *);
void restore_tooltip_C_004ca930(struct restore_tooltip_frame *);
void restore_tooltip_C_004ca980(struct restore_tooltip_frame *);
void restore_tooltip_C_004cabd0(struct restore_tooltip_frame *);
void restore_tooltip_C_004caf10(struct restore_tooltip_frame *);
void restore_tooltip_C_004c92a0(struct restore_tooltip_frame *);
void restore_tooltip_C_004c9340(struct restore_tooltip_frame *);
void restore_tooltip_C_004c95d0(struct restore_tooltip_frame *);
void restore_tooltip_C_004ca9a0(struct restore_tooltip_frame *);
void restore_tooltip_C_004cb0b0(struct restore_tooltip_frame *);
void restore_tooltip_C_004cb300(struct restore_tooltip_frame *);
uint32_t FUN_004c8750(void);
uint32_t FUN_004ca930(void);
void FUN_004ca980(void);
void FUN_004cabd0(void);
void FUN_004caf10(void);
void FUN_004ca9a0(void);
void FUN_004cb0b0(void);
void FUN_004cb300(void);
int32_t FUN_004c92a0(void);
int32_t FUN_004c9340(uint8_t *expanded);
int32_t FUN_004c95d0(uint8_t *expanded);
#endif
