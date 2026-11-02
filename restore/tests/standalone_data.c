#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
static int verify_segments(void)
{
    size_t i,j;
    for(i=0;i<restore_data_segment_count;++i) {
        const restore_data_segment *segment=&restore_data_segments[i];
        const unsigned char *memory=(const unsigned char *)segment->address;
        MEMORY_BASIC_INFORMATION info;
        uint32_t hash=0x811c9dc5u;
        if(!VirtualQuery(memory,&info,sizeof(info))) return 1;
        if(info.Protect!=(segment->writable?PAGE_READWRITE:PAGE_READONLY)) return 2;
        if(memcmp(memory,segment->initial,segment->initial_size)) return 3;
        for(j=0;j<segment->initial_size;++j) hash=(hash^memory[j])*0x1000193u;
        if(hash!=segment->initial_fnv1a) return 4;
        for(j=segment->initial_size;j<segment->size;++j) if(memory[j]) return 5;
    }
    return 0;
}
int main(void)
{
    int result;
    MEMORY_BASIC_INFORMATION info;
    if(!restore_data_open()) return 12;
    if(restore_data_open()) return 13;
    if((result=verify_segments())!=0) return 20+result;
    if(strcmp((const char *)0x00579044u,"Pharaoh_Text.eng")) return 30;
    if(!VirtualQuery((void *)restore_data_arena_address,&info,sizeof(info))||info.Protect!=PAGE_NOACCESS||
        info.AllocationBase!=(void *)GetModuleHandleA(0)) return 31;
    *(unsigned char *)0x00ea1870u=0xa5;
    restore_data_close();
    if(!restore_data_open()) return 32;
    if((result=verify_segments())!=0) return 40+result;
    restore_data_close();
    puts("Data sections: exact initial bytes, zero-fill, image ownership, protections and reset verified; no original code mapped.");
    return 0;
}
