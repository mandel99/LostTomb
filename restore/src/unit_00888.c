#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00ea60d0 (*(undefined4 *)0x00ea60d0u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f566b4 (*(undefined *)0x00f566b4u)
#define DAT_00f566bc (*(undefined *)0x00f566bcu)
#define DAT_00f57204 (*(undefined *)0x00f57204u)
extern undefined4 FUN_0045b490();
extern undefined4 FUN_0047d350();

void FUN_0045b700(int param_1,int param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_4;
  
  if (((&DAT_007999e8)[param_1 * 0x84] == 0x48) || ((&DAT_007999e8)[param_1 * 0x84] == 0xc1)) {
    local_4 = 0;
    iVar5 = DAT_00eaac28;
    do {
      if (param_2 < 1) {
        return;
      }
      param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108);
      if (0 < param_1) {
        sVar1 = (&DAT_00799a14)[param_1 * 0x84];
        if (0 < sVar1) {
          iVar4 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
          iVar6 = (int)sVar1;
          if (param_2 < iVar6) {
            iVar6 = iVar5 * 0x1276 + iVar4;
            (&DAT_00799a14)[param_1 * 0x84] = sVar1 - (short)param_2;
            (&DAT_00ea60d0)[iVar6] = (&DAT_00ea60d0)[iVar6] + param_2;
            iVar2 = (&DAT_00ea6160)[iVar6] - param_2;
            param_2 = 0;
            (&DAT_00ea6160)[iVar6] = iVar2;
          }
          else {
            iVar2 = iVar5 * 0x1276 + iVar4;
            param_2 = param_2 - iVar6;
            (&DAT_00ea60d0)[iVar2] = (&DAT_00ea60d0)[iVar2] + iVar6;
            (&DAT_00ea6160)[iVar2] = (&DAT_00ea6160)[iVar2] - iVar6;
            (&DAT_00799a14)[param_1 * 0x84] = 0;
            (&DAT_007999ea)[param_1 * 0x84] = 0;
          }
          iVar6 = *(int *)(&DAT_007999e4 + param_1 * 0x108);
          iVar2 = DAT_00f566b4;
          if (0 < (short)(&DAT_00799a14)[param_1 * 0x84]) {
            if (iVar4 < 0x1f) {
              iVar2 = DAT_00f566bc + -4 + iVar4 * 4;
            }
            else {
              iVar2 = DAT_00f57204 + -0x7c + iVar4 * 4;
            }
            iVar4 = FUN_0047d350(iVar4,0);
            iVar3 = FUN_0045b490((int)(short)(&DAT_00799a14)[param_1 * 0x84]);
            iVar5 = DAT_00eaac28;
            iVar2 = iVar2 + iVar4 + iVar3;
          }
          (&DAT_00b7482c)[iVar6] = iVar2;
        }
      }
      local_4 = local_4 + 1;
    } while (local_4 < 8);
  }
  return;
}
