#include "restore_types.h"
#define DAT_00e39174 (*(undefined *)0x00e39174u)
#define DAT_00e39178 (*(undefined *)0x00e39178u)
#define DAT_00e92e04 (*(undefined *)0x00e92e04u)
#define DAT_00f53ff4 (*(undefined *)0x00f53ff4u)
#define DAT_00f54004 (*(undefined *)0x00f54004u)
#define DAT_00f54008 (*(undefined *)0x00f54008u)
#define DAT_00f5400c (*(undefined4 *)0x00f5400cu)
#define DAT_00f54054 (*(undefined *)0x00f54054u)
#define DAT_00f540a8 (*(undefined *)0x00f540a8u)
#define DAT_00f540b8 (*(undefined *)0x00f540b8u)
#define DAT_00f540cc (*(undefined *)0x00f540ccu)
#define DAT_00f54114 (*(undefined *)0x00f54114u)
#define s_ERR_SMK_releasing_write_to_prima_00576364 (*(char (*)[56])0x00576364u)
#define s_MEM_Max_alloc_after___005f8fe4 (*(char (*)[23])0x005f8fe4u)
#define s_OK__SMK_natural_end_of_005f902c (*(char (*)[24])0x005f902cu)
extern int FUN_00427510();
extern void FUN_004ce2d0();
extern void FUN_004ce6e0();
extern undefined4 FUN_004cff20();
extern void FUN_00539220();

undefined4 FUN_00538fa0(void)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  
  if (DAT_00f54054 == 0) {
    return 0;
  }
  iVar1 = _SmackWait_4(DAT_00f54114);
  if (iVar1 != 0) {
    return 0;
  }
  puVar3 = &DAT_00f540a8;
  for (iVar1 = 0x1b; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  DAT_00f540a8 = 0x6c;
  if (DAT_00e92e04 == 0) {
    iVar1 = (**(code **)(*(undefined4 *)(DAT_00e39174) + 100))(DAT_00e39174,0,&DAT_00f540a8,1,0);
    if (iVar1 != 0) goto LAB_005390f5;
    _SmackToBuffer_28(DAT_00f54114,DAT_00f54008,DAT_00f54004,DAT_00f540b8,
                      *(undefined4 *)(DAT_00f54114 + 8),DAT_00f540cc,DAT_00f53ff4);
    _SmackDoFrame_4(DAT_00f54114);
    piVar2 = DAT_00e39174;
  }
  else {
    iVar1 = (**(code **)(*(undefined4 *)(DAT_00e39178) + 100))(DAT_00e39178,0,&DAT_00f540a8,1,0);
    if (iVar1 == -0x7789fe3e) {
      do {
        iVar1 = (**(code **)(*(undefined4 *)(DAT_00e39178) + 0x6c))(DAT_00e39178);
      } while (iVar1 == 0);
      goto LAB_005390f5;
    }
    if (iVar1 != 0) {
      iVar1 = FUN_00427510(iVar1);
      FUN_004ce6e0(s_ERR_SMK_releasing_write_to_prima_00576364,0,iVar1 + 1);
      DAT_00e92e04 = 0;
      goto LAB_005390f5;
    }
    _SmackToBuffer_28(DAT_00f54114,DAT_00f54008,DAT_00f54004,DAT_00f540b8,
                      *(undefined4 *)(DAT_00f54114 + 8),DAT_00f540cc,DAT_00f53ff4);
    _SmackDoFrame_4(DAT_00f54114);
    piVar2 = DAT_00e39178;
  }
  (**(code **)(*piVar2 + 0x80))(piVar2,DAT_00f540cc);
LAB_005390f5: ;
  if (*(uint *)(DAT_00f54114 + 0x374) < *(int *)(DAT_00f54114 + 0xc) - 1U) {
    _SmackNextFrame_4(DAT_00f54114);
    return 1;
  }
  if (DAT_00f54114 != 0) {
    _SmackClose_4(DAT_00f54114);
    DAT_00f54114 = 0;
  }
  FUN_004ce2d0();
  FUN_004ce6e0(s_OK__SMK_natural_end_of_005f902c,&DAT_00f5400c,0);
  iVar1 = FUN_004cff20();
  FUN_004ce6e0(s_MEM_Max_alloc_after___005f8fe4,0,iVar1 + 1);
  FUN_00539220(0);
  return 1;
}
