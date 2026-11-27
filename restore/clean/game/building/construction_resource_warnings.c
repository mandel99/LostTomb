#include "pharaoh/building/construction_resource_warnings.h"
#include <string.h>

typedef enum ResourceType {
    ResourceNone = 0,
    ResourceGrain = 1,
    ResourceMeat = 2,
    ResourceLettuce = 3,
    ResourceChickpeas = 4,
    ResourcePomegranates = 5,
    ResourceFigs = 6,
    ResourceFish = 7,
    ResourceGamemeat = 8,
    ResourceStraw = 9,
    ResourceWeapons = 10,
    ResourceClay = 11,
    ResourceBricks = 12,
    ResourcePottery = 13,
    ResourceBarley = 14,
    ResourceBeer = 15,
    ResourceFlax = 16,
    ResourceLinen = 17,
    ResourceGems = 18,
    ResourceLuxuryGoods = 19,
    ResourceTimber = 20,
    ResourceGold = 21,
    ResourceReeds = 22,
    ResourcePapyrus = 23,
    ResourceStone = 24,
    ResourceLimestone = 25,
    ResourceGranite = 26,
    ResourceUnused = 27,
    ResourceChariots = 28,
    ResourceCopper = 29,
    ResourceSandstone = 30,
    ResourceOil = 31,
    ResourceHenna = 32,
    ResourcePaint = 33,
    ResourceLamps = 34,
    ResourceMarble = 35,
    ResourceDeben = 36,
    ResourceTroops = 37,
    ResourceMin = 1,
    ResourceMax = 36,
    ResourceMinFood = 1,
    ResourceMaxFood = 9,
} ResourceType;

enum {
    BuildingAlexandriaLibrary = 225,
    BuildingArtisansGuild = 231,
    BuildingBrewery = 110,
    BuildingBrickworks = 204,
    BuildingCaesareum = 227,
    BuildingCarpentersGuild = 177,
    BuildingCattleRanch = 194,
    BuildingJeweler = 113,
    BuildingLampMaker = 232,
    BuildingLibrary = 53,
    BuildingLighthouse = 228,
    BuildingMausoleum = 222,
    BuildingMortuary = 47,
    BuildingPaintMaker = 233,
    BuildingPapyrusMaker = 203,
    BuildingPotter = 114,
    BuildingPyramid = 183,
    BuildingScribalSchool = 51,
    BuildingSenetHouse = 32,
    BuildingSunTemple = 213,
    BuildingWeaver = 111,
    BuildingZoo = 226,
    TradeStatusImport = 1,
    TradeStatusImportAsNeeded = 3,
    WarningBuildBarleyFarm = 34,
    WarningBuildBrewery = 138,
    WarningBuildBrickworks = 160,
    WarningBuildClayPit = 36,
    WarningBuildFlaxFarm = 35,
    WarningBuildGemMine = 37,
    WarningBuildGrainFarm = 83,
    WarningBuildHennaFarm = 262,
    WarningBuildHuntingLodge = 238,
    WarningBuildHuntingLodgeOrImport = 239,
    WarningBuildLimestoneQuarry = 154,
    WarningBuildPaintMaker = 269,
    WarningBuildPaintMakerOrImport = 267,
    WarningBuildPapyrusMaker = 140,
    WarningBuildPotter = 256,
    WarningBuildReedGatherer = 81,
    WarningBuildSandstoneQuarry = 165,
    WarningBuildStoneQuarry = 151,
    WarningBuildWeaver = 142,
    WarningBuildWoodCutter = 144,
    WarningBuildingNeedsBeer = 137,
    WarningBuildingNeedsClay = 29,
    WarningBuildingNeedsClayAndPaint = 270,
    WarningBuildingNeedsClayAndStraw = 145,
    WarningBuildingNeedsFlax = 28,
    WarningBuildingNeedsGameMeat = 237,
    WarningBuildingNeedsHenna = 261,
    WarningBuildingNeedsLinen = 141,
    WarningBuildingNeedsOil = 265,
    WarningBuildingNeedsOilAndPottery = 255,
    WarningBuildingNeedsPottery = 266,
    WarningBuildingNeedsReeds = 80,
    WarningBuildingNeedsStraw = 82,
    WarningBuildingNeedsWood = 143,
    WarningGameMeatImportTradeRoute = 240,
    WarningImportBarleyOverseer = 168,
    WarningImportBarleyTradeRoute = 169,
    WarningImportBricksOverseer = 162,
    WarningImportBricksTradeRoute = 161,
    WarningImportClayOverseer = 147,
    WarningImportClayTradeRoute = 146,
    WarningImportFlaxOverseer = 174,
    WarningImportFlaxTradeRoute = 175,
    WarningImportHennaOverseer = 263,
    WarningImportHennaTradeRoute = 264,
    WarningImportLimestoneOverseer = 156,
    WarningImportLimestoneTradeRoute = 155,
    WarningImportReedsOverseer = 170,
    WarningImportReedsTradeRoute = 171,
    WarningImportStoneOverseer = 153,
    WarningImportStoneTradeRoute = 152,
    WarningImportStrawOverseer = 149,
    WarningImportStrawTradeRoute = 148,
    WarningLinenImportOverseer = 195,
    WarningLinenImportTradeRoute = 196,
    WarningMonumentNeedsBricks = 163,
    WarningMonumentNeedsBricksAndLimestone = 159,
    WarningMonumentNeedsCopper = 241,
    WarningMonumentNeedsGranite = 248,
    WarningMonumentNeedsLimestone = 158,
    WarningMonumentNeedsMarble = 242,
    WarningMonumentNeedsPlainStone = 157,
    WarningMonumentNeedsSandstone = 164,
    WarningMonumentNeedsStoneAndLimestone = 150,
    WarningNeedsBarley = 27,
    WarningNeedsGems = 30,
    WarningNeedsPapyrus = 139,
    WarningOverseerOFCommerceToImport = 32,
    WarningPaintImportOverseer = 268,
    WarningSetupTradeRouteToImport = 31
};

