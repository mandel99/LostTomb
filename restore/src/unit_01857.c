#include "restore_types.h"

void FUN_004d66f0(int param_1,int *param_2)
{
  int *node = param_2;

  while (node) {
    unsigned short *rectangle = (unsigned short *)(uintptr_t)
        ((unsigned int)param_1 + (unsigned int)node[0] * 0x1cu - 0x1cu);
    rectangle[0] = (unsigned short)node[2];
    rectangle[1] = (unsigned short)node[3];
    rectangle[2] = (unsigned short)((unsigned short)node[4] + (unsigned short)node[2]);
    rectangle[3] = (unsigned short)((unsigned short)node[5] + (unsigned short)node[3]);
    node = (int *)(uintptr_t)(unsigned int)node[6];
  }
}
