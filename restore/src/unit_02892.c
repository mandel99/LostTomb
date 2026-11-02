#include "legacy_streams.h"
#include "host_allocator.h"
restore_legacy_FILE *FUN_00564a39(void)
{
    restore_legacy_FILE **table;
    int i,count;
    if(!restore_legacy_streams_open())return NULL;
    table=*(restore_legacy_FILE ***)(uintptr_t)0x1266a90u;
    count=*(int32_t *)(uintptr_t)0x1267aa0u;
    for(i=0;i<count;++i) {
        restore_legacy_FILE *stream=table[i];
        if(!stream) {stream=restore_heap_allocate(32);table[i]=stream;if(!stream)return NULL;}
        else if(stream->_flag&0x83)continue;
        stream->_file=-1;stream->_cnt=0;stream->_flag=0;
        stream->_base=NULL;stream->_ptr=NULL;stream->_tmpfname=NULL;
        return stream;
    }
    return NULL;
}
