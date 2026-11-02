#include "restore_types.h"
#define DAT_0057626c (*(undefined4 *)0x0057626cu)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784904 (*(undefined *)0x00784904u)
#define DAT_00784908 (*(undefined *)0x00784908u)
#define DAT_0078490c (*(undefined *)0x0078490cu)
#define DAT_00784910 (*(undefined *)0x00784910u)
#define DAT_00784914 (*(undefined4 *)0x00784914u)
#define DAT_00784954 (*(undefined4 *)0x00784954u)
#define DAT_00784be0 (*(undefined4 *)0x00784be0u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define PTR_DAT_00576274 (*(unsigned char * *)0x00576274u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
#define _DAT_00784c90 (*(undefined4 *)0x00784c90u)
#define _DAT_00784c92 (*(undefined4 *)0x00784c92u)
extern void FUN_00430530();
extern void FUN_00444a80();
extern void FUN_0046d480();
extern void FUN_0046f7b0();
extern undefined4 FUN_0046f930();
extern void FUN_0046fa40();
extern void FUN_004bb090();
extern void FUN_004bb3f0();
extern void FUN_004bbc10();
extern void FUN_004bbd40();
extern void FUN_004cbea0();
extern void FUN_004cc560();
extern void FUN_004ce3a0();
extern void FUN_004ce3c0();
extern undefined4 FUN_004d5820();
extern void FUN_004d5980();
extern void FUN_004ddf90();
extern void FUN_004de080();
extern void FUN_004dea50();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178b0();
extern void FUN_005178e0();
extern void FUN_00517a20();

void FUN_004de600(void)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  short *psVar6;
  char *pcVar7;
  char *pcVar8;
  char local_64 [100];
  
  FUN_0046fa40();
  FUN_004ce3a0(&DAT_00e37078);
  FUN_004cbea0(&PTR_DAT_00576274,&DAT_00e38aec,3);
  FUN_004ce3c0(&DAT_00e37078);
  uVar2 = 0xffffffff;
  pcVar5 = &DAT_0057626c;
  do {
    pcVar8 = pcVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar8 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar8;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar5 = pcVar8 + -uVar2;
  pcVar8 = local_64;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar8 = *pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar8 = pcVar8 + 1;
  }
  uVar2 = 0xffffffff;
  pcVar5 = (char *)&DAT_00e37078;
  do {
    pcVar8 = pcVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar8 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar8;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  iVar4 = -1;
  pcVar5 = local_64;
  do {
    pcVar7 = pcVar5;
    if (iVar4 == 0) break;
    iVar4 = iVar4 + -1;
    pcVar7 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar7;
  } while (cVar1 != '\0');
  pcVar5 = pcVar8 + -uVar2;
  pcVar8 = pcVar7 + -1;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar8 = *pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar8 = pcVar8 + 1;
  }
  FUN_004d5820(local_64,0);
  FUN_004ce3a0(&DAT_00e37078);
  FUN_00444a80();
  DAT_00bca2d0 = DAT_00784904;
  DAT_00bca2d4 = DAT_00784908;
  DAT_00bca2d8 = DAT_0078490c;
  DAT_00bca2dc = DAT_00784910;
  FUN_0046f7b0();
  psVar6 = &DAT_00784be0;
  do {
    cVar1 = FUN_0046f930(DAT_00bca2e8 + psVar6[-0x10],DAT_00bca2ec + *psVar6);
    if (cVar1 == '\0') {
      psVar6[-0x10] = -1;
      *psVar6 = -1;
    }
    cVar1 = FUN_0046f930(DAT_00bca2e8 + psVar6[-0x10],DAT_00bca2ec + *psVar6);
    if (cVar1 == '\0') {
      psVar6[-0x10] = -1;
      *psVar6 = -1;
    }
    psVar6 = psVar6 + 1;
  } while ((int)psVar6 < 0x784c00);
  FUN_004cc560(5,&DAT_00784914,0x18,0x118,0,DAT_00f56f84);
  FUN_004cc560(6,&DAT_00784954,500,400,0,DAT_00f56f84);
  FUN_00430530(DAT_00e94e04,1);
  if (DAT_00605a3c == 1) {
    FUN_004bb090();
    if ((((_DAT_00784c8c != -1) && (_DAT_00784c8e != -1)) && (_DAT_00784c90 != -1)) &&
       (_DAT_00784c92 != -1)) {
      FUN_004bbd40((int)_DAT_00784c8c,(int)_DAT_00784c8e,1);
    }
  }
  FUN_004bbc10();
  FUN_004bb3f0();
  FUN_004d5980();
  FUN_004dea50();
  FUN_0046d480();
  FUN_004ddf90();
  FUN_004de080();
  FUN_00517a20();
  FUN_005178b0();
  FUN_00517370();
  FUN_00517660();
  FUN_005178e0();
  return;
}
