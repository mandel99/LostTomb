#include "restore_types.h"
#define DAT_00d3b500 (*(int32_t *)0x00d3b500u)
#define DAT_00e20760 (*(int32_t *)0x00e20760u)
#define DAT_00e2076c (*(int32_t *)0x00e2076cu)
#define DAT_00e20774 (*(int32_t *)0x00e20774u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e20780 (*(int32_t *)0x00e20780u)
#define DAT_00e20799 (*(char *)0x00e20799u)
#define DAT_00e3439c (*(undefined4 *)0x00e3439cu)
extern void FUN_004cc770();
extern int FUN_004cd120();

void FUN_004cd010(char *param_1,undefined4 param_2,int param_3,int param_4,undefined4 param_5,
                 undefined4 param_6)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  
  bVar2 = true;
  iVar3 = 0;
  do {
    iVar3 = iVar3 + 1;
    if (99 < iVar3) {
      return;
    }
    iVar6 = 0;
    puVar7 = &DAT_00e3439c;
    for (iVar5 = 0x32; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar7 = 0;
      puVar7 = puVar7 + 1;
    }
    iVar5 = 0;
    if (bVar2) {
      do {
        while( true ) {
          if (param_4 <= iVar5) goto LAB_004cd09f;
          iVar4 = FUN_004cd120(param_1,param_5);
          iVar5 = iVar5 + iVar4;
          if (param_4 <= iVar5) break;
          iVar4 = DAT_00e20760;
          if (0 < DAT_00e20760) {
            do {
              cVar1 = *param_1;
              param_1 = param_1 + 1;
              *(char *)((int)&DAT_00e3439c + iVar6) = cVar1;
              iVar6 = iVar6 + 1;
              iVar4 = iVar4 + -1;
            } while (iVar4 != 0);
          }
          if (*param_1 == '\0') goto LAB_004cd097;
        }
      } while (iVar5 != 0);
LAB_004cd097: ;
      bVar2 = false;
    }
LAB_004cd09f: ;
    DAT_00e20780 = 1;
    DAT_00e20778 = 0;
    DAT_00e20799 = 1;
    FUN_004cc770(&DAT_00e3439c,param_2,param_3,param_5,param_6);
    if (DAT_00e2076c < DAT_00e20778) {
      DAT_00e2076c = DAT_00e20778;
    }
    param_3 = param_3 + 5 + DAT_00d3b500;
    DAT_00e20774 = DAT_00e20774 + 5 + DAT_00d3b500;
    if (!bVar2) {
      return;
    }
  } while( true );
}
