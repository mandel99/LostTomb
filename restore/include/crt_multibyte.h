#ifndef RESTORE_CRT_MULTIBYTE_H
#define RESTORE_CRT_MULTIBYTE_H
#include <stdint.h>
#include <stddef.h>
#include <windows.h>
#define RESTORE_CRT_CODEPAGE (*(volatile uint32_t *)0x01266710u)
#define RESTORE_CRT_DEFAULT_CODEPAGE (*(volatile uint32_t *)0x012666e4u)
#define RESTORE_CRT_MB_ENABLED (*(volatile uint32_t *)0x0126672cu)
#define RESTORE_CRT_MB_LOCALE (*(volatile uint32_t *)0x01266944u)
/* Three DWORD transfer units; the underlying logical symbol granularity is unresolved. */
#define RESTORE_CRT_MB_SYMBOLS ((volatile uint32_t *)0x01266720u)
#define RESTORE_CRT_MB_FLAGS ((volatile uint8_t *)0x01266840u)
#define RESTORE_CRT_MB_CASE_MAP ((volatile uint8_t *)0x01266740u)
#define RESTORE_CRT_LOCALE_CODEPAGE (*(volatile uint32_t *)0x0126649cu)
#define RESTORE_CRT_MB_CLASS_MASKS ((volatile const uint8_t *)0x00605398u)
typedef struct RestoreCrtMultibyteCodepageRecord {
 uint32_t codepage,symbols[3];
 uint8_t ranges[4][8];
} RestoreCrtMultibyteCodepageRecord;
typedef char RestoreCrtMultibyteRecordLayout[(sizeof(RestoreCrtMultibyteCodepageRecord)==48 &&
 offsetof(RestoreCrtMultibyteCodepageRecord,ranges)==16)?1:-1];
#define RESTORE_CRT_MB_CODEPAGE_RECORDS ((volatile const RestoreCrtMultibyteCodepageRecord *)0x006053a0u)
uint32_t restore_crt_test_multibyte_character(uint8_t character,uint32_t scalar_mask,uint8_t multibyte_mask);
uint32_t FUN_00569800(uint8_t character,uint32_t scalar_mask,uint8_t multibyte_mask);
uint32_t restore_crt_resolve_codepage(int32_t requested);
uint32_t FUN_005699ca(int32_t requested);
uint32_t restore_crt_set_multibyte_codepage(int32_t requested);
uint32_t FUN_00569831(int32_t requested);
void restore_crt_reset_multibyte(void);
void FUN_00569a47(void);
void restore_crt_build_multibyte_case_tables(void);
void FUN_00569a70(void);
uint32_t FUN_00569a14(int32_t codepage);
#define RESTORE_CRT_CTYPE_HANDLE (*(volatile uint32_t *)0x0126648cu)
#define RESTORE_CRT_MAX_MULTIBYTE_BYTES (*(volatile uint32_t *)0x00604e64u)
#define RESTORE_CRT_ERRNO (*(volatile uint32_t *)0x01266430u)
uint32_t restore_crt_uppercase_multibyte(uint32_t character);
uint32_t FUN_0056b363(uint32_t character);
uint8_t *restore_crt_find_multibyte_character(uint8_t *text,uint32_t character);
uint8_t *FUN_0056b104(uint8_t *text,uint32_t character);
int32_t restore_crt_collate_multibyte_case_insensitive(const uint8_t *first,const uint8_t *second,uint32_t bytes);
int32_t FUN_0056a857(const uint8_t *first,const uint8_t *second,uint32_t bytes);
int32_t restore_crt_wide_character_to_multibyte(LPSTR destination,uint16_t character);
int32_t FUN_005691e5(LPSTR destination,uint16_t character);
void restore_crt_initialize_timezone(void);
void FUN_00566bc3(void);
#endif
