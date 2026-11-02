#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799b4c (*(undefined4 *)0x00799b4cu)

void FUN_00454e20(void)

{
  byte *pbVar1;
  int iVar2;
  
  if (0 < DAT_0078d790) {
    pbVar1 = (byte *)&DAT_00799b4c;
    iVar2 = DAT_0078d790;
    do {
      if ((pbVar1[-0x6c] == 1) && (pbVar1[-0x67] != 0)) {
        if (*pbVar1 < 2) {
          *pbVar1 = 0;
        }
        else {
          *pbVar1 = *pbVar1 - 1;
        }
        if (pbVar1[1] < 2) {
          pbVar1[1] = 0;
        }
        else {
          pbVar1[1] = pbVar1[1] - 1;
        }
        if (pbVar1[3] < 2) {
          pbVar1[3] = 0;
        }
        else {
          pbVar1[3] = pbVar1[3] - 1;
        }
        if (pbVar1[5] < 2) {
          pbVar1[5] = 0;
        }
        else {
          pbVar1[5] = pbVar1[5] - 1;
        }
        if (pbVar1[0x1a] < 2) {
          pbVar1[0x1a] = 0;
        }
        else {
          pbVar1[0x1a] = pbVar1[0x1a] - 1;
        }
        if (pbVar1[4] < 2) {
          pbVar1[4] = 0;
        }
        else {
          pbVar1[4] = pbVar1[4] - 1;
        }
        if (pbVar1[6] < 2) {
          pbVar1[6] = 0;
        }
        else {
          pbVar1[6] = pbVar1[6] - 1;
        }
        if (pbVar1[7] < 2) {
          pbVar1[7] = 0;
        }
        else {
          pbVar1[7] = pbVar1[7] - 1;
        }
        if (pbVar1[9] < 2) {
          pbVar1[9] = 0;
        }
        else {
          pbVar1[9] = pbVar1[9] - 1;
        }
        if (pbVar1[10] < 2) {
          pbVar1[10] = 0;
        }
        else {
          pbVar1[10] = pbVar1[10] - 1;
        }
        if (pbVar1[2] < 2) {
          pbVar1[2] = 0;
        }
        else {
          pbVar1[2] = pbVar1[2] - 1;
        }
        if (pbVar1[0xb] < 2) {
          pbVar1[0xb] = 0;
        }
        else {
          pbVar1[0xb] = pbVar1[0xb] - 1;
        }
        if (pbVar1[0xc] < 2) {
          pbVar1[0xc] = 0;
        }
        else {
          pbVar1[0xc] = pbVar1[0xc] - 1;
        }
        if (pbVar1[0xd] < 2) {
          pbVar1[0xd] = 0;
        }
        else {
          pbVar1[0xd] = pbVar1[0xd] - 1;
        }
        if (pbVar1[0xe] < 2) {
          pbVar1[0xe] = 0;
        }
        else {
          pbVar1[0xe] = pbVar1[0xe] - 1;
        }
        if (pbVar1[0xf] < 2) {
          pbVar1[0xf] = 0;
        }
        else {
          pbVar1[0xf] = pbVar1[0xf] - 1;
        }
        if (pbVar1[0x10] < 2) {
          pbVar1[0x10] = 0;
        }
        else {
          pbVar1[0x10] = pbVar1[0x10] - 1;
        }
        if (pbVar1[0x11] < 2) {
          pbVar1[0x11] = 0;
        }
        else {
          pbVar1[0x11] = pbVar1[0x11] - 1;
        }
      }
      pbVar1 = pbVar1 + 0x108;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}
