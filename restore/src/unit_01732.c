#include "menu_list_scroll.h"
#include <stdlib.h>
int32_t FUN_004cde80(int32_t value,int32_t divisor)
{
 int32_t quotient;
 if(!divisor)return 0;
 /* The original IDIV traps in this one unrepresentable signed quotient. */
 if(value==INT32_MIN&&divisor==-1)abort();
 quotient=value/divisor;
 return quotient+(value%divisor!=0);
}
