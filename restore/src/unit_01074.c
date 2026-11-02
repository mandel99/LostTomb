#include "restore_types.h"
#define DAT_005cc77c (*(undefined4 *)0x005cc77cu)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00605a1c (*(undefined *)0x00605a1cu)
#define DAT_00605a20 (*(undefined *)0x00605a20u)
#define DAT_00605a24 (*(undefined *)0x00605a24u)
#define DAT_00605a28 (*(undefined *)0x00605a28u)
#define DAT_00605a30 (*(undefined *)0x00605a30u)
#define DAT_00605a34 (*(undefined *)0x00605a34u)
#define DAT_00784b62 (*(undefined *)0x00784b62u)
#define DAT_00784b6a (*(undefined4 *)0x00784b6au)
#define DAT_00784b72 (*(undefined4 *)0x00784b72u)
#define DAT_00784b82 (*(undefined4 *)0x00784b82u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_00784bde (*(undefined4 *)0x00784bdeu)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00784ccc (*(undefined *)0x00784cccu)
#define DAT_00784dc0 (*(undefined4 *)0x00784dc0u)
#define DAT_00784dcc (*(undefined *)0x00784dccu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca30c (*(char *)0x00bca30cu)
#define DAT_00e92e20 (*(undefined *)0x00e92e20u)
#define _DAT_00784c60 (*(undefined4 *)0x00784c60u)
#define _DAT_00784c62 (*(undefined4 *)0x00784c62u)
#define _DAT_00784c64 (*(undefined4 *)0x00784c64u)
#define _DAT_00784c66 (*(undefined4 *)0x00784c66u)
#define _DAT_00784c68 (*(undefined4 *)0x00784c68u)
#define _DAT_00784c6a (*(undefined4 *)0x00784c6au)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern void FUN_0046ac10();
extern bool FUN_0046f9a0();
extern void FUN_00477960();
extern void FUN_004779f0();
extern void FUN_00477a40();
extern void FUN_00477ab0();
extern void FUN_00477b10();
extern void FUN_00477b70();
extern void FUN_00477bd0();
extern void FUN_00477c20();
extern void FUN_00477c80();
extern void FUN_00477ce0();
extern undefined4 FUN_00477d40();
extern void FUN_00477dd0();
extern undefined1 FUN_00477f60();
extern undefined4 FUN_00477f70();
extern undefined4 FUN_00478090();
extern uint FUN_00478270();
extern void FUN_00478d40();
extern void FUN_00478e10();
extern void FUN_00478ea0();
extern void FUN_00478f40();
extern void FUN_00478fd0();
extern void FUN_00479060();
extern void FUN_004790e0();
extern void FUN_00479170();
extern void FUN_00479200();
extern void FUN_004792b0();
extern void FUN_00479340();
extern void FUN_004793e0();
extern void FUN_00479460();
extern void FUN_00479510();
extern void FUN_0047c480();
extern uint FUN_0047ed60();
extern void FUN_00487450();
extern void FUN_00488e90();
extern void FUN_004bbd40();
extern uint FUN_004ec410();
extern void FUN_0051f780();

void FUN_00478440(int param_1,int param_2)

