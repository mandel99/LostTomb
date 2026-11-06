#include "crt_calendar.h"

static void build_windows_transition(int32_t start,uint32_t year,
 volatile const SYSTEMTIME *rule)
{
 if(rule->wYear==0)
  FUN_00566fcd(start,1,year,rule->wMonth,rule->wDay,rule->wDayOfWeek,0,
   rule->wHour,rule->wMinute,rule->wSecond,rule->wMilliseconds);
 else
  FUN_00566fcd(start,0,year,rule->wMonth,0,0,rule->wDay,
   rule->wHour,rule->wMinute,rule->wSecond,rule->wMilliseconds);
}
uint32_t restore_crt_is_daylight(const volatile RestoreCrtTm *time)
{
 int32_t year,day,start,end,milliseconds;
 if(RESTORE_CRT_DAYLIGHT==0)return 0;
 year=time->year;
 if(year!=RESTORE_CRT_DST_START.year || year!=RESTORE_CRT_DST_END.year){
  if(RESTORE_CRT_TIMEZONE_FROM_OS==0){
   FUN_00566fcd(1,1,(uint32_t)year,4,1,0,0,2,0,0,0);
   FUN_00566fcd(0,1,(uint32_t)time->year,10,5,0,0,2,0,0,0);
  }else{
   build_windows_transition(1,(uint32_t)year,&RESTORE_CRT_TIMEZONE_INFO.DaylightDate);
   build_windows_transition(0,(uint32_t)time->year,&RESTORE_CRT_TIMEZONE_INFO.StandardDate);
  }
 }
 start=RESTORE_CRT_DST_START.yearday;
 end=RESTORE_CRT_DST_END.yearday;
 day=time->yearday;
 if(start<end){
  if(day<start || day>end)return 0;
  if(day>start && day<end)return 1;
 }else{
  if(day<end || day>start)return 1;
  if(day>end && day<start)return 0;
 }
 milliseconds=(int32_t)((((uint32_t)time->hour*60u+(uint32_t)time->minute)*60u+
  (uint32_t)time->second)*1000u);
 if(day==start)return (uint32_t)(milliseconds>=RESTORE_CRT_DST_START.milliseconds);
 return (uint32_t)(milliseconds<RESTORE_CRT_DST_END.milliseconds);
}
uint32_t FUN_00566e21(const volatile RestoreCrtTm *time)
{return restore_crt_is_daylight(time);}
