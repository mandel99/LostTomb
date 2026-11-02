#include "restore_types.h"
#define DAT_00e92e3c (*(undefined4 *)0x00e92e3cu)
#define DAT_00e92e40 (*(undefined4 *)0x00e92e40u)
#define DAT_00e92e44 (*(undefined4 *)0x00e92e44u)
extern void FUN_004cbea0();
extern void * FUN_00561b68();
extern void FUN_00561bdc();

undefined4 FUN_004cee00(int param_1)

{
  undefined1 *puVar1;
  void *pvVar2;
  int iVar3;
  undefined1 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  void *pvVar8;
  undefined1 local_38 [10];
  int local_2e;
  undefined1 local_28 [4];
  int local_24;
  int local_20;
  size_t local_14;
  
  FUN_004cbea0(param_1,local_38,0xe);
  FUN_004cbea0(param_1 + 0xe,local_28,0x28);
  DAT_00e92e3c = local_24;
  DAT_00e92e40 = local_20;
  DAT_00e92e44 = local_14;
  pvVar2 = FUN_00561b68(local_14);
  if (pvVar2 != (void *)0x0) {
    iVar3 = DAT_00e92e44 + local_2e + param_1;
    iVar6 = 0;
    iVar5 = DAT_00e92e3c;
    pvVar8 = pvVar2;
    if (0 < DAT_00e92e40) {
      do {
        iVar7 = 0;
        if (0 < iVar5) {
          puVar4 = (undefined1 *)(iVar5 + -1 + (int)pvVar8);
          do {
            puVar1 = (undefined1 *)(iVar3 + -1);
            iVar3 = iVar3 + -1;
            *puVar4 = *puVar1;
            iVar7 = iVar7 + 1;
            puVar4 = puVar4 + -1;
            iVar5 = DAT_00e92e3c;
          } while (iVar7 < DAT_00e92e3c);
        }
        iVar6 = iVar6 + 1;
        pvVar8 = (void *)((int)pvVar8 + iVar5);
      } while (iVar6 < DAT_00e92e40);
    }
    FUN_004cbea0(pvVar2,local_2e + param_1,DAT_00e92e44);
    FUN_00561bdc(pvVar2);
    return 1;
  }
  return 0;
}
