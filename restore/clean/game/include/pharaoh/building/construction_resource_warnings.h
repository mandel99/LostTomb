#ifndef PHARAOH_BUILDING_CONSTRUCTION_RESOURCE_WARNINGS_H
#define PHARAOH_BUILDING_CONSTRUCTION_RESOURCE_WARNINGS_H
#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/monuments/definitions.h"
#include "pharaoh/runtime/city_warning.h"

typedef struct PharaohConstructionResourceWarnings {
    const uint32_t *editor_active;
    const int32_t *industry_active,*industry_total,*building_totals;
    const PharaohEmpireSupplyContext *supply;
    const PharaohEmpireObjectRecord *objects;
    PharaohCityWarningContext *warnings;
    const PharaohMonumentDefinition *definitions;
    const int16_t *goal_definition_ids;
} PharaohConstructionResourceWarnings;
void pharaoh_construction_warning_brewery(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_weaver(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_jeweler(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_potter(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_papyrus(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_paint(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_cattle(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_carpenters(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_sandstone(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_bricks(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_lamps(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_senet(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_zoo(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_papyrus_users(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_mortuary(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_artisans(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_pyramid(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_ptolemaic(const PharaohConstructionResourceWarnings *c,int32_t tool);
void pharaoh_construction_warning_venue_schools(
    const PharaohConstructionResourceWarnings *c,int32_t tool);
#endif
