#include "legacy_streams.h"
#include "host_allocator.h"
#include <errno.h>
#define TABLE (*(restore_legacy_FILE ***)(uintptr_t)0x1266a90u)
#define COUNT (*(int32_t *)(uintptr_t)0x1267aa0u)
static restore_legacy_FILE **owned_table;
extern uint32_t FUN_00561994(restore_legacy_FILE *);
int restore_legacy_streams_open(void)
{
    int i,count;
    if(TABLE) return COUNT>0;
    count=COUNT;
    if(!count) count=512;
    if(count<20)count=20;
    owned_table=restore_heap_calloc((size_t)count,sizeof(*owned_table));
    if(!owned_table) {count=20;owned_table=restore_heap_calloc(20,sizeof(*owned_table));}
    if(!owned_table) {errno=ENOMEM;return 0;}
    for(i=0;i<20;++i)owned_table[i]=(restore_legacy_FILE *)(uintptr_t)(0x6049a0u+(uint32_t)i*32u);
    TABLE=owned_table;COUNT=count;return 1;
}
void restore_legacy_streams_close(void)
{
    int i;
    if(!owned_table)return;
    for(i=3;i<COUNT;++i)if(owned_table[i]) {
        if(owned_table[i]->_flag&0x83)FUN_00561994(owned_table[i]);
        if(i>=20)restore_heap_free(owned_table[i]);
    }
    restore_heap_free(owned_table);owned_table=NULL;TABLE=NULL;COUNT=0;
}