/* Typed relative views of the native 49D8-byte player record. Keep the
 * shared opaque player header/staged changes intact. Native loads prove
 * signed DWORD stocks at48F0 and WORD trade states at2826; memcpy avoids
 * imposing a second incompatible C struct type on the same saved bytes. */
_Static_assert(sizeof(PharaohScenarioPlayerRuntime)==0x49d8,"native player stride");
static int32_t stored(const PharaohScenarioPlayerRuntime *player,int32_t resource)
{
    int32_t value;
    memcpy(&value,(const uint8_t *)player+0x48f0+(uint32_t)resource*4u,sizeof(value));
    return value;
}
static int16_t trade_status(const PharaohScenarioPlayerRuntime *player,int32_t resource)
{
    int16_t value;
    memcpy(&value,(const uint8_t *)player+0x2826+(uint32_t)resource*2u,sizeof(value));
    return value;
}
static uint32_t can_import(const PharaohConstructionResourceWarnings *c,int32_t resource)
{
    const PharaohScenarioPlayerRuntime *player=&c->supply->players[*c->supply->current_player];
    return pharaoh_empire_can_import_resource(c->supply->cities,c->supply->route_limits,resource,
        player->ra_trade_bonus_months,player->ra_trade_penalty_months,player->ra_trade_minor_penalty_months);
}
static uint32_t can_import_potentially(const PharaohConstructionResourceWarnings *c,int32_t resource)
{
    const PharaohScenarioPlayerRuntime *player=&c->supply->players[*c->supply->current_player];
    return pharaoh_empire_can_import_resource_potentially(c->supply->cities,c->supply->route_limits,resource,
        player->ra_trade_bonus_months,player->ra_trade_penalty_months,player->ra_trade_minor_penalty_months);
}
enum ConstructionResourceMessage {
    SenetWarningBeerImportDisabled = 191,
    SenetWarningBeerRouteUnavailable = 192,
    PapyrusWarningImportDisabled = 193,
    PapyrusWarningRouteUnavailable = 194,
    LampWarningPotteryImportDisabled = 257,
    LampWarningPotteryRouteUnavailable = 258,
    LampWarningOilImportDisabled = 259,
    LampWarningOilRouteUnavailable = 260,
};

