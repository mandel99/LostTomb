#ifndef RESTORE_DIPLOMACY_INPUT_ACTIONS_H
#define RESTORE_DIPLOMACY_INPUT_ACTIONS_H
#include <stdint.h>
struct restore_diplomacy_frame;
typedef void (*restore_diplomacy_call)(uint32_t,uint32_t,struct restore_diplomacy_frame *);
struct restore_diplomacy_frame {uint32_t r[8],flags;restore_diplomacy_call boundary;};
void restore_diplomacy_C_0040ae10(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040af30(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040b140(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040b1c0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040b1f0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040b380(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0040b3b0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0044d520(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0045a970(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0046ec70(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0046e880(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0046edd0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_00402340(struct restore_diplomacy_frame *);
void restore_diplomacy_C_0044c1a0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_004b84b0(struct restore_diplomacy_frame *);
void restore_diplomacy_C_004b8460(struct restore_diplomacy_frame *);
/* Native nullary cdecl/RET0 callbacks, including the actual402340 confirmation. */
void restore_diplomacy_0040ae10(void);
void restore_diplomacy_0040af30(void);
void restore_diplomacy_0040b140(void);
void restore_diplomacy_0040b1c0(void);
void restore_diplomacy_0040b1f0(void);
void restore_diplomacy_0040b380(void);
void restore_diplomacy_0040b3b0(void);
void restore_diplomacy_00402340(void);
void restore_diplomacy_0045a970(void);
int restore_diplomacy_input_action(uint32_t token);
void restore_diplomacy_input_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
#endif
