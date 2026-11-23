#ifndef PHARAOH_ASSETS_SPRITE_FETCH_ROWS_H
#define PHARAOH_ASSETS_SPRITE_FETCH_ROWS_H
#include "pharaoh/runtime/file_read_part.h"

typedef struct PharaohSpriteFetchRowsContext {
    const PharaohFileReadPartContext *files;
    int16_t source_width;
    const int32_t *pixel_mode;
    uint8_t *const *cache_pixels;
} PharaohSpriteFetchRowsContext;

/* Native0053F7D0. The filename is relative to data\\; the native555 extension
 * replacement and file/media fallback order are retained. Paths must fit260. */
int32_t pharaoh_sprite_fetch_rows(const PharaohSpriteFetchRowsContext *context,
    const char *name,uint8_t *destination,int32_t row_bytes,
    int32_t rows,int32_t file_offset);
#endif
