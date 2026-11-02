#include "family_menu.h"
#include "file_list.h"
#include "host_allocator.h"
#include <stdio.h>

void FUN_00516dd0(void)
{
    if(*(uint8_t *)0xd3b439u==1) {
        RestoreFileList saves={0x56f548u,0,0,0,0,0,0};
        char pattern[260],path[260];
        int i,n;
        restore_file_unlink(FUN_00516a20());
        n=snprintf(pattern,sizeof(pattern),"%s\\*.sav",FUN_005169c0());
        if(n>=0&&n<(int)sizeof(pattern))FUN_0041c740(&saves,pattern,0);
        for(i=0;i<saves.count;++i) {
            n=snprintf(path,sizeof(path),"%s\\%s",FUN_005169c0(),saves.strings+saves.offsets[i]);
            /* Avoid the original overflowing stack buffer for malformed names. */
            if(n>=0&&n<(int)sizeof(path))restore_file_unlink(path);
        }
        restore_file_remove_directory(FUN_005169c0());
        if(saves.strings)restore_heap_free(saves.strings);
        if(saves.offsets)restore_heap_free(saves.offsets);
    }
    FUN_00516bb0();
    *(uint8_t *)0xe38dcau=1;
}
