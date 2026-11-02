/* Independent native references; never compare a replacement with itself. */
#include "core_helpers.h"
#include <windows.h>
#include <string.h>

static BYTE *original[4];
static const DWORD addresses[]={0x56233e,0x562348,0x5632a0,0x563400};
static const SIZE_T lengths[]={0xa,0x1e,0x80,10};

static int open_fixtures(void)
{
    unsigned int i;
    DWORD previous;
    if (original[0]) return 1;
    for (i=0;i<4;i++) {
        original[i]=VirtualAlloc(NULL,lengths[i],MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
        if (!original[i]) return 0;
        memcpy(original[i],(void *)(uintptr_t)addresses[i],i==3 ? 5 : lengths[i]);
        if (i==2) {
            DWORD displacement=(DWORD)(0x5685e6u-(uintptr_t)(original[i]+0x73));
            memcpy(original[i]+0x6f,&displacement,4);
        } else if (i==3) {
            DWORD displacement=(DWORD)(0x563405u-(uintptr_t)(original[i]+10));
            original[i][5]=0xe9;
            memcpy(original[i]+6,&displacement,4);
        }
        if (!VirtualProtect(original[i],lengths[i],PAGE_EXECUTE_READ,&previous)) return 0;
        FlushInstructionCache(GetCurrentProcess(),original[i],lengths[i]);
    }
    return 1;
}

static uint32_t next_word(uint32_t *state)
{
    *state=*state*1664525u+1013904223u;
    return *state;
}

static unsigned int direction_flag(void)
{
    unsigned int flags;
    __asm { pushfd }
    __asm { pop eax }
    __asm { mov flags,eax }
    return flags&0x400;
}

static ptrdiff_t match_offset(const char *result,const char *base)
{
    return result ? result-base : -1;
}

typedef struct GuardedPage { BYTE *allocation,*data,*end; DWORD size; } GuardedPage;
static int open_page(GuardedPage *page)
{
    SYSTEM_INFO system;
    DWORD previous;
    GetSystemInfo(&system);
    page->size=system.dwPageSize;
    page->allocation=VirtualAlloc(NULL,page->size*3,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS);
    if (!page->allocation) return 0;
    page->data=page->allocation+page->size;
    page->end=page->data+page->size;
    return VirtualProtect(page->data,page->size,PAGE_READWRITE,&previous)!=0;
}

static int check_pages(FILE *log,RestoreFind find,RestoreMove move,unsigned int *checks,const char *phase)
{
    GuardedPage left={0},right={0},pattern={0};
    unsigned int count,side,variant;
    int ok=0;
    if (!open_page(&left) || !open_page(&right) || !open_page(&pattern)) goto done;
    for (count=0;count<=256;count++) {
        for (side=0;side<2;side++) {
            BYTE *a=side ? left.end-count : left.data;
            BYTE *b=side ? right.end-count : right.data;
            memset(left.data,0xa9,left.size);
            memset(pattern.data,0x53,pattern.size);
            memcpy(right.data,left.data,left.size);
            if (((RestoreMove)original[3])(count ? a : NULL,count ? pattern.end-count : NULL,count)!=(count ? a : NULL) ||
                move(count ? b : NULL,count ? pattern.end-count : NULL,count)!=(count ? b : NULL) ||
                memcmp(left.data,right.data,left.size) || direction_flag()) goto mismatch;
            (*checks)++;
        }
        /* Both overlap directions, with the range touching an unreadable page. */
        for (side=0;side<2;side++) {
            BYTE *a=left.end-count-1,*b=right.end-count-1;
            memset(left.data,0xa9,left.size);
            if (count) a[count/2]=0x27;
            memcpy(right.data,left.data,left.size);
            if (((RestoreMove)original[3])(a+side,a+!side,count)!=a+side ||
                move(b+side,b+!side,count)!=b+side || memcmp(left.data,right.data,left.size) || direction_flag()) goto mismatch;
            (*checks)++;
        }
        for (variant=0;variant<4;variant++) {
            char *text=(char *)left.end-count-1,*needle=(char *)pattern.end-2;
            char *a,*b;
            memset(left.data,0x80,left.size);
            left.end[-1]=0;
            memset(pattern.data,0x80,pattern.size);
            pattern.end[-1]=0;
            if (variant==0) needle=(char *)pattern.end-1;
            else if (variant==2) needle=(char *)pattern.end-count-1;
            else if (variant==3) pattern.end[-2]=0x7f;
            memcpy(right.data,left.data,left.size);
            a=((RestoreFind)original[2])(text,needle);
            b=find((char *)right.end-count-1,needle);
            if (match_offset(a,text)!=match_offset(b,(char *)right.end-count-1) ||
                memcmp(left.data,right.data,left.size)) goto mismatch;
            (*checks)++;
        }
    }
    ok=1;
    goto done;
mismatch:
    fprintf(log,"Core protected-page mismatch in %s, length %u.\n",phase,count);
done:
    if (left.allocation) VirtualFree(left.allocation,0,MEM_RELEASE);
    if (right.allocation) VirtualFree(right.allocation,0,MEM_RELEASE);
    if (pattern.allocation) VirtualFree(pattern.allocation,0,MEM_RELEASE);
    return ok;
}

int restore_verify_core(FILE *log,RestoreSeed seed,RestoreRandom random,RestoreFind find,RestoreMove move,const char *phase)
{
    unsigned char left[2048],right[2048],needle[128],needle_copy[128];
    volatile uint32_t *shared=(volatile uint32_t *)0x604950;
    uint32_t saved=*shared,state=0x563400u;
    unsigned int iteration,checks=0,i,step;
    int ok=0;
    static const uint32_t seeds[]={0,1,0x7fffffff,0x80000000,0xffffffff,0x12345678};
    static const size_t boundaries[]={0,1,2,3,4,7,8,15,16,31,32,33,63,64,127,128,255,256,511,512,1024};
    if (!open_fixtures()) goto done;
    for (iteration=0;iteration<25000;iteration++) {
        unsigned int source=512+(iteration%16),destination;
        size_t count=iteration<sizeof(boundaries)/sizeof(boundaries[0]) ? boundaries[iteration] : next_word(&state)%1025;
        void *a,*b;
        uint32_t input=iteration<6 ? seeds[iteration] : next_word(&state),expected_state,actual_state;
        uint32_t expected,actual;
        for (i=0;i<sizeof(left);i++) left[i]=(unsigned char)(next_word(&state)>>24);
        destination=iteration%4==0 ? source : iteration%4==1 ? source-1-iteration%127 :
            iteration%4==2 ? source+1+iteration%127 : 32+iteration%16;
        memcpy(right,left,sizeof(left));
        a=((RestoreMove)original[3])(count ? left+destination : NULL,count ? left+source : NULL,count);
        b=move(count ? right+destination : NULL,count ? right+source : NULL,count);
        if (a!=(count ? left+destination : NULL) || b!=(count ? right+destination : NULL) ||
            memcmp(left,right,sizeof(left)) || direction_flag()) goto mismatch;
        checks++;

        for (i=0;i<sizeof(left);i++) left[i]=(unsigned char)(1+next_word(&state)%255);
        count=next_word(&state)%1025;
        left[32+count]=0;
        for (i=0;i<sizeof(needle);i++) needle[i]=(unsigned char)(1+next_word(&state)%255);
        needle[next_word(&state)%65]=0;
        if (iteration%5==0) needle[0]=0;
        else if (iteration%5==1) needle[1]=0;
        else if (iteration%5==2 && count>80) memcpy(left+32+next_word(&state)%(count-80),needle,65);
        memcpy(right,left,sizeof(left));
        memcpy(needle_copy,needle,sizeof(needle));
        {
            const char *pattern=iteration%5==3 ? (char *)left+32+count/2 : (char *)needle;
            const char *pattern_copy=iteration%5==3 ? (char *)right+32+count/2 : (char *)needle_copy;
            char *expected_match=((RestoreFind)original[2])((char *)left+32,pattern);
            char *actual_match=find((char *)right+32,pattern_copy);
            if (match_offset(expected_match,(char *)left+32)!=match_offset(actual_match,(char *)right+32) ||
                memcmp(left,right,sizeof(left)) || memcmp(needle,needle_copy,sizeof(needle))) goto mismatch;
        }
        checks++;

        *shared=~input;
        expected=((RestoreSeed)original[0])(input); expected_state=*shared;
        *shared=~input;
        actual=seed(input); actual_state=*shared;
        if (expected!=actual || expected_state!=actual_state) goto mismatch;
        checks++;
        for (step=0;step<8;step++) {
            *shared=expected_state;
            expected=((RestoreRandom)original[1])(); expected_state=*shared;
            *shared=actual_state;
            actual=random(); actual_state=*shared;
            if (expected!=actual || expected_state!=actual_state) goto mismatch;
            checks++;
        }
    }
    if (!check_pages(log,find,move,&checks,phase)) goto done;
    fprintf(log,"PASS: %u copy/search/random comparisons including RNG state/sequences and protected pages (%s).\n",checks,phase);
    ok=1;
    goto done;
mismatch:
    fprintf(log,"Core mismatch in %s case %u after %u checks.\n",phase,iteration,checks);
done:
    *shared=saved;
    return ok;
}

void restore_core_fixtures_close(void)
{
    unsigned int i;
    for (i=0;i<4;i++) {
        if (original[i]) VirtualFree(original[i],0,MEM_RELEASE);
        original[i]=NULL;
    }
}
