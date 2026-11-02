#include "restore_types.h"
#define DAT_0078d788 (*(undefined *)0x0078d788u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00790186 (*(undefined4 *)0x00790186u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_0045dc50();

int FUN_0045bb50(int param_1,int param_2,char param_3)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  int iVar8;
  int local_1c;
  int local_14;
  int local_10;
  int local_c;
  
  local_1c = 10000;
  local_14 = 0;
  if ((param_2 < 1) || (bVar2 = true, 8 < param_2)) {
    bVar2 = false;
  }
  if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + param_2) * 4) != 0) {
    return 0;
  }
  bVar1 = true;
  iVar7 = param_1 * 0x108;
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x47: ;
  case 0x48: ;
    if (*(char *)((int)&DAT_007900c2 +
                 param_2 + (uint)*(byte *)((int)&DAT_00799a98 + iVar7 + 1) * 0xc4) != '\x02')
    goto switchD_0045bbc8_caseD_49;
  case 0xb7: ;
  case 0xd5: ;
  case 0xde: ;
  case 0xe1: ;
  case 0xe3: ;
  case 0xe4: ;
  case 0xe5: ;
  case 0xe6: ;
  case 0xea: ;
  case 0xeb: ;
  case 0xec: ;
    bVar1 = false;
default: ;
switchD_0045bbc8_caseD_49: ;
    local_c = 1;
    if (0 < DAT_0078d788) {
      pcVar6 = &DAT_00790186 + param_2;
      do {
        if (((((pcVar6[-2 - param_2] != '\0') &&
              (iVar8 = (int)*(short *)(pcVar6 + (-6 - param_2)), iVar8 != 0)) &&
             (iVar4 = iVar8 * 0x108, (&DAT_007999d8)[iVar4] == '\x01')) && (iVar8 != param_1)) &&
           ((!bVar1 || ((&DAT_007999ec)[iVar4] == (&DAT_007999ec)[iVar7])))) {
          if ((&DAT_007999e8)[iVar8 * 0x84] == 0x48) {
            iVar5 = 0;
            local_10 = 8;
            iVar3 = iVar8;
            do {
              iVar3 = (int)*(short *)(&DAT_00799a12 + iVar3 * 0x108);
              if (((0 < iVar3) && (0 < (short)(&DAT_00799a14)[iVar3 * 0x84])) &&
                 ((short)(&DAT_007999ea)[iVar3 * 0x84] == param_2)) {
                iVar5 = iVar5 + (short)(&DAT_00799a14)[iVar3 * 0x84];
              }
              local_10 = local_10 + -1;
            } while (local_10 != 0);
            if ((((399 < iVar5) || (param_3 == '\0')) &&
                ((0 < iVar5 && ((*pcVar6 != '\x02' || (param_3 != '\0')))))) &&
               (iVar4 = FUN_0045dc50((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                                     (int)*(short *)(&DAT_007999e0 + iVar4),
                                     (int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                                     (int)*(short *)(&DAT_007999e0 + iVar7),
                                     (int)(short)(&DAT_007999f8)[param_1 * 0x84],
                                     (int)(short)(&DAT_007999f8)[iVar8 * 0x84]), iVar4 < local_1c))
            {
              local_1c = iVar4;
              local_14 = iVar8;
            }
          }
          else if (((bVar2) && ((&DAT_007999e8)[iVar8 * 0x84] == 0x47)) &&
                  ((399 < (short)(&DAT_00799a2e)[param_2 + iVar8 * 0x84] &&
                   ((*pcVar6 != '\x02' &&
                    (iVar4 = FUN_0045dc50((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                                          (int)*(short *)(&DAT_007999e0 + iVar4),
                                          (int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                                          (int)*(short *)(&DAT_007999e0 + iVar7),
                                          (int)(short)(&DAT_007999f8)[param_1 * 0x84],
                                          (int)(short)(&DAT_007999f8)[iVar8 * 0x84]),
                    iVar4 < local_1c)))))) {
            local_1c = iVar4;
            local_14 = iVar8;
          }
        }
        local_c = local_c + 1;
        pcVar6 = pcVar6 + 0xc4;
      } while (local_c <= DAT_0078d788);
      if (0 < local_14) {
        _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[local_14 * 0x84];
        _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[local_14 * 0x84];
        return local_14;
      }
    }
    return 0;
  }
}
