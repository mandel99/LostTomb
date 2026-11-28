#include "pharaoh/monuments/provision_kinds.h"

/* Complete C translation of Pharaoh.exe 0x0040CC40. */
int32_t pharaoh_monument_count_provision_kinds(
    const int32_t *amounts, int32_t excluded_resource)
{
    int32_t resource;
    int32_t count = 0;
    for (resource = 0; resource < PHARAOH_MONUMENT_PROVISION_KIND_COUNT;
         ++resource) {
        if (resource != excluded_resource && amounts[resource] > 0) ++count;
    }
    return count;
}
