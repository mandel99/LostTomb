#include "restore_types.h"
#define DAT_005da498 (*(undefined4 *)0x005da498u)
#define DAT_005da4f0 (*(undefined4 *)0x005da4f0u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0045a0a0();

undefined4 FUN_0045a140(int param_1)

{
  undefined4 uVar1;
  
  if ((&DAT_007999d8)[param_1 * 0x108] == '\x01') {
    switch((&DAT_007999e8)[param_1 * 0x84]) {
    case 0x4d: ;
      uVar1 = FUN_0045a0a0((&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276],0,&DAT_005da498);
      return uVar1;
    case 0x4e: ;
      uVar1 = FUN_0045a0a0((&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276],1,&DAT_005da498);
      return uVar1;
    case 0x4f: ;
      uVar1 = FUN_0045a0a0((&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276],2,&DAT_005da498);
      return uVar1;
    case 0x54: ;
    case 0x55: ;
    case 0xbb: ;
    case 0xbc: ;
    case 0xbd: ;
      uVar1 = FUN_0045a0a0(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),2,&DAT_005da4f0);
      return uVar1;
    case 0x56: ;
    case 0x57: ;
      uVar1 = FUN_0045a0a0(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),0,&DAT_005da4f0);
      return uVar1;
    case 0xb8: ;
      uVar1 = FUN_0045a0a0(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),1,&DAT_005da4f0);
      return uVar1;
    }
  }
  return 0;
}
