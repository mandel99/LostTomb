#include "restore_types.h"
#define DAT_005794b4 (*(undefined4 *)0x005794b4u)
#define DAT_005ce3bc (*(undefined4 *)0x005ce3bcu)
#define DAT_00605a0c (*(undefined *)0x00605a0cu)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007027b8 (*(char *)0x007027b8u)
#define DAT_0070a8dc (*(undefined4 *)0x0070a8dcu)
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_007848e4 (*(undefined *)0x007848e4u)
#define DAT_007848e6 (*(undefined *)0x007848e6u)
#define DAT_007848e8 (*(undefined *)0x007848e8u)
#define DAT_007848ea (*(undefined *)0x007848eau)
#define DAT_00784ce4 (*(undefined *)0x00784ce4u)
#define DAT_00784ce6 (*(undefined *)0x00784ce6u)
#define DAT_00784ce8 (*(undefined *)0x00784ce8u)
#define DAT_00784cea (*(undefined *)0x00784ceau)
#define DAT_00784cec (*(undefined *)0x00784cecu)
#define DAT_00784cee (*(undefined *)0x00784ceeu)
#define DAT_00784cf0 (*(undefined *)0x00784cf0u)
#define DAT_00784cf2 (*(undefined *)0x00784cf2u)
#define DAT_00784cf4 (*(undefined *)0x00784cf4u)
#define DAT_00784cf6 (*(undefined *)0x00784cf6u)
#define DAT_00784cf8 (*(undefined *)0x00784cf8u)
#define DAT_00784cfa (*(undefined *)0x00784cfau)
#define DAT_00784cfc (*(undefined *)0x00784cfcu)
#define DAT_00784cfe (*(undefined *)0x00784cfeu)
#define DAT_00784d00 (*(undefined *)0x00784d00u)
#define DAT_00784d02 (*(undefined *)0x00784d02u)
#define DAT_00784d04 (*(undefined *)0x00784d04u)
#define DAT_00784d06 (*(undefined *)0x00784d06u)
#define DAT_00784d08 (*(undefined *)0x00784d08u)
#define DAT_00784d0a (*(undefined *)0x00784d0au)
#define DAT_00784d0c (*(undefined *)0x00784d0cu)
#define DAT_00784d0e (*(undefined *)0x00784d0eu)
#define DAT_00784d10 (*(undefined *)0x00784d10u)
#define DAT_00784d12 (*(undefined *)0x00784d12u)
#define DAT_00784d14 (*(undefined *)0x00784d14u)
#define DAT_00784d16 (*(undefined *)0x00784d16u)
#define DAT_00784d18 (*(undefined *)0x00784d18u)
#define DAT_00784d1a (*(undefined *)0x00784d1au)
#define DAT_00784d1c (*(undefined *)0x00784d1cu)
#define DAT_00784d1e (*(undefined *)0x00784d1eu)
#define DAT_00784d20 (*(undefined *)0x00784d20u)
#define DAT_00784d22 (*(undefined *)0x00784d22u)
#define DAT_00784d24 (*(undefined *)0x00784d24u)
#define DAT_00784d26 (*(undefined *)0x00784d26u)
#define DAT_00784d28 (*(undefined *)0x00784d28u)
#define DAT_00784d2a (*(undefined *)0x00784d2au)
#define DAT_00784d2c (*(undefined *)0x00784d2cu)
#define DAT_00784d2e (*(undefined *)0x00784d2eu)
#define DAT_00784d30 (*(undefined *)0x00784d30u)
#define DAT_00784d32 (*(undefined *)0x00784d32u)
#define DAT_00784d34 (*(undefined *)0x00784d34u)
#define DAT_00784d36 (*(undefined *)0x00784d36u)
#define DAT_00784d38 (*(undefined *)0x00784d38u)
#define DAT_00784d3a (*(undefined *)0x00784d3au)
#define DAT_00784db0 (*(undefined *)0x00784db0u)
#define DAT_00784db2 (*(undefined *)0x00784db2u)
#define DAT_00784db4 (*(undefined *)0x00784db4u)
#define DAT_00784db6 (*(undefined *)0x00784db6u)
#define DAT_00784db8 (*(undefined *)0x00784db8u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00403ef0();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern undefined4 FUN_00443d90();
extern uint FUN_004f5470();

