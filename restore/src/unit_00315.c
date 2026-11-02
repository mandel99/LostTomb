#include "restore_types.h"
#define DAT_005be9bc (*(undefined4 *)0x005be9bcu)
#define DAT_005be9c4 (*(undefined4 *)0x005be9c4u)
#define DAT_005be9cc (*(undefined4 *)0x005be9ccu)
#define DAT_005be9d8 (*(undefined4 *)0x005be9d8u)
#define DAT_005be9dc (*(undefined4 *)0x005be9dcu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_010d6a1a (*(undefined4 *)0x010d6a1au)
#define DAT_010d6a1c (*(undefined4 *)0x010d6a1cu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
#define PTR_DAT_005be9b8 (*(unsigned char * *)0x005be9b8u)
#define PTR_DAT_005be9c0 (*(unsigned char * *)0x005be9c0u)
#define PTR_DAT_005be9c8 (*(unsigned char * *)0x005be9c8u)

void FUN_0041fad0(int *param_1,int *param_2,int *param_3,int param_4)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *in_ECX;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  
  *param_1 = 0;
  *param_2 = 0;
  *param_3 = 0;
  if ((((short)in_ECX[4] != 3) && (iVar4 = *in_ECX, -1 < iVar4)) && (iVar4 < 0xfd)) {
    iVar6 = in_ECX[1];
    iVar5 = iVar4 * 0x28;
    if ((-1 < iVar6) && (iVar6 < *(int *)(&DAT_005be9cc + iVar5))) {
      piVar8 = (int *)((&PTR_DAT_005be9c8)[iVar4 * 10] + iVar6 * 0x30);
      param_4 = param_4 - DAT_00bca2e0 / 2;
      if (param_4 < 0) {
        param_4 = param_4 + 4;
      }
      if (*(char *)((int)in_ECX + 0x15) == '\0') {
        iVar6 = piVar8[param_4 + 4];
      }
      else {
        iVar6 = piVar8[param_4 + 8];
      }
      if (((-1 < iVar6) && (iVar3 = piVar8[2], -1 < iVar3)) &&
         (iVar3 < *(int *)(&DAT_005be9c4 + iVar5))) {
        piVar7 = (int *)((&PTR_DAT_005be9c0)[iVar4 * 10] + iVar3 * 0x10);
        if ((((-1 < *piVar8) && (*piVar8 < *(int *)(&DAT_005be9bc + iVar5))) && (-1 < piVar8[1])) &&
           (piVar8[1] < *(int *)(&DAT_005be9bc + iVar5))) {
          piVar2 = (int *)((&PTR_DAT_005be9b8)[iVar4 * 10] + (param_4 + *piVar8 * 4) * 8);
          piVar1 = (int *)((&PTR_DAT_005be9b8)[iVar4 * 10] + (param_4 + piVar8[1] * 4) * 8);
          iVar4 = in_ECX[2];
          if ((-1 < iVar4) && (iVar4 <= piVar8[3])) {
            *param_3 = (iVar4 % piVar7[2]) * piVar7[3] + (&DAT_00f55bb4)[*piVar7 * 2] + piVar7[1] +
                       iVar6;
            iVar4 = *piVar2;
            *param_1 = ((*piVar1 - iVar4) * in_ECX[2]) / piVar8[3] + *(int *)(&DAT_005be9d8 + iVar5)
                       + iVar4;
            iVar4 = piVar2[1];
            *param_2 = ((piVar1[1] - iVar4) * in_ECX[2]) / piVar8[3] +
                       *(int *)(&DAT_005be9dc + iVar5) + iVar4;
            iVar4 = *param_3;
            iVar5 = iVar4;
            if (DAT_0126517c <= iVar4) {
              iVar5 = 0;
            }
            *param_1 = *param_1 - (int)(short)(&DAT_010d6a1a)[iVar5 * 0x20];
            if (DAT_0126517c <= iVar4) {
              iVar4 = 0;
            }
            *param_2 = *param_2 - (int)(short)(&DAT_010d6a1c)[iVar4 * 0x20];
          }
        }
      }
    }
  }
  return;
}
