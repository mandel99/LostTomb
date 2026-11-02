#include "restore_types.h"
#define DAT_00ea5b7c (*(undefined4 *)0x00ea5b7cu)
#define DAT_00ea5b80 (*(undefined4 *)0x00ea5b80u)
#define DAT_00ea5b84 (*(undefined4 *)0x00ea5b84u)
#define DAT_00ea5b94 (*(undefined4 *)0x00ea5b94u)
#define DAT_00ea5b98 (*(undefined4 *)0x00ea5b98u)
#define DAT_00ea5bac (*(undefined4 *)0x00ea5bacu)
#define DAT_00ea5bc0 (*(undefined4 *)0x00ea5bc0u)
#define DAT_00ea5bc4 (*(undefined4 *)0x00ea5bc4u)
#define DAT_00ea5bc8 (*(undefined4 *)0x00ea5bc8u)
#define DAT_00ea5bf8 (*(undefined4 *)0x00ea5bf8u)
#define DAT_00ea5c80 (*(undefined4 *)0x00ea5c80u)
#define DAT_00ea5c8c (*(undefined4 *)0x00ea5c8cu)
#define DAT_00ea5c90 (*(undefined4 *)0x00ea5c90u)
#define DAT_00ea5c94 (*(undefined4 *)0x00ea5c94u)
#define DAT_00ea5c98 (*(undefined4 *)0x00ea5c98u)
#define DAT_00ea5db0 (*(undefined4 *)0x00ea5db0u)
#define DAT_00ea5fbc (*(undefined4 *)0x00ea5fbcu)
#define DAT_00eaac28 (*(undefined4 *)0x00eaac28u)

void FUN_004e9500(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  iVar3 = (&DAT_00ea5db0)[DAT_00eaac28 * 0x1276];
  if (iVar3 == 1) {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 2;
  }
  else if (iVar3 == 2) {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 1;
  }
  else if (iVar3 == 3) {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 3;
  }
  else if (iVar3 == 4) {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 0;
  }
  else if (iVar3 == 5) {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 4;
  }
  else {
    (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] = 5;
  }
  (&DAT_00ea5c8c)[iVar1 * 0x1276] = 0;
  iVar3 = 0;
  if ((0 < (int)(&DAT_00ea5b98)[iVar1 * 0x1276]) ||
      (0 < *(int *)(0x00ea5fbcu + (uint)iVar2))) {
    if ((int)(&DAT_00ea5b98)[iVar1 * 0x1276] <
        *(int *)(0x00ea5fbcu + (uint)iVar2)) {
      (&DAT_00ea5c8c)[iVar1 * 0x1276] = 4;
      iVar3 = *(int *)(0x00ea5fbcu + (uint)iVar2);
    }
    else {
      (&DAT_00ea5c8c)[iVar1 * 0x1276] = 1;
      iVar3 = (&DAT_00ea5b98)[iVar1 * 0x1276];
    }
  }
  if (iVar3 < (int)(&DAT_00ea5b94)[iVar1 * 0x1276]) {
    (&DAT_00ea5c8c)[iVar1 * 0x1276] = 2;
    iVar3 = (&DAT_00ea5b94)[iVar1 * 0x1276];
  }
  if (iVar3 < (int)(&DAT_00ea5bac)[iVar1 * 0x1276]) {
    (&DAT_00ea5c8c)[iVar1 * 0x1276] = 3;
  }
  (&DAT_00ea5c94)[iVar1 * 0x1276] = 0;
  iVar3 = (&DAT_00ea5b80)[iVar1 * 0x1276];
  iVar2 = (&DAT_00ea5b84)[iVar1 * 0x1276];
  if (iVar2 < iVar3) {
    (&DAT_00ea5c94)[iVar1 * 0x1276] = 1;
  }
  else if (iVar3 < iVar2) {
    (&DAT_00ea5c94)[iVar1 * 0x1276] = 2;
  }
  else if ((iVar3 != 0) || (iVar2 != 0)) {
    (&DAT_00ea5c94)[iVar1 * 0x1276] = 3;
  }
  (&DAT_00ea5c98)[iVar1 * 0x1276] = 0;
  if ((int)(&DAT_00ea5b7c)[iVar1 * 0x1276] < (int)(&DAT_00ea5bf8)[iVar1 * 0x1276]) {
    (&DAT_00ea5c98)[iVar1 * 0x1276] = 1;
  }
  else if ((&DAT_00ea5b7c)[iVar1 * 0x1276] != 0) {
    (&DAT_00ea5c98)[iVar1 * 0x1276] = 2;
  }
  (&DAT_00ea5c90)[iVar1 * 0x1276] = 0;
  iVar3 = 0;
  if (0 < (int)(&DAT_00ea5bc0)[iVar1 * 0x1276]) {
    (&DAT_00ea5c90)[iVar1 * 0x1276] = 1;
    iVar3 = (&DAT_00ea5bc0)[iVar1 * 0x1276];
  }
  if (iVar3 < (int)(&DAT_00ea5bc4)[iVar1 * 0x1276]) {
    (&DAT_00ea5c90)[iVar1 * 0x1276] = 2;
    iVar3 = (&DAT_00ea5bc4)[iVar1 * 0x1276];
  }
  if (iVar3 < (int)(&DAT_00ea5bc8)[iVar1 * 0x1276]) {
    (&DAT_00ea5c90)[iVar1 * 0x1276] = 3;
  }
  return;
}