typedef struct ResourceProductionWarning {
    int32_t expectedTool;
    int32_t alternateTool;
    ResourceType industryInput;
    ResourceType storedProduct;
    ResourceType storedInput;
    ResourceType locallyProducedInput;
    ResourceType importedInput;
    int32_t missingInputMessage;
    int32_t localProducerMessage;
    int32_t importDisabledMessage;
    int32_t noTradeRouteMessage;
} ResourceProductionWarning;

typedef struct RequiredInputWarning {
    ResourceType activeIndustry;
    ResourceType storedResource;
    ResourceType locallyProducedResource;
    ResourceType importedResource;
    int32_t missingMessage;
    int32_t localProducerMessage;
    int32_t dedicatedNoRouteMessage;
    int32_t dedicatedImportDisabledMessage;
} RequiredInputWarning;

static void ShowConstructionWarning(const PharaohConstructionResourceWarnings *c,int32_t message)
{
    pharaoh_city_warning_post(c->warnings,message);
}

static bool ResourceImportEnabled(const PharaohScenarioPlayerRuntime *player, ResourceType resource)
{
    int16_t status = trade_status(player,resource);
    return status == TradeStatusImport || status == TradeStatusImportAsNeeded;
}

static bool RequiredInputAvailable(const PharaohConstructionResourceWarnings *c,const PharaohScenarioPlayerRuntime *player, const RequiredInputWarning *input)
{
    return c->industry_active[input->activeIndustry] > 0 ||
           stored(player,input->storedResource) > 0;
}

static void WarnDedicatedInput(const PharaohConstructionResourceWarnings *c,const PharaohScenarioPlayerRuntime *player, const RequiredInputWarning *input)
{
    if (input->locallyProducedResource != ResourceNone &&
        pharaoh_empire_can_produce_resource(c->supply,input->locallyProducedResource)) {
        ShowConstructionWarning(c,input->localProducerMessage);
    } else if (!can_import(c,input->importedResource)) {
        ShowConstructionWarning(c,input->dedicatedNoRouteMessage);
    } else if (!ResourceImportEnabled(player, input->importedResource)) {
        ShowConstructionWarning(c,input->dedicatedImportDisabledMessage);
    }
}

static void WarnGenericInput(const PharaohConstructionResourceWarnings *c,const PharaohScenarioPlayerRuntime *player, const RequiredInputWarning *input)
{
    if (input->locallyProducedResource != ResourceNone &&
        pharaoh_empire_can_produce_resource(c->supply,input->locallyProducedResource)) {
        ShowConstructionWarning(c,input->localProducerMessage);
    } else if (!can_import(c,input->importedResource)) {
        ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
    } else if (!ResourceImportEnabled(player, input->importedResource)) {
        ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
    }
}

static void CheckTwoRequiredInputs(const PharaohConstructionResourceWarnings *c,const PharaohScenarioPlayerRuntime *player,
                                   const RequiredInputWarning *first,
                                   const RequiredInputWarning *second,
                                   int32_t bothMissingMessage)
{
    bool firstAvailable = RequiredInputAvailable(c,player, first);
    bool secondAvailable = RequiredInputAvailable(c,player, second);

    if (firstAvailable && secondAvailable) {
        return;
    }
    if (!firstAvailable && !secondAvailable) {
        ShowConstructionWarning(c,bothMissingMessage);
        WarnDedicatedInput(c,player, first);
        WarnDedicatedInput(c,player, second);
    } else if (!firstAvailable) {
        ShowConstructionWarning(c,first->missingMessage);
        WarnGenericInput(c,player, first);
    } else {
        ShowConstructionWarning(c,second->missingMessage);
        WarnGenericInput(c,player, second);
    }
}

