#include "crt_calendar.h"
#include "crt_eh3_frames.h"
static void prepare_local_time(void *frame,const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}
static RestoreCrtTm *run_local_time(void *frame,const volatile uint32_t *arguments,
 uint32_t parent_frame,uint32_t recovered)
{
 volatile const int32_t *input=(volatile const int32_t *)(uintptr_t)arguments[0];
 volatile uint32_t *scratch=(volatile uint32_t *)arguments;
 RestoreCrtTm *time;
 int32_t seconds,value,remainder,days;
 (void)frame;(void)parent_frame;(void)recovered;
 if(*input<0)return 0;
 FUN_00566bae();
 seconds=*input;
 if(seconds>0x3f480 && seconds<0x7ffc0b7f){
  *scratch=(uint32_t)seconds-RESTORE_CRT_TIMEZONE_SECONDS;
  time=(RestoreCrtTm *)FUN_0056710d((int32_t *)scratch);
  if(RESTORE_CRT_DAYLIGHT!=0 && FUN_00566e21(time)!=0){
   *scratch=*scratch-RESTORE_CRT_DAYLIGHT_SECONDS;
   time=(RestoreCrtTm *)FUN_0056710d((int32_t *)scratch);
   time->daylight=1;
  }
  return time;
 }
 time=(RestoreCrtTm *)FUN_0056710d((int32_t *)input);
 {
  uint32_t daylight=FUN_00566e21(time);
  value=time->second;
  if(daylight!=0)value=(int32_t)((uint32_t)value-RESTORE_CRT_DAYLIGHT_SECONDS);
 }
 *scratch=(uint32_t)value-RESTORE_CRT_TIMEZONE_SECONDS;
 value=(int32_t)*scratch;
 remainder=value%60;time->second=remainder;
 if(remainder<0){time->second=remainder+60;*scratch=*scratch-60u;}
 *scratch=(uint32_t)((int32_t)*scratch/60)+(uint32_t)time->minute;
 value=(int32_t)*scratch;remainder=value%60;time->minute=remainder;
 if(remainder<0){time->minute=remainder+60;*scratch=*scratch-60u;}
 *scratch=(uint32_t)((int32_t)*scratch/60)+(uint32_t)time->hour;
 value=(int32_t)*scratch;remainder=value%24;time->hour=remainder;
 if(remainder<0){time->hour=remainder+24;*scratch=*scratch-24u;}
 days=(int32_t)*scratch/24;*scratch=(uint32_t)days;
 if(days>0){
  time->weekday=restore_crt_calendar_add32(time->weekday,days)%7;
  time->day=restore_crt_calendar_add32(time->day,days);
 }else if(days<0){
  time->weekday=restore_crt_calendar_add32(restore_crt_calendar_add32(time->weekday,7),days)%7;
  time->day=restore_crt_calendar_add32(time->day,days);
  if(time->day<=0){
   int32_t previous_day=restore_crt_calendar_add32(time->day,31);
   time->year=restore_crt_calendar_sub32(time->year,1);
   time->day=previous_day;
   time->yearday=364;time->month=11;return time;
  }
 }else return time;
 time->yearday=restore_crt_calendar_add32(time->yearday,days);
 return time;
}
RESTORE_EH3_PARENT(restore_crt_local_time,RestoreCrtTm *,
 (const int32_t *seconds),0,prepare_local_time,run_local_time)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) RestoreCrtTm *FUN_00562644(int32_t *seconds)
{__asm jmp restore_crt_local_time}
#pragma warning(pop)
