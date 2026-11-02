#include "restore_types.h"
#define DAT_0078a624 (*(char *)0x0078a624u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00ba746c (*(undefined *)0x00ba746cu)
#define DAT_00ba7888 (*(undefined *)0x00ba7888u)
#define DAT_00ba7898 (*(undefined4 *)0x00ba7898u)
#define DAT_00bc95f0 (*(undefined *)0x00bc95f0u)
#define DAT_00bc9688 (*(undefined *)0x00bc9688u)
#define DAT_00bc968c (*(undefined *)0x00bc968cu)
#define DAT_00bc9690 (*(undefined *)0x00bc9690u)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
extern void FUN_0040f8f0();

void FUN_00485dc0(void)

{
  char cVar1;
  short sVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  short *psVar6;
  
  if ((DAT_00bc9694 == 1) && (DAT_00bc968c == 1)) {
    if ((DAT_00bc9688 < 1) || (DAT_0078a624 != '\0')) {
      DAT_00bc9694 = 0;
      DAT_00e38dca = 1;
      FUN_0040f8f0(&DAT_00ba7888,0x21d68,0);
      FUN_0040f8f0(&DAT_00ba746c,0x41a,0);
    }
    else {
      DAT_00bc9688 = DAT_00bc9688 + -1;
      if (((((((DAT_00bc9690 != 9) && (DAT_00bc9690 != 8)) && (DAT_00bc9690 != 5)) &&
            ((DAT_00bc9690 != 6 && (DAT_00bc9690 != 0xa8)))) &&
           ((DAT_00bc9690 != 0xa9 && ((DAT_00bc9690 != 0x52 && (DAT_00bc9690 != 0x53)))))) &&
          (DAT_00bc9690 != 0x26)) && (DAT_00bc9690 != 0x27)) {
        if (DAT_00bc95f0 < 1) {
          DAT_00bc9694 = 0;
          DAT_00e38dca = 1;
          return;
        }
        if ((DAT_00bc9690 == 10) && (iVar5 = 0, 0 < DAT_00bc95f0)) {
          psVar4 = &DAT_00ba746c;
          do {
            if ((*psVar4 != 0) && ((&DAT_007999f4)[*psVar4 * 0x84] != 0)) {
              DAT_00bc9694 = 0;
              DAT_00e38dca = 1;
              return;
            }
            iVar5 = iVar5 + 1;
            psVar4 = psVar4 + 1;
          } while (iVar5 < DAT_00bc95f0);
        }
        iVar5 = 0;
        if (0 < DAT_00bc95f0) {
          psVar4 = &DAT_00ba7898;
          psVar6 = &DAT_00ba746c;
          do {
            sVar2 = *psVar6;
            if (sVar2 != 0) {
              iVar3 = sVar2 * 0x108;
              cVar1 = (&DAT_007999d8)[iVar3];
              if (cVar1 == '\x02') {
                DAT_00bc9694 = 0;
                DAT_00e38dca = 1;
                return;
              }
              if (cVar1 == '\x04') {
                DAT_00bc9694 = 0;
                DAT_00e38dca = 1;
                return;
              }
              if (cVar1 == '\x05') {
                DAT_00bc9694 = 0;
                DAT_00e38dca = 1;
                return;
              }
              if (*(int *)(&DAT_007999e4 + iVar3) != *(int *)(psVar4 + -2)) {
                DAT_00bc9694 = 0;
                DAT_00e38dca = 1;
                return;
              }
              if ((&DAT_007999e8)[sVar2 * 0x84] != *psVar4) {
                DAT_00bc9694 = 0;
                DAT_00e38dca = 1;
                return;
              }
            }
            iVar5 = iVar5 + 1;
            psVar6 = psVar6 + 1;
            psVar4 = psVar4 + 0x84;
            if (DAT_00bc95f0 <= iVar5) {
              return;
            }
          } while( true );
        }
      }
    }
  }
  return;
}
