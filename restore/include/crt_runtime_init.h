#ifndef RESTORE_CRT_RUNTIME_INIT_H
#define RESTORE_CRT_RUNTIME_INIT_H
#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error These x87 and canonical frame adapters require MSVC i386.
#endif
void restore_crt_initialize_floating_runtime(void);
uint32_t restore_crt_append_exit_callback(uint32_t callback);
int32_t restore_crt_register_exit_callback(uint32_t callback);
uint32_t restore_crt_probe_fdiv_erratum(void);
uint32_t restore_crt_query_fdiv_erratum(void);
uint32_t restore_crt_control87(uint32_t value,uint32_t mask,volatile uint32_t *native_mask_slot);
uint32_t restore_crt_controlfp(uint32_t value,uint32_t mask);
void FUN_00561896(void);
uint32_t FUN_005618e6(uint32_t callback);
int32_t FUN_00561953(uint32_t callback);
void FUN_005618ae(void);
void FUN_005640ec(void);
uint32_t FUN_005640fe(void);
uint32_t FUN_0056413c(void);
uint32_t FUN_005687bf(uint32_t value,uint32_t mask);
uint32_t FUN_005687f4(uint32_t value,uint32_t mask);
uint32_t FUN_0056880a(uint32_t control);
uint32_t FUN_0056889c(uint32_t control);
#endif
