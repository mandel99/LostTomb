#ifndef RESTORE_CRT_CALENDAR_H
#define RESTORE_CRT_CALENDAR_H
#include "crt_environment.h"

typedef struct RestoreCrtTm {
 int32_t second,minute,hour,day,month,year,weekday,yearday,daylight;
} RestoreCrtTm;
typedef struct RestoreCrtDstTransition {
 int32_t year,yearday,milliseconds;
} RestoreCrtDstTransition;
typedef char RestoreCrtCalendarLayout[(sizeof(RestoreCrtTm)==36 &&
 offsetof(RestoreCrtTm,year)==20 && offsetof(RestoreCrtTm,yearday)==28 &&
 sizeof(RestoreCrtDstTransition)==12)?1:-1];
#define RESTORE_CRT_TIMEZONE_SECONDS (*(volatile uint32_t *)0x00604e78u)
#define RESTORE_CRT_DAYLIGHT (*(volatile uint32_t *)0x00604e7cu)
#define RESTORE_CRT_DAYLIGHT_SECONDS (*(volatile uint32_t *)0x00604e80u)
#define RESTORE_CRT_DST_START (*(volatile RestoreCrtDstTransition *)0x00604f10u)
#define RESTORE_CRT_DST_END (*(volatile RestoreCrtDstTransition *)0x00604f20u)
#define RESTORE_CRT_TIMEZONE_FROM_OS (*(volatile uint32_t *)0x012664b0u)
#define RESTORE_CRT_TIMEZONE_INFO (*(volatile TIME_ZONE_INFORMATION *)0x012664b8u)
/* Padded overlay bases: month1 reads the -1 sentinel, not element0.
   Month-end views begin four bytes later; retain each native base explicitly. */
#define RESTORE_CRT_COMMON_MONTH_PREFIX ((volatile const int32_t *)0x00605360u)
#define RESTORE_CRT_LEAP_MONTH_PREFIX ((volatile const int32_t *)0x0060532cu)
#define RESTORE_CRT_COMMON_MONTH_END ((volatile const int32_t *)0x00605364u)
#define RESTORE_CRT_LEAP_MONTH_END ((volatile const int32_t *)0x00605330u)
static __inline int32_t restore_crt_calendar_add32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t restore_crt_calendar_sub32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int32_t restore_crt_calendar_mul32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a*(uint32_t)b);}
RestoreCrtTm *restore_crt_local_time(const int32_t *seconds);
RestoreCrtTm *FUN_00562644(int32_t *seconds);
uint32_t restore_crt_is_daylight(const volatile RestoreCrtTm *time);
uint32_t FUN_00566e21(const volatile RestoreCrtTm *time);
void restore_crt_build_dst_transition(int32_t start,int32_t relative,uint32_t year,
 int32_t month,int32_t week,int32_t weekday,int32_t day,int32_t hour,
 int32_t minute,int32_t second,int32_t millisecond);
void FUN_00566fcd(int32_t start,int32_t relative,uint32_t year,
 int32_t month,int32_t week,int32_t weekday,int32_t day,int32_t hour,
 int32_t minute,int32_t second,int32_t millisecond);
int32_t restore_crt_make_time(int32_t year,int32_t month,int32_t day,int32_t hour,
 int32_t minute,int32_t second,int32_t daylight);
int32_t FUN_00567206(int32_t year,int32_t month,int32_t day,int32_t hour,
 int32_t minute,int32_t second,int32_t daylight);
/* Existing complete UTC conversion owner is retained; its pointer bits are
   reinterpreted as the nine actual tm DWORD fields by typed callers. */
uint8_t *FUN_0056710d(int32_t *seconds);
#endif