static void ConstructionWarningCheckResource(const PharaohConstructionResourceWarnings *c,int32_t tool, const ResourceProductionWarning *rule)
{
    const PharaohScenarioPlayerRuntime *player;

    if ((*c->editor_active) == 1 ||
        (tool != rule->expectedTool && tool != rule->alternateTool) ||
        c->industry_active[rule->industryInput] > 0) {
        return;
    }

    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,rule->storedProduct) > 0 ||
        (rule->storedInput != ResourceNone && stored(player,rule->storedInput) > 0)) {
        return;
    }

    ShowConstructionWarning(c,rule->missingInputMessage);
    if (pharaoh_empire_can_produce_resource(c->supply,rule->locallyProducedInput)) {
        ShowConstructionWarning(c,rule->localProducerMessage);
    } else if (!can_import(c,rule->importedInput)) {
        ShowConstructionWarning(c,rule->noTradeRouteMessage);
    } else if (!ResourceImportEnabled(player, rule->importedInput)) {
        ShowConstructionWarning(c,rule->importDisabledMessage);
    }
}

/* Complete typed reuse, native004E33A0. */
void pharaoh_construction_warning_brewery(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingBrewery, BuildingBrewery, ResourceBarley, ResourceBeer, ResourceBarley,
        ResourceBarley, ResourceBarley, WarningNeedsBarley, WarningBuildBarleyFarm,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E3460. */
void pharaoh_construction_warning_weaver(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingWeaver, BuildingWeaver, ResourceFlax, ResourceLinen, ResourceFlax,
        ResourceFlax, ResourceFlax, WarningBuildingNeedsFlax, WarningBuildFlaxFarm,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E3520. */
void pharaoh_construction_warning_jeweler(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingJeweler, BuildingJeweler, ResourceGems, ResourceLuxuryGoods, ResourceGems,
        ResourceGems, ResourceGems, WarningNeedsGems, WarningBuildGemMine,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E35E0. */
void pharaoh_construction_warning_potter(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingPotter, BuildingPotter, ResourceClay, ResourcePottery, ResourceClay,
        ResourceClay, ResourceClay, WarningBuildingNeedsClay, WarningBuildClayPit,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E36A0. */
void pharaoh_construction_warning_papyrus(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingPapyrusMaker, BuildingPapyrusMaker, ResourceReeds, ResourcePapyrus, ResourceReeds,
        ResourceReeds, ResourceReeds, WarningBuildingNeedsReeds, WarningBuildReedGatherer,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E39D0. */
void pharaoh_construction_warning_paint(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingPaintMaker, BuildingPaintMaker, ResourceHenna, ResourcePaint, ResourceHenna,
        ResourceHenna, ResourceHenna, WarningBuildingNeedsHenna, WarningBuildHennaFarm,
        WarningImportHennaOverseer, WarningImportHennaTradeRoute,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E3CF0. */
void pharaoh_construction_warning_cattle(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingCattleRanch, BuildingCattleRanch, ResourceGrain, ResourceStraw, ResourceNone,
        ResourceGrain, ResourceStraw, WarningBuildingNeedsStraw, WarningBuildGrainFarm,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E4490. */
void pharaoh_construction_warning_carpenters(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingCarpentersGuild, BuildingCarpentersGuild, ResourceTimber, ResourceTimber, ResourceNone,
        ResourceTimber, ResourceTimber, WarningBuildingNeedsWood, WarningBuildWoodCutter,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E4E10. */
void pharaoh_construction_warning_sandstone(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const ResourceProductionWarning rule = {
        BuildingSunTemple, BuildingMausoleum, ResourceSandstone, ResourceSandstone, ResourceNone,
        ResourceSandstone, ResourceSandstone, WarningMonumentNeedsSandstone, WarningBuildSandstoneQuarry,
        WarningOverseerOFCommerceToImport, WarningSetupTradeRouteToImport,
    };
    ConstructionWarningCheckResource(c,tool, &rule);
}

/* Complete typed reuse, native004E3760. */
void pharaoh_construction_warning_bricks(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const RequiredInputWarning clay = {
        ResourceClay, ResourceClay, ResourceClay, ResourceClay,
        WarningBuildingNeedsClay, WarningBuildClayPit,
        WarningImportClayTradeRoute, WarningImportClayOverseer,
    };
    static const RequiredInputWarning straw = {
        ResourceGrain, ResourceStraw, ResourceGrain, ResourceStraw,
        WarningBuildingNeedsStraw, WarningBuildGrainFarm,
        WarningImportStrawTradeRoute, WarningImportStrawOverseer,
    };
    const PharaohScenarioPlayerRuntime *player;

    if ((*c->editor_active) == 1 || tool != BuildingBrickworks) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceBricks) > 0) {
        return;
    }
    CheckTwoRequiredInputs(c,player, &clay, &straw, WarningBuildingNeedsClayAndStraw);
}

/* Complete typed reuse, native004E3AA0. */
void pharaoh_construction_warning_lamps(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const RequiredInputWarning oil = {
        ResourceOil, ResourceOil, ResourceNone, ResourceOil,
        WarningBuildingNeedsOil, 0,
        LampWarningOilRouteUnavailable, LampWarningOilImportDisabled,
    };
    static const RequiredInputWarning pottery = {
        ResourcePottery, ResourcePottery, ResourcePottery, ResourcePottery,
        WarningBuildingNeedsPottery, WarningBuildPotter,
        LampWarningPotteryRouteUnavailable, LampWarningPotteryImportDisabled,
    };
    const PharaohScenarioPlayerRuntime *player;

    if ((*c->editor_active) == 1 || tool != BuildingLampMaker) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceLamps) > 0) {
        return;
    }
    CheckTwoRequiredInputs(c,player, &oil, &pottery, WarningBuildingNeedsOilAndPottery);
}

/* Complete typed reuse, native004E3DA0. */
void pharaoh_construction_warning_senet(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;
    int32_t barleyPotential;

    if ((*c->editor_active) == 1 || tool != BuildingSenetHouse) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceBeer) > 0) {
        return;
    }
    ShowConstructionWarning(c,WarningBuildingNeedsBeer);
    if (c->industry_total[ResourceBarley] > 0) {
        return;
    }

    barleyPotential = can_import_potentially(c,ResourceBarley);
    if ((!pharaoh_empire_our_city_sells_resource(c->objects,ResourceBarley) && !barleyPotential) ||
        c->industry_total[ResourceBeer] != 0) {
        barleyPotential = can_import_potentially(c,ResourceBarley);
        if (!barleyPotential || c->industry_total[ResourceBeer] == 0) {
            if (can_import_potentially(c,ResourceBeer)) {
                if (!can_import(c,ResourceBeer)) {
                    ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
                } else if (!ResourceImportEnabled(player, ResourceBeer)) {
                    ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
                }
            }
        } else if (!can_import(c,ResourceBarley)) {
            ShowConstructionWarning(c,WarningImportBarleyTradeRoute);
        } else if (!ResourceImportEnabled(player, ResourceBarley)) {
            ShowConstructionWarning(c,WarningImportBarleyOverseer);
        }
        return;
    }

    if (!can_import_potentially(c,ResourceBeer)) {
        ShowConstructionWarning(c,WarningBuildBrewery);
    } else if (!can_import(c,ResourceBeer)) {
        ShowConstructionWarning(c,SenetWarningBeerRouteUnavailable);
    } else if (!ResourceImportEnabled(player, ResourceBeer)) {
        ShowConstructionWarning(c,SenetWarningBeerImportDisabled);
    }
}

/* Complete typed reuse, native004E3F60. */
void pharaoh_construction_warning_zoo(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;
    int32_t gameMeatPotential;

    if ((*c->editor_active) == 1 || tool != BuildingZoo) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceGamemeat) > 0) {
        return;
    }
    ShowConstructionWarning(c,WarningBuildingNeedsGameMeat);
    if (c->industry_total[ResourceGamemeat] > 0) {
        return;
    }
    gameMeatPotential = can_import_potentially(c,ResourceGamemeat);
    if ((!pharaoh_empire_our_city_sells_resource(c->objects,ResourceGamemeat) && !gameMeatPotential) ||
        c->industry_total[ResourceGamemeat] != 0) {
        if (can_import_potentially(c,ResourceGamemeat) &&
            !can_import(c,ResourceGamemeat)) {
            ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
        } else if (can_import_potentially(c,ResourceGamemeat) &&
                   !ResourceImportEnabled(player, ResourceGamemeat)) {
            ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
        }
    } else if (!can_import_potentially(c,ResourceGamemeat)) {
        ShowConstructionWarning(c,WarningBuildHuntingLodge);
    } else if (!can_import(c,ResourceGamemeat)) {
        ShowConstructionWarning(c,WarningGameMeatImportTradeRoute);
    } else {
        /* The original checks the meat slot here, not game meat. */
        int16_t status = trade_status(player,ResourceMeat);
        if (status != TradeStatusImport && status != TradeStatusImportAsNeeded) {
            ShowConstructionWarning(c,WarningBuildHuntingLodgeOrImport);
        }
    }

    if (c->industry_active[ResourceGrain] < 1 &&
        stored(player,ResourceStraw) < 1) {
        ShowConstructionWarning(c,WarningBuildingNeedsStraw);
        if (pharaoh_empire_can_produce_resource(c->supply,ResourceGrain)) {
            ShowConstructionWarning(c,WarningBuildGrainFarm);
        } else if (!can_import(c,ResourceStraw)) {
            ShowConstructionWarning(c,WarningImportStrawTradeRoute);
        } else if (!ResourceImportEnabled(player, ResourceStraw)) {
            ShowConstructionWarning(c,WarningImportStrawOverseer);
        }
    }
}

