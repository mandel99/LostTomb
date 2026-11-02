#include "restore_types.h"
#define DAT_005c5448 (*(undefined4 *)0x005c5448u)
#define DAT_005c5a78 (*(undefined4 *)0x005c5a78u)
#define DAT_005c5a98 (*(undefined4 *)0x005c5a98u)
#define DAT_005c5ab8 (*(undefined4 *)0x005c5ab8u)
#define DAT_005c5ad8 (*(undefined4 *)0x005c5ad8u)
#define DAT_005c5ba8 (*(undefined4 *)0x005c5ba8u)
#define DAT_005c5bc8 (*(undefined4 *)0x005c5bc8u)
#define DAT_005c5be8 (*(undefined4 *)0x005c5be8u)
#define DAT_005c5cb0 (*(undefined4 *)0x005c5cb0u)
#define DAT_005c6768 (*(undefined4 *)0x005c6768u)
#define DAT_005c68c8 (*(undefined4 *)0x005c68c8u)
#define DAT_005c7b10 (*(undefined4 *)0x005c7b10u)
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcd729 (*(undefined4 *)0x00bcd729u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38df8 (*(undefined1 *)0x00e38df8u)
#define DAT_00eadcc0 (*(undefined *)0x00eadcc0u)
#define DAT_00eadccc (*(undefined *)0x00eadcccu)
#define DAT_00eadce4 (*(undefined *)0x00eadce4u)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define DAT_00eadd40 (*(undefined *)0x00eadd40u)
#define DAT_00eadd48 (*(undefined *)0x00eadd48u)
#define DAT_00eadd50 (*(undefined *)0x00eadd50u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
#define PTR_DAT_005c5188 (*(unsigned char * *)0x005c5188u)
#define PTR_DAT_005c66d8 (*(unsigned char * *)0x005c66d8u)
#define PTR_DAT_005c6788 (*(unsigned char * *)0x005c6788u)
#define PTR_DAT_005c6818 (*(unsigned char * *)0x005c6818u)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
extern void FUN_00406720();
extern undefined4 FUN_0040d1b0();
extern int FUN_00425990();
extern void FUN_00425bc0();
extern int FUN_004261d0();
extern int FUN_00426450();
extern int FUN_004cde80();
extern int FUN_004d6740();
extern undefined4 FUN_005013a0();
extern undefined4 FUN_00502780();
extern void FUN_00515c60();
extern void FUN_00516020();

void FUN_004f96e0(void)

{
  short sVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  int iVar7;
  undefined **ppuVar8;
  
  iVar6 = 0;
  DAT_007056c8 = 0;
  if (DAT_00e38df8 != '\0') {
    if ((DAT_00eadd50 == 4) && (iVar6 = DAT_00eadccc * 0x90, (&DAT_00bcb9f0)[iVar6] != '\0')) {
      if ((*(short *)(&DAT_00bcb9cc + iVar6) == 0xd) && (*(short *)(&DAT_00bcb9f2 + iVar6) == 0)) {
        DAT_00e38dda = 0x49;
        DAT_00e38dca = 1;
        return;
      }
      if (*(short *)(&DAT_00bcb9f2 + iVar6) != 6) {
        switch(*(short *)(&DAT_00bcb9f2 + iVar6)) {
        case 1: ;
        case 2: ;
          DAT_00d3b198 = 0;
          FUN_00406720();
          DAT_00e38dda = 0x1e;
          DAT_00e38dca = 1;
          return;
        case 3: ;
        case 4: ;
          DAT_00d3b198 = 1;
          FUN_00406720();
          DAT_00e38dda = 0x1e;
          DAT_00e38dca = 1;
          return;
        case 0xf: ;
          DAT_00d3b198 = 2;
        }
        FUN_00406720();
        DAT_00e38dda = 0x1e;
        DAT_00e38dca = 1;
        return;
      }
    }
    DAT_00e38dda = 1;
    DAT_00e38dca = 1;
    return;
  }
  if (DAT_00eadcc0 == 0) {
    if (DAT_00eadd50 == 5) {
      FUN_00425bc0(DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c7b10,2,10);
      iVar6 = DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c;
    }
    else {
      if (DAT_00eadd50 != 6) {
        if (DAT_00eadd50 == 2) {
          FUN_00515c60();
        }
        FUN_00425bc0(DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c7b10,2,10);
        iVar6 = FUN_00425990(DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c7b10,2
                            );
        goto joined_r0x004f9a2b;
      }
      FUN_00425bc0(DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c7b10,2,10);
      iVar6 = DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c;
    }
    iVar6 = FUN_00425990(DAT_00eadd90,iVar6,&DAT_005c7b10,2);
  }
  else {
    sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
    if ((sVar1 == 0x48) || (sVar1 == 0xc1)) {
      iVar6 = FUN_00502780();
      iVar4 = DAT_00f53e6c;
      if (iVar6 < DAT_00f53e6c) {
        iVar4 = FUN_00502780();
      }
    }
    else if (((sVar1 == 0x46) || (iVar4 = DAT_00f53dd8, sVar1 == 0xbe)) &&
            (iVar7 = 0, iVar4 = iVar6, 0 < DAT_00f53e6c)) {
      puVar5 = &DAT_00f53e70;
      do {
        iVar4 = FUN_005013a0(*puVar5);
        if (iVar4 != 0) {
          iVar6 = iVar6 + 1;
        }
        iVar7 = iVar7 + 1;
        puVar5 = puVar5 + 1;
        iVar4 = iVar6;
      } while (iVar7 < DAT_00f53e6c);
    }
    uVar2 = FUN_00426450(DAT_00eadd90 + 0xfa,0x4e,&DAT_005c68c8,iVar4,DAT_00eadd90,DAT_00eadd8c);
    DAT_007056c4 = FUN_004d6740(DAT_00eadd90 + 0xfa,0x4e,&DAT_005c68c8,iVar4,uVar2);
    iVar6 = FUN_004261d0(DAT_00eadd90 + 0xfa,0x4e,&DAT_005c68c8,iVar4);
    if (iVar6 != 0) {
      return;
    }
    iVar6 = FUN_004cde80(iVar4 * 4,3);
    iVar6 = (iVar6 + 7) * 0x10 + -8;
    FUN_00425bc0(DAT_00eadd90,iVar6,&DAT_005c7b10,2,10);
    iVar6 = FUN_00425990(DAT_00eadd90,iVar6,&DAT_005c7b10,2);
  }
joined_r0x004f9a2b: ;
  if ((iVar6 == 0) && (DAT_00eadd50 != 0)) {
    if (DAT_00eadd50 == 4) {
      uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c66d8,5,DAT_00eadd90,DAT_00eadd8c);
      DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c66d8,5,uVar2);
      iVar6 = FUN_004261d0(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c66d8,5);
      if (iVar6 == 0) {
        uVar2 = FUN_00426450(((DAT_00eadd88 + -0x14) * 0x10) / 2 + DAT_00eadd90,
                             DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c6768,1,DAT_00eadd90
                             ,DAT_00eadd8c);
        DAT_007056c4 = FUN_004d6740(((DAT_00eadd88 + -0x14) * 0x10) / 2 + DAT_00eadd90,
                                    DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c6768,1,uVar2
                                   );
        FUN_004261d0(((DAT_00eadd88 + -0x14) * 0x10) / 2 + DAT_00eadd90,
                     DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,&DAT_005c6768,1);
        return;
      }
    }
    else {
      if (DAT_00eadd50 == 6) {
        uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c6788,5,DAT_00eadd90,DAT_00eadd8c
                            );
        DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c6788,5,uVar2);
        FUN_004261d0(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c6788,5);
        return;
      }
      if (DAT_00eadd50 == 5) {
        uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c6818,5,DAT_00eadd90,DAT_00eadd8c
                            );
        DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c6818,5,uVar2);
        uVar2 = 5;
        ppuVar8 = &PTR_DAT_005c6818;
