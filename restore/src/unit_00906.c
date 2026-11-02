#include "restore_types.h"
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00ea5e54 (*(undefined4 *)0x00ea5e54u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

int FUN_0045e220(int param_1,int param_2,int param_3,int param_4)

{
  char cVar1;
  short sVar2;
  uint uVar3;
  short sVar4;
  
  uVar3 = (uint)*(byte *)((int)&DAT_00799a98 + param_1 * 0x108 + 1);
  if (param_1 < 1) {
    return 1;
  }
  if (((((&DAT_007999e8)[param_1 * 0x84] == 0x47) || ((&DAT_007999e8)[param_1 * 0x84] == 0xbf)) &&
      (0 < param_2)) && ((param_2 < 9 && (sVar2 = (&DAT_00799a2e)[param_1 * 0x84], 0 < sVar2)))) {
    cVar1 = *(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + param_2);
    if (cVar1 == '\0') {
      sVar4 = *(short *)((int)&DAT_007900ea + (param_2 + uVar3 * 0x62) * 2 + 2);
    }
    else {
      if (cVar1 != '\x02') goto LAB_0045e2e0;
      sVar4 = *(short *)(&DAT_00790134 + (param_2 + uVar3 * 0x62) * 2);
    }
    if ((short)(&DAT_00799a2e)[param_2 + param_1 * 0x84] < sVar4) {
LAB_0045e2e0: ;
      if (param_3 != 0) {
        (&DAT_00ea5e54)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5e54)[DAT_00eaac28 * 0x1276] + param_4;
      }
      if (param_4 < sVar2) {
        (&DAT_00799a2e)[param_2 + param_1 * 0x84] =
             (&DAT_00799a2e)[param_2 + param_1 * 0x84] + (short)param_4;
        (&DAT_00799a2e)[param_1 * 0x84] = (&DAT_00799a2e)[param_1 * 0x84] - (short)param_4;
        return param_4;
      }
      (&DAT_00799a2e)[param_2 + param_1 * 0x84] = (&DAT_00799a2e)[param_2 + param_1 * 0x84] + sVar2;
      sVar2 = (&DAT_00799a2e)[param_1 * 0x84];
      (&DAT_00799a2e)[param_1 * 0x84] = 0;
      return (int)sVar2;
    }
  }
  return 0;
}
