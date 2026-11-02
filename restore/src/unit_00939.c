#include "restore_types.h"
#define DAT_0078c400 (*(undefined *)0x0078c400u)
#define DAT_0078c404 (*(undefined *)0x0078c404u)
#define DAT_0078c408 (*(undefined *)0x0078c408u)
#define DAT_00a69fdc (*(undefined4 *)0x00a69fdcu)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00ea5f20 (*(undefined4 *)0x00ea5f20u)
#define DAT_00ea5f64 (*(undefined4 *)0x00ea5f64u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0040f8f0();
extern void FUN_004689e0();

void FUN_00468860(void)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_8;
  int local_4;
  
  puVar2 = (undefined4 *)(&DAT_00ea5f20 + DAT_00eaac28 * 0x49d8);
  iVar5 = 1;
  iVar4 = 10;
  do {
    puVar2[-1] = 0;
    *puVar2 = 0;
    puVar2 = puVar2 + 2;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  FUN_0040f8f0(&DAT_00a76aec,0xcb10,0);
  FUN_0040f8f0(&DAT_00a69fdc,0xcb10,0);
  DAT_0078c408 = 1;
  DAT_0078c404 = 0;
  DAT_00bc9768 = DAT_00bca2d8;
  local_4 = 0;
  iVar4 = DAT_00bca2d0;
  if (0 < DAT_00bca2d4) {
    do {
      local_8 = 0;
      if (0 < iVar4) {
        do {
          if ((((byte)(&DAT_00af598c)[DAT_00bc9768] & 0x44) == 0x40) &&
             ((&DAT_00a76aec)[DAT_00bc9768] == '\0')) {
            DAT_0078c400 = 1;
            FUN_004689e0(DAT_00bc9768,iVar5);
            iVar1 = DAT_00eaac28;
            iVar4 = DAT_0078c400;
            iVar3 = 0;
            piVar6 = (int *)(&DAT_00ea5f20 + DAT_00eaac28 * 0x49d8);
LAB_0046893a: ;
            if (DAT_0078c400 <= *piVar6) goto code_r0x0046893e;
            if (iVar3 < 9) {
              puVar2 = (undefined4 *)(&DAT_00ea5f64 + DAT_00eaac28 * 0x49d8);
              iVar7 = 9 - iVar3;
              do {
                *puVar2 = puVar2[-2];
                puVar2[1] = puVar2[-1];
                puVar2 = puVar2 + -2;
                iVar7 = iVar7 + -1;
              } while (iVar7 != 0);
            }
            iVar3 = iVar1 * 0x93b + iVar3;
            (&DAT_00ea5f1c)[iVar3 * 2] = iVar5;
            *(int *)(&DAT_00ea5f20 + iVar3 * 8) = iVar4;
LAB_00468985: ;
            iVar5 = iVar5 + 1;
            iVar4 = DAT_00bca2d0;
          }
          local_8 = local_8 + 1;
          DAT_00bc9768 = DAT_00bc9768 + 1;
        } while (local_8 < iVar4);
      }
      local_4 = local_4 + 1;
      DAT_00bc9768 = DAT_00bc9768 + DAT_00bca2dc;
    } while (local_4 < DAT_00bca2d4);
  }
  return;
code_r0x0046893e: ;
  iVar3 = iVar3 + 1;
  piVar6 = piVar6 + 2;
  if (9 < iVar3) goto LAB_00468985;
  goto LAB_0046893a;
}
