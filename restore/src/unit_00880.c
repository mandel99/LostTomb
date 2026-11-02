#include "restore_types.h"
#define DAT_005795ac (*(undefined *)0x005795acu)
#define DAT_005795c8 (*(undefined *)0x005795c8u)
#define DAT_0057961c (*(undefined *)0x0057961cu)
#define DAT_007027c1 (*(char *)0x007027c1u)
#define DAT_007027c2 (*(char *)0x007027c2u)
#define DAT_007027c4 (*(char *)0x007027c4u)
#define DAT_007027c5 (*(char *)0x007027c5u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00799af2 (*(undefined4 *)0x00799af2u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea60d0 (*(undefined4 *)0x00ea60d0u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00ea6190 (*(undefined4 *)0x00ea6190u)
#define DAT_00ea6194 (*(undefined4 *)0x00ea6194u)
#define DAT_00ea619c (*(undefined4 *)0x00ea619cu)
extern void FUN_00417ae0();
extern int FUN_00475b50();
extern undefined4 FUN_00480180();

void FUN_0045aae0(int param_1)

{
  short sVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  iVar4 = 0x24;
  iVar5 = param_1 * 0x49d8;
  puVar2 = &DAT_00ea6160 + param_1 * 0x1276;
  do {
    puVar2[-0x24] = 0;
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  iVar4 = 1;
  if (0 < DAT_0078d790) {
    psVar6 = &DAT_00799af0;
    do {
      if (((char)psVar6[-8] == '\x01') && ((*psVar6 == 0x48 || (*psVar6 == 0xc1)))) {
        *(undefined1 *)(psVar6 + 0x1a) = 0;
        iVar3 = FUN_00480180((int)psVar6[-5],(int)psVar6[-4],*(undefined1 *)((int)psVar6 + -0xd));
        if (iVar3 == 0) {
          iVar3 = FUN_00480180((int)psVar6[-5],(int)psVar6[-4],3);
          if (iVar3 != 0) {
            *(undefined1 *)(psVar6 + 0x1a) = 2;
          }
        }
        else {
          *(undefined1 *)(psVar6 + 0x1a) = 1;
        }
      }
      iVar4 = iVar4 + 1;
      psVar6 = psVar6 + 0x84;
    } while (iVar4 <= DAT_0078d790);
  }
  iVar4 = 1;
  if (0 < DAT_0078d790) {
    psVar6 = &DAT_00799af2;
    do {
      if (((char)psVar6[-9] == '\x01') && (psVar6[-1] == 0x49)) {
        iVar3 = FUN_00475b50(iVar4);
        if ((&DAT_00799a1c)[iVar3 * 0x108] != '\0') {
          *(undefined *)(psVar6 + 0x19) = (&DAT_00799a1c)[iVar3 * 0x108];
          if (*psVar6 == 0) {
            (&DAT_00ea60d0)[param_1 * 0x1276] = (&DAT_00ea60d0)[param_1 * 0x1276] + 400;
          }
          else {
            sVar1 = psVar6[0x15];
            iVar3 = param_1 * 0x1276 + (int)*psVar6;
            (&DAT_00ea60d0)[iVar3] = (&DAT_00ea60d0)[iVar3] + (400 - sVar1);
            (&DAT_00ea6160)[iVar3] = (&DAT_00ea6160)[iVar3] + (int)sVar1;
          }
        }
      }
      iVar4 = iVar4 + 1;
      psVar6 = psVar6 + 0x84;
    } while (iVar4 <= DAT_0078d790);
  }
  if ((((DAT_00e38ea8 == DAT_005795ac) && (DAT_007027c1 == '\0')) && (DAT_00e38ebc == 0)) &&
     (199 < *(int *)(&DAT_00ea6194 + iVar5))) {
    FUN_00417ae0(9);
  }
  if (((DAT_00e38ea8 == DAT_005795c8) && (DAT_007027c2 == '\0')) &&
     ((DAT_00e38ebc == 0 && (299 < *(int *)(&DAT_00ea619c + iVar5))))) {
    FUN_00417ae0(10);
  }
  if (((DAT_00e38ea8 == DAT_0057961c) && (DAT_007027c5 == '\0')) &&
     ((DAT_007027c4 != '\0' && ((DAT_00e38ebc == 0 && (799 < *(int *)(&DAT_00ea6190 + iVar5))))))) {
    FUN_00417ae0(0xd);
  }
  return;
}
