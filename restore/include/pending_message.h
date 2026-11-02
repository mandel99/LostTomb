#ifndef RESTORE_PENDING_MESSAGE_H
#define RESTORE_PENDING_MESSAGE_H
#include <stdint.h>
void FUN_0041d310(void);
void FUN_004e2970(int32_t message);
int32_t FUN_004cbf30(const uint8_t *text,int32_t font);
int32_t FUN_004cc000(char *text);
int32_t FUN_0041d050(const char *encoded);
uint8_t FUN_0041d010(int32_t index);
void FUN_0041d3f0(void);
/* Consumed once by the matching command callback; NULL outside the exact
   decoder -> index dispatcher -> three-argument wrapper call context. */
const uint8_t *restore_command_take_native_payload(void);
#endif
