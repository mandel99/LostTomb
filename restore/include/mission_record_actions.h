#ifndef RESTORE_MISSION_RECORD_ACTIONS_H
#define RESTORE_MISSION_RECORD_ACTIONS_H
#include <stdint.h>
struct restore_mission_record_frame;
typedef void (*restore_mission_record_call)(uint32_t,uint32_t,struct restore_mission_record_frame *);
struct restore_mission_record_frame {uint32_t r[8],flags;restore_mission_record_call boundary;};
void restore_mission_record_C_00418bf0(struct restore_mission_record_frame *);
void restore_mission_record_C_004e1770(struct restore_mission_record_frame *);
void restore_mission_record_C_004e1830(struct restore_mission_record_frame *);
void restore_mission_record_C_004df2c0(struct restore_mission_record_frame *);
void restore_mission_record_C_0040d7f0(struct restore_mission_record_frame *);
void restore_mission_record_C_004e65a0(struct restore_mission_record_frame *);
void restore_mission_record_C_00547ad0(struct restore_mission_record_frame *);
void restore_mission_record_C_00545f70(struct restore_mission_record_frame *);
void restore_mission_record_C_00559470(struct restore_mission_record_frame *);
void restore_mission_record_C_005567c0(struct restore_mission_record_frame *);
void FUN_00418bf0(void);
void FUN_004e1770(void);
void FUN_004e1830(void);
void restore_mission_record_C_00481f20(struct restore_mission_record_frame *);
void FUN_00481f20(void);
#endif
