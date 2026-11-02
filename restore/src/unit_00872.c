#include "restore_types.h"
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea4528 (*(undefined4 *)0x00ea4528u)
#define DAT_00ea5d50 (*(undefined4 *)0x00ea5d50u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde40();

void FUN_0045a7b0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  if (0 < *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8)) {
    iVar2 = *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
    if (0 < (int)(&DAT_00ea4528)[DAT_00eaac28 * 0x1276]) {
      if (iVar2 < 0x1389) {
        if (iVar2 < 0xbb9) {
          if (iVar2 < 0x7d1) {
            if (iVar2 < 0x3e9) {
              if (iVar2 < 0x1f5) {
                (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 0x32;
              }
              else {
                (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 0x96;
              }
            }
            else {
              (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 0xe1;
            }
          }
          else {
            (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 300;
          }
        }
        else {
          (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 400;
        }
      }
      else {
        (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 500;
      }
      iVar2 = FUN_004cde40((&DAT_00ea4528)[iVar1 * 0x1276],0x19);
      iVar1 = DAT_00eaac28 * 0x1276;
      if ((int)(&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] < iVar2) {
        (&DAT_00ea5d50)[iVar1] = iVar2;
      }
      (&DAT_00ea5d50)[iVar1] = 0;
      return;
    }
    if (2000 < iVar2) {
      (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 200;
      (&DAT_00ea5d50)[iVar1 * 0x1276] = 0;
      return;
    }
    if (1000 < iVar2) {
      (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 100;
      (&DAT_00ea5d50)[iVar1 * 0x1276] = 0;
      return;
    }
  }
  (&DAT_00ea5d50)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea5d50)[iVar1 * 0x1276] = 0;
  return;
}
