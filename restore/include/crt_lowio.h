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
#define RESTORE_CRT_LOWIO_STD_HANDLE_MODE (*(volatile uint32_t *)0x0060496cu)
int32_t restore_crt_allocate_descriptor(void);
int32_t FUN_00566840(void);
uint32_t restore_crt_set_os_handle(uint32_t descriptor,uint32_t handle_bits);
uint32_t FUN_005668d5(uint32_t descriptor,uint32_t handle_bits);
uint32_t restore_crt_free_os_handle(uint32_t descriptor);
uint32_t FUN_0056694c(uint32_t descriptor);
uint32_t restore_crt_seek(uint32_t descriptor,int32_t offset,uint32_t origin);
uint32_t FUN_005662e3(uint32_t descriptor,int32_t offset,uint32_t origin);
int32_t restore_crt_file_length(uint32_t descriptor);
int32_t FUN_0056b1b6(uint32_t descriptor);
void restore_crt_translate_os_error(uint32_t error);
void FUN_005672c8(uint32_t error);
int32_t restore_crt_close(uint32_t descriptor);
int32_t FUN_005647eb(uint32_t descriptor);
int32_t restore_crt_read(uint32_t descriptor,uint8_t *buffer,uint32_t count);
int32_t FUN_005660ed(uint32_t descriptor,uint8_t *buffer,uint32_t count);
int32_t restore_crt_write(uint32_t descriptor,const uint8_t *buffer,uint32_t count);
int32_t FUN_005664d5(uint32_t descriptor,const uint8_t *buffer,uint32_t count);
#endif
