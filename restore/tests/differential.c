/* Compare compiled C with five isolated original x86 routines. */
#include <windows.h>
#include <stdio.h>
#include <limits.h>
#include "original_fixtures.h"

extern unsigned int FUN_0041ba00(int);
extern unsigned int FUN_0041bad0(unsigned int);
extern unsigned int FUN_00451bc0(int);
extern unsigned int FUN_0045b490(int);
extern char FUN_0045bb30(int,int);

static void *load_original(const unsigned char *bytes, unsigned int size)
{
    DWORD previous;
    void *memory = VirtualAlloc(NULL,size,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    if (!memory) return NULL;
    CopyMemory(memory,bytes,size);
    if (!VirtualProtect(memory,size,PAGE_EXECUTE_READ,&previous)) {
        VirtualFree(memory,0,MEM_RELEASE);
        return NULL;
    }
    FlushInstructionCache(GetCurrentProcess(),memory,size);
    return memory;
}

int main(void)
{
    unsigned int (__stdcall *original_a)(int) = load_original(original_0041ba00,sizeof(original_0041ba00));
    unsigned int (__stdcall *original_b)(unsigned int) = load_original(original_0041bad0,sizeof(original_0041bad0));
    unsigned int (__cdecl *original_c)(int) = load_original(original_00451bc0,sizeof(original_00451bc0));
    unsigned int (__cdecl *original_d)(int) = load_original(original_0045b490,sizeof(original_0045b490));
    unsigned char (__cdecl *original_e)(int,int) = load_original(original_0045bb30,sizeof(original_0045bb30));
    const int boundaries[] = {INT_MIN,INT_MIN+1,-400,-1,0,1,8,9,24,25,26,199,200,201,299,300,301,399,400,401,INT_MAX};
    unsigned int random_state = 0x41ba00, i, checks = 0;
    if (!original_a || !original_b || !original_c || !original_d || !original_e) {
        fprintf(stderr,"Could not allocate isolated executable fixtures: %lu\n",GetLastError());
        return 2;
    }
    for (i=0;i<100021;i++) {
        int x, y;
        unsigned int expected, actual;
        random_state = random_state*1664525u+1013904223u;
        x = i<21 ? boundaries[i] : (int)random_state;
        y = (i%3)==0 ? 0 : (int)(random_state^0x71ba01u);
#define CHECK(address,original,restored,arg) \
        expected=original(arg); actual=restored(arg); checks++; \
        if (expected!=actual) { \
            fprintf(stderr,"Mismatch at 0x%08x input %d: original %u, C %u\n",address,x,expected,actual); \
            return 1; \
        }
        CHECK(0x41ba00,original_a,FUN_0041ba00,x)
        CHECK(0x41bad0,original_b,FUN_0041bad0,(unsigned int)x)
        CHECK(0x451bc0,original_c,FUN_00451bc0,x)
        CHECK(0x45b490,original_d,FUN_0045b490,x)
        expected = original_e(x,y);
        actual = (unsigned char)FUN_0045bb30(x,y);
        checks++;
        if (expected!=actual) {
            fprintf(stderr,"Mismatch at 0x0045bb30 input %d,%d\n",x,y);
            return 1;
        }
    }
    VirtualFree(original_a,0,MEM_RELEASE);
    VirtualFree(original_b,0,MEM_RELEASE);
    VirtualFree(original_c,0,MEM_RELEASE);
    VirtualFree(original_d,0,MEM_RELEASE);
    VirtualFree(original_e,0,MEM_RELEASE);
    printf("PASS: %u differential comparisons across five original x86 routines.\n",checks);
    return 0;
}
