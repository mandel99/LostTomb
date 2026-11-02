#ifndef RESTORE_BRIEFING_DRAW_H
#define RESTORE_BRIEFING_DRAW_H
#include <stdint.h>
struct restore_briefing_frame;
typedef void (*restore_briefing_call)(uint32_t,uint32_t,struct restore_briefing_frame *);
struct restore_briefing_frame {uint32_t r[8],flags;restore_briefing_call boundary;};
void restore_briefing_C_0041a180(struct restore_briefing_frame *);
void restore_briefing_C_00537000(struct restore_briefing_frame *);
void restore_briefing_C_004e2530(struct restore_briefing_frame *);
void restore_briefing_C_00536820(struct restore_briefing_frame *);
void FUN_0041a180(void);
uint8_t FUN_004e2530(void);
void FUN_004e2720(void);
int32_t FUN_00536fd0(int32_t style);
void FUN_00537000(int32_t style,int32_t x,int32_t y,int32_t columns,int32_t immediate,int32_t variant);
#endif
