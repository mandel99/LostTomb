#include "crt_calendar.h"
#include "crt_eh3_frames.h"

/* Native EBP-24h..EBP-1 is the complete tm storage. Only four DWORDs are
   assigned. The DST predicate may read the actual untouched second/minute
   bytes at a transition boundary; never zero or synthesize those fields. */
typedef char RestoreCrtMakeTimeFrameLayout[(sizeof(RestoreCrtTm)==36 &&
 offsetof(RestoreCrtTm,second)==0 && offsetof(RestoreCrtTm,minute)==4 &&
 offsetof(RestoreCrtTm,hour)==8 && offsetof(RestoreCrtTm,month)==16 &&
 offsetof(RestoreCrtTm,year)==20 && offsetof(RestoreCrtTm,yearday)==28)?1:-1];
static void prepare_make_time(volatile RestoreCrtTm *frame,
 const volatile uint32_t *arguments,uint32_t saved_sp)
{(void)frame;(void)arguments;(void)saved_sp;}

/* Retain the recovered arithmetic and branch order, with native DWORD wrap. */
static int32_t run_make_time(volatile RestoreCrtTm *frame,
 const volatile uint32_t *arguments,uint32_t parent_frame,uint32_t recovered)
{
 volatile uint32_t *native_arguments=(volatile uint32_t *)(uintptr_t)arguments;
 uint32_t year_since_1900=native_arguments[0]-1900u;
 uint32_t month,ordinal,days,second,hour,value;
 (void)parent_frame;(void)recovered;
 if((int32_t)year_since_1900<70 || (int32_t)year_since_1900>138)return -1;
 month=native_arguments[1];
 /* This is the padded605360 view: month1 reads the -1 sentinel at605364. */
 ordinal=(uint32_t)RESTORE_CRT_COMMON_MONTH_PREFIX[(int32_t)month]+native_arguments[2];
 if((year_since_1900&3u)==0 && (int32_t)month>2)++ordinal;
 FUN_00566bae();
 frame->yearday=(int32_t)ordinal;
 frame->year=(int32_t)year_since_1900;
 days=year_since_1900*365u+ordinal+
  (uint32_t)((int32_t)(year_since_1900-1u)>>2);
 /* Native reads these argument cells after the timezone callback, while
    year/month/ordinal retain their earlier captured values. */
 second=native_arguments[5];
 hour=native_arguments[3];
 frame->hour=(int32_t)hour;
 value=(hour+days*24u)*60u;
 value+=native_arguments[4];
 value=value*60u+RESTORE_CRT_TIMEZONE_SECONDS;
 frame->month=(int32_t)(month-1u);
 value+=second+0x7c558180u;
 /* Native uses the incoming year cell as the base timestamp scratch slot.
    It remains unadjusted even when the returned value receives DST bias. */
 native_arguments[0]=value;
 if((int32_t)native_arguments[6]==1)value+=RESTORE_CRT_DAYLIGHT_SECONDS;
 else if((int32_t)native_arguments[6]==-1 && RESTORE_CRT_DAYLIGHT!=0){
  uint32_t is_daylight=FUN_00566e21(frame);
  value=native_arguments[0];
  if(is_daylight!=0)value+=RESTORE_CRT_DAYLIGHT_SECONDS;
 }
 return (int32_t)value;
}
/* Reuse physical stack ownership without installing an EH3 registration. */
RESTORE_EH3_PARENT(restore_crt_make_time,int32_t,
 (int32_t year,int32_t month,int32_t day,int32_t hour,int32_t minute,
 int32_t second,int32_t daylight),36,prepare_make_time,run_make_time)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) int32_t FUN_00567206(int32_t year,int32_t month,int32_t day,
 int32_t hour,int32_t minute,int32_t second,int32_t daylight)
{__asm jmp restore_crt_make_time}
#pragma warning(pop)