/* Complete typed reuse, native004E4120. */
void pharaoh_construction_warning_papyrus_users(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;
    int32_t reedsPotential;

    if ((*c->editor_active) == 1 || (tool != BuildingScribalSchool && tool != BuildingLibrary)) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourcePapyrus) > 0) {
        return;
    }
    ShowConstructionWarning(c,WarningNeedsPapyrus);
    if (c->industry_total[ResourceReeds] >= 1) {
        return;
    }

    reedsPotential = can_import_potentially(c,ResourceReeds);
    if ((pharaoh_empire_our_city_sells_resource(c->objects,ResourceReeds) || reedsPotential) &&
        c->industry_total[ResourcePapyrus] == 0) {
        if (!can_import_potentially(c,ResourcePapyrus)) {
            ShowConstructionWarning(c,WarningBuildPapyrusMaker);
        } else if (can_import(c,ResourcePapyrus)) {
            if (trade_status(player,ResourcePapyrus) != TradeStatusImport) {
                ShowConstructionWarning(c,PapyrusWarningImportDisabled);
            }
        } else {
            ShowConstructionWarning(c,PapyrusWarningRouteUnavailable);
        }
        return;
    }

    reedsPotential = can_import_potentially(c,ResourceReeds);
    if (!reedsPotential || c->industry_total[ResourcePapyrus] == 0) {
        if (can_import_potentially(c,ResourcePapyrus)) {
            if (!can_import(c,ResourcePapyrus)) {
                ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
            } else if (!ResourceImportEnabled(player, ResourcePapyrus)) {
                ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
            }
        }
    } else if (!can_import(c,ResourceReeds)) {
        ShowConstructionWarning(c,WarningImportReedsTradeRoute);
    } else if (!ResourceImportEnabled(player, ResourceReeds)) {
        ShowConstructionWarning(c,WarningImportReedsOverseer);
    }
}

