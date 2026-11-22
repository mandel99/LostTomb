#ifndef PHARAOH_MONUMENTS_BUILD_REQUIREMENTS_H
#define PHARAOH_MONUMENTS_BUILD_REQUIREMENTS_H

#include "pharaoh/commerce/export_keep.h"
#include "pharaoh/monuments/base_progress.h"
#include "pharaoh/monuments/definitions.h"

typedef struct PharaohMonumentBuildRequirements {
    PharaohMonumentResourceDemand *demand;
    const PharaohMonumentDefinition *definitions;
    PharaohBaseMonumentProgress *library, *caesareum, *lighthouse, *royal_tombs;
} PharaohMonumentBuildRequirements;

void pharaoh_monument_requirements_bind_records(PharaohMonumentBuildRequirements *c);
uint8_t pharaoh_monument_resource_is_required(PharaohMonumentBuildRequirements *c,
    int32_t resource,uint8_t include_completed);
uint8_t pharaoh_monument_guild_is_required(PharaohMonumentBuildRequirements *c,
    int32_t building_type);

#endif
