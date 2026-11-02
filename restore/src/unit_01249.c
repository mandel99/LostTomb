#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00978d88 (*(undefined *)0x00978d88u)
#define DAT_00978d8c (*(undefined *)0x00978d8cu)
#define DAT_00978d90 (*(undefined *)0x00978d90u)
#define DAT_00978d94 (*(undefined *)0x00978d94u)
#define DAT_00978d98 (*(char *)0x00978d98u)
#define DAT_00978d99 (*(undefined1 *)0x00978d99u)
#define DAT_00978d9a (*(char *)0x00978d9au)
#define DAT_00978d9b (*(undefined1 *)0x00978d9bu)
#define DAT_00a43eac (*(undefined4 *)0x00a43eacu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_00486600();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern int FUN_004cde40();
extern int FUN_005190a0();

void FUN_004860d0(void)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  short *psVar10;
  int local_18;
  int local_14;
  undefined4 local_10;
  uint *local_c;
  int local_8;
  int local_4;
  
  iVar5 = 3999;
  DAT_00978d98 = '\0';
  DAT_00978d99 = 0;
  DAT_00978d9a = '\0';
  DAT_00978d9b = 0;
  local_10 = 100;
  local_18 = 0;
  local_14 = 3999;
  if ((((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) &&
      (iVar2 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar2 == 0)) &&
     (cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2), cVar1 != '\0')) {
    local_10 = 0x7d;
  }
  local_4 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar2 = (&DAT_00bc9804)[local_4];
      if ((int)(&DAT_00bc9f24)[local_4] < (int)(&DAT_00bc9b94)[local_4]) {
        local_c = &DAT_00af598c + iVar2;
        local_8 = (&DAT_00bc9b94)[local_4] - (&DAT_00bc9f24)[local_4];
        do {
          if ((*local_c & 1) == 0) {
            if ((*local_c & 0x40000) != 0) {
              if (*(char *)((int)&DAT_00a43eac + iVar2) == -1) {
                if (DAT_00978d9a == '\0') {
                  (&DAT_0096e6b4)[iVar5] = iVar2;
                  iVar5 = iVar5 + -1;
                  local_14 = iVar5;
                }
              }
              else {
                (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
                DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
                if (99 < DAT_00d3b1c4) {
                  DAT_00d3b1c4 = 0;
                }
                iVar5 = 0x1f;
                do {
                  uVar3 = DAT_00d3b384 >> 4;
                  uVar8 = DAT_00d3b384 & 1;
                  uVar9 = DAT_00d3b380 >> 4;
                  uVar6 = DAT_00d3b380 & 1;
                  DAT_00d3b384 = DAT_00d3b384 >> 1;
                  DAT_00d3b380 = DAT_00d3b380 >> 1;
                  if ((uVar3 & 1) != uVar8) {
                    DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
                  }
                  if ((uVar9 & 1) != uVar6) {
                    DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
                  }
                  iVar5 = iVar5 + -1;
                } while (iVar5 != 0);
                _DAT_00d3b378 = DAT_00d3b384 & 7;
                _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
                _DAT_00d3b36c = DAT_00d3b380 & 7;
                _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
                _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
                DAT_00d3b374 = DAT_00d3b384 & 0x7f;
                cVar1 = FUN_004cde40(_DAT_00d3b370 % 10 + DAT_00978d90 + DAT_00978d88,local_10);
                *(char *)((int)&DAT_00a43eac + iVar2) =
                     *(char *)((int)&DAT_00a43eac + iVar2) + cVar1;
                FUN_00486600(iVar2);
                iVar5 = local_14;
              }
              DAT_00978d9b = 1;
            }
          }
          else if (*(char *)((int)&DAT_00a43eac + iVar2) == -1) {
            if (DAT_00978d98 == '\0') {
              (&DAT_0096e6b4)[local_18] = iVar2;
              local_18 = local_18 + 1;
            }
            DAT_00978d99 = 1;
          }
          else {
            (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
            DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
            if (99 < DAT_00d3b1c4) {
              DAT_00d3b1c4 = 0;
            }
            iVar5 = 0x1f;
            do {
              uVar3 = DAT_00d3b384 >> 4;
              uVar8 = DAT_00d3b384 & 1;
              uVar9 = DAT_00d3b380 >> 4;
              uVar6 = DAT_00d3b380 & 1;
              DAT_00d3b384 = DAT_00d3b384 >> 1;
              DAT_00d3b380 = DAT_00d3b380 >> 1;
              if ((uVar3 & 1) != uVar8) {
                DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
              }
              if ((uVar9 & 1) != uVar6) {
                DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
              }
              iVar5 = iVar5 + -1;
            } while (iVar5 != 0);
            _DAT_00d3b378 = DAT_00d3b384 & 7;
            _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
            _DAT_00d3b36c = DAT_00d3b380 & 7;
            _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
            _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
            DAT_00d3b374 = DAT_00d3b384 & 0x7f;
            cVar1 = FUN_004cde40(_DAT_00d3b370 % 10 + DAT_00978d8c + DAT_00978d94,local_10);
            *(char *)((int)&DAT_00a43eac + iVar2) = *(char *)((int)&DAT_00a43eac + iVar2) + cVar1;
            FUN_00486600(iVar2);
            DAT_00978d99 = 1;
            iVar5 = local_14;
          }
          if (local_18 == iVar5) {
            iVar7 = 1;
            if (0 < DAT_0078d790) {
              psVar10 = &DAT_00799af0;
              do {
                if (((char)psVar10[-8] == '\x01') && (psVar10[0x18] != 0)) {
                  if ((*psVar10 == 0x6c) && (DAT_00978d98 == '\0')) {
                    iVar4 = FUN_005190a0((int)psVar10[0xb],(int)psVar10[0xc],&DAT_0096e6b4,local_18)
                    ;
                    if (iVar4 != 0) {
                      DAT_00978d98 = '\x01';
                    }
                  }
                  else if (((*psVar10 == 0xc3) && (DAT_00978d9a == '\0')) &&
                          (iVar4 = FUN_005190a0((int)psVar10[0xb],(int)psVar10[0xc],
                                                &DAT_0096e6b4 + iVar5,3999 - iVar5), iVar4 != 0)) {
                    DAT_00978d9a = '\x01';
                  }
                }
                iVar7 = iVar7 + 1;
                psVar10 = psVar10 + 0x84;
              } while (iVar7 <= DAT_0078d790);
            }
            iVar5 = 3999;
            local_18 = 0;
            local_14 = 3999;
          }
          iVar2 = iVar2 + 1;
          local_c = local_c + 1;
          local_8 = local_8 + -1;
        } while (local_8 != 0);
      }
      local_4 = local_4 + 1;
    } while (local_4 < DAT_00bca2d4);
    if (((local_18 != 0) || (iVar5 < 3999)) && (iVar2 = 1, 0 < DAT_0078d790)) {
      psVar10 = &DAT_00799af0;
      do {
        if (((char)psVar10[-8] == '\x01') && (psVar10[0x18] != 0)) {
          if ((*psVar10 == 0x6c) && (DAT_00978d98 == '\0')) {
            iVar7 = FUN_005190a0((int)psVar10[0xb],(int)psVar10[0xc],&DAT_0096e6b4,local_18);
            if (iVar7 != 0) {
              DAT_00978d98 = '\x01';
            }
          }
          else if (((*psVar10 == 0xc3) && (DAT_00978d9a == '\0')) &&
                  (iVar7 = FUN_005190a0((int)psVar10[0xb],(int)psVar10[0xc],&DAT_0096e6b4 + iVar5,
                                        3999 - iVar5), iVar7 != 0)) {
            DAT_00978d9a = '\x01';
          }
        }
        iVar2 = iVar2 + 1;
        psVar10 = psVar10 + 0x84;
      } while (iVar2 <= DAT_0078d790);
    }
  }
  return;
}
