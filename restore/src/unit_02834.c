#include "restore_types.h"
#define DAT_012663e8 (*(undefined *)0x012663e8u)
#define DAT_012663f0 (*(undefined *)0x012663f0u)
#define DAT_012663f4 (*(undefined4 *)0x012663f4u)
#define DAT_012663f8 (*(undefined *)0x012663f8u)
#define DAT_012663fc (*(undefined4 *)0x012663fcu)
extern int FUN_00567206();

void FUN_005627a4(undefined4 *param_1)

{
  DWORD DVar1;
  undefined4 uVar2;
  _TIME_ZONE_INFORMATION local_d0;
  _SYSTEMTIME local_24;
  _SYSTEMTIME local_14;
  
  GetLocalTime(&local_14);
  GetSystemTime(&local_24);
  if (local_24.wMinute == RESTORE_SLICE2(DAT_012663f8,2)) {
    if (local_24.wHour == (WORD)DAT_012663f8) {
      if (local_24.wDay == RESTORE_SLICE2(DAT_012663f4,2)) {
        if (local_24.wMonth == RESTORE_SLICE2(DAT_012663f0,2)) {
          if (local_24.wYear == (WORD)DAT_012663f0) goto LAB_0056284e;
        }
      }
    }
  }
  DVar1 = GetTimeZoneInformation(&local_d0);
  if (DVar1 == 0xffffffff) {
    DAT_012663e8 = 0xffffffff;
  }
  else if (((DVar1 == 2) && (local_d0.DaylightDate.wMonth != 0)) && (local_d0.DaylightBias != 0)) {
    DAT_012663e8 = 1;
  }
  else {
    DAT_012663e8 = 0;
  }
  RESTORE_SLICE2(DAT_012663f0,0) = local_24.wYear;
  RESTORE_SLICE2(DAT_012663f0,2) = local_24.wMonth;
  RESTORE_SLICE2(DAT_012663f4,0) = local_24.wDayOfWeek;
  RESTORE_SLICE2(DAT_012663f4,2) = local_24.wDay;
  RESTORE_SLICE2(DAT_012663f8,0) = local_24.wHour;
  RESTORE_SLICE2(DAT_012663f8,2) = local_24.wMinute;
  RESTORE_SLICE2(DAT_012663fc,0) = local_24.wSecond;
  RESTORE_SLICE2(DAT_012663fc,2) = local_24.wMilliseconds;
LAB_0056284e: ;
  uVar2 = FUN_00567206(local_14.wYear,local_14.wMonth,local_14.wDay,local_14.wHour,local_14.wMinute,
                       local_14.wSecond,DAT_012663e8);
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = uVar2;
  }
  return;
}
