#ifndef PHARAOH_COMMERCE_EMPIRE_IMAGES_H
#define PHARAOH_COMMERCE_EMPIRE_IMAGES_H

#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/assets/sprite_libraries.h"

typedef struct PharaohEmpireImagesContext {
    PharaohEmpireObjectRecord *objects; /* 200 records */
    const int32_t *scenario_rank;
    const PharaohEmpireCityImageCatalog *catalog;
    const int32_t *default_image_base; /* native group459 */
    const int32_t *default_image_limit; /* native group460 */
    const PharaohSpriteImageRecord *images;
    const int32_t *image_count;
} PharaohEmpireImagesContext;

void pharaoh_empire_refresh_object_images(const PharaohEmpireImagesContext *c);
void pharaoh_empire_refresh_city_image_dimensions(const PharaohEmpireImagesContext *c);

#endif
