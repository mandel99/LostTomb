#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00ea5b34 (*(undefined4 *)0x00ea5b34u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern bool FUN_004435f0();
extern undefined4 FUN_0045b230();
extern int FUN_004cde60();

undefined4 FUN_0045d180(int param_1,int param_2,int param_3,undefined4 param_4)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  sVar1 = (&DAT_007999e8)[param_3 * 0x84];
  if (((sVar1 == 0x48) || (sVar1 == 0xc1)) &&
     (FUN_004cde60((int)(short)(&DAT_00799a18)[param_3 * 0x84],
                   *(undefined4 *)(&DAT_00705eb8 + sVar1 * 0x28)),
     *(short *)(&DAT_00bcd66c + param_1 * 0x184) < 8)) {
    iVar4 = 0;
    iVar5 = 1;
    iVar6 = (uint)*(byte *)((int)&DAT_00799a98 + param_3 * 0x108 + 1) * 0xc4;
    do {
      cVar2 = *(char *)((int)&DAT_007900c2 + iVar5 + iVar6);
      if (((cVar2 != '\x01') && (cVar2 != '\x03')) &&
         ((cVar2 = FUN_0045b230(param_3,0,iVar5), cVar2 == '\0' &&
          ((cVar2 = FUN_0045b230(param_3,100,iVar5), cVar2 == '\0' &&
           (iVar3 = FUN_004435f0(param_2,param_4,iVar5), iVar3 != 0)))))) {
        iVar4 = iVar4 + 1;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < 0x24);
    if (0 < iVar4) {
      iVar4 = FUN_004435f0(param_2,param_4,(&DAT_00ea5b34)[param_2 * 0x1276]);
      if ((*(char *)((int)&DAT_007900c2 + iVar6 + (&DAT_00ea5b34)[DAT_00eaac28 * 0x1276]) != '\x01')
         && (iVar4 != 0)) {
LAB_0045d2fe: ;
        iVar4 = 0;
        do {
          param_3 = (int)*(short *)(&DAT_00799a12 + param_3 * 0x108);
          if (0 < param_3) {
            if (((short)(&DAT_00799a14)[param_3 * 0x84] < 400) &&
               (((&DAT_00799a14)[param_3 * 0x84] == 0 ||
                (iVar5 = FUN_004435f0(param_2,param_4,(int)(short)(&DAT_007999ea)[param_3 * 0x84]),
                iVar5 != 0)))) {
              return 1;
            }
          }
          iVar4 = iVar4 + 1;
          if (7 < iVar4) {
            return 0;
          }
        } while( true );
      }
      iVar4 = 1;
      do {
        iVar5 = (&DAT_00ea5b34)[param_2 * 0x1276];
        (&DAT_00ea5b34)[param_2 * 0x1276] = iVar5 + 1;
        if (0x23 < iVar5 + 1) {
          (&DAT_00ea5b34)[param_2 * 0x1276] = 1;
        }
        if ((*(char *)((int)&DAT_007900c2 + iVar6 + (&DAT_00ea5b34)[DAT_00eaac28 * 0x1276]) !=
             '\x01') &&
           (iVar5 = FUN_004435f0(param_2,param_4,(&DAT_00ea5b34)[param_2 * 0x1276]), iVar5 != 0))
        goto LAB_0045d2fe;
        iVar4 = iVar4 + 1;
      } while (iVar4 < 0x24);
    }
  }
  return 0;
}
