#ifndef RESTORE_CRT_ENVIRONMENT_H
#define RESTORE_CRT_ENVIRONMENT_H
#include "crt_startup_arguments.h"
#include "crt_multibyte.h"
#define RESTORE_CRT_INITIAL_ENVIRONMENT_VECTOR (*(char **volatile *)0x0126645cu)
#define RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR (*(WCHAR **volatile *)0x01266460u)
#define RESTORE_CRT_TIMEZONE_INITIALIZED (*(volatile uint32_t *)0x01266568u)
char *restore_crt_get_environment_variable(const uint8_t *name);
int32_t FUN_005696a2(uint8_t *name);
uint32_t restore_crt_convert_wide_environment(void);
uint32_t FUN_0056a896(void);
uint32_t restore_crt_set_environment_entry(char *entry,int32_t update_os);
uint32_t FUN_0056aebe(char *entry,int32_t update_os);
int32_t restore_crt_find_environment_name(const uint8_t *name,uint32_t length);
int32_t FUN_0056b045(const uint8_t *name,uint32_t length);
void restore_crt_initialize_timezone_once(void);
void FUN_00566bae(void);
#endif
