#ifndef RESTORE_CRT_STARTUP_ARGUMENTS_H
#define RESTORE_CRT_STARTUP_ARGUMENTS_H
#include <stdint.h>
#include <stddef.h>
#include <windows.h>

/* Units describe the observed native accesses, rather than Ghidra DAT units. */
#define RESTORE_CRT_MB_INITIALIZED (*(volatile uint32_t *)0x01266a84u)
#define RESTORE_CRT_COMMAND_LINE (*(uint8_t *volatile *)0x01267aa4u)
#define RESTORE_CRT_ENVIRONMENT_BLOCK (*(char *volatile *)0x01266400u)
#define RESTORE_CRT_ENVIRONMENT_VECTOR (*(char **volatile *)0x01266458u)
#define RESTORE_CRT_ENVIRONMENT_READY (*(volatile uint32_t *)0x01266a80u)
#define RESTORE_CRT_ARGUMENT_VECTOR (*(uint8_t **volatile *)0x01266450u)
#define RESTORE_CRT_ARGUMENT_COUNT (*(volatile uint32_t *)0x0126644cu)
#define RESTORE_CRT_PROGRAM_NAME (*(uint8_t *volatile *)0x01266468u)
#define RESTORE_CRT_MODULE_NAME ((uint8_t *)0x01266598u)

uint8_t *restore_crt_command_line_tail(void);
void restore_crt_initialize_environment(void);
void restore_crt_initialize_arguments(void);
void restore_crt_initialize_multibyte(void);
LPSTR restore_crt_acquire_environment_block(void);
LPSTR FUN_00567c9a(void);
uint8_t *FUN_0056793c(void);
void FUN_00567994(void);
void FUN_00567a4d(void);
void FUN_00569bf5(void);
/* Existing complete C parser is retained. Native count outputs are DWORDs;
   argv is an array of four-byte pointers, not a one-byte DAT view. */
void FUN_00567ae6(uint8_t *command_line,uint8_t **argv,uint8_t *characters,
 uint32_t *pointer_count,uint32_t *character_count);
#endif
