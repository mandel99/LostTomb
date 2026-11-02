#include "restore_types.h"
#define DAT_00784e78 (*(undefined4 *)0x00784e78u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
extern int FUN_004b28f0();
extern void FUN_004cc110();
extern void FUN_004e0cc0();
extern void FUN_004f6420();
extern undefined4 FUN_00545a30();
extern undefined4 FUN_0054f8a0();
extern undefined4 FUN_00556230();
extern undefined4 FUN_00558f00();

undefined4 FUN_00492d20(undefined4 param_1)

{
  short sVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  short *psVar6;
  int local_8;
  int local_4;
  
  DAT_0096e6b0 = 0;
  bVar2 = false;
  FUN_004cc110(0x133,(int)DAT_00e38ea8);
  piVar4 = &DAT_00784e78;
  iVar3 = -1;
  do {
    switch((int)(iVar3)) {
    case 0: ;
    case 7: ;
    case 9: ;
    case 0xc: ;
    case 0xe: ;
    case 0x10: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x16: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1b: ;
    case 0x1d: ;
      if (0 < *piVar4) {
        bVar2 = true;
        goto LAB_00492d7a;
      }
    }
    piVar4 = piVar4 + 1;
    iVar5 = iVar3 + 2;
    iVar3 = iVar3 + 1;
  } while (iVar5 < 0x24);
LAB_00492d7a: ;
  iVar5 = 1;
  iVar3 = DAT_0096e6b0;
  if (0 < DAT_0078d790) {
    psVar6 = &DAT_00799af0;
    do {
      if ((char)psVar6[-8] != '\0') {
        sVar1 = *psVar6;
        if ((sVar1 == 0xde) && ((psVar6[3] == -1 || (bVar2)))) {
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_004e0cc0(iVar5,&local_4,&local_8);
        }
        else if ((sVar1 == 0xb7) && (bVar2)) {
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_004f6420(iVar5,&local_4,&local_8);
        }
        else if ((sVar1 == 0xe5) &&
                (((psVar6[1] == 0 && (psVar6[3] == -1)) || ((psVar6[1] == 0 && (bVar2)))))) {
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_00545a30(iVar5,&local_4,&local_8);
        }
        else if ((sVar1 == 0xea) &&
                (((psVar6[1] == 0 && (psVar6[3] == -1)) || ((psVar6[1] == 0 && (bVar2)))))) {
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_00558f00(iVar5,&local_4,&local_8);
        }
        else if ((sVar1 == 0xeb) &&
                (((psVar6[1] == 0 && (psVar6[3] == -1)) || ((psVar6[1] == 0 && (bVar2)))))) {
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_00556230(iVar5,&local_4,&local_8);
        }
        else {
          if ((sVar1 != 0xec) ||
             (((psVar6[1] != 0 || (psVar6[3] != -1)) && ((psVar6[1] != 0 || (!bVar2))))))
          goto LAB_00492f46;
          (&DAT_00972534)[iVar3] = iVar5;
          FUN_0054f8a0(iVar5,&local_4,&local_8);
        }
        (&DAT_0096e6b4)[DAT_0096e6b0] = DAT_00bca2d8 + local_8 * 0xe4 + local_4;
        iVar3 = DAT_0096e6b0 + 1;
        DAT_0096e6b0 = iVar3;
      }
LAB_00492f46: ;
      iVar5 = iVar5 + 1;
      psVar6 = psVar6 + 0x84;
    } while (iVar5 <= DAT_0078d790);
  }
  if ((iVar3 != 0) && (iVar3 = FUN_004b28f0(param_1,&DAT_0096e6b4,iVar3), 0 < iVar3)) {
    return (&DAT_00972530)[iVar3];
  }
  return 0;
}
