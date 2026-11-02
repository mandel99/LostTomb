#include <stdint.h>
#include <stdio.h>

extern uint32_t FUN_004ea9a0(int);
extern int FUN_004ea9c0(int);
extern int FUN_004eada0(int);

struct row { int32_t input; uint32_t expected; };
static int check(const char *name,uint32_t (*function)(int),const struct row *rows,unsigned int count)
{
    unsigned int i;
    for(i=0;i<count;++i) {
        uint32_t actual=function(rows[i].input);
        if(actual!=rows[i].expected) {
            fprintf(stderr,"FAIL %s(%08x): %08x != %08x\n",name,(uint32_t)rows[i].input,actual,rows[i].expected);
            return 0;
        }
    }
    return 1;
}
int main(void)
{
    static const struct row a[]={
        {-1,0xffffff00u},{0,0},{0x43,0},{0x44,1},{0x45,1},{0x46,1},{0x47,0},
        {0x12345644,0x12345600u},{INT32_MIN,0x80000000u},{INT32_MAX,0x7fffff00u}
    };
    static const struct row b[]={
        {-1,0xffffff00u},{0,0xffffff00u},{81,0xffffff00u},{82,1},{83,1},{84,1},{85,0},
        {101,0},{102,1},{103,1},{104,1},{105,0},{0x152,0x100u},
        {INT32_MIN,0x7fffff00u},{INT32_MAX,0x7fffff00u}
    };
    static const struct row c[]={
        {-1,0xffffff00u},{0,0xffffff00u},{24,0xffffff00u},{25,1},{26,0},
        {76,0},{77,1},{78,1},{79,0},{92,1},{93,1},{94,0},{100,1},{101,1},{102,0},
        {INT32_MIN,0x7fffff00u},{INT32_MAX,0x7fffff00u}
    };
    if(!check("004ea9a0",FUN_004ea9a0,a,sizeof(a)/sizeof(a[0])) ||
       !check("004ea9c0",(uint32_t (*)(int))FUN_004ea9c0,b,sizeof(b)/sizeof(b[0])) ||
       !check("004eada0",(uint32_t (*)(int))FUN_004eada0,c,sizeof(c)/sizeof(c[0]))) return 1;
    puts("PASS: C classification return values, full EAX bytes and signed boundaries; no original EXE.");
    return 0;
}
