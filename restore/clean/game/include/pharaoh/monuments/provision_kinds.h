#ifndef PHARAOH_MONUMENTS_PROVISION_KINDS_H
#define PHARAOH_MONUMENTS_PROVISION_KINDS_H

#include <stdint.h>

enum { PHARAOH_MONUMENT_PROVISION_KIND_COUNT = 36 };

int32_t pharaoh_monument_count_provision_kinds(
    const int32_t *amounts, int32_t excluded_resource);

#endif
