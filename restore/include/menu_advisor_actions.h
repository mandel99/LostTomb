#ifndef RESTORE_MENU_ADVISOR_ACTIONS_H
#define RESTORE_MENU_ADVISOR_ACTIONS_H
#include <stdint.h>
struct restore_advisor_frame;
typedef void (*restore_advisor_call)(uint32_t,uint32_t,struct restore_advisor_frame *);
struct restore_advisor_frame {uint32_t r[8],flags;restore_advisor_call boundary;};
void restore_advisor_C_0040b520(struct restore_advisor_frame *);
void restore_advisor_C_004179a0(struct restore_advisor_frame *);
void restore_advisor_C_0040a160(struct restore_advisor_frame *);
void restore_advisor_C_0045a8c0(struct restore_advisor_frame *);
void restore_advisor_C_0045a930(struct restore_advisor_frame *);
void restore_advisor_C_0045d5d0(struct restore_advisor_frame *);
void restore_advisor_C_0045d630(struct restore_advisor_frame *);
void restore_advisor_C_00468170(struct restore_advisor_frame *);
void restore_advisor_C_0046e1d0(struct restore_advisor_frame *);
void restore_advisor_C_0046e1c0(struct restore_advisor_frame *);
void restore_advisor_C_004269b0(struct restore_advisor_frame *);
void restore_advisor_C_00426a00(struct restore_advisor_frame *);
void restore_advisor_C_00426a80(struct restore_advisor_frame *);
void restore_advisor_C_00426b00(struct restore_advisor_frame *);
void restore_advisor_C_0046e410(struct restore_advisor_frame *);
void restore_advisor_C_004c38f0(struct restore_advisor_frame *);
void restore_advisor_C_00451b10(struct restore_advisor_frame *);
void restore_advisor_C_004e9500(struct restore_advisor_frame *);
void restore_advisor_C_004c06e0(struct restore_advisor_frame *);
void restore_advisor_C_004e94a0(struct restore_advisor_frame *);
void restore_advisor_C_004e94d0(struct restore_advisor_frame *);
void restore_advisor_C_004e7e60(struct restore_advisor_frame *);
void restore_advisor_C_004f6ad0(struct restore_advisor_frame *);
void restore_advisor_C_004ea280(struct restore_advisor_frame *);
void restore_advisor_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
int restore_menu_advisor_action(uint32_t token);
#endif
