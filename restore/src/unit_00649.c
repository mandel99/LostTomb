#include "restore_types.h"
#define DAT_00705c1c (*(undefined *)0x00705c1cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de58 (*(undefined4 *)0x0077de58u)
#define DAT_0077de6f (*(undefined4 *)0x0077de6fu)
#define DAT_0078d728 (*(undefined *)0x0078d728u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd667 (*(undefined4 *)0x00bcd667u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00e9519c (*(undefined *)0x00e9519cu)
#define DAT_00ea3fe8 (*(undefined4 *)0x00ea3fe8u)
#define DAT_00ea3fea (*(undefined4 *)0x00ea3feau)
#define DAT_00ea5cec (*(undefined4 *)0x00ea5cecu)
#define DAT_00ea5cee (*(undefined4 *)0x00ea5ceeu)
#define DAT_00ea5cf0 (*(undefined4 *)0x00ea5cf0u)
#define DAT_00ea5cf2 (*(undefined4 *)0x00ea5cf2u)
#define DAT_00ea5f14 (*(undefined4 *)0x00ea5f14u)
#define DAT_00ea6024 (*(undefined4 *)0x00ea6024u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
extern bool FUN_00447790();
extern int FUN_004b4800();
extern void FUN_004e0cf0();
extern int FUN_00559fc0();

