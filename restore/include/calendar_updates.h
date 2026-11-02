#ifndef RESTORE_CALENDAR_UPDATES_H
#define RESTORE_CALENDAR_UPDATES_H
#include <stdint.h>
#include "financial_updates.h"
struct restore_calendar_frame;
typedef void (*restore_calendar_call)(uint32_t,uint32_t,struct restore_calendar_frame *);
struct restore_calendar_frame {uint32_t r[8],flags;restore_calendar_call boundary;};
void restore_calendar_C_004599d0(struct restore_calendar_frame *);
void restore_calendar_C_0045a350(struct restore_calendar_frame *);
void restore_calendar_C_0045a5d0(struct restore_calendar_frame *);
void FUN_00452370(void);
void FUN_00452160(void);
void FUN_00452200(void);
void FUN_004be640(void);
void FUN_00459e50(void);
void FUN_0045a2e0(void);
void FUN_0045a3f0(void);
void FUN_0045a450(void);
void FUN_0045a490(void);
void FUN_0045a4d0(void);
void FUN_0045a500(void);
void FUN_0045a530(void);
void FUN_0045a560(void);
void FUN_0045a5a0(void);
void restore_calendar_C_004e81d0(struct restore_calendar_frame *);
void restore_calendar_C_004e8840(struct restore_calendar_frame *);
void restore_calendar_C_004e8db0(struct restore_calendar_frame *);
void restore_calendar_C_00430060(struct restore_calendar_frame *);
void restore_calendar_C_00430040(struct restore_calendar_frame *);
void restore_calendar_C_00430250(struct restore_calendar_frame *);
void restore_calendar_C_004302b0(struct restore_calendar_frame *);
void restore_calendar_C_00458f80(struct restore_calendar_frame *);
void restore_calendar_C_004e8ec0(struct restore_calendar_frame *);
void restore_calendar_C_0042fb30(struct restore_calendar_frame *);
void restore_calendar_C_0042fbb0(struct restore_calendar_frame *);
void restore_calendar_C_004e90f0(struct restore_calendar_frame *);
void restore_calendar_C_004e9400(struct restore_calendar_frame *);
void restore_calendar_C_004e7700(struct restore_calendar_frame *);
void restore_calendar_C_004bf920(struct restore_calendar_frame *);
void restore_calendar_C_004b03b0(struct restore_calendar_frame *);
#endif