/* Complete typed reuse, native004E42D0. */
void pharaoh_construction_warning_mortuary(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;
    int32_t flaxPotential;

    if ((*c->editor_active) == 1 || tool != BuildingMortuary) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceLinen) > 0) {
        return;
    }
    ShowConstructionWarning(c,WarningBuildingNeedsLinen);
    if (c->industry_total[ResourceFlax] > 0) {
        return;
    }

    flaxPotential = can_import_potentially(c,ResourceFlax);
    if ((!pharaoh_empire_our_city_sells_resource(c->objects,ResourceFlax) && !flaxPotential) ||
        c->industry_total[ResourceLinen] != 0) {
        flaxPotential = can_import_potentially(c,ResourceFlax);
        if (!flaxPotential || c->industry_total[ResourceLinen] == 0) {
            if (can_import_potentially(c,ResourceLinen)) {
                if (!can_import(c,ResourceLinen)) {
                    ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
                } else if (!ResourceImportEnabled(player, ResourceLinen)) {
                    ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
                }
            }
        } else if (!can_import(c,ResourceFlax)) {
            ShowConstructionWarning(c,WarningImportFlaxTradeRoute);
        } else if (!ResourceImportEnabled(player, ResourceFlax)) {
            ShowConstructionWarning(c,WarningImportFlaxOverseer);
        }
        return;
    }

    if (!can_import_potentially(c,ResourceLinen)) {
        ShowConstructionWarning(c,WarningBuildWeaver);
    } else if (!can_import(c,ResourceLinen)) {
        ShowConstructionWarning(c,WarningLinenImportTradeRoute);
    } else if (!ResourceImportEnabled(player, ResourceLinen)) {
        ShowConstructionWarning(c,WarningLinenImportOverseer);
    }
}

