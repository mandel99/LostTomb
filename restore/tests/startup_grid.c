#include "standalone_data.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern void FUN_004ddec0(void);
extern void FUN_004ddf20(void);
extern void FUN_004ddf70(void);
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Startup grid failure at line %d\n",__LINE__);abort();}}while(0)
int main(int argc,char **argv)
{
 unsigned int op,width,i;static const unsigned int widths[]={640,800,1024,123};
 REQUIRE(restore_data_open());
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
  puts("{\"grid_bytes\":100}");fflush(stdout);
  while(scanf("%u %u",&op,&width)==2) {
   memset((void *)0x70e430u,0x5a,100);U(0xe38db0)=width;
   if(op==0)FUN_004ddec0();else if(op==1)FUN_004ddf20();else FUN_004ddf70();
   putchar('"');for(i=0;i<100;++i)printf("%02x",*((unsigned char *)0x70e430u+i));puts("\"");fflush(stdout);
  }
 } else {
  for(op=0;op<2;++op)for(i=0;i<4;++i) {
   unsigned int cols=op==0?(i==0?10:i==1?12:i==2?16:8):(i==1?10:i==2?14:8),rows=i==1?39:i==2?50:31;
   U(0xe38db0)=widths[i];if(op==0)FUN_004ddec0();else FUN_004ddf20();
   REQUIRE(U(0x70e464)==cols&&U(0x70e460)==rows&&U(0x70e438)==60*cols-2);
   REQUIRE(U(0x70e430)==(rows-1)*15+30&&U(0x70e468)==228-cols);
  }
  FUN_004ddf70();REQUIRE(U(0x70e464)==5&&U(0x70e460)==15);
  puts("Startup grid: full dimensions/coordinates, mode selection and side-effect-only void dispatch verified.");
 }
 restore_data_close();return 0;
}