void FUN_00444c20(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  int local_44;
  uint local_40 [16];
  
  iVar5 = DAT_00eaac28;
  (&DAT_00ea5cec)[DAT_00eaac28 * 0x24ec] = 0;
  (&DAT_00ea5cee)[iVar5 * 0x24ec] = 0;
  (&DAT_00ea5f14)[iVar5 * 0x1276] = 0;
  local_40[1] = 0;
  local_40[2] = 0;
  local_40[3] = 0;
  local_40[4] = 0;
  local_40[5] = 0;
  local_40[6] = 0;
  local_40[7] = 0;
  local_40[8] = 0;
  local_40[9] = 0;
  local_40[10] = 0;
  local_40[0xb] = 0;
  local_40[0xc] = 0;
  local_40[0xd] = 0;
  local_40[0xe] = 0;
  local_40[0xf] = 0;
  local_40[0] = (uint)(0 < DAT_0078d728);
  iVar3 = FUN_00559fc0(local_40);
  (&DAT_00ea5f14)[iVar5 * 0x1276] = (&DAT_00ea5f14)[iVar5 * 0x1276] + iVar3;
  if ((short)(&DAT_00ea5cf0)[iVar5 * 0x24ec] < 1) {
    (&DAT_00ea5cf0)[iVar5 * 0x24ec] = 0;
  }
  else {
    (&DAT_00ea5cf0)[iVar5 * 0x24ec] = (&DAT_00ea5cf0)[iVar5 * 0x24ec] + -1;
  }
  if ((short)(&DAT_00ea5cf2)[iVar5 * 0x24ec] < 1) {
    (&DAT_00ea5cf2)[iVar5 * 0x24ec] = 0;
  }
  else {
    (&DAT_00ea5cf2)[iVar5 * 0x24ec] = (&DAT_00ea5cf2)[iVar5 * 0x24ec] + -1;
  }
  local_44 = 1;
  pcVar6 = &DAT_0077de6f;
  do {
    iVar5 = 0;
    if ((pcVar6[-5] != '\0') && (*pcVar6 != '\0')) {
      if (pcVar6[0x53] == '\x01') {
        if (DAT_00705c1c < 1) {
          if (DAT_00e9519c < 1) {
            DAT_00e950dc = 1;
            FUN_004e0cf0(0x13c,0,0,0,0);
            DAT_00e9519c = 0x180;
          }
          else {
            DAT_00e9519c = DAT_00e9519c + -1;
          }
        }
        else if ((_DAT_00784c8c != -1) || (_DAT_00784c8e != -1)) {
          psVar4 = &DAT_00ea5cec + DAT_00eaac28 * 0x24ec;
          goto LAB_00444da3;
        }
      }
      else {
        psVar4 = &DAT_00ea5cee + DAT_00eaac28 * 0x24ec;
LAB_00444da3: ;
        *psVar4 = *psVar4 + 1;
        iVar3 = 0;
        iVar7 = 1;
        do {
          if (((pcVar6[iVar7 + 1] != '\0') && (cVar2 = FUN_00447790(local_44,iVar7), cVar2 != '\0'))
             || ((pcVar6[iVar7 + 0x25] != '\0' &&
                 (cVar2 = FUN_00447790(local_44,iVar7), cVar2 != '\0')))) {
            iVar5 = iVar5 + 1;
            iVar1 = *(int *)(&DAT_0077646c + (iVar7 + (uint)(byte)pcVar6[-1] * 0x24) * 4);
            if (iVar1 < 0x1a) {
              if (iVar1 < 0x10) {
                if (0 < iVar1) {
                  iVar3 = iVar3 + 1;
                }
              }
              else {
                iVar3 = iVar3 + 2;
              }
            }
            else {
              iVar3 = iVar3 + 3;
            }
          }
          iVar7 = iVar7 + 1;
        } while (iVar7 < 0x24);
        if (1 < iVar5) {
          if (iVar3 % iVar5 == 0) {
            iVar3 = iVar3 / iVar5;
          }
          else {
            iVar3 = iVar3 / iVar5 + 1;
          }
        }
        if ((0 < iVar3) && (*(short *)(pcVar6 + 0x4f) == 0)) {
          if (iVar3 == 1) {
            if (*(short *)(pcVar6 + 0x55) != 0) goto LAB_00444f37;
            iVar5 = 0;
          }
          else if (iVar3 == 2) {
            if (*(short *)(pcVar6 + 0x55) == 0) {
              iVar5 = 0;
            }
            else {
              if (*(short *)(pcVar6 + 0x57) != 0) goto LAB_00444f37;
              iVar5 = 1;
            }
          }
          else if (*(short *)(pcVar6 + 0x55) == 0) {
            iVar5 = 0;
          }
          else if (*(short *)(pcVar6 + 0x57) == 0) {
            iVar5 = 1;
          }
          else {
            if (*(short *)(pcVar6 + 0x59) != 0) goto LAB_00444f37;
            iVar5 = 2;
          }
          if (*(short *)(pcVar6 + 0x4d) < 1) {
            *(ushort *)(pcVar6 + 0x4d) = (-(ushort)(pcVar6[0x53] != '\x01') & 0xffe6) + 0x1e;
            if (pcVar6[0x53] == '\x01') {
              if (((0 < DAT_00705c1c) &&
                  (((_DAT_00784c8c != -1 || (_DAT_00784c8e != -1)) &&
                   ((&DAT_00ea5cf0)[DAT_00eaac28 * 0x24ec] == 0)))) &&
                 (*(int *)(&DAT_00ea6024 + DAT_00eaac28 * 0x49d8) < 1)) {
                iVar3 = FUN_004b4800(DAT_00eaac28,0x14,(int)_DAT_00784c8c,(int)_DAT_00784c8e,0);
                iVar7 = iVar3 * 0x184;
                *(undefined2 *)(&DAT_00bcd66c + iVar7) = 0xc;
                (&DAT_00bcd667)[iVar7] = 0;
                (&DAT_00bcd666)[iVar7] = (char)local_44;
                (&DAT_00bcd630)[iVar7] = 8;
                *(undefined2 *)(&DAT_00bcd62e + iVar7) = 10;
                *(short *)((int)&DAT_0077de58 + (local_44 * 0x35 + iVar5) * 2 + 2) = (short)iVar3;
                (&DAT_00bcd672)[iVar7] = 0;
                return;
              }
            }
            else if (((&DAT_00ea5cf2)[DAT_00eaac28 * 0x24ec] == 0) &&
                    (*(int *)(&DAT_00ea6024 + DAT_00eaac28 * 0x49d8) < 1)) {
              iVar3 = FUN_004b4800(DAT_00eaac28,0x13,
                                   (int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
                                   (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0);
              *(short *)((int)&DAT_0077de58 + (local_44 * 0x35 + iVar5) * 2 + 2) = (short)iVar3;
              iVar5 = iVar3 * 0x184;
              (&DAT_00bcd666)[iVar5] = (char)local_44;
              (&DAT_00bcd630)[iVar5] = 8;
              *(undefined2 *)(&DAT_00bcd62e + iVar5) = 10;
              iVar5 = FUN_004b4800(DAT_00eaac28,0x15,
                                   (int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
                                   (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0);
              (&DAT_00bcd630)[iVar5 * 0x184] = 8;
              *(short *)(&DAT_00bcd660 + iVar5 * 0x184) = (short)iVar3;
              iVar3 = FUN_004b4800(DAT_00eaac28,0x15,
                                   (int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
                                   (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0);
              (&DAT_00bcd630)[iVar3 * 0x184] = 8;
              *(short *)(&DAT_00bcd660 + iVar3 * 0x184) = (short)iVar5;
              return;
            }
          }
          else {
            *(short *)(pcVar6 + 0x4d) = *(short *)(pcVar6 + 0x4d) + -1;
          }
        }
      }
    }
LAB_00444f37: ;
    pcVar6 = pcVar6 + 0x6a;
    local_44 = local_44 + 1;
    if (0x77f746 < (int)pcVar6) {
      return;
    }
  } while( true );
}