/* Complete typed reuse, native004E4550. */
void pharaoh_construction_warning_artisans(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;
    int32_t hennaPotential;

    if ((*c->editor_active) == 1 || tool != BuildingArtisansGuild) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if ((c->industry_active[ResourceClay] > 0 ||
         stored(player,ResourceClay) > 0) &&
        (c->industry_active[ResourcePaint] > 0 ||
         stored(player,ResourcePaint) > 0)) {
        return;
    }

    ShowConstructionWarning(c,WarningBuildingNeedsClayAndPaint);
    if (c->industry_active[ResourceClay] == 0 &&
        stored(player,ResourceClay) == 0) {
        if (pharaoh_empire_can_produce_resource(c->supply,ResourceClay)) {
            ShowConstructionWarning(c,WarningBuildClayPit);
        } else if (!can_import(c,ResourceClay)) {
            ShowConstructionWarning(c,WarningImportClayTradeRoute);
        } else if (!ResourceImportEnabled(player, ResourceClay)) {
            ShowConstructionWarning(c,WarningImportClayOverseer);
        }
    }

    if (c->industry_active[ResourcePaint] != 0 ||
        stored(player,ResourcePaint) != 0) {
        return;
    }
    hennaPotential = can_import_potentially(c,ResourceHenna);
    if ((!pharaoh_empire_our_city_sells_resource(c->objects,ResourceHenna) && !hennaPotential) ||
        c->industry_total[ResourcePaint] != 0) {
        hennaPotential = can_import_potentially(c,ResourceHenna);
        if (!hennaPotential || c->industry_total[ResourcePaint] == 0) {
            if (can_import_potentially(c,ResourcePaint)) {
                if (!can_import(c,ResourcePaint)) {
                    ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
                } else if (!ResourceImportEnabled(player, ResourcePaint)) {
                    ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
                }
            }
        } else if (!can_import(c,ResourceHenna)) {
            ShowConstructionWarning(c,WarningImportHennaTradeRoute);
        } else if (!ResourceImportEnabled(player, ResourceHenna)) {
            ShowConstructionWarning(c,WarningImportHennaOverseer);
        }
        return;
    }

    if (!can_import_potentially(c,ResourcePaint)) {
        ShowConstructionWarning(c,WarningBuildPaintMaker);
    } else if (!can_import(c,ResourcePaint)) {
        ShowConstructionWarning(c,WarningBuildPaintMakerOrImport);
    } else if (!ResourceImportEnabled(player, ResourcePaint)) {
        ShowConstructionWarning(c,WarningPaintImportOverseer);
    }
}

