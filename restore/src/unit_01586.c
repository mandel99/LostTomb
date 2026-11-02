#include "restore_types.h"
#define DAT_005df8b0 (*(undefined4 *)0x005df8b0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56d84 (*(undefined *)0x00f56d84u)
#define DAT_00f56d8c (*(undefined *)0x00f56d8cu)
#define DAT_00f56dac (*(undefined *)0x00f56dacu)
extern void FUN_0047bf50();
extern void FUN_004c0be0();
extern bool FUN_004c52d0();

void FUN_004c0d50(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  if (*(short *)(&DAT_00799a10 + param_1 * 0x108) == 0) {
    uVar3 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84);
    iVar4 = 0;
    if (param_1 != 0) {
      iVar6 = param_1;
      do {
        iVar5 = 0;
        if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
          if ((uVar3 == 2) || (uVar3 == 6)) goto LAB_004c0dcb;
        }
        else if ((uVar3 != 2) && (uVar3 != 6)) {
LAB_004c0dcb: ;
          iVar5 = 3;
        }
        iVar2 = *(int *)(&DAT_005df8b0 +
                        (iVar4 + (DAT_00bca2e0 / 2 + ((int)uVar3 >> 1) * 4) * 3) * 4);
        if (iVar2 == 0x23b) {
          cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1);
          if (cVar1 == '\0') {
            iVar5 = DAT_00f56d8c + iVar5;
            goto LAB_004c0e71;
          }
          iVar2 = DAT_00f56dac + 2;
          if (iVar5 != 0) {
            iVar2 = DAT_00f56dac + 3;
          }
        }
        else {
          if (iVar2 == 0x23a) {
            cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2);
            if (cVar1 != '\0') {
              iVar2 = DAT_00f56dac;
              if (iVar5 != 0) {
                iVar2 = DAT_00f56dac + 1;
              }
              goto LAB_004c0e73;
            }
            iVar5 = DAT_00f56d84 + iVar5;
          }
          else {
            iVar5 = (&DAT_00f55bb4)[iVar2 * 2] + iVar5;
          }
LAB_004c0e71: ;
          iVar2 = iVar5 + param_2;
        }
LAB_004c0e73: ;
        iVar5 = iVar6 * 0x108;
        FUN_0047bf50(iVar6,(int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar5),3,3,iVar2,8);
        iVar6 = (int)*(short *)(&DAT_00799a12 + iVar5);
        iVar4 = iVar4 + 1;
      } while (iVar6 != 0);
    }
    FUN_004c0be0(param_1);
  }
  return;
}
