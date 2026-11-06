#include "restore_types.h"
#include "crt_multibyte.h"
#include "byte_string_compare.h"
#define RESTORE_CRT_TIMEZONE_SECONDS (*(volatile uint32_t *)0x00604e78u)
#define RESTORE_CRT_DAYLIGHT (*(volatile int32_t *)0x00604e7cu)
#define RESTORE_CRT_DAYLIGHT_SECONDS (*(volatile uint32_t *)0x00604e80u)
#define RESTORE_CRT_TZ_CACHE_A (*(volatile uint32_t *)0x00604f10u)
#define RESTORE_CRT_TZ_CACHE_B (*(volatile uint32_t *)0x00604f20u)
#define RESTORE_CRT_STANDARD_TZ_NAME (*(char *volatile *)0x00604f04u)
#define RESTORE_CRT_DAYLIGHT_TZ_NAME (*(char *volatile *)0x00604f08u)
#define RESTORE_CRT_TIMEZONE_FROM_OS (*(volatile uint32_t *)0x012664b0u)
#define RESTORE_CRT_TIMEZONE_INFO (*(volatile TIME_ZONE_INFORMATION *)0x012664b8u)
#define RESTORE_CRT_CACHED_TZ (*(char *volatile *)0x01266564u)
/* Access widths follow the native instructions, independent of old DAT types. */
typedef char RestoreCrtTimezoneLayout[(sizeof(void *)==4 &&
 sizeof(TIME_ZONE_INFORMATION)==172 && sizeof(BOOL)==4 &&
 offsetof(TIME_ZONE_INFORMATION,StandardName)==4 &&
 offsetof(TIME_ZONE_INFORMATION,StandardDate)+offsetof(SYSTEMTIME,wMonth)==70 &&
 offsetof(TIME_ZONE_INFORMATION,StandardBias)==84 &&
 offsetof(TIME_ZONE_INFORMATION,DaylightName)==88 &&
 offsetof(TIME_ZONE_INFORMATION,DaylightDate)+offsetof(SYSTEMTIME,wMonth)==154 &&
 offsetof(TIME_ZONE_INFORMATION,DaylightBias)==168)?1:-1];
extern void * __cdecl FUN_00561b68(size_t bytes);
extern void FUN_00561bdc(void *allocation);
extern int32_t FUN_00561c0b(const uint8_t *text);
extern char * __cdecl FUN_00562240(char *destination,char *source,size_t count);
extern size_t FUN_00563380(const char *text);
extern uint32_t *FUN_00566750(void *destination,const void *source);
/* Existing getenv owner returns its full pointer bits as a native int32. */
extern int32_t FUN_005696a2(uint8_t *name);

/* Branches and string lifetimes are retained from the recovered C. The BOOL
   lives in the actual PUSH ECX cell and is never initialized by this body. */
static void initialize_timezone_body(volatile BOOL *used_default)

