#include "restore_types.h"
#include "city_prepare.h"
#include "mission_schema.h"
#define DAT_007848dc (*(int16_t *)0x007848dcu)
#define DAT_00784904 (*(int32_t *)0x00784904u)
#define DAT_00784908 (*(int32_t *)0x00784908u)
#define DAT_0078490c (*(int32_t *)0x0078490cu)
#define DAT_00784910 (*(int32_t *)0x00784910u)
#define DAT_00784be0 (*(undefined4 *)0x00784be0u)
#define DAT_00784f10 (*(int32_t *)0x00784f10u)
#define DAT_0078d828 (*(int32_t *)0x0078d828u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9694 (*(int32_t *)0x00bc9694u)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2d8 (*(int32_t *)0x00bca2d8u)
#define DAT_00bca2dc (*(int32_t *)0x00bca2dcu)
#define DAT_00bca2e0 (*(int32_t *)0x00bca2e0u)
#define DAT_00bca2e8 (*(int32_t *)0x00bca2e8u)
#define DAT_00bca2ec (*(int32_t *)0x00bca2ecu)
#define DAT_00bca30c (*(char *)0x00bca30cu)
#define DAT_00bcb9b0 (*(int32_t *)0x00bcb9b0u)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00e38ebc (*(int32_t *)0x00e38ebcu)
#define DAT_00e945c0 (*(int32_t *)0x00e945c0u)
#define DAT_00e94e04 (*(int32_t *)0x00e94e04u)
#define DAT_00e94e0c (*(int32_t *)0x00e94e0cu)
#define DAT_00e94e10 (*(int32_t *)0x00e94e10u)
#define DAT_00e9511c (*(int32_t *)0x00e9511cu)
#define DAT_00e95120 (*(int32_t *)0x00e95120u)
#define DAT_00ea5f90 (*(undefined4 *)0x00ea5f90u)
#define DAT_00ea5f94 (*(undefined4 *)0x00ea5f94u)
#define DAT_00eaac28 (*(int32_t *)0x00eaac28u)
#define DAT_00eaac2c (*(undefined1 *)0x00eaac2cu)
extern void FUN_00404350();
extern void FUN_004051a0();
extern void FUN_0040f8f0();
extern void FUN_00417e70();
extern void FUN_0041d730();
extern void FUN_0041df60();
extern void FUN_00430530();
extern void FUN_00442660();
extern void FUN_004426c0();
extern void FUN_00443330();
extern void FUN_00457af0();
extern void FUN_0045aa30();
extern void FUN_0045e040();
extern void FUN_004677c0();
extern void FUN_00468170();
extern void FUN_00468860();
extern void FUN_0046d840();
extern int FUN_0046f190();
extern void FUN_0046f7b0();
extern undefined4 FUN_0046f930();
extern void FUN_00476750();
extern void FUN_004771c0();
extern void FUN_00485f70();
extern void FUN_00486e70();
extern void FUN_00489840();
extern void FUN_004a3560();
extern void FUN_004b32c0();
extern void FUN_004bc720();
extern void FUN_004bc7a0();
extern void FUN_004bca90();

extern void FUN_004d5ad0();
extern void FUN_004d5bc0();
extern void FUN_004ddf90();
extern void FUN_004de080();
extern void FUN_004dea50();
extern void FUN_004e1830();
extern void FUN_004e59a0();
extern void FUN_004e7e20();
extern void FUN_004f4520();
extern void FUN_004f61e0();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178b0();
extern void FUN_005178e0();
extern void FUN_00517a20();
extern void FUN_0053d970();
extern undefined4 FUN_00540c90();
extern undefined4 FUN_00540ed0();
extern undefined4 FUN_00541160();
extern void FUN_005596d0();

void FUN_004d52a0(void)

{
  int iVar1;
  char cVar2;
  short *psVar3;
  
  FUN_0040f8f0(&DAT_00b7482c,0x32c40,0);
  DAT_00784f10 = (int)DAT_007848dc;
  DAT_00bca2d0 = DAT_00784904;
  DAT_00bca2d4 = DAT_00784908;
  DAT_00bca2d8 = DAT_0078490c;
  DAT_00bca2dc = DAT_00784910;
  FUN_0046f7b0();
  FUN_00540c90();
  FUN_00540ed0();
  FUN_00541160();
  FUN_004f4520();
  FUN_0053d970();
  FUN_004bc720();
  FUN_004771c0();
  psVar3 = &DAT_00784be0;
  do {
    cVar2 = FUN_0046f930(DAT_00bca2e8 + psVar3[-0x10],DAT_00bca2ec + *psVar3);
    if (cVar2 == '\0') {
      psVar3[-0x10] = -1;
      *psVar3 = -1;
    }
    cVar2 = FUN_0046f930(DAT_00bca2e8 + psVar3[-0x10],DAT_00bca2ec + *psVar3);
    if (cVar2 == '\0') {
      psVar3[-0x10] = -1;
      *psVar3 = -1;
    }
    psVar3 = psVar3 + 1;
  } while ((int)psVar3 < 0x784c00);
  FUN_004e7e20();
  FUN_004677c0();
  FUN_00468170();
  if ((DAT_00e94e04 < 0) || (6 < DAT_00e94e04)) {
    DAT_00e94e04 = 0;
  }
  DAT_00bca2e0 = DAT_00e94e04;
  FUN_004e59a0();
  FUN_00485f70();
  restore_mission_schema_recover_terrain();
  if (DAT_00e945c0 < 0x8d) {
    DAT_00bca30c = 1;
    FUN_00486e70();
  }
  if (DAT_00e945c0 < 0x8e) {
    FUN_00489840();
  }
  FUN_004dea50();
  FUN_004be160(RESTORE_CITY_WORLD,0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  FUN_00430530(DAT_00e94e04,1);
  FUN_004ddf90();
  FUN_004de080();
  FUN_00517a20();
  FUN_005178b0();
  FUN_00517370();
  FUN_00517660();
  FUN_005178e0();
  FUN_004b32c0();
  FUN_00468860();
  FUN_00457af0();
  FUN_0045e040();
  FUN_00404350();
  FUN_004051a0();
  FUN_004e1830();
  FUN_0041d730();
  FUN_0041df60();
  DAT_00bc9694 = 0;
  DAT_00e94e0c = 0;
  DAT_00e94e10 = 0;
  FUN_00417e70();
  iVar1 = DAT_00eaac28;
  (&DAT_00ea5f94)[DAT_00eaac28 * 0x1276] = 1;
  (&DAT_00ea5f90)[iVar1 * 0x1276] = 1;
  FUN_00476750();
  FUN_0046d840();
  DAT_00e9511c = 0;
  DAT_00e95120 = 0;
  DAT_00e38e60 = 0;
  FUN_004bc7a0();
  FUN_004bca90();
  DAT_00eaac2c = 1;
  FUN_00442660();
  FUN_004426c0();
  DAT_00bcb9b0 = FUN_0046f190();
  FUN_004d5ad0();
  FUN_004d5bc0();
  if (DAT_00e38ebc != 0) {
    DAT_0078d828 = 0;
  }
  DAT_00e945c0 = 0xa0;
  FUN_00443330();
  FUN_005596d0();
  FUN_0045aa30();
  FUN_004f61e0();
  FUN_004a3560();
  return;
}
