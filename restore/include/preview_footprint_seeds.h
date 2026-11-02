#ifndef RESTORE_PREVIEW_FOOTPRINT_SEEDS_H
#define RESTORE_PREVIEW_FOOTPRINT_SEEDS_H
#include <stdint.h>
/* Initialize only x,y,size,type. Run the eight startup geometry methods after
   this call to derive each record's remaining nine DWORDs. */
void restore_preview_footprint_seeds_initialize(void);
/* Share the existing seed tuples with complete constructor bodies. */
uint32_t restore_preview_footprint_seed(uint32_t class_index,uint32_t piece,int32_t output[4]);
#endif
