#include "legacy_format.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Format ABI failure at line %d\n",__LINE__);abort();}}while(0)
int main(void)
{
    char storage[1024],*buffer=storage+16;int result;unsigned int i;
    memset(storage,0x5a,sizeof(storage));
    result=FUN_00561a36(buffer,"%s %d %u %08x %.2f %% %c","fixture",-123,4000000000u,0xfedcba98u,1.25,'Z');
    REQUIRE(!strcmp(buffer,"fixture -123 4000000000 fedcba98 1.25 % Z")&&result==(int)strlen(buffer));
    REQUIRE(FUN_00561a36(buffer,"%*.*s|%+06d|%I64u",8,3,"abcdef",-42,(unsigned __int64)0xfedcba9876543210ull)>0);
    REQUIRE(!strcmp(buffer,"     abc|-00042|18364758544493064720"));
    REQUIRE(FUN_00561a36(buffer,"%p",(void *)(uintptr_t)0x12345678u)==8&&!strcmp(buffer,"12345678"));
    REQUIRE(FUN_00561a36(buffer,"%s","")==0&&buffer[0]==0);
    for(i=0;i<10000;++i){result=FUN_00561a36(buffer,"x:%u/%u",i,i+1);REQUIRE(result==(int)strlen(buffer));}
    for(i=0;i<16;++i)REQUIRE((unsigned char)storage[i]==0x5a);
    REQUIRE((unsigned char)storage[1023]==0x5a);
    puts("Legacy sprintf C ABI: real variadic integers, pointers/strings, doubles, dynamic widths, 64-bit values, return count and repeated stack cleanup verified.");
    return 0;
}
