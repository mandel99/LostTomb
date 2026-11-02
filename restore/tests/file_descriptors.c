#include "standalone_data.h"
#include "host_allocator.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
extern int FUN_00566840(void);
extern uint32_t FUN_005668d5(uint32_t,HANDLE);
extern uint32_t FUN_005669c6(uint32_t);
extern uint32_t FUN_005647eb(uint32_t);
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Descriptor failure at line %d\n",__LINE__);abort();}}while(0)
int main(void)
{
    unsigned int i;HANDLE handles[300];
    REQUIRE(restore_data_open());
    for(i=0;i<64;++i)U(0x1266960u+i*4)=0;
    U(0x1266a60)=0;U(0x60496c)=0; /* Leave this test's standard handles intact. */
    for(i=0;i<300;++i) {
        uintptr_t record;
        REQUIRE(FUN_00566840()==(int)i);
        handles[i]=CreateFileA("NUL",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
        REQUIRE(handles[i]!=INVALID_HANDLE_VALUE);
        REQUIRE(FUN_005668d5(i,handles[i])==0);
        record=U(0x1266960u+(i>>5)*4)+(i&31u)*8;
        *(unsigned char *)(record+4)=1; /* The _open caller owns the open flag. */
        REQUIRE((HANDLE)(uintptr_t)FUN_005669c6(i)==handles[i]);
    }
    REQUIRE(U(0x1266a60)==320); /* Count crosses the old eight-bit truncation. */
    REQUIRE(FUN_005669c6(320)==UINT32_MAX);
    for(i=0;i<300;++i) {DWORD flags;REQUIRE(FUN_005647eb(i)==0);REQUIRE(!GetHandleInformation(handles[i],&flags));}
    REQUIRE(FUN_005647eb(299)==UINT32_MAX);
    REQUIRE(FUN_00566840()==0);
    for(i=0;i<64;++i) {void *table=(void *)(uintptr_t)U(0x1266960u+i*4);if(table)restore_heap_free(table);U(0x1266960u+i*4)=0;}
    U(0x1266a60)=0;restore_data_close();
    puts("Legacy descriptor slots: 300 real NUL handles, full table pointers, >255 descriptor count, lookup, close and reuse verified.");
    return 0;
}
