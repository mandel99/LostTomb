/* Independent original x86 reference, full-buffer comparison and protected pages. */
#include "memory_strings.h"
#include <windows.h>
#include <string.h>

static BYTE *original[4];
static const DWORD addresses[4]={0x562240,0x5631b0,0x563240,0x563380};
static const SIZE_T lengths[4]={0xfe,0x84,0x58,0x7b};

static int open_fixtures(void)
{
    unsigned int i;
    DWORD previous;
    if (original[0]) return 1;
    for (i=0;i<4;i++) {
        original[i]=VirtualAlloc(NULL,lengths[i],MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
        if (!original[i]) return 0;
        memcpy(original[i],(void *)(uintptr_t)addresses[i],lengths[i]);
        if (!VirtualProtect(original[i],lengths[i],PAGE_EXECUTE_READ,&previous)) return 0;
        FlushInstructionCache(GetCurrentProcess(),original[i],lengths[i]);
    }
    return 1;
}

static uint32_t random_word(uint32_t *state)
{
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void random_buffer(unsigned char *buffer,size_t count,uint32_t *state,int nonzero)
{
    size_t i;
    for (i=0;i<count;i++) {
        unsigned char byte=(unsigned char)(random_word(state)>>24);
        buffer[i]=nonzero && !byte ? 1 : byte;
    }
}

typedef struct GuardedPage {
    BYTE *allocation;
    BYTE *data;
    BYTE *end;
    DWORD size;
} GuardedPage;

static int open_page(GuardedPage *page)
{
    SYSTEM_INFO system;
    DWORD previous;
    GetSystemInfo(&system);
    page->size=system.dwPageSize;
    page->allocation=VirtualAlloc(NULL,page->size*3,MEM_COMMIT|MEM_RESERVE,PAGE_NOACCESS);
    if (!page->allocation) return 0;
    page->data=page->allocation+page->size;
    page->end=page->data+page->size;
    return VirtualProtect(page->data,page->size,PAGE_READWRITE,&previous)!=0;
}

static int check_pages(FILE *log,RestoreBoundedStringCopy copy,RestoreStringCompare compare,
    RestoreMemoryFill fill,RestoreStringLength length,unsigned int *checks,const char *phase)
{
    GuardedPage source={0},left={0},right={0};
    unsigned int count;
    int ok=0;
    if (!open_page(&source) || !open_page(&left) || !open_page(&right)) goto done;
    for (count=0;count<=128;count++) {
        void *a,*b;
        char *source_start=(char *)source.end-count;
        char *left_start=count ? (char *)left.end-count : NULL;
        char *right_start=count ? (char *)right.end-count : NULL;
        memset(left.data,0x5a,left.size);
        memcpy(right.data,left.data,left.size);
        a=((RestoreMemoryFill)original[2])(left_start,-85,count);
        b=fill(right_start,-85,count);
        if (a!=left_start || b!=right_start || memcmp(left.data,right.data,left.size)) goto mismatch;
        (*checks)++;

        memset(source.data,'Q',source.size);
        memset(left.data,0x5a,left.size);
        memcpy(right.data,left.data,left.size);
        a=((RestoreBoundedStringCopy)original[0])(left_start,count ? source_start : NULL,count);
        b=copy(right_start,count ? source_start : NULL,count);
        if (a!=left_start || b!=right_start || memcmp(left.data,right.data,left.size)) goto mismatch;
        (*checks)++;

        source.end[-1]=0;
        memset(left.data,0x5a,left.size);
        memcpy(right.data,left.data,left.size);
        a=((RestoreBoundedStringCopy)original[0])(left_start,(char *)source.end-1,count);
        b=copy(right_start,(char *)source.end-1,count);
        if (a!=left_start || b!=right_start || memcmp(left.data,right.data,left.size)) goto mismatch;
        (*checks)++;

        memset(source.data,0x80,source.size);
        source.end[-1]=0;
        source_start=(char *)source.end-count-1;
        if (((RestoreStringLength)original[3])(source_start)!=length(source_start)) goto mismatch;
        (*checks)++;
        memcpy(left.data,source.data,source.size);
        left_start=(char *)left.end-count-1;
        if (((RestoreStringCompare)original[1])(source_start,left_start)!=compare(source_start,left_start)) goto mismatch;
        (*checks)++;
        if (count) {
            left.end[-2]=0x7f;
            if (((RestoreStringCompare)original[1])(source_start,left_start)!=compare(source_start,left_start)) goto mismatch;
            (*checks)++;
        }
    }
    ok=1;
    goto done;
mismatch:
    fprintf(log,"Protected-page mismatch in %s case %u.\n",phase,count);
done:
    if (source.allocation) VirtualFree(source.allocation,0,MEM_RELEASE);
    if (left.allocation) VirtualFree(left.allocation,0,MEM_RELEASE);
    if (right.allocation) VirtualFree(right.allocation,0,MEM_RELEASE);
    return ok;
}

int restore_verify_memory_strings(FILE *log,RestoreBoundedStringCopy copy,RestoreStringCompare compare,
    RestoreMemoryFill fill,RestoreStringLength length,const char *phase)
{
    __declspec(align(16)) unsigned char left[1024],right[1024],first[1024],second[1024],first_copy[1024],second_copy[1024];
    uint32_t state=0x563240u;
    unsigned int iteration,checks=0;
    static const size_t boundaries[]={0,1,2,3,4,5,7,8,15,16,31,32,255,256,511,512};
    if (!open_fixtures()) { fputs("Memory/string fixtures could not be created.\n",log); return 0; }
    for (iteration=0;iteration<25000;iteration++) {
        unsigned int offset=32+(iteration%8),source=192+(iteration%4),destination;
        size_t count=iteration<16 ? boundaries[iteration] : random_word(&state)%601;
        int value=(int)random_word(&state);
        void *expected,*actual;
        random_buffer(left,sizeof(left),&state,0);
        memcpy(right,left,sizeof(left));
        expected=((RestoreMemoryFill)original[2])(count ? left+offset : NULL,value,count);
        actual=fill(count ? right+offset : NULL,value,count);
        if (expected!=(count ? left+offset : NULL) || actual!=(count ? right+offset : NULL) || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        random_buffer(left,sizeof(left),&state,1);
        left[source+(random_word(&state)%300)]=0;
        destination=iteration%3 ? (unsigned int)((int)source+(int)(iteration%65)-32) : 512;
        count=random_word(&state)%257;
        memcpy(right,left,sizeof(left));
        expected=((RestoreBoundedStringCopy)original[0])(count ? (char *)left+destination : NULL,count ? (char *)left+source : NULL,count);
        actual=copy(count ? (char *)right+destination : NULL,count ? (char *)right+source : NULL,count);
        if (expected!=(count ? left+destination : NULL) || actual!=(count ? right+destination : NULL) || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        random_buffer(first,sizeof(first),&state,1);
        count=random_word(&state)%601;
        first[offset+count]=0;
        memcpy(first_copy,first,sizeof(first));
        if (((RestoreStringLength)original[3])((char *)first+offset)!=length((char *)first_copy+offset) || memcmp(first,first_copy,sizeof(first))) goto mismatch;
        checks++;

        memcpy(second,first,sizeof(first));
        if (iteration%3==1) second[offset+(count ? random_word(&state)%count : 0)]^=0x80;
        else if (iteration%3==2) second[offset+count]=0x80;
        second[sizeof(second)-1]=0;
        memcpy(first_copy,first,sizeof(first));
        memcpy(second_copy,second,sizeof(second));
        if (((RestoreStringCompare)original[1])((char *)first+offset,(char *)second+offset)!=
            compare((char *)first_copy+offset,(char *)second_copy+offset) ||
            memcmp(first,first_copy,sizeof(first)) || memcmp(second,second_copy,sizeof(second))) goto mismatch;
        checks++;
    }
    fprintf(log,"Checking protected-page string/memory boundaries (%s).\n",phase);
    if (!check_pages(log,copy,compare,fill,length,&checks,phase)) return 0;
    fprintf(log,"PASS: %u memory/string comparisons including protected-page boundaries (%s).\n",checks,phase);
    return 1;
mismatch:
    fprintf(log,"Memory/string mismatch in %s case %u after %u successful checks.\n",phase,iteration,checks);
    return 0;
}

void restore_memory_string_fixtures_close(void)
{
    unsigned int i;
    for (i=0;i<4;i++) {
        if (original[i]) VirtualFree(original[i],0,MEM_RELEASE);
        original[i]=NULL;
    }
}