void FUN_00404350(void)

{
  int iVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  char cVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  undefined4 uVar12;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  do {
    local_10 = 0;
    do {
      if ((DAT_00605a0c == 1) || (DAT_00605a3c == 1)) {
        (&DAT_0070a8dc)[local_10 + local_c * 0x1e] = 1;
      }
      else {
        iVar1 = local_10 + local_c * 0x1e;
        iVar2 = *(int *)(&DAT_005ce3bc + iVar1 * 4);
        (&DAT_0070a8dc)[iVar1] = 0;
        if ((9 < iVar2) && (iVar2 < 0x1e)) {
          (&DAT_0070a8dc)[iVar1] = 1;
        }
        if ((iVar2 == 9) || (iVar2 == 0x5c)) {
LAB_0040470d: ;
          (&DAT_0070a8dc)[iVar1] = 1;
        }
        else {
          if (iVar2 == 0xb4) {
            bVar11 = DAT_00784d04 == 0;
            goto LAB_004046ac;
          }
          if ((((iVar2 == 0x89) || (iVar2 == 5)) || (iVar2 == 0x51)) || (iVar2 == 0xa7))
          goto LAB_0040470d;
          if (iVar2 == 0x73) {
            iVar9 = FUN_00443b90(8);
            goto LAB_004046a7;
          }
          if (iVar2 == 0x47) {
            bVar11 = DAT_00784cee == 0;
LAB_004046ac: ;
            if (!bVar11) goto LAB_0040470d;
          }
          else {
            if (iVar2 == 0x33) {
              bVar11 = DAT_00784d00 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x35) {
              bVar11 = DAT_00784d02 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xce) {
              bVar11 = DAT_00784d0a == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x2e) {
              bVar11 = DAT_00784d08 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x2f) {
              bVar11 = DAT_00784d0c == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xb8) {
              bVar11 = DAT_00784d10 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x37) goto LAB_0040470d;
            if (iVar2 == 0x88) {
              bVar11 = DAT_00784d1a == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xa1) {
              bVar11 = DAT_00784ce4 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x1f) {
              bVar11 = DAT_00784cf6 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x24) {
              bVar11 = DAT_00784cf6 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xd1) {
              bVar11 = DAT_00784cfe == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xc4) {
              iVar9 = FUN_00443b90(6);
LAB_004046a7: ;
              bVar11 = iVar9 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x65) {
              iVar9 = FUN_00443b90(0x10);
              goto LAB_004046a7;
            }
            if (iVar2 == 0xe0) {
              iVar9 = FUN_00443b90(0x20);
              goto LAB_004046a7;
            }
            if (iVar2 == 100) {
              iVar9 = FUN_00443b90(0xe);
              goto LAB_004046a7;
            }
            if (iVar2 == 0x6d) {
              iVar9 = FUN_00443b90(0xb);
              goto LAB_004046a7;
            }
            if (iVar2 == 0x72) {
              uVar12 = 0xb;
              goto LAB_004046a2;
            }
            if (iVar2 == 0x27) {
              bVar11 = DAT_00784d1c == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x6e) {
              uVar12 = 0xf;
              goto LAB_004046a2;
            }
            if (iVar2 == 0x8a) {
              bVar11 = DAT_00784d16 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x52) {
              bVar11 = DAT_00784d18 == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0x56) {
              bVar11 = DAT_00784d0e == 0;
              goto LAB_004046ac;
            }
            if (iVar2 == 0xcb) {
              uVar12 = 0x16;
LAB_004046a2: ;
              iVar9 = FUN_00443d90(uVar12);
              goto LAB_004046a7;
            }
            if (iVar2 != 0xe8) {
              if (iVar2 != 0xe9) {
                if (iVar2 == 0xc3) {
                  iVar9 = FUN_00443b90(0x16);
                  goto LAB_004046a7;
                }
                if (iVar2 == 0x46) {
                  bVar11 = DAT_00784cf0 == 0;
                }
                else if (iVar2 == 0x48) {
                  bVar11 = DAT_00784cf2 == 0;
                }
                else if (iVar2 == 199) {
                  bVar11 = DAT_00784cec == 0;
                }
                else {
                  if (iVar2 == 0x6f) {
                    uVar12 = 0x10;
                    goto LAB_004046a2;
                  }
                  if (iVar2 == 0x3c) {
                    bVar11 = DAT_007848e2 == 0;
                  }
                  else if (iVar2 == 0x40) {
                    bVar11 = DAT_007848ea == 0;
                  }
                  else if (iVar2 == 0x8c) {
                    bVar11 = DAT_007848e2 == 0;
                  }
                  else {
                    if (iVar2 != 0x90) {
                      if (iVar2 == 0xb1) {
                        uVar12 = 0x14;
                      }
                      else {
                        if (iVar2 != 0xb2) {
                          if (iVar2 == 0x4a) {
                            if ((DAT_00784cea == 0) && (sVar3 = DAT_00784d36, DAT_00784d38 == 0))
                            goto joined_r0x004046ce;
                          }
                          else {
                            sVar3 = DAT_00784d06;
                            if (iVar2 == 0x31) {
joined_r0x004046ce: ;
                              if (sVar3 == 0) goto LAB_00404713;
                            }
                            else if (((iVar2 != 0xbb) || (DAT_00784d12 == 0)) ||
                                    (((int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276] < 0 ||
                                     (5 < (int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276]))))
                            goto LAB_00404713;
                          }
                          goto LAB_0040470d;
                        }
                        iVar9 = FUN_00443b90(0xc);
                        if (iVar9 != 0) goto LAB_0040470d;
                        uVar12 = 0xc;
                      }
                      goto LAB_004046a2;
                    }
                    bVar11 = DAT_007848ea == 0;
                  }
                }
                goto LAB_004046ac;
              }
              uVar12 = 0x20;
              goto LAB_004046a2;
            }
            iVar9 = FUN_00443d90(0x1f);
            if (iVar9 != 0) {
              uVar12 = 0xd;
              goto LAB_004046a2;
            }
          }
        }
LAB_00404713: ;
        cVar6 = FUN_00403ef0();
        if ((cVar6 == '\0') || (iVar2 != 4)) {
          if (iVar2 == 3) {
            iVar9 = FUN_00443b90(0x16);
            if ((((iVar9 == 0) && (iVar9 = FUN_00443b90(0xb), iVar9 == 0)) &&
                (iVar9 = FUN_00443b90(0x14), iVar9 == 0)) &&
               (((iVar9 = FUN_00443b90(0x18), iVar9 == 0 && (iVar9 = FUN_00443b90(0x19), iVar9 == 0)
                 ) && ((iVar9 = FUN_00443b90(0x1e), iVar9 == 0 &&
                       ((iVar9 = FUN_00443b90(0x1d), iVar9 == 0 &&
                        (iVar9 = FUN_00443b90(0x12), sVar3 = DAT_00784ce4, iVar9 == 0))))))))
            goto joined_r0x00404a24;
          }
          else if (iVar2 == 2) {
            iVar9 = FUN_00443b90(0xe);
            if (((((iVar9 == 0) && (iVar9 = FUN_00443b90(1), iVar9 == 0)) &&
                 (iVar9 = FUN_00443b90(3), iVar9 == 0)) &&
                ((iVar9 = FUN_00443b90(4), iVar9 == 0 && (iVar9 = FUN_00443b90(0x10), iVar9 == 0))))
               && ((iVar9 = FUN_00443b90(5), iVar9 == 0 &&
                   ((iVar9 = FUN_00443b90(6), iVar9 == 0 && (iVar9 = FUN_00443b90(0x20), iVar9 == 0)
                    ))))) goto LAB_00404b13;
          }
          else {
            sVar3 = DAT_00784db0;
            if ((((iVar2 != 0x41) && (sVar3 = DAT_00784db4, iVar2 != 0x43)) &&
                (sVar3 = DAT_00784db6, iVar2 != 0x44)) &&
               ((sVar3 = DAT_00784db2, iVar2 != 0x42 && (sVar3 = DAT_00784db8, iVar2 != 0x45)))) {
              if (iVar2 == 0x61) {
                if (((DAT_00784db0 != 0) || (DAT_00784db4 != 0)) ||
                   (sVar4 = DAT_00784db2, DAT_00784db6 != 0)) goto LAB_00404b0d;
joined_r0x004049e3: ;
                if (sVar4 != 0) goto LAB_00404b0d;
              }
              else {
                sVar3 = DAT_007848e4;
                if (((iVar2 != 0x3d) && (sVar3 = DAT_007848e6, iVar2 != 0x3e)) &&
                   (sVar3 = DAT_007848e8, iVar2 != 0x3f)) {
                  if (iVar2 == 0x60) {
joined_r0x004049bc: ;
                    if ((DAT_007848e2 == 0) &&
                       (sVar4 = DAT_007848e4, sVar3 = DAT_007848ea, sVar5 = DAT_007848e8,
                       DAT_007848e6 == 0)) {
joined_r0x004049d6: ;
                      if (sVar5 == 0) goto joined_r0x004049e3;
                    }
                    goto LAB_00404b0d;
                  }
                  sVar3 = DAT_007848e4;
                  if (((iVar2 != 0x8d) && (sVar3 = DAT_007848e6, iVar2 != 0x8e)) &&
                     (sVar3 = DAT_007848e8, iVar2 != 0x8f)) {
                    if (iVar2 == 0x96) goto joined_r0x004049bc;
                    sVar3 = DAT_00784d20;
                    if ((iVar2 != 0x29) && ((iVar2 != 0x2a && (iVar2 != 0x2b)))) {
                      sVar4 = DAT_00784d1e;
                      sVar3 = DAT_00784d1c;
                      sVar5 = DAT_00784d20;
                      if (iVar2 == 0x5b) goto joined_r0x004049d6;
                      sVar3 = DAT_00784d1e;
                      if (iVar2 != 0x26) {
                        sVar3 = DAT_00784d18;
                        sVar4 = DAT_00784d1a;
                        if (iVar2 != 0x34) goto LAB_00404b13;
                        goto joined_r0x004049e3;
                      }
                    }
                  }
                }
              }
            }
joined_r0x00404a24: ;
            if (sVar3 == 0) goto LAB_00404b13;
          }
LAB_00404b0d: ;
          (&DAT_0070a8dc)[iVar1] = 1;
        }
        else {
          cVar6 = FUN_004f5470(0xb3);
          if ((cVar6 != '\0') ||
             ((cVar6 = FUN_004f5470(0xb2), cVar6 != '\0' ||
              (cVar6 = FUN_004f5470(0xb1), cVar6 != '\0')))) goto LAB_00404b0d;
        }
LAB_00404b13: ;
        if (((DAT_00e38ebc == 0) && (DAT_00e38ea8 == '\0')) && (iVar2 == 0x37)) {
          (&DAT_0070a8dc)[iVar1] = 0;
          iVar9 = 0;
          local_8 = 0;
          piVar7 = &DAT_005794b4;
        }
        else {
          if (iVar2 == 0x66) {
            uVar12 = 1;
LAB_00404b4a: ;
            iVar9 = FUN_00443b90(uVar12);
LAB_00404b4f: ;
            bVar11 = iVar9 == 0;
LAB_00404b54: ;
            if (!bVar11) {
LAB_004050ba: ;
              (&DAT_0070a8dc)[iVar1] = 1;
            }
LAB_004050c0: ;
            if (('\n' < DAT_00e38ea8) || (DAT_00e38ebc != 0)) goto LAB_0040513f;
          }
          else {
            if (iVar2 == 0xc2) {
              uVar12 = 2;
              goto LAB_00404b4a;
            }
            if (iVar2 == 8) {
              bVar11 = DAT_00784ce8 == 0;
              goto LAB_00404b54;
            }
            sVar3 = DAT_00784d12;
            if (iVar2 == 0xbc) {
LAB_00404b88: ;
              if ((sVar3 != 0) && (iVar9 = (&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276], 5 < iVar9)) {
                bVar11 = SBORROW4(iVar9,8);
                iVar9 = iVar9 + -8;
LAB_00404bac: ;
                if (bVar11 != iVar9 < 0) goto LAB_004050ba;
              }
              goto LAB_004050c0;
            }
            if (iVar2 == 0xbd) {
              if (((DAT_00784d12 == 0) || ((int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276] < 8)) ||
                 (10 < (int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276])) goto LAB_004050c0;
              goto LAB_004050ba;
            }
            if (iVar2 == 0x4d) {
              if (((DAT_00784d14 != 0) && (-1 < (int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276])) &&
                 ((int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276] < 6)) goto LAB_004050ba;
              goto LAB_004050c0;
            }
            sVar3 = DAT_00784d14;
            if (iVar2 == 0x4e) goto LAB_00404b88;
            if (iVar2 == 0x4f) {
              if ((DAT_00784d14 == 0) || (iVar9 = (&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276], iVar9 < 8)
                 ) goto LAB_004050c0;
              bVar11 = SBORROW4(iVar9,0xb);
              iVar9 = iVar9 + -0xb;
              goto LAB_00404bac;
            }
            if (iVar2 == 7) {
              bVar11 = DAT_00784ce6 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x1e) {
              bVar11 = DAT_00784cf8 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x22) {
              bVar11 = DAT_00784cf8 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x21) {
              bVar11 = DAT_00784cfa == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x23) {
              bVar11 = DAT_00784cfa == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x20) {
              bVar11 = DAT_00784cfc == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xe2) {
              bVar11 = DAT_00784d3a == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x4b) {
              bVar11 = DAT_00784cf4 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xc0) {
              bVar11 = DAT_00784cf4 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x4c) {
              iVar9 = FUN_00443b90(7);
              if (iVar9 == 0) goto LAB_004050c0;
              bVar11 = DAT_00784cea == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xb5) {
              bVar11 = DAT_00784d38 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xb6) {
              bVar11 = DAT_00784d36 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xa9) {
              bVar11 = DAT_00784d22 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xad) {
              bVar11 = DAT_00784d24 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xca) {
              bVar11 = DAT_00784d26 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x39) {
              if ((DAT_00784d2a != 0) || (DAT_00784d2c != 0)) goto LAB_004050ba;
              bVar11 = DAT_00784d2e == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x5e) {
              bVar11 = DAT_00784d30 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x28) {
              bVar11 = DAT_00784d2e == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x2c) {
              bVar11 = DAT_00784d2c == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x2d) {
              bVar11 = DAT_00784d2a == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x5f) {
              bVar11 = DAT_00784d28 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xbe) {
              bVar11 = DAT_00784cf0 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xbf) {
              bVar11 = DAT_00784cee == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xc1) {
              bVar11 = DAT_00784cf2 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0xb0) {
              if ((DAT_00784d22 != 0) || (DAT_00784d24 != 0)) goto LAB_004050ba;
              bVar11 = DAT_00784d26 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 == 0x67) {
              uVar12 = 3;
              goto LAB_00404b4a;
            }
            if (iVar2 == 0x69) {
              uVar12 = 4;
              goto LAB_00404b4a;
            }
            if (iVar2 == 0x68) {
              uVar12 = 5;
              goto LAB_00404b4a;
            }
            if (iVar2 == 0x70) {
              bVar11 = DAT_00784d32 == 0;
              goto LAB_00404b54;
            }
            if (iVar2 != 0x71) {
              if (iVar2 == 0x6a) {
                uVar12 = 0x18;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xa2) {
                uVar12 = 0x12;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xd9) {
                uVar12 = 0x1d;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0x6c) {
                uVar12 = 0x14;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xdd) {
                uVar12 = 0x1e;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0x6b) {
                uVar12 = 0x19;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xd8) {
                uVar12 = 0x1a;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xb3) {
                uVar12 = 0x18;
                goto LAB_00404b4a;
              }
              if (iVar2 == 0xcd) {
                bVar11 = DAT_00784d34 == 0;
                goto LAB_00404b54;
              }
              if (iVar2 == 0xcc) {
                if ((DAT_00e38ebc == 0) && (DAT_00e38ea8 < '\a')) goto LAB_004050c0;
                iVar9 = FUN_00443b90(0xb);
                if ((((iVar9 != 0) || (iVar9 = FUN_00443d90(0xb), iVar9 != 0)) &&
                    (iVar9 = FUN_00443b90(1), iVar9 != 0)) ||
                   ((((iVar9 = FUN_00443b90(0xb), iVar9 != 0 ||
                      (iVar9 = FUN_00443d90(0xb), iVar9 != 0)) &&
                     (iVar9 = FUN_004439c0(9), iVar9 != 0)) ||
                    ((iVar9 = FUN_00443b90(1), iVar9 != 0 && (iVar9 = FUN_004439c0(0xb), iVar9 != 0)
                     ))))) goto LAB_004050ba;
                iVar9 = FUN_004439c0(9);
                if (iVar9 == 0) goto LAB_004050c0;
                iVar9 = FUN_004439c0(0xb);
                goto LAB_00404b4f;
              }
              if (((iVar2 == 0x9c) || (iVar2 == 0x9d)) ||
                 ((iVar2 == 0x9e || ((iVar2 == 0x9f || (iVar2 == 0xa0)))))) {
                (&DAT_0070a8dc)[iVar1] = 0;
              }
              if ((((iVar2 == 0x97) || (iVar2 == 0x98)) || (iVar2 == 0x99)) ||
                 ((iVar2 == 0x9a || (iVar2 == 0x9b)))) {
                (&DAT_0070a8dc)[iVar1] = 0;
              }
              if (((iVar2 == 0x91) || (iVar2 == 0x92)) ||
                 ((iVar2 == 0x93 || ((iVar2 == 0x94 || (iVar2 == 0x95)))))) {
                (&DAT_0070a8dc)[iVar1] = 0;
              }
              if ((iVar2 == 0xd3) || (iVar2 == 0xd4)) goto LAB_004050ba;
              goto LAB_004050c0;
            }
            iVar9 = FUN_00443d90(0x12);
            if (iVar9 == 0) goto LAB_004050c0;
            if ((DAT_00e38ea8 != '\x05') || (DAT_00e38ebc != 0)) goto LAB_004050ba;
          }
          local_8 = (int)DAT_00e38ea8;
          iVar9 = 0;
          piVar7 = &DAT_005794b4;
        }
        do {
          if ((local_8 == piVar7[-1]) && (iVar10 = *piVar7, 0 < iVar10)) {
            piVar8 = (int *)piVar7[1];
            do {
              if (iVar2 == *piVar8) {
                (&DAT_0070a8dc)[iVar1] = (uint)((&DAT_007027b8)[iVar9] != '\0');
              }
              piVar8 = piVar8 + 1;
              iVar10 = iVar10 + -1;
            } while (iVar10 != 0);
          }
          piVar7 = piVar7 + 7;
          iVar9 = iVar9 + 1;
        } while ((int)piVar7 < 0x57978c);
      }
LAB_0040513f: ;
      local_10 = local_10 + 1;
    } while (local_10 < 0x1e);
    local_c = local_c + 1;
    if (0x25 < local_c) {
      return;
    }
  } while( true );
}