LAB_004fa0c4: ;
        FUN_004261d0(DAT_00eadd90,DAT_00eadd8c,ppuVar8,uVar2);
        return;
      }
      if (DAT_00eadd48 == 0) {
        if (DAT_00eadd50 == 2) {
          if ((9 < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84]) &&
             ((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 0x1e)) {
            uVar2 = FUN_00426450(DAT_00eadd90 + 0x70,(DAT_00eadd84 + -2) * 0x10 + DAT_00eadd8c,
                                 &DAT_005c5a78,1,DAT_00eadd90,DAT_00eadd8c);
            DAT_007056c4 = FUN_004d6740(DAT_00eadd90 + 0x70,
                                        (DAT_00eadd84 + -2) * 0x10 + DAT_00eadd8c,&DAT_005c5a78,1,
                                        uVar2);
            iVar6 = FUN_004261d0(DAT_00eadd90 + 0x70,(DAT_00eadd84 + -2) * 0x10 + DAT_00eadd8c,
                                 &DAT_005c5a78,1);
            if (iVar6 != 0) {
              return;
            }
          }
          if (((&DAT_007999e8)[DAT_00eadd6c * 0x84] == 0x47) ||
             ((&DAT_007999e8)[DAT_00eadd6c * 0x84] == 0xbf)) {
            if (DAT_00eadcc0 == 0) {
              uVar2 = FUN_00426450(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
                                   &DAT_005c5a98,1,DAT_00eadd90,DAT_00eadd8c);
              DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 100,
                                          DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,&DAT_005c5a98,1
                                          ,uVar2);
              iVar6 = FUN_004261d0(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
                                   &DAT_005c5a98,1);
            }
            else {
              iVar6 = FUN_004cde80(DAT_00f53dd8 * 4,3);
              iVar6 = (iVar6 + 7) * 0x10 + -0x10;
              uVar2 = FUN_00426450(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5ba8,1,DAT_00eadd90,
                                   DAT_00eadd8c);
              DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5ba8,1,uVar2);
              iVar6 = FUN_004261d0(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5ba8,1);
            }
            if (iVar6 != 0) {
              return;
            }
          }
          sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
          if ((sVar1 == 0x46) || (sVar1 == 0xbe)) {
            if (DAT_00eadcc0 != 0) {
              uVar2 = FUN_00426450(DAT_00eadd90 + 0x50,0x1b4,&DAT_005c5ba8,1,DAT_00eadd90,
                                   DAT_00eadd8c);
              DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 0x50,0x1b4,&DAT_005c5ba8,1,uVar2);
              FUN_004261d0(DAT_00eadd90 + 0x50,0x1b4,&DAT_005c5ba8,1);
              return;
            }
            uVar2 = FUN_00426450(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
                                 &DAT_005c5ab8,1,DAT_00eadd90,DAT_00eadd8c);
            DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 100,
                                        DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,&DAT_005c5ab8,1,
                                        uVar2);
            FUN_004261d0(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,&DAT_005c5ab8
                         ,1);
          }
          else if ((sVar1 == 0x48) || (sVar1 == 0xc1)) {
            if (DAT_00eadcc0 == 0) {
              uVar2 = FUN_00426450(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
                                   &DAT_005c5ad8,1,DAT_00eadd90,DAT_00eadd8c);
              DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 100,
                                          DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,&DAT_005c5ad8,1
                                          ,uVar2);
              FUN_004261d0(DAT_00eadd90 + 100,DAT_00eadd84 * 0x10 + -0x22 + DAT_00eadd8c,
                           &DAT_005c5ad8,1);
              return;
            }
            iVar4 = FUN_00502780();
            iVar6 = DAT_00f53e6c;
            if (iVar4 < DAT_00f53e6c) {
              iVar6 = FUN_00502780();
            }
            iVar6 = FUN_004cde80(iVar6 * 4,3);
            iVar4 = FUN_0040d1b0();
            if (iVar4 == 0) {
              iVar6 = (iVar6 + 7) * 0x10 + -0x18;
              uVar2 = FUN_00426450(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5bc8,1,DAT_00eadd90,
                                   DAT_00eadd8c);
              DAT_007056c8 = FUN_004d6740(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5bc8,1,uVar2);
              FUN_004261d0(DAT_00eadd90 + 0x50,iVar6,&DAT_005c5bc8,1);
              return;
            }
          }
          else if (sVar1 == 0xd1) {
            uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c5188,1,DAT_00eadd90,
                                 DAT_00eadd8c);
            DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c5188,1,uVar2);
            iVar6 = FUN_004261d0(DAT_00eadd90,DAT_00eadd8c,&PTR_DAT_005c5188,1);
            if (iVar6 == 0) {
              FUN_00516020();
              return;
            }
          }
          else if (((sVar1 == 0x4d) || (sVar1 == 0x4e)) || (sVar1 == 0x4f)) {
            uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&DAT_005c5448,1,DAT_00eadd90,DAT_00eadd8c
                                );
            DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&DAT_005c5448,1,uVar2);
            uVar2 = 1;
            ppuVar8 = (undefined **)&DAT_005c5448;
            goto LAB_004fa0c4;
          }
        }
      }
      else {
        uVar2 = FUN_00426450(DAT_00eadd90,DAT_00eadd8c,&DAT_005c5be8,DAT_00eadd40,DAT_00eadd90,
                             DAT_00eadd8c);
        DAT_007056c8 = FUN_004d6740(DAT_00eadd90,DAT_00eadd8c,&DAT_005c5be8,DAT_00eadd40,uVar2);
        iVar6 = FUN_004261d0(DAT_00eadd90,DAT_00eadd8c,&DAT_005c5be8,DAT_00eadd40);
        if (iVar6 == 0) {
          uVar2 = FUN_00426450(DAT_00eadd90 + 0x11d,DAT_00eadd8c + 0xc3,&DAT_005c5cb0,1,DAT_00eadd90
                               ,DAT_00eadd8c);
          DAT_007056c4 = FUN_004d6740(DAT_00eadd90 + 0x11d,DAT_00eadd8c + 0xc3,&DAT_005c5cb0,1,uVar2
                                     );
          iVar6 = FUN_004261d0(DAT_00eadd90 + 0x11d,DAT_00eadd8c + 0xc3,&DAT_005c5cb0,1);
          if (iVar6 == 0) {
            iVar6 = (&DAT_00eadd24)[DAT_00eadce4];
            uVar3 = (uint)(byte)(&DAT_00bcd729)[iVar6 * 0x184];
            if ((&DAT_006fdc88)[uVar3] != '\0') {
              FUN_00425bc0(DAT_00eadd90 + 0x28,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,
                           (&PTR_DAT_005f78f0)[uVar3],1,uVar3 + 0x28);
              FUN_00425990(DAT_00eadd90 + 0x28,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,
                           (&PTR_DAT_005f78f0)[(byte)(&DAT_00bcd729)[iVar6 * 0x184]],1);
              return;
            }
          }
        }
      }
    }
  }
  return;
}