/* Complete typed reuse, native004E47D0. */
void pharaoh_construction_warning_pyramid(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    static const RequiredInputWarning stone = {
        ResourceStone, ResourceStone, ResourceStone, ResourceStone,
        WarningMonumentNeedsPlainStone, WarningBuildStoneQuarry,
        WarningImportStoneTradeRoute, WarningImportStoneOverseer,
    };
    static const RequiredInputWarning limestone = {
        ResourceLimestone, ResourceLimestone, ResourceLimestone, ResourceLimestone,
        WarningMonumentNeedsLimestone, WarningBuildLimestoneQuarry,
        WarningImportLimestoneTradeRoute, WarningImportLimestoneOverseer,
    };
    static const RequiredInputWarning bricks = {
        ResourceBricks, ResourceBricks, ResourceBricks, ResourceBricks,
        WarningMonumentNeedsBricks, WarningBuildBrickworks,
        WarningImportBricksTradeRoute, WarningImportBricksOverseer,
    };
    const PharaohScenarioPlayerRuntime *player;

    if ((*c->editor_active) == 1 || tool != BuildingPyramid) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    switch (pharaoh_scenario_get_pyramid_style(c->definitions,c->goal_definition_ids)) {
    case 1:
    case 4:
        CheckTwoRequiredInputs(c,player, &stone, &limestone,
                               WarningMonumentNeedsStoneAndLimestone);
        break;
    case 2:
        CheckTwoRequiredInputs(c,player, &bricks, &limestone,
                               WarningMonumentNeedsBricksAndLimestone);
        break;
    case 3:
        if (!RequiredInputAvailable(c,player, &stone)) {
            ShowConstructionWarning(c,stone.missingMessage);
            WarnGenericInput(c,player, &stone);
        }
        break;
    case 5:
        if (!RequiredInputAvailable(c,player, &bricks)) {
            ShowConstructionWarning(c,bricks.missingMessage);
            WarnGenericInput(c,player, &bricks);
        }
        break;
    }
}

/* Complete typed reuse, native004E4ED0. */
void pharaoh_construction_warning_ptolemaic(const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    const PharaohScenarioPlayerRuntime *player;

    if ((*c->editor_active) == 1 ||
        (tool != BuildingLighthouse && tool != BuildingCaesareum &&
         tool != BuildingAlexandriaLibrary)) {
        return;
    }
    player = (&c->supply->players[*c->supply->current_player]);
    if (stored(player,ResourceMarble) == 0) {
        ShowConstructionWarning(c,WarningMonumentNeedsMarble);
        if (!can_import(c,ResourceMarble)) {
            ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
        } else if (!ResourceImportEnabled(player, ResourceMarble)) {
            ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
        }
    }

    if (tool == BuildingAlexandriaLibrary) {
        if (c->industry_active[ResourceCopper] != 0 ||
            stored(player,ResourceCopper) != 0) {
            return;
        }
        ShowConstructionWarning(c,WarningMonumentNeedsCopper);
        if (can_import(c,ResourceCopper)) {
            if (!ResourceImportEnabled(player, ResourceCopper)) {
                ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
            }
            return;
        }
    } else {
        if (tool != BuildingCaesareum) {
            return;
        }
        if (c->industry_active[ResourceGranite] != 0 ||
            stored(player,ResourceGranite) != 0) {
            return;
        }
        ShowConstructionWarning(c,WarningMonumentNeedsGranite);
        if (can_import(c,ResourceGranite)) {
            if (!ResourceImportEnabled(player, ResourceGranite)) {
                ShowConstructionWarning(c,WarningOverseerOFCommerceToImport);
            }
            return;
        }
    }
    ShowConstructionWarning(c,WarningSetupTradeRouteToImport);
}

/* Native entries in order: 004E33A0, 004E3460, 004E3520, 004E35E0,
 * 004E36A0, 004E3760, 004E39D0, 004E3AA0, 004E3CF0, 004E3DA0,
 * 004E3F60, 004E4120, 004E42D0, 004E4490, 004E4550, 004E47D0,
 * 004E4E10 and 004E4ED0. */

/* Complete VenueSchoolWarnings C reuse, native004E5070. Three independent
 * tests intentionally continue after a posted warning using live counts. */
void pharaoh_construction_warning_venue_schools(
    const PharaohConstructionResourceWarnings *c,int32_t tool)
{
    if(*c->editor_active==1)return;
    if(tool!=31&&tool!=30&&tool!=33)return;
    if(c->building_totals[36]<1)pharaoh_city_warning_post(c->warnings,208);
    if(tool!=31&&c->building_totals[34]<1)pharaoh_city_warning_post(c->warnings,209);
    if(tool==33&&c->building_totals[35]<1)pharaoh_city_warning_post(c->warnings,210);
}
