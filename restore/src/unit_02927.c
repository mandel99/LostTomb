#include "crt_calendar.h"
#include "crt_eh3_frames.h"
static void prepare_dst_transition(void *frame,const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}
static void run_dst_transition(void *frame,const volatile uint32_t *arguments,
 uint32_t parent_frame,uint32_t recovered)
{
 uint32_t year=arguments[2];
 int32_t month=(int32_t)arguments[3],yearday,milliseconds;
 volatile uint32_t *mutable_arguments=(volatile uint32_t *)arguments;
 (void)frame;(void)parent_frame;(void)recovered;
 if(arguments[1]==1){
  int32_t month_before,first_weekday,first_day,occurrence=(int32_t)arguments[4];
  mutable_arguments[2]=year&3u;
  month_before=mutable_arguments[2]==0?RESTORE_CRT_LEAP_MONTH_PREFIX[month]:RESTORE_CRT_COMMON_MONTH_PREFIX[month];
  first_day=restore_crt_calendar_add32(month_before,1);
  first_weekday=(int32_t)(year*365u+(uint32_t)(((int32_t)(year-1u))>>2)+
   (uint32_t)first_day-25563u)%7;
  yearday=(int32_t)((uint32_t)first_day+(uint32_t)occurrence*7u-(uint32_t)first_weekday+arguments[5]);
  if(first_weekday<(int32_t)arguments[5])yearday=restore_crt_calendar_sub32(yearday,7);
  if(occurrence==5){
   int32_t month_end=mutable_arguments[2]==0?RESTORE_CRT_LEAP_MONTH_END[month]:RESTORE_CRT_COMMON_MONTH_END[month];
   if(yearday>month_end)yearday=restore_crt_calendar_sub32(yearday,7);
  }
 }else{
  int32_t before=(year&3u)==0?RESTORE_CRT_LEAP_MONTH_PREFIX[month]:RESTORE_CRT_COMMON_MONTH_PREFIX[month];
  yearday=(int32_t)((uint32_t)before+arguments[6]);
 }
 if(arguments[0]==1){
  RESTORE_CRT_DST_START.yearday=yearday;
  RESTORE_CRT_DST_START.year=(int32_t)year;
  RESTORE_CRT_DST_START.milliseconds=(int32_t)(((arguments[7]*60u+arguments[8])*60u+arguments[9])*1000u+arguments[10]);
 }else{
  RESTORE_CRT_DST_END.yearday=yearday;
  milliseconds=(int32_t)(((arguments[7]*60u+arguments[8])*60u+RESTORE_CRT_DAYLIGHT_SECONDS+arguments[9])*1000u+arguments[10]);
  RESTORE_CRT_DST_END.milliseconds=milliseconds;
  if(milliseconds<0){
   RESTORE_CRT_DST_END.milliseconds=restore_crt_calendar_add32(milliseconds,86400000);
   RESTORE_CRT_DST_END.yearday=restore_crt_calendar_sub32(yearday,1);
  }else if(milliseconds>=86400000){
   RESTORE_CRT_DST_END.milliseconds=restore_crt_calendar_sub32(milliseconds,86400000);
   RESTORE_CRT_DST_END.yearday=restore_crt_calendar_add32(yearday,1);
  }
  RESTORE_CRT_DST_END.year=(int32_t)year;
 }
}
RESTORE_EH3_PARENT(restore_crt_build_dst_transition,void,
 (int32_t start,int32_t relative,uint32_t year,int32_t month,int32_t week,int32_t weekday,
  int32_t day,int32_t hour,int32_t minute,int32_t second,int32_t millisecond),0,
 prepare_dst_transition,run_dst_transition)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) void FUN_00566fcd(int32_t start,int32_t relative,uint32_t year,
 int32_t month,int32_t week,int32_t weekday,int32_t day,int32_t hour,
 int32_t minute,int32_t second,int32_t millisecond)
{__asm jmp restore_crt_build_dst_transition}
#pragma warning(pop)