{
  char cVar1;
  char cVar2;
  char *_Str1;
  DWORD DVar3;
  int iVar4;
  size_t sVar5;
  char *_Source;
  uint32_t bias,standard_bias;
  uint16_t standard_month;
  
  RESTORE_CRT_TIMEZONE_FROM_OS = 0;
  RESTORE_CRT_TZ_CACHE_B = 0xffffffff;
  RESTORE_CRT_TZ_CACHE_A = 0xffffffff;
  _Str1 = (char *)(uintptr_t)(uint32_t)FUN_005696a2((uint8_t *)0x005707a4u);
  if (_Str1 == (char *)0x0) {
    DVar3 = GetTimeZoneInformation((LPTIME_ZONE_INFORMATION)&RESTORE_CRT_TIMEZONE_INFO);
    if (DVar3 == 0xffffffff) {
      return;
    }
    bias = (uint32_t)RESTORE_CRT_TIMEZONE_INFO.Bias;
    standard_bias = (uint32_t)RESTORE_CRT_TIMEZONE_INFO.StandardBias;
    standard_month = RESTORE_CRT_TIMEZONE_INFO.StandardDate.wMonth;
    RESTORE_CRT_TIMEZONE_SECONDS = bias * 60u;
    RESTORE_CRT_TIMEZONE_FROM_OS = 1;
    if (standard_month != 0) {
      RESTORE_CRT_TIMEZONE_SECONDS = RESTORE_CRT_TIMEZONE_SECONDS + standard_bias * 60u;
    }
    if ((RESTORE_CRT_TIMEZONE_INFO.DaylightDate.wMonth == 0) || (RESTORE_CRT_TIMEZONE_INFO.DaylightBias == 0)) {
      RESTORE_CRT_DAYLIGHT = 0;
      RESTORE_CRT_DAYLIGHT_SECONDS = 0;
    }
    else {
      RESTORE_CRT_DAYLIGHT = 1;
      RESTORE_CRT_DAYLIGHT_SECONDS = ((uint32_t)RESTORE_CRT_TIMEZONE_INFO.DaylightBias - standard_bias) * 60u;
    }
    iVar4 = WideCharToMultiByte(RESTORE_CRT_LOCALE_CODEPAGE,0x220,(LPCWSTR)RESTORE_CRT_TIMEZONE_INFO.StandardName,-1,RESTORE_CRT_STANDARD_TZ_NAME,0x3f,
                                (LPCSTR)0x0,(LPBOOL)used_default);
    if ((iVar4 == 0) || (*used_default != 0)) {
      *RESTORE_CRT_STANDARD_TZ_NAME = 0;
    }
    else {
      RESTORE_CRT_STANDARD_TZ_NAME[0x3f] = 0;
    }
    iVar4 = WideCharToMultiByte(RESTORE_CRT_LOCALE_CODEPAGE,0x220,(LPCWSTR)RESTORE_CRT_TIMEZONE_INFO.DaylightName,-1,RESTORE_CRT_DAYLIGHT_TZ_NAME,0x3f,
                                (LPCSTR)0x0,(LPBOOL)used_default);
    if ((iVar4 != 0) && (*used_default == 0)) {
      RESTORE_CRT_DAYLIGHT_TZ_NAME[0x3f] = 0;
      return;
    }
  }
  else {
    if (*_Str1 == '\0') {
      return;
    }
    if ((RESTORE_CRT_CACHED_TZ != (char *)0x0) && (iVar4 = FUN_005631b0(_Str1,RESTORE_CRT_CACHED_TZ), iVar4 == 0)) {
      return;
    }
    FUN_00561bdc(RESTORE_CRT_CACHED_TZ);
    sVar5 = FUN_00563380(_Str1);
    RESTORE_CRT_CACHED_TZ = FUN_00561b68(sVar5 + 1);
    if (RESTORE_CRT_CACHED_TZ == (char *)0x0) {
      return;
    }
    FUN_00566750(RESTORE_CRT_CACHED_TZ,_Str1);
    FUN_00562240(RESTORE_CRT_STANDARD_TZ_NAME,_Str1,3);
    _Source = _Str1 + 3;
    RESTORE_CRT_STANDARD_TZ_NAME[3] = 0;
    cVar1 = *_Source;
    if (cVar1 == '-') {
      _Source = _Str1 + 4;
    }
    iVar4 = FUN_00561c0b((const uint8_t *)_Source);
    RESTORE_CRT_TIMEZONE_SECONDS = (uint32_t)iVar4 * 3600u;
    for (; (cVar2 = *_Source, cVar2 == '+' || (('/' < cVar2 && (cVar2 < ':'))));
        _Source = _Source + 1) {
    }
    if (*_Source == ':') {
      _Source = _Source + 1;
      iVar4 = FUN_00561c0b((const uint8_t *)_Source);
      RESTORE_CRT_TIMEZONE_SECONDS = RESTORE_CRT_TIMEZONE_SECONDS + (uint32_t)iVar4 * 60u;
      for (; ('/' < *_Source && (*_Source < ':')); _Source = _Source + 1) {
      }
      if (*_Source == ':') {
        _Source = _Source + 1;
        iVar4 = FUN_00561c0b((const uint8_t *)_Source);
        RESTORE_CRT_TIMEZONE_SECONDS = RESTORE_CRT_TIMEZONE_SECONDS + (uint32_t)iVar4;
        for (; ('/' < *_Source && (*_Source < ':')); _Source = _Source + 1) {
        }
      }
    }
    if (cVar1 == '-') {
      RESTORE_CRT_TIMEZONE_SECONDS = 0u - RESTORE_CRT_TIMEZONE_SECONDS;
    }
    RESTORE_CRT_DAYLIGHT = (int32_t)(int8_t)*_Source;
    if (RESTORE_CRT_DAYLIGHT != 0) {
      FUN_00562240(RESTORE_CRT_DAYLIGHT_TZ_NAME,_Source,3);
      RESTORE_CRT_DAYLIGHT_TZ_NAME[3] = 0;
      return;
    }
  }
  *RESTORE_CRT_DAYLIGHT_TZ_NAME = 0;
  return;
}

/* Native566BC3 reserves its BOOL with PUSH ECX, before saving four registers.
   Retain that actual cell even when a controlled conversion leaves it unwritten.
   Only stack ownership is assembly; timezone/string behavior remains C. */
__declspec(naked) void restore_crt_initialize_timezone(void)
{
 __asm {
  push ecx
  push ebx
  push ebp
  push esi
  push edi
  lea eax,[esp+16]
  push eax
  call initialize_timezone_body
  add esp,4
  pop edi
  pop esi
  pop ebp
  pop ebx
  pop ecx
  ret
 }
}
__declspec(naked) void FUN_00566bc3(void)
{__asm jmp restore_crt_initialize_timezone}