{
  uint uVar1;
  char cVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  byte bVar6;
  uint uVar7;
  int iVar8;
  short sVar9;
  short sVar10;
  int iVar11;
  int iVar12;
  int *local_c;
  int local_4;
  
  iVar5 = DAT_00605a24;
  DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  uVar1 = (&DAT_00af598c)[DAT_00bc9768];
  uVar7 = uVar1 & 0xeefed73f;
  sVar10 = (short)param_2;
  sVar9 = (short)param_1;
  if (DAT_006059e8 == 0x77) {
    if (uVar7 == 0) {
      _DAT_00784c62 = sVar10;
      _DAT_00784c60 = sVar9;
      return;
    }
  }
  else if (DAT_006059e8 == 0x7f) {
    if ((uVar7 == 0) &&
       (cVar2 = FUN_0046f9a0(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2,4), cVar2 != '\0')) {
      _DAT_00784c66 = sVar10;
      _DAT_00784c64 = sVar9;
      return;
    }
  }
  else if (DAT_006059e8 == 0x80) {
    if ((uVar7 == 0) &&
       (cVar2 = FUN_0046f9a0(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2,4), cVar2 != '\0')) {
      _DAT_00784c6a = sVar10;
      _DAT_00784c68 = sVar9;
      return;
    }
  }
  else if (DAT_006059e8 == 0x7e) {
    if (DAT_00605a28 < 9) {
      if (uVar7 != 0) {
        return;
      }
    }
    else if ((uVar1 & 4) == 0) {
      return;
    }
    cVar2 = FUN_0046f9a0(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2,4);
    iVar5 = DAT_00605a28;
    if (cVar2 != '\0') {
      *(short *)((int)&DAT_00784bbc + DAT_00605a28 * 2 + 2) = sVar9;
      *(short *)(&DAT_00784bde + iVar5 * 2) = sVar10;
      return;
    }
  }
  else if (DAT_006059e8 == 0x84) {
    if ((uVar1 & 4) != 0) {
      *(short *)(&DAT_00784b72 + DAT_00605a24 * 2) = sVar9;
      *(short *)(&DAT_00784b82 + iVar5 * 2) = sVar10;
      return;
    }
  }
  else if (DAT_006059e8 == 0x85) {
    cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bca300],0);
    iVar5 = DAT_00605a1c;
    if (cVar2 != '\0') {
      (&DAT_00784b62)[DAT_00605a1c] = sVar9;
      *(short *)(&DAT_00784b6a + iVar5 * 2) = sVar10;
      return;
    }
  }
  else if (DAT_006059e8 == 0x8b) {
    cVar2 = FUN_004ec410((&DAT_00af598c)[DAT_00bca300],1);
    iVar5 = DAT_00605a1c;
    if (cVar2 != '\0') {
      (&DAT_00784cbc)[DAT_00605a1c] = param_1;
      *(int *)(&DAT_00784ccc + iVar5 * 4) = param_2;
      return;
    }
  }
  else if (DAT_006059e8 == 0xd6) {
    if (((uVar1 & 4) != 0) &&
       (iVar4 = FUN_00477d40(param_1,param_2), iVar5 = DAT_00605a20, iVar4 != 0)) {
      *(int *)(&DAT_00784dc0 + DAT_00605a20 * 4) = param_1;
      *(int *)(&DAT_00784dcc + iVar5 * 4) = param_2;
      return;
    }
  }
  else if (DAT_006059e8 == 0x82) {
    cVar2 = FUN_0046f9a0(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2,4);
    if ((cVar2 != '\0') && (iVar5 = FUN_0047ed60(DAT_00bc9768), iVar5 != 0)) {
      _DAT_00784c8c = sVar9;
      _DAT_00784c8e = sVar10;
      FUN_004bbd40((int)sVar9,(int)sVar10,1);
      return;
    }
  }
  else {
    if (DAT_006059e8 != 0x83) {
      if (DAT_00605a34 == 0) {
        local_4 = 1;
      }
      else if (DAT_00605a34 == 1) {
        local_4 = 5;
      }
      else if (DAT_00605a34 == 2) {
        local_4 = 0xd;
      }
      else {
        local_4 = (-(uint)(DAT_00605a34 != 3) & 0x10) + 0x19;
      }
      if ((DAT_00605a30 == 10) || (DAT_006059e8 == 0x81)) {
        local_4 = 1;
      }
      bVar6 = 0;
      iVar5 = DAT_006059e8;
      iVar4 = DAT_00605a34;
      if (local_4 != 0) {
        local_c = &DAT_005cc77c;
        do {
          iVar12 = local_c[-1] + param_1;
          iVar11 = *local_c + param_2;
          if ((((-1 < iVar12) && (iVar12 < DAT_00bca2d0)) && (-1 < iVar11)) &&
             (iVar11 < DAT_00bca2d4)) {
            iVar8 = DAT_00bca2d8 + iVar11 * 0xe4 + iVar12;
            iVar4 = (int)*(short *)(&DAT_00b4e6fc + iVar8 * 2);
            if (iVar4 != 0) {
              FUN_0047c480(iVar4,iVar12,iVar11);
              FUN_0046ac10(iVar4);
              iVar5 = DAT_006059e8;
            }
            uVar1 = (&DAT_00af598c)[iVar8];
            if (iVar5 == 0x74) {
              FUN_00477960(iVar12,iVar11);
            }
            else if (iVar5 == 0x75) {
              FUN_004779f0(iVar12,iVar11);
            }
            else if (iVar5 == 0x76) {
              FUN_00477c20(iVar12,iVar11);
            }
            else if (iVar5 == 0x78) {
              FUN_0051f780(iVar12,iVar11);
            }
            else if (iVar5 == 0xa3) {
              FUN_00477a40(iVar12,iVar11,2);
            }
            else if (iVar5 == 0xdf) {
              FUN_00477a40(iVar12,iVar11,0x20000002);
            }
            else if (iVar5 == 0xa4) {
              FUN_00477a40(iVar12,iVar11,0x100002);
            }
            else if (iVar5 == 0xa5) {
              FUN_00477a40(iVar12,iVar11,0x200002);
            }
            else if (iVar5 == 0xa6) {
              FUN_00477a40(iVar12,iVar11,0x300002);
            }
            else if (iVar5 == 0xc6) {
              FUN_00477ab0(iVar12,iVar11);
            }
            else if (iVar5 == 0x7a) {
              FUN_00477b10(iVar12,iVar11);
            }
            else if (iVar5 == 0xc5) {
              FUN_00477b70(iVar12,iVar11);
            }
            else if (iVar5 == 0x86) {
              FUN_00477bd0(iVar12,iVar11);
            }
            else if (iVar5 == 0x87) {
              FUN_00477ce0(iVar12,iVar11);
            }
            else if (iVar5 == 0x7d) {
              FUN_00477c80(iVar12,iVar11);
            }
            else if (iVar5 == 0x7b) {
              FUN_004793e0(iVar12,iVar11);
            }
            else if (iVar5 == 0x7c) {
              FUN_00479460(iVar12,iVar11);
            }
            else if (iVar5 == 0x81) {
              FUN_0051f780(iVar12,iVar11);
            }
            uVar7 = (&DAT_00af598c)[iVar8];
            if ((uVar7 & 0x10000) == 0) {
              if ((uVar7 & 0x22341a33) == 0) {
                if ((uVar7 & 4) == 0) {
                  bVar3 = FUN_00478270(iVar12,iVar11);
                }
                else {
                  bVar3 = FUN_00477f60(iVar12,iVar11,1);
                }
              }
              else {
                bVar3 = FUN_00478090(iVar12,iVar11);
              }
            }
            else {
              bVar3 = FUN_00477f70(iVar12,iVar11);
            }
            bVar6 = bVar6 | bVar3;
            iVar5 = DAT_006059e8;
            iVar4 = DAT_00605a34;
            if (((uVar1 ^ (&DAT_00af598c)[iVar8]) & 0x50004) != 0) {
              bVar6 = bVar6 | 1;
            }
          }
          local_c = local_c + 2;
          local_4 = local_4 + -1;
        } while (local_4 != 0);
        if (bVar6 != 0) {
          if (DAT_00e92e20 == 0) {
            DAT_00bca30c = 1;
            return;
          }
          iVar5 = 0x10;
          if (DAT_00784cb0 == '\0') {
            iVar5 = 0x2e;
          }
          else if (DAT_00784cb0 == '\x02') {
            iVar5 = 8;
          }
          iVar5 = iVar5 + 1;
          FUN_00487450(param_1 - iVar5,param_2 - iVar5,iVar5 + param_1,iVar5 + param_2);
          FUN_00477dd0(param_1,param_2,iVar5);
          return;
        }
      }
      if (iVar4 == 0) {
        iVar4 = 1;
      }
      else if (iVar4 == 1) {
        iVar4 = 2;
      }
      else if (iVar4 == 2) {
        iVar4 = 3;
      }
      else {
        iVar4 = (iVar4 != 3) + 4;
      }
      iVar4 = iVar4 + 1;
      if (DAT_00605a30 == 10) {
        iVar4 = 1;
      }
      if (iVar5 == 0x74) {
        FUN_00478d40(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
        return;
      }
      if (iVar5 == 0x75) {
        FUN_00478e10(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
        return;
      }
      if (iVar5 == 0x76) {
        FUN_00478ea0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
        return;
      }
      if (iVar5 == 0x78) {
        FUN_00478f40(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
        return;
      }
      if (iVar5 == 0xa3) {
        FUN_00478fd0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
        return;
      }
      if (iVar5 != 0xdf) {
        if (iVar5 == 0xa4) {
          FUN_00478fd0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
          return;
        }
        if (iVar5 == 0xa5) {
          FUN_00478fd0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
          return;
        }
        if (iVar5 != 0xa6) {
          if (iVar5 == 0xc6) {
            FUN_00479060(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
            return;
          }
          if (iVar5 == 0xc5) {
            FUN_00479170(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
            return;
          }
          if (iVar5 == 0x7a) {
            FUN_00488e90((param_1 - iVar4) + -1,(param_2 - iVar4) + -1,iVar4 + 1 + param_1,
                         iVar4 + 1 + param_2,1);
            FUN_004790e0((param_1 - iVar4) + -4,(param_2 - iVar4) + -4,iVar4 + 4 + param_1,
                         iVar4 + 4 + param_2);
            return;
          }
          if (iVar5 != 0x86) {
            if (iVar5 == 0x87) {
              FUN_00479340(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
              return;
            }
            if (iVar5 != 0x7d) {
              if (((iVar5 != 0x7b) && (iVar5 != 0x7c)) && (iVar5 != 0x81)) {
                return;
              }
              FUN_00479510(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
              return;
            }
            FUN_004792b0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
            return;
          }
          FUN_00479200(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
          return;
        }
      }
      FUN_00478fd0(param_1 - iVar4,param_2 - iVar4,iVar4 + param_1,iVar4 + param_2);
      return;
    }
    cVar2 = FUN_0046f9a0(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2,4);
    if ((cVar2 != '\0') && (iVar5 = FUN_0047ed60(DAT_00bc9768), iVar5 != 0)) {
      _DAT_00784c92 = sVar10;
      _DAT_00784c90 = sVar9;
      return;
    }
  }
  return;
}
