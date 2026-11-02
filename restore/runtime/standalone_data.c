#include "standalone_data.h"
#include <windows.h>
#include <string.h>
#include <stdio.h>
/* Reserve the legacy data window in the new program's own zero-initialized
   image. Reserving it after CRT startup is unreliable: Windows can place the
   process heap there first. No original code or PE loader is involved. */
#pragma bss_seg(push, ".restore")
static unsigned char owned_data_window[0x01400000];
#pragma bss_seg(pop)
static int data_open;
int restore_data_open(void)
{
    size_t i;
    DWORD previous;
    if (data_open) return 0;
    if ((uintptr_t)owned_data_window>restore_data_arena_address ||
        restore_data_arena_address+restore_data_arena_size>(uintptr_t)owned_data_window+sizeof(owned_data_window)) {
        fprintf(stderr,"The linker must place the standalone data owner below 0x%08x; actual %p.\n",
            (unsigned int)restore_data_arena_address,owned_data_window);
        return 0;
    }
    if(!VirtualProtect((void *)restore_data_arena_address,restore_data_arena_size,PAGE_NOACCESS,&previous)) return 0;
    data_open=1;
    for (i=0;i<restore_data_segment_count;++i) {
        const restore_data_segment *segment=&restore_data_segments[i];
        DWORD previous;
        if (segment->address<restore_data_arena_address ||
            segment->size>restore_data_arena_size ||
            segment->address-restore_data_arena_address>restore_data_arena_size-segment->size ||
            segment->initial_size>segment->size ||
            !VirtualProtect((void *)segment->address,segment->size,PAGE_READWRITE,&previous)) {
            restore_data_close();
            return 0;
        }
        memset((void *)segment->address,0,segment->size);
        memcpy((void *)segment->address,segment->initial,segment->initial_size);
        if (!segment->writable && !VirtualProtect((void *)segment->address,segment->size,PAGE_READONLY,&previous)) {
            restore_data_close();
            return 0;
        }
    }
    return 1;
}
void restore_data_close(void)
{
    DWORD previous;
    if (data_open)
        VirtualProtect((void *)restore_data_arena_address,restore_data_arena_size,PAGE_NOACCESS,&previous);
    data_open=0;
}
