#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ee (*(undefined4 *)0x007999eeu)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00ea5fac (*(undefined4 *)0x00ea5facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004f4b70();

void FUN_004f61e0(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  iVar2 = 3;
  puVar1 = (undefined4 *)(&DAT_00ea5fac + DAT_00eaac28 * 0x49d8);
  do {
    puVar1[-3] = 0;
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  iVar2 = 1;
  if (0 < DAT_0078d790) {
    do {
      iVar5 = iVar2 * 0x108;
      iVar4 = iVar2;
      if ((((&DAT_007999d8)[iVar5] != '\x01') && ((&DAT_007999d8)[iVar5] != '\x03')) ||
         (*(short *)(&DAT_007999ee + iVar5) == -1)) goto switchD_004f6261_caseD_b8;
      switch((&DAT_007999e8)[iVar2 * 0x84]) {
      case 0xb7: ;
        iVar5 = FUN_004f4b70(iVar2);
        if (DAT_005e4ef4 == 5) {
          iVar4 = iVar2 + -1 +
                  *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar2 * 0x84) * 4);
        }
        else {
          iVar4 = iVar2 + -1 +
                  *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar2 * 0x84) * 4);
        }
        break;
default: ;
        goto switchD_004f6261_caseD_b8;
      case 0xcf: ;
      case 0xd0: ;
      case 0xe4: ;
switchD_004f6261_caseD_cf: ;
        iVar5 = FUN_004f4b70(iVar2);
        break;
      case 0xd2: ;
        if (*(short *)(&DAT_00799a10 + iVar5) == 0) goto switchD_004f6261_caseD_cf;
        goto switchD_004f6261_caseD_b8;
      case 0xd5: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 99;
        break;
      case 0xde: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0x73;
        break;
      case 0xe1: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0x18;
        break;
      case 0xe3: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 8;
        break;
      case 0xe5: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0xa6;
        break;
      case 0xe6: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0xf;
        break;
      case 0xea: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0x9d;
        break;
      case 0xeb: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0x1b4;
        break;
      case 0xec: ;
        iVar5 = FUN_004f4b70(iVar2);
        iVar4 = iVar2 + 0x200;
      }
      if (0 < iVar5) {
        iVar3 = 0;
        psVar6 = &DAT_00784de0;
        do {
          if ((iVar5 == *psVar6) &&
             (*(int *)(&DAT_00ea5fac + (DAT_00eaac28 * 0x1276 + iVar3) * 4) == 0)) {
            if (iVar3 < 3) {
              iVar3 = DAT_00eaac28 * 0x1276 + iVar3;
              (&DAT_00ea5fa0)[iVar3] = iVar5;
              *(int *)(&DAT_00ea5fac + iVar3 * 4) = iVar2;
            }
            break;
          }
          psVar6 = psVar6 + 1;
          iVar3 = iVar3 + 1;
        } while ((int)psVar6 < 0x784de6);
      }
switchD_004f6261_caseD_b8: ;
      iVar2 = iVar4 + 1;
    } while (iVar2 <= DAT_0078d790);
  }
  return;
}
