#include "pharaoh/commerce/empire_images.h"

#include <stddef.h>

_Static_assert(offsetof(PharaohEmpireObjectRecord, image_width) == 8 &&
    offsetof(PharaohEmpireObjectRecord, image_height) == 10 &&
    offsetof(PharaohEmpireObjectRecord, image_id) == 12 &&
    offsetof(PharaohEmpireObjectRecord, image_offset) == 14 &&
    sizeof(PharaohEmpireObjectRecord) == 98, "native empire image fields");

/* Reused MissionLoadClosure00442660, preserving DWORD limit and WORD sum. */
void pharaoh_empire_refresh_object_images(const PharaohEmpireImagesContext *c)
{
    int32_t limit = *c->default_image_limit;
    for (int32_t i = 0; i < PHARAOH_EMPIRE_OBJECT_COUNT; ++i) {
        PharaohEmpireObjectRecord *const object = &c->objects[i];
        if (object->in_use == 0) continue;
        if (object->kind == 1) {
            const int32_t image = pharaoh_empire_object_city_image(
                c->objects, i, *c->scenario_rank, c->catalog);
            limit = *c->default_image_limit;
            object->image_id = (uint16_t)image;
        } else if (object->kind == 0) {
            const uint16_t image = (uint16_t)(object->image_offset +
                (uint16_t)*c->default_image_base);
            object->image_id = image;
            if ((int32_t)image > limit) object->image_id = (uint16_t)limit;
        }
    }
}

/* Reused Unit00608; native truncates the image to AX before header lookup. */
void pharaoh_empire_refresh_city_image_dimensions(const PharaohEmpireImagesContext *c)
{
    for (int32_t i = 0; i < PHARAOH_EMPIRE_OBJECT_COUNT; ++i) {
        PharaohEmpireObjectRecord *const object = &c->objects[i];
        if (object->in_use == 0 || object->kind != 1) continue;
        const int32_t image = pharaoh_empire_object_city_image(
            c->objects, i, *c->scenario_rank, c->catalog);
        const int32_t count = *c->image_count;
        object->image_id = (uint16_t)image;
        const uint32_t index = (uint16_t)image;
        const uint32_t selected = (int32_t)index < count ? index : 0;
        object->image_width = (uint16_t)c->images[selected].width;
        object->image_height = (uint16_t)c->images[selected].height;
    }
}
