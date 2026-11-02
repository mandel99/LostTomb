/* Compare every byte of guarded buffers with independent original x86 copies. */
#include "record_geometry.h"
#include <windows.h>
#include <string.h>

static BYTE *original_copy,*original_update,*original_batch;

static BYTE *fixture(DWORD address,SIZE_T length)
{
    BYTE *memory=VirtualAlloc(NULL,length,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
    if (memory) memcpy(memory,(void *)(uintptr_t)address,length);
    return memory;
}

static int fixtures_open(void)
{
    DWORD old;
    int32_t displacement,original_displacement;
    if (original_copy) return 1;
    original_copy=fixture(0x4e5140,0x4c);
    original_update=fixture(0x4e5190,0x74);
    original_batch=fixture(0x4e52b0,0x20);
    if (!original_copy || !original_update || !original_batch) return 0;
    /* The batch routine has one relative call; bind it to the untouched
       update fixture, not to the now-replaceable image entry. */
    if (original_batch[0x10]!=0xe8) return 0;
    memcpy(&original_displacement,original_batch+0x11,4);
    if (0x4e52c5+original_displacement!=0x4e5190) return 0;
    displacement=(int32_t)((uintptr_t)original_update-(uintptr_t)(original_batch+0x15));
    memcpy(original_batch+0x11,&displacement,4);
    if (!VirtualProtect(original_copy,0x4c,PAGE_EXECUTE_READ,&old) ||
        !VirtualProtect(original_update,0x74,PAGE_EXECUTE_READ,&old) ||
        !VirtualProtect(original_batch,0x20,PAGE_EXECUTE_READ,&old)) return 0;
    FlushInstructionCache(GetCurrentProcess(),original_copy,0x4c);
    FlushInstructionCache(GetCurrentProcess(),original_update,0x74);
    FlushInstructionCache(GetCurrentProcess(),original_batch,0x20);
    return 1;
}

static uint32_t next_word(uint32_t *state)
{
    *state=*state*1664525u+1013904223u;
    return *state;
}

static void fill(unsigned char *buffer,SIZE_T size,uint32_t *state)
{
    SIZE_T i;
    for (i=0;i<size;i++) buffer[i]=(unsigned char)(next_word(state)>>24);
}

int restore_verify_geometry(FILE *log,RestoreCopyRecord copy,RestoreUpdateRecord update,
    RestoreUpdateRecords batch,const char *phase)
{
    volatile uint32_t *scale=(uint32_t *)0x70e44c,*step=(uint32_t *)0x70e45c;
    uint32_t old_scale=*scale,old_step=*step,seed=0x4e5190u;
    unsigned char left[1800],right[1800];
    unsigned int iteration,checks=0;
    int ok=0;
    const uint32_t boundaries[]={0,1,0xffffffffu,0x7fffffffu,0x80000000u,228,40,20};
    if (!fixtures_open()) { fputs("Could not create independent geometry fixtures.\n",log); goto done; }
    for (iteration=0;iteration<25000;iteration++) {
        unsigned int offset=64+(iteration%4);
        unsigned int destination=offset,source;
        int overlap=(int)(iteration%105)-52;
        void *expected,*actual;
        /* Aligned and unaligned inputs, both overlap directions, self-copy,
           and disjoint buffers; guard bytes are part of every comparison. */
        source=iteration%3 ? (unsigned int)((int)destination+overlap) : 256;
        fill(left,sizeof(left),&seed);
        memcpy(right,left,sizeof(left));
        expected=((RestoreCopyRecord)original_copy)(left+destination,NULL,left+source);
        actual=copy(right+destination,NULL,right+source);
        if (expected!=left+destination || actual!=right+destination || memcmp(left,right,sizeof(left))) {
            fprintf(log,"Copy mismatch in %s case %u (delta %d).\n",phase,iteration,overlap); goto done;
        }
        checks++;

        fill(left,sizeof(left),&seed);
        *scale=iteration<8 ? boundaries[iteration] : next_word(&seed);
        *step=iteration<8 ? boundaries[7-iteration] : next_word(&seed);
        if (iteration<8) {
            memcpy(left+offset,boundaries+iteration,4);
            memcpy(left+offset+4,boundaries+7-iteration,4);
        }
        memcpy(right,left,sizeof(left));
        {
            uint32_t a=((RestoreUpdateRecord)original_update)(left+offset,NULL);
            uint32_t b=update(right+offset,NULL);
            if (a!=b || memcmp(left,right,sizeof(left))) {
                fprintf(log,"Update mismatch in %s case %u.\n",phase,iteration); goto done;
            }
        }
        checks++;

        fill(left,sizeof(left),&seed);
        memcpy(right,left,sizeof(left));
        {
            int count=(int)(iteration%35)-2;
            ((RestoreUpdateRecords)original_batch)(count>0 ? left+offset : NULL,count);
            batch(count>0 ? right+offset : NULL,count);
            if (memcmp(left,right,sizeof(left))) {
                fprintf(log,"Batch mismatch in %s case %u (count %d).\n",phase,iteration,count); goto done;
            }
        }
        checks++;
    }
    fprintf(log,"PASS: %u geometry memory/return comparisons (%s).\n",checks,phase);
    ok=1;
done:
    *scale=old_scale;
    *step=old_step;
    return ok;
}

void restore_geometry_fixtures_close(void)
{
    if (original_copy) VirtualFree(original_copy,0,MEM_RELEASE);
    if (original_update) VirtualFree(original_update,0,MEM_RELEASE);
    if (original_batch) VirtualFree(original_batch,0,MEM_RELEASE);
    original_copy=original_update=original_batch=NULL;
}
