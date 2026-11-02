#include "restore_types.h"
#define DAT_00bcba82 (*(undefined4 *)0x00bcba82u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)

void FUN_004b8130(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  bool bVar7;
  bool bVar8;
  
  psVar4 = &DAT_00bcba82;
  do {
    if (((char)psVar4[-0x15] == '\x01') && (*(char *)((int)psVar4 + 0x3f) == '\0')) {
      sVar1 = psVar4[2];
      sVar2 = psVar4[0x21];
      if (sVar1 < sVar2) {
        iVar5 = (int)sVar2 - (int)sVar1;
      }
      else {
        iVar5 = (int)sVar1 - (int)sVar2;
      }
      sVar1 = psVar4[3];
      sVar2 = psVar4[0x22];
      if (sVar1 < sVar2) {
        iVar6 = (int)sVar2 - (int)sVar1;
      }
      else {
        iVar6 = (int)sVar1 - (int)sVar2;
      }
      if ((char)psVar4[0x23] == '\0') {
        if (psVar4[0x19] == 0) {
          sVar3 = *psVar4;
          if ((((sVar3 != 6) && (sVar3 != 7)) && (sVar3 != 8)) && ((sVar3 != 9 && (sVar3 != 0xc))))
          {
            if ((sVar3 == 1) || (sVar3 == 3)) {
              if (sVar1 < sVar2) {
                *(undefined1 *)((int)psVar4 + 0x41) = 0;
              }
              else {
joined_r0x004b825b: ;
                if (sVar2 < sVar1) {
                  *(undefined1 *)((int)psVar4 + 0x41) = 4;
                }
              }
            }
            else if ((sVar3 == 2) || (sVar3 == 4)) {
              sVar1 = psVar4[2];
              sVar2 = psVar4[0x21];
              bVar8 = SBORROW2(sVar1,sVar2);
              sVar3 = sVar1 - sVar2;
              bVar7 = sVar1 == sVar2;
              if (sVar1 < sVar2) {
                *(undefined1 *)((int)psVar4 + 0x41) = 6;
              }
              else {
LAB_004b8248: ;
                if (!bVar7 && bVar8 == sVar3 < 0) {
                  *(undefined1 *)((int)psVar4 + 0x41) = 2;
                }
              }
            }
            else if ((sVar3 == 0) || (sVar3 == 5)) {
              if (iVar6 < iVar5) {
                sVar1 = psVar4[2];
                sVar2 = psVar4[0x21];
                bVar8 = SBORROW2(sVar1,sVar2);
                sVar3 = sVar1 - sVar2;
                bVar7 = sVar1 == sVar2;
                if (sVar2 <= sVar1) goto LAB_004b8248;
                *(undefined1 *)((int)psVar4 + 0x41) = 6;
              }
              else {
                if (sVar2 <= sVar1) goto joined_r0x004b825b;
                *(undefined1 *)((int)psVar4 + 0x41) = 0;
              }
            }
          }
        }
        else {
          *(undefined *)((int)psVar4 + 0x41) =
              *(undefined *)(0x00bcd609u + (uint)(int)psVar4[-0x11] * 0x184u);
        }
      }
      else {
        *(char *)(psVar4 + 0x23) = (char)psVar4[0x23] + -1;
      }
      psVar4[0x21] = psVar4[2];
      psVar4[0x22] = psVar4[3];
    }
    psVar4 = psVar4 + 0x48;
    if (0xbcd611 < (int)psVar4) {
      return;
    }
  } while( true );
}
