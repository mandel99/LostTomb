#include "pharaoh/commerce/storage_queries.h"

int32_t pharaoh_storage_image_level(int32_t amount)
{
    if (amount == 400) return 3;
    if (amount >= 300 && amount < 400) return 2;
    if (amount >= 200 && amount < 300) return 1;
    return 0;
}
