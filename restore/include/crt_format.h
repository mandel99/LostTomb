#ifndef RESTORE_CRT_FORMAT_H
#define RESTORE_CRT_FORMAT_H
#include <stdint.h>
#include <stddef.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error The native binary64 load/spill boundary requires MSVC i386.
#endif
/* Native 80-bit value: two significand DWORDs and a sign/exponent WORD.
   The unused upper half of its third argument DWORD is never consumed. */
typedef union RestoreCrt96 {
 uint32_t word[3];
 uint16_t half[6];
 uint8_t byte[12];
} RestoreCrt96;
typedef struct RestoreCrtDecimal {
 int32_t sign;
 int32_t exponent;
 uint32_t status;
 char *digits;
} RestoreCrtDecimal;
typedef struct RestoreCrtDecimalDigits {
 int16_t exponent;
 int8_t sign;
 int8_t length;
 char digits[24];
} RestoreCrtDecimalDigits;
typedef char RestoreCrtDecimalLayout[(sizeof(RestoreCrtDecimal)==16 && offsetof(RestoreCrtDecimal,digits)==12)?1:-1];
typedef char RestoreCrtDigitsLayout[(offsetof(RestoreCrtDecimalDigits,digits)==4)?1:-1];
#define RESTORE_CRT_DECIMAL (*(RestoreCrtDecimal * volatile *)(uintptr_t)0x1266420u)
#define RESTORE_CRT_REUSE (*(volatile uint8_t *)(uintptr_t)0x1266424u)
#define RESTORE_CRT_EXPONENT (*(volatile int32_t *)(uintptr_t)0x1266428u)
#define RESTORE_CRT_ROUNDED (*(volatile uint8_t *)(uintptr_t)0x126642cu)
#define RESTORE_CRT_POINT (*(volatile char *)(uintptr_t)0x604e68u)
static __inline int32_t restore_crt_add32(int32_t a,int32_t b){return(int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t restore_crt_neg32(int32_t a){return(int32_t)(0u-(uint32_t)a);}
static __inline char *restore_crt_offset(char *p,int32_t n){return(char *)((uintptr_t)p+(uint32_t)n);}
static __inline uint64_t restore_crt_binary64_load_spill(const double *source)
{
 uint64_t value;
 __asm {
  mov eax,source
  fld qword ptr [eax]
  fstp qword ptr [value]
 }
 return value;
}
char *restore_crt_format_float(const double *,char *,int32_t,int32_t,int32_t);
char *restore_crt_format_scientific(const double *,char *,int32_t,int32_t);
char *restore_crt_format_fixed(const double *,char *,int32_t);
char *restore_crt_format_general(const double *,char *,int32_t,int32_t);
char *restore_crt_general_scientific(const double *,char *,int32_t,int32_t);
char *restore_crt_general_fixed(const double *,char *,int32_t);
RestoreCrtDecimal *restore_crt_binary64_decimal(uint64_t);
uint32_t restore_crt_raw80_decimal(uint32_t,uint32_t,uint32_t,int32_t,uint32_t,RestoreCrtDecimalDigits *);
void restore_crt_multiply_raw96(RestoreCrt96 *,const RestoreCrt96 *);
void restore_crt_scale_raw96(RestoreCrt96 *,int32_t,int32_t);
char *FUN_0056452a(const double *,char *,int32_t,int32_t,int32_t);
char *FUN_00564263(const double *,char *,int32_t,int32_t);
char *FUN_00564367(const double *,char *,int32_t);
char *FUN_00564445(const double *,char *,int32_t,int32_t);
char *FUN_005644e0(const double *,char *,int32_t,int32_t);
char *FUN_00564507(const double *,char *,int32_t);
RestoreCrtDecimal *FUN_00568d88(uint64_t);
uint32_t FUN_0056a47e(uint32_t,uint32_t,uint32_t,int32_t,uint32_t,RestoreCrtDecimalDigits *);
void FUN_0056a904(RestoreCrt96 *,const RestoreCrt96 *);
void FUN_0056ab24(RestoreCrt96 *,int32_t,int32_t);
/* Reused compiled C owners retain their original source bytes and algorithms. */
void FUN_00568dec(uint32_t *,const uint32_t *);
void FUN_00568d11(char *,int32_t,RestoreCrtDecimal *);
void FUN_0056457b(char *,int32_t);
uint32_t *FUN_00566750(void *,const void *);
void *FUN_00563240(void *,int32_t,size_t);
void FUN_00569e8b(uint32_t *);
void FUN_00569eb9(uint32_t *);
void FUN_00569e2d(uint32_t *,const uint32_t *);
#endif
