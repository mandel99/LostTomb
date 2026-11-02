#include "restore_types.h"
#include <stdint.h>
int *FUN_004ded50(int *output,int index,int rotation,int orientation)
{
  const int *source=(const int *)(uintptr_t)(0x005e2ad8u+(uint32_t)index*0x34u);
  int i;

  for (i=0;i<13;i++) output[i]=source[i];

  if (orientation!=0 && orientation!=4) {
    int old_x;
    if (rotation==4 || rotation==6) {
      output[0]=22-output[0]-output[2];
      output[1]=8-output[1]-output[2];
    }
    old_x=output[0];
    output[0]=output[1];
    output[1]=old_x;
    output[4]=output[0]+old_x*228;
    return output;
  }

  if (rotation==4 || rotation==2) {
    int x=22-output[0]-output[2];
    int y=8-output[1]-output[2];
    output[0]=x;
    output[1]=y;
    output[4]=x+y*228;
  }
  return output;
}
