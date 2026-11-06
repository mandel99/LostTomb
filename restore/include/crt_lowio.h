#ifndef RESTORE_CRT_LOWIO_H
#define RESTORE_CRT_LOWIO_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreCrtLowioRecord {
 uint32_t handle;
 uint8_t flags, text_state;
 uint16_t reserved;
} RestoreCrtLowioRecord;
typedef char RestoreCrtLowioLayout[(sizeof(RestoreCrtLowioRecord)==8 && offsetof(RestoreCrtLowioRecord,flags)==4 && offsetof(RestoreCrtLowioRecord,text_state)==5)?1:-1];
#define RESTORE_CRT_LOWIO_BLOCKS ((RestoreCrtLowioRecord *volatile *)0x01266960u)
#define RESTORE_CRT_LOWIO_CAPACITY (*(volatile uint32_t *)0x01266a60u)
#ifndef RESTORE_CRT_ERRNO
#define RESTORE_CRT_ERRNO (*(volatile uint32_t *)0x01266430u)
#endif
#define RESTORE_CRT_DOSERRNO (*(volatile uint32_t *)0x01266434u)
void restore_crt_initialize_lowio(void);
void FUN_00566a03(void);
uint32_t restore_crt_get_os_handle(uint32_t descriptor);
uint32_t FUN_005669c6(uint32_t descriptor);
#endif
