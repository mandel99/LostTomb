#ifndef RESTORE_SPRITE_LIBRARIES_H
#define RESTORE_SPRITE_LIBRARIES_H
#include <stdint.h>
#include <stddef.h>
typedef struct {uint8_t bytes[64];} RestoreSpriteImage;
typedef struct {uint32_t library,image;} RestoreSpriteTag;
typedef struct {
    char name[260],files[200][260];
    uint8_t *pixels;
    uint32_t pixel_bytes,image_count,tag_count,first_image,first_tag;
} RestoreSpriteLibrary;
typedef struct {
    RestoreSpriteTag tags[1000];
    RestoreSpriteLibrary libraries[30];
    RestoreSpriteImage images[25500];
    uint32_t tag_limits[30];
    void *scratch,*alternate;
    uint32_t next_tag,next_image,pixel_bytes,loaded;
} RestoreSpriteLibraries;
typedef char restore_sprite_library_size[(sizeof(RestoreSpriteLibrary)==0xcc3c)?1:-1];
typedef char restore_sprite_library_pixels[(offsetof(RestoreSpriteLibrary,pixels)==0xcc24)?1:-1];

typedef char restore_sprite_library_image_count[(offsetof(RestoreSpriteLibrary,image_count)==0xcc2c)?1:-1];
typedef char restore_sprite_image_offset[(offsetof(RestoreSpriteLibraries,images)==0x180e48)?1:-1];
typedef char restore_sprite_limits_offset[(offsetof(RestoreSpriteLibraries,tag_limits)==0x30f548)?1:-1];
typedef char restore_sprite_manager_size[(sizeof(RestoreSpriteLibraries)==0x30f5d8)?1:-1];

typedef char restore_sprite_manager_loaded[(offsetof(RestoreSpriteLibraries,loaded)==0x30f5d4)?1:-1];
#define RESTORE_SPRITE_LIBRARIES ((RestoreSpriteLibraries *)(uintptr_t)0xf55bb0u)
void restore_sprite_libraries_initialize(RestoreSpriteLibraries *self);
uint8_t FUN_0053dd10(RestoreSpriteLibraries *self,const char *const names[30]);
void FUN_0053dd40(RestoreSpriteLibraries *self,const char *const names[30]);
uint8_t FUN_0053dd80(RestoreSpriteLibraries *self,const char *name);
uint8_t FUN_0053dd90(RestoreSpriteLibraries *self,const char *name,int index);
uint8_t FUN_0053dff0(RestoreSpriteLibrary *self,const char *name,int index,RestoreSpriteImage *images,uint32_t first_image,RestoreSpriteTag *tags,uint32_t first_tag);
uint32_t FUN_0053f660(const char *name,uint8_t **pixels,uint8_t convert,uint32_t bytes,uint8_t compressed,int destination_offset,uint32_t file_offset);
#endif
