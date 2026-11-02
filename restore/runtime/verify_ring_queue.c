/* Differential checks against complete original bodies copied before patching. */
#include "ring_queue.h"
#include <windows.h>
#include <string.h>

static BYTE *original[6];
static const DWORD addresses[]={0x4f6520,0x4f6560,0x4f6570,0x4f65b0,0x4f65f0,0x4f6630};
static const SIZE_T lengths[]={0x33,9,0x35,0x31,0x2a,0x13};

static int open_fixtures(RestoreQueueFunctions *functions)
{
    unsigned int i;
    DWORD old;
    if (!original[0]) for (i=0;i<6;i++) {
        original[i]=VirtualAlloc(NULL,lengths[i],MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
        if (!original[i]) return 0;
        memcpy(original[i],(void *)(uintptr_t)addresses[i],lengths[i]);
        if (!VirtualProtect(original[i],lengths[i],PAGE_EXECUTE_READ,&old)) return 0;
        FlushInstructionCache(GetCurrentProcess(),original[i],lengths[i]);
    }
    functions->init=(RestoreQueueOne)original[0];
    functions->clear=(RestoreQueueOne)original[1];
    functions->find=(RestoreQueueFind)original[2];
    functions->push=(RestoreQueuePush)original[3];
    functions->next=(RestoreQueueNext)original[4];
    functions->count=(RestoreQueueOne)original[5];
    return 1;
}

static uint32_t word(const void *p,unsigned int index)
{
    uint32_t value;
    memcpy(&value,(const BYTE *)p+4*index,4);
    return value;
}
static void put(void *p,unsigned int index,uint32_t value) { memcpy((BYTE *)p+4*index,&value,4); }
static uint32_t random_word(uint32_t *state) { *state=*state*1664525u+1013904223u; return *state; }
static void fill(BYTE *p,size_t size,uint32_t *state)
{
    size_t i;
    for (i=0;i<size;i++) p[i]=(BYTE)(random_word(state)>>24);
}

static int check_sequences(const RestoreQueueFunctions *native,const RestoreQueueFunctions *c,unsigned int *checks)
{
    BYTE left[284],right[284];
    uint32_t state=0x65b0,scenario,step;
    for (scenario=0;scenario<64;scenario++) {
        BYTE *a=left+32,*b=right+32;
        uint32_t cursor_a=0,cursor_b=0,capacity=1+scenario%50;
        fill(left,sizeof(left),&state); memcpy(right,left,sizeof(left));
        if (native->init(a,NULL)!=c->init(b,NULL) || memcmp(left,right,sizeof(left))) return 0;
        (*checks)++;
        put(a,3,capacity); put(b,3,capacity);
        for (step=0;step<256;step++) {
            uint32_t value=random_word(&state),expected,actual;
            unsigned int action=random_word(&state)>>24;
            switch (action%6) {
            case 0:
                expected=native->push(a,NULL,&value); actual=c->push(b,NULL,&value); break;
            case 1:
                expected=native->find(a,NULL,value); actual=c->find(b,NULL,value); break;
            case 2:
                cursor_a=word(a,1); cursor_b=word(b,1);
                expected=native->next(a,NULL,&cursor_a); actual=c->next(b,NULL,&cursor_b); break;
            case 3:
                expected=native->count(a,NULL); actual=c->count(b,NULL); break;
            case 4:
                expected=native->clear(a,NULL); actual=c->clear(b,NULL); break;
            default:
                /* Model a caller consuming one item, then compare the count. */
                if (word(a,1)!=word(a,2)) {
                    uint32_t next=(word(a,1)+1)%capacity;
                    put(a,1,next); put(b,1,next);
                }
                expected=native->count(a,NULL); actual=c->count(b,NULL); break;
            }
            if (expected!=actual || cursor_a!=cursor_b || memcmp(left,right,sizeof(left))) return 0;
            (*checks)++;
        }
    }
    return 1;
}

static int check_pages(const RestoreQueueFunctions *native,const RestoreQueueFunctions *c,unsigned int *checks)
{
    SYSTEM_INFO system;
    BYTE *alloc_a=NULL,*alloc_b=NULL,*a,*b;
    unsigned int side,offset;
    DWORD old;
    int ok=0;
    GetSystemInfo(&system);
    alloc_a=VirtualAlloc(NULL,system.dwPageSize*3,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS);
    alloc_b=VirtualAlloc(NULL,system.dwPageSize*3,MEM_RESERVE|MEM_COMMIT,PAGE_NOACCESS);
    if (!alloc_a || !alloc_b ||
        !VirtualProtect(alloc_a+system.dwPageSize,system.dwPageSize,PAGE_READWRITE,&old) ||
        !VirtualProtect(alloc_b+system.dwPageSize,system.dwPageSize,PAGE_READWRITE,&old)) goto done;
    for (side=0;side<2;side++) for (offset=0;offset<4;offset++) {
        BYTE *page_a=alloc_a+system.dwPageSize,*page_b=alloc_b+system.dwPageSize;
        uint32_t value=0x80000001,cursor_a=49,cursor_b=49;
        a=side ? page_a+system.dwPageSize-220-offset : page_a+offset;
        b=side ? page_b+system.dwPageSize-220-offset : page_b+offset;
        memset(page_a,0xa9,system.dwPageSize); memcpy(page_b,page_a,system.dwPageSize);
#define PAGE_CHECK(expression) do { if (!(expression) || memcmp(page_a,page_b,system.dwPageSize)) goto done; (*checks)++; } while(0)
        PAGE_CHECK(native->init(a,NULL)==c->init(b,NULL));
        put(a,1,49); put(b,1,49); put(a,2,49); put(b,2,49);
        PAGE_CHECK(native->push(a,NULL,&value)==c->push(b,NULL,&value));
        PAGE_CHECK(native->find(a,NULL,value)==c->find(b,NULL,value));
        PAGE_CHECK(native->next(a,NULL,&cursor_a)==c->next(b,NULL,&cursor_b) && cursor_a==cursor_b);
        PAGE_CHECK(native->count(a,NULL)==c->count(b,NULL));
        PAGE_CHECK(native->clear(a,NULL)==c->clear(b,NULL));
        /* Full queue must refuse before accessing an unreadable source page. */
        put(a,1,0); put(b,1,0); put(a,2,49); put(b,2,49);
        PAGE_CHECK(native->push(a,NULL,alloc_a)==c->push(b,NULL,alloc_b));
#undef PAGE_CHECK
    }
    ok=1;
done:
    if (alloc_a) VirtualFree(alloc_a,0,MEM_RELEASE);
    if (alloc_b) VirtualFree(alloc_b,0,MEM_RELEASE);
    return ok;
}

int restore_verify_queue(FILE *log,const RestoreQueueFunctions *c,const char *phase)
{
    RestoreQueueFunctions native;
    BYTE left[320],right[320];
    uint32_t state=0x4f6520;
    unsigned int iteration,checks=0;
    if (!open_fixtures(&native)) { fputs("Could not create native queue fixtures.\n",log); return 0; }
    for (iteration=0;iteration<25000;iteration++) {
        unsigned int offset=32+iteration%4,capacity=1+iteration%50;
        BYTE *a=left+offset,*b=right+offset;
        uint32_t head=random_word(&state)%capacity,tail=random_word(&state)%capacity;
        uint32_t expected,actual,cursor_a,cursor_b,value=random_word(&state);
        unsigned int alias;
        fill(left,sizeof(left),&state); memcpy(right,left,sizeof(left));
        expected=native.init(a,NULL); actual=c->init(b,NULL);
        if (expected!=actual || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        fill(left,sizeof(left),&state);
        put(a,1,head); put(a,2,tail); put(a,3,capacity);
        memcpy(right,left,sizeof(left));
        expected=native.clear(a,NULL); actual=c->clear(b,NULL);
        if (expected!=actual || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        put(a,1,head); put(a,2,tail);
        if (iteration%2) value=word(a,5+head);
        memcpy(right,left,sizeof(left));
        expected=native.find(a,NULL,value); actual=c->find(b,NULL,value);
        if (expected!=actual || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        alias=iteration%55;
        expected=native.push(a,NULL,iteration%3 ? (const void *)(a+alias*4) : &value);
        actual=c->push(b,NULL,iteration%3 ? (const void *)(b+alias*4) : &value);
        if (expected!=actual || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        /* Test independent and aliased cursor storage; capacity alias is in range. */
        put(a,1,head); put(a,2,tail); put(a,3,capacity);
        cursor_a=iteration%2 ? head : tail; cursor_b=cursor_a;
        alias=iteration%5==0 ? 1 : iteration%5==1 ? 2 : iteration%5==2 ? 3 : 5+head;
        if (alias>=5) put(a,alias,cursor_a);
        memcpy(right,left,sizeof(left));
        expected=native.next(a,NULL,iteration%5==4 ? (void *)&cursor_a : a+alias*4);
        actual=c->next(b,NULL,iteration%5==4 ? (void *)&cursor_b : b+alias*4);
        if (expected!=actual || cursor_a!=cursor_b || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;

        /* Count has no indexing; include arbitrary signed/overflow bit patterns. */
        if (iteration%2) {
            put(a,1,random_word(&state)); put(a,2,random_word(&state)); put(a,3,random_word(&state));
        }
        memcpy(right,left,sizeof(left));
        expected=native.count(a,NULL); actual=c->count(b,NULL);
        if (expected!=actual || memcmp(left,right,sizeof(left))) goto mismatch;
        checks++;
    }
    if (!check_sequences(&native,c,&checks)) { fprintf(log,"Queue sequence mismatch (%s).\n",phase); return 0; }
    if (!check_pages(&native,c,&checks)) { fprintf(log,"Queue protected-page mismatch (%s).\n",phase); return 0; }
    fprintf(log,"PASS: %u ring queue comparisons including sequences, aliases and protected pages (%s).\n",checks,phase);
    return 1;
mismatch:
    fprintf(log,"Queue mismatch in %s case %u after %u checks.\n",phase,iteration,checks);
    return 0;
}

void restore_queue_fixtures_close(void)
{
    unsigned int i;
    for (i=0;i<6;i++) { if (original[i]) VirtualFree(original[i],0,MEM_RELEASE); original[i]=NULL; }
}
