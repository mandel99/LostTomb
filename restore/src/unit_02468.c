#include "restore_types.h"
#define DAT_00e38e63 (*(char *)0x00e38e63u)
#define DAT_00e3914c (*(undefined *)0x00e3914cu)
#define DAT_00f53ff4 (*(undefined *)0x00f53ff4u)
#define DAT_00f53ffc (*(undefined1 *)0x00f53ffcu)
#define DAT_00f54004 (*(undefined *)0x00f54004u)
#define DAT_00f54008 (*(undefined *)0x00f54008u)
#define DAT_00f5400c (*(undefined4 *)0x00f5400cu)
#define DAT_00f5404c (*(undefined *)0x00f5404cu)
#define DAT_00f54050 (*(undefined *)0x00f54050u)
#define DAT_00f54054 (*(undefined *)0x00f54054u)
#define DAT_00f54114 (*(undefined *)0x00f54114u)
#define DAT_012663ac (*(uint32_t *)0x012663acu)
#define _DAT_00f54000 (*(undefined4 *)0x00f54000u)
#define s_ERR_SMK_Could_not_open_005f8ffc (*(char (*)[24])0x005f8ffcu)
#define s_MEM_Max_alloc_after___005f8fe4 (*(char (*)[23])0x005f8fe4u)
#define s_OK__SMK_starting_smack_005f9014 (*(char (*)[24])0x005f9014u)
extern void FUN_004cbea0();
extern bool FUN_004ce100();
extern void FUN_004ce2d0();
extern undefined4 FUN_004ce450();
extern void FUN_004ce6e0();
extern undefined4 FUN_004cff20();
extern void FUN_004d0280();
extern void FUN_0051f780();
extern undefined4 FUN_00538fa0();

undefined4 FUN_00538e30(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  undefined4 uVar2;
  
  DAT_00f54054 = 0;
  if (DAT_00f54050 == 0) {
    _SmackSoundUseMSS_4(*(undefined4 *)(DAT_012663ac + 0x178));
    DAT_00f54050 = 1;
  }
  FUN_004ce6e0(s_OK__SMK_starting_smack_005f9014,param_1,0);
  DAT_00f54008 = param_2;
  uVar2 = 0;
  DAT_00f53ff4 = (-(uint)(DAT_00e3914c != 0x565) & 0xc0000000) + 0xc0000000;
  DAT_00f54114 = 0;
  DAT_00f54004 = param_3;
  _DAT_00f54000 = param_4;
  FUN_004cbea0(param_1,&DAT_00f5400c,0x40);
  DAT_00f5404c = FUN_004ce450(&DAT_00f5400c);
  if (DAT_00f5404c == 0) {
    FUN_004ce100(&DAT_00f5400c);
  }
  if (*(int *)(DAT_012663ac + 0x178) != 0) {
    uVar2 = 0x2000;
    if (((DAT_00f53ffc != 0x41) && (DAT_00f53ffc != 0x3c)) && (DAT_00e38e63 == '\0')) {
      uVar2 = 0;
    }
  }
  DAT_00f54114 = _SmackOpen_12(&DAT_00f5400c,uVar2,0xffffffff);
  if (DAT_00f54114 == 0) {
    FUN_004ce2d0();
    FUN_004ce6e0(s_ERR_SMK_Could_not_open_005f8ffc,&DAT_00f5400c,0);
    iVar1 = FUN_004cff20();
    FUN_004ce6e0(s_MEM_Max_alloc_after___005f8fe4,0,iVar1 + 1);
    return 0;
  }
  FUN_0051f780(0);
  FUN_0051f780(1);
  FUN_004d0280(1);
  DAT_00f54054 = 1;
  FUN_00538fa0(1);
  FUN_004ce2d0();
  return 1;
}
