#include "host_allocator.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
static unsigned int checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Allocator failure at line %d\n",__LINE__);abort();}}while(0)
int main(void)
{
    static const size_t sizes[]={0,1,15,16,17,224,480,481,1024,20000,65536};
    size_t i,j,k;
    for(i=0;i<sizeof(sizes)/sizeof(sizes[0]);++i) {
        size_t size=sizes[i],expected=((size?size:1)+15)&~(size_t)15;
        unsigned char *p=FUN_00561ba6((uint32_t)size),*q;
        REQUIRE(p&&FUN_005647c2(p)>=expected);
        memset(p,0x5a,expected);
        q=FUN_005645a0(p,(uint32_t)(expected+37));REQUIRE(q);
        for(j=0;j<expected;++j)REQUIRE(q[j]==0x5a);
        p=FUN_005645a0(q,1);REQUIRE(p&&p[0]==0x5a);
        REQUIRE(FUN_005645a0(p,0)==NULL);
        p=FUN_00563129((uint32_t)size,3);REQUIRE(p);
        expected=(((size*3)?size*3:1)+15)&~(size_t)15;
        for(k=0;k<expected;++k)REQUIRE(p[k]==0);
        FUN_00561bdc(p);
    }
    REQUIRE(FUN_00561ba6(UINT32_MAX)==NULL);
    REQUIRE(FUN_00563129(UINT32_MAX,2)==NULL);
    REQUIRE(FUN_005647c2(NULL)==SIZE_MAX&&errno==EINVAL);
    {
        unsigned char *p=FUN_00561ba6(16);REQUIRE(p);memset(p,0x42,16);
        REQUIRE(FUN_005645a0(p,UINT32_MAX)==NULL);
        REQUIRE(p[0]==0x42&&p[15]==0x42);FUN_00561bdc(p);
    }
    FUN_00561bdc(NULL);
    puts("Independent CRT allocation: matched allocator/free/realloc/size, rounded blocks, zero fill, overflow and failure ownership verified.");
    printf("Allocator checks: %u\n",checks);return 0;
}
