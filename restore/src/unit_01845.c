#include "restore_types.h"

int * FUN_004d5fe0(int param_1,int param_2,int param_3,int param_4)
{
  int *node;

  if (param_1 == 1) {
    node = *(int **)(uintptr_t)(0x00e94618u + (unsigned int)param_2 * 0x10u);
  }
  else if (param_1 == 2) {
    node = *(int **)(uintptr_t)(0x00e9461cu + (unsigned int)param_2 * 0x10u);
  }
  else {
    if (param_1 != 3 && param_1 != 4) return NULL;
    node = *(int **)(uintptr_t)(0x00e94620u + (unsigned int)param_2 * 0x10u);
  }

  while (node) {
    if (node[0] == param_3 && node[1] == param_4) return node;
    node = (int *)(uintptr_t)(unsigned int)node[6];
  }
  return NULL;
}
