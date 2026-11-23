#include "pharaoh/assets/sprite_fetch_rows.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"
#include "pharaoh/runtime/file_extension_operations.h"
#include <string.h>

static int32_t open_primary_or_media(const PharaohFileReadPartContext *f,
    const char *path)
{
    const PharaohFileSizeOperations *const ops=f->files->operations;
    int32_t handle=ops->open_file(f->files->user_data,path,0x8000);
    if(handle!=-1)return handle;
    char *const media=f->media_prefixed_path;
    media[0]=f->media_root->media_drive_letter;
    media[3]='\0';
    memcpy(media+3,path,strlen(path)+1);
    handle=ops->open_file(f->files->user_data,media,0x8000);
    int32_t retry=1;
    while(handle==-1 && retry==1) {
        retry=pharaoh_file_load_retry_prompt(f->retry_text_selection);
        handle=ops->open_file(f->files->user_data,media,0x8000);
    }
    return handle;
}

static void read_rows(const PharaohFileReadPartContext *f,int32_t handle,
    uint8_t **destination,int32_t row_bytes,int32_t rows,int32_t offset,
    int32_t stride_delta,int32_t *total)
{
    if(f->operations->seek_file(f->user_data,handle,offset,0)!=-1) {
        while(rows>0) {
            const int32_t got=f->operations->read_file(f->user_data,handle,
                *destination,(uint32_t)row_bytes);
            *total=(int32_t)((uint32_t)*total+(uint32_t)got);
            (void)f->operations->seek_file(f->user_data,handle,stride_delta,1);
            *destination+=row_bytes;
            --rows;
        }
    }
    (void)f->files->operations->close_file(f->files->user_data,handle);
}

/* Complete typed reuse of Unit02530's file algorithm. Native53F7E9..53F81E
 * captures EBP=2*WORDwidth-row_bytes
 * before any file callback, and passes it to both relative seek loops. */
int32_t pharaoh_sprite_fetch_rows(const PharaohSpriteFetchRowsContext *c,
    const char *name,uint8_t *destination,int32_t row_bytes,
    int32_t rows,int32_t file_offset)
{
    const PharaohFileReadPartContext *const f=c->files;
    const int32_t stride_delta=(int32_t)((uint32_t)(int32_t)c->source_width*2u-
        (uint32_t)row_bytes);
    char path[260]={0};
    memcpy(path,"data\\",6);
    memcpy(path+5,name,strlen(name)+1);
    pharaoh_file_change_extension(f->media_root->path_state,path,"555");
    int32_t total=0;
    int32_t handle=open_primary_or_media(f,path);
    if(handle!=-1)read_rows(f,handle,&destination,row_bytes,rows,file_offset,
        stride_delta,&total);
    if(total<1) {
        (void)pharaoh_file_change_to_media_root(f->media_root,path);
        handle=f->files->operations->open_file(f->files->user_data,path,0x8000);
        /* Possible original bug: after an unsuccessful row read, the second
         * attempt retains the advanced destination instead of restoring it. */
        if(handle!=-1)read_rows(f,handle,&destination,row_bytes,rows,file_offset,
            stride_delta,&total);
        pharaoh_file_directory_reset(f->directory_reset);
        if(total==0)return 0;
    }
    pharaoh_sprite_convert_raw_555_to_565(*c->pixel_mode,*c->cache_pixels,total);
    return total;
}
