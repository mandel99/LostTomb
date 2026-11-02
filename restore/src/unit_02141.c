#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaae8c (*(undefined *)0x00eaae8cu)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern int FUN_0046a400();
extern void FUN_00470990();
extern void FUN_0047bae0();
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern void FUN_004edf50();
extern void FUN_004ee5d0();
extern undefined4 FUN_004ef100();
extern void FUN_004ef200();
extern void FUN_004ef450();
extern void FUN_004f25c0();
extern void FUN_004f31f0();
extern undefined4 FUN_004f3850();
extern void FUN_00517370();
extern void FUN_00517660();

void FUN_004f3890(void)

{
  byte bVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar5 = 1;
  iVar7 = DAT_00bca2e0 / 2;
  do {
    iVar6 = iVar5 * 0x108;
    if (((&DAT_007999d8)[iVar6] == '\x01') && ((&DAT_007999e8)[iVar5 * 0x84] == 0xb7)) {
      if (*(byte *)((int)&DAT_00799a2c + iVar6 + 1) < 2) {
        FUN_004ef450(iVar5);
        FUN_004f25c0();
        iVar4 = DAT_00eaac44;
        if (DAT_00eaaf58 < 1) {
          bVar1 = *(byte *)(&DAT_00799a2c + iVar5 * 0x84);
          cVar2 = FUN_004ecea0((uint)bVar1);
          if (cVar2 != '\0') {
            iVar6 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + iVar6)) -
                    DAT_00bca2d8;
            FUN_004ed360(iVar6 % 0xe4,iVar6 / 0xe4,3,0);
            if (DAT_00eaae90 != 0) {
              iVar4 = (DAT_00eaae90 - DAT_00bca2d8) % 0xe4;
              iVar6 = (DAT_00eaae90 - DAT_00bca2d8) / 0xe4;
              uVar3 = FUN_0046a400(DAT_00eaac28,DAT_00eaae8c,iVar4,iVar6,0,0);
              FUN_00470990(DAT_00eaac28,DAT_00eaae8c,uVar3,iVar4,iVar6,2,1,0);
              FUN_0047bae0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
              FUN_00517370();
              FUN_00517660();
            }
          }
        }
        else {
          FUN_004f31f0(DAT_00eaac44);
          iVar6 = FUN_004ef100(iVar4);
          if (iVar6 != 0) {
            FUN_004ef200(iVar4);
          }
          FUN_004edf50(iVar4,iVar7,
                       *(undefined1 *)
                        (&DAT_00799a2c + (iVar4 - (short)(&DAT_007999ea)[iVar4 * 0x84]) * 0x84));
        }
      }
      else {
        cVar2 = FUN_004f3850();
        if (cVar2 == '\0') {
          FUN_004ee5d0(iVar5);
        }
        FUN_00517370();
      }
      if (DAT_005e4ef4 == 5) {
        iVar6 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
      }
      else {
        iVar6 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
      }
      iVar5 = iVar5 + -1 + iVar6;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 4000);
  return;
}
