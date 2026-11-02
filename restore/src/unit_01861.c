#include "restore_types.h"
#define DAT_005763bc (*(undefined4 *)0x005763bcu)
#define DAT_00e94618 (*(undefined4 *)0x00e94618u)
#define DAT_00e9461c (*(undefined4 *)0x00e9461cu)
#define DAT_00e94620 (*(undefined4 *)0x00e94620u)
#define DAT_00e94624 (*(undefined4 *)0x00e94624u)
#define s_dialog_loc_005e1c44 (*(char (*)[11])0x005e1c44u)
extern void FUN_004d6900();
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern uint FUN_00561ca1();

void FUN_004d69b0(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined4 local_4;
  
  iVar1 = FUN_00561a0a(s_dialog_loc_005e1c44,&DAT_005763bc);
  if (iVar1 != 0) {
    iVar2 = 0;
    piVar3 = &DAT_00e9461c;
    while( true ) {
      FUN_00561ca1(&local_4,4,1,iVar1);
      FUN_004d6900(iVar1,local_4,piVar3);
      if (*piVar3 != 0) {
        piVar3[2] = 0;
      }
      FUN_00561ca1(&local_4,4,1,iVar1);
      FUN_004d6900(iVar1,local_4,piVar3 + -1);
      FUN_00561ca1(&local_4,4,1,iVar1);
      FUN_004d6900(iVar1,local_4,piVar3 + 1);
      if ((*(byte *)(iVar1 + 0xc) & 0x30) != 0) break;
      piVar3 = piVar3 + 4;
      iVar2 = iVar2 + 1;
      if (0xe94d9b < (int)piVar3) {
        FUN_00561994(iVar1);
        return;
      }
    }
    (&DAT_00e9461c)[iVar2 * 4] = 0;
    (&DAT_00e94618)[iVar2 * 4] = 0;
    (&DAT_00e94620)[iVar2 * 4] = 0;
    (&DAT_00e94624)[iVar2 * 4] = 1;
    FUN_00561994(iVar1);
  }
  return;
}
