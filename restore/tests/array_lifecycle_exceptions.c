#include "array_lifecycle.h"
#include "sound_file_lookup.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Array lifetime failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
#define USER_EXCEPTION 0xe0123456u
static uint32_t objects[8],events[64],used,ctor_throw,dtor_throw,throw_code,guard_allocation,freed;
static void record(uint32_t event){CHECK(used<64);events[used++]=event;}
static uint32_t index_of(void *object){return (uint32_t)(((uintptr_t)object-(uintptr_t)objects)/sizeof(objects[0]));}
static void construct(void *object)
{
    uint32_t i=index_of(object);record(100u+i);
    if(i==ctor_throw)RaiseException(throw_code,0,0,NULL);
    *(uint32_t *)object=i+1u;
}
static void destroy(void *object)
{
    uint32_t i=index_of(object);record(200u+i);
    if(i==dtor_throw)RaiseException(throw_code,0,0,NULL);
    *(uint32_t *)object=0;
}
void FUN_005637ee(void){ExitProcess(77);}
void *FUN_00561a1d(uint32_t size)
{record(300);CHECK(size==28);return (void *)(uintptr_t)guard_allocation;}
void FUN_00561a2b(void *pointer)
{record(301);freed=(uint32_t)(uintptr_t)pointer;}
void FUN_00561bdc(void *pointer){(void)pointer;CHECK(0);}
static void reset(void)
{memset(objects,0,sizeof(objects));used=0;ctor_throw=dtor_throw=UINT32_MAX;throw_code=USER_EXCEPTION;freed=0;}
static void expected(const uint32_t *values,uint32_t n)
{CHECK(used==n&&!memcmp(events,values,n*sizeof(values[0])));}
int main(int argc,char **argv)
{
    uint32_t caught=0;RestoreSoundFileTable table={23,7,0};void *page;
    static const uint32_t forward[]={100,101,102};
    static const uint32_t ctor_cleanup[]={100,101,102,201,200};
    static const uint32_t reverse[]={203,202,201,200};
    static const uint32_t failed_allocation[]={300,301};
    reset();
    if(argc==2&&!strcmp(argv[1],"--cpp-unwind")){
        dtor_throw=1;throw_code=0xe06d7363u;
        restore_array_unwind(objects+3,4,3,destroy);
        CHECK(0);
    }
    restore_array_construct(objects,4,3,construct,destroy);expected(forward,3);CHECK(objects[0]==1&&objects[2]==3);
    reset();ctor_throw=2;
    __try{restore_array_construct(objects,4,4,construct,destroy);CHECK(0);}
    __except(EXCEPTION_EXECUTE_HANDLER){caught=GetExceptionCode();}
    CHECK(caught==USER_EXCEPTION);expected(ctor_cleanup,5);CHECK(objects[0]==0&&objects[1]==0);
    reset();dtor_throw=2;caught=0;
    __try{restore_array_destroy(objects,4,4,destroy);CHECK(0);}
    __except(EXCEPTION_EXECUTE_HANDLER){caught=GetExceptionCode();}
    CHECK(caught==USER_EXCEPTION);expected(reverse,4);
    reset();dtor_throw=1;caught=0;
    __try{restore_array_unwind(objects+3,4,3,destroy);CHECK(0);}
    __except(EXCEPTION_EXECUTE_HANDLER){caught=GetExceptionCode();}
    CHECK(caught==USER_EXCEPTION&&used==2&&events[0]==202&&events[1]==201);
    reset();page=VirtualAlloc(NULL,4096,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS);CHECK(page!=NULL);guard_allocation=(uint32_t)(uintptr_t)page;caught=0;
    __try{(void)restore_sound_file_table_resize(&table,3);CHECK(0);}
    __except(EXCEPTION_EXECUTE_HANDLER){caught=GetExceptionCode();}
    CHECK(caught==EXCEPTION_ACCESS_VIOLATION&&freed==guard_allocation&&table.capacity==23&&table.count==7&&table.entries==0);expected(failed_allocation,2);
    CHECK(VirtualFree(page,0,MEM_RELEASE));
    puts("C lifetime: forward construction, partial rollback, reverse destruction, propagated exception and allocation cleanup passed.");
    return 0;
}
