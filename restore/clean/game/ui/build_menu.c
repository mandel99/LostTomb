#include "pharaoh/ui/build_menu.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/scenario/definition_counts.h"

static uint8_t tutorial_ok(const PharaohBuildMenuContext *c)
{
    return pharaoh_mission_is_unlocked(c->settings->mission_id,
        c->final_mission_id, c->tutorial_flags[13]);
}

static uint32_t can_import(const PharaohBuildMenuContext *c, int32_t resource)
{
    const PharaohScenarioPlayerRuntime *p =
        &c->supply->players[*c->supply->current_player];
    return pharaoh_empire_can_import_resource_potentially(c->supply->cities,
        c->supply->route_limits, resource, p->ra_trade_bonus_months,
        p->ra_trade_penalty_months, p->ra_trade_minor_penalty_months);
}

enum {
    BuildMenuGroups = 38,
    BuildMenuItems = 30,
    BuildingApothecary = 46,
    BuildingArchitectPost = 81,
    BuildingBandstand = 30,
    BuildingBarleyFarm = 100,
    BuildingBazaar = 70,
    BuildingBazaarUp = 190,
    BuildingBooth = 31,
    BuildingBrewery = 110,
    BuildingBricklayersGuild = 178,
    BuildingBrickworks = 204,
    BuildingCarpentersGuild = 177,
    BuildingCattleRanch = 194,
    BuildingChariotMaker = 205,
    BuildingChickpeasFarm = 105,
    BuildingCityPalace = 189,
    BuildingClayPit = 109,
    BuildingClearLand = 9,
    BuildingConservatory = 34,
    BuildingCopperMine = 217,
    BuildingCourthouse = 184,
    BuildingDanceSchool = 35,
    BuildingDentist = 49,
    BuildingDock = 75,
    BuildingDockUp = 192,
    BuildingDynastyMansion = 79,
    BuildingFamilyMansion = 78,
    BuildingFerryLanding = 136,
    BuildingFestivalSquare = 209,
    BuildingFigFarm = 196,
    BuildingFirehouse = 167,
    BuildingFishingWharf = 76,
    BuildingFlaxFarm = 101,
    BuildingFort = 57,
    BuildingFortArchers = 44,
    BuildingFortCharioteers = 40,
    BuildingFortInfantry = 45,
    BuildingGardens = 39,
    BuildingGatehouseUp = 202,
    BuildingGemstoneMine = 162,
    BuildingGoldMine = 161,
    BuildingGrainFarm = 102,
    BuildingGranary = 71,
    BuildingGranaryUp = 191,
    BuildingGraniteQuarry = 216,
    BuildingHennaFarm = 224,
    BuildingHouseCrudeHut = 10,
    BuildingHousePalatialEstate = 29,
    BuildingHuntingLodge = 115,
    BuildingIrrigationDitch = 8,
    BuildingJeweler = 113,
    BuildingJugglerSchool = 36,
    BuildingLampMaker = 232,
    BuildingLargeStatue = 43,
    BuildingLettuceFarm = 103,
    BuildingLibrary = 53,
    BuildingLimestoneQuarry = 107,
    BuildingLowBridge = 82,
    BuildingMediumStatue = 42,
    BuildingMenuBeautification = 91,
    BuildingMenuConstructionGuilds = 4,
    BuildingMenuDefenses = 176,
    BuildingMenuFarms = 2,
    BuildingMenuRawMaterials = 3,
    BuildingMenuReserved137 = 137,
    BuildingMenuShrines = 150,
    BuildingMenuTempleComplexes = 97,
    BuildingMenuTemples = 96,
    BuildingMenuWaterCrossings = 52,
    BuildingMenuWaterLift = 7,
    BuildingMilitaryAcademy = 94,
    BuildingMortuary = 47,
    BuildingPaintMaker = 233,
    BuildingPapyrusMaker = 203,
    BuildingPavilion = 33,
    BuildingPersonalMansion = 77,
    BuildingPhysician = 206,
    BuildingPlaza = 38,
    BuildingPoliceStation = 55,
    BuildingPomegranatesFarm = 104,
    BuildingPotter = 114,
    BuildingRecruiter = 95,
    BuildingReedGatherer = 195,
    BuildingRoad = 5,
    BuildingRoadblock = 138,
    BuildingSandstoneQuarry = 221,
    BuildingScribalSchool = 51,
    BuildingSenetHouse = 32,
    BuildingShipwright = 74,
    BuildingShrineBast = 144,
    BuildingShrineGod5 = 145,
    BuildingShrineGod6 = 146,
    BuildingShrineGod7 = 147,
    BuildingShrineGod8 = 148,
    BuildingShrineGod9 = 149,
    BuildingShrineOsiris = 140,
    BuildingShrinePtah = 142,
    BuildingShrineRa = 141,
    BuildingShrineSeth = 143,
    BuildingSmallStatue = 41,
    BuildingStoneQuarry = 106,
    BuildingStonemasonsGuild = 179,
    BuildingStorageYard = 72,
    BuildingStorageYardUp = 193,
    BuildingTaxCollector = 86,
    BuildingTempleBast = 64,
    BuildingTempleComplexAltar = 211,
    BuildingTempleComplexBast = 69,
    BuildingTempleComplexGod5 = 156,
    BuildingTempleComplexGod6 = 157,
    BuildingTempleComplexGod7 = 158,
    BuildingTempleComplexGod8 = 159,
    BuildingTempleComplexGod9 = 160,
    BuildingTempleComplexOracle = 212,
    BuildingTempleComplexOsiris = 65,
    BuildingTempleComplexPtah = 67,
    BuildingTempleComplexRa = 66,
    BuildingTempleComplexSeth = 68,
    BuildingTempleGod5 = 151,
    BuildingTempleGod6 = 152,
    BuildingTempleGod7 = 153,
    BuildingTempleGod8 = 154,
    BuildingTempleGod9 = 155,
    BuildingTempleOsiris = 60,
    BuildingTemplePtah = 62,
    BuildingTempleRa = 61,
    BuildingTempleSeth = 63,
    BuildingTower = 173,
    BuildingTownPalaceUp = 188,
    BuildingTransportWharf = 181,
    BuildingVillagePalaceUp = 187,
    BuildingWall = 169,
    BuildingWarshipWharf = 182,
    BuildingWaterSupply = 180,
    BuildingWeaponsmith = 112,
    BuildingWeaver = 111,
    BuildingWell = 92,
    BuildingWoodCutter = 108,
    BuildingWorkCamp = 199,
    BuildingZoo = 226,
    ResourceBarley = 14,
    ResourceBeer = 15,
    ResourceBricks = 12,
    ResourceChickpeas = 4,
    ResourceClay = 11,
    ResourceCopper = 29,
    ResourceFigs = 6,
    ResourceFish = 7,
    ResourceFlax = 16,
    ResourceGamemeat = 8,
    ResourceGems = 18,
    ResourceGrain = 1,
    ResourceGranite = 26,
    ResourceHenna = 32,
    ResourceLettuce = 3,
    ResourceLimestone = 25,
    ResourceMeat = 2,
    ResourceOil = 31,
    ResourcePomegranates = 5,
    ResourcePottery = 13,
    ResourceReeds = 22,
    ResourceSandstone = 30,
    ResourceStone = 24,
    ResourceStraw = 9,
    ResourceTimber = 20,
    ScenarioAllowedBuildingApothecary = 20,
    ScenarioAllowedBuildingArchersFort = 38,
    ScenarioAllowedBuildingBandstandAndConservatory = 12,
    ScenarioAllowedBuildingBazaar = 8,
    ScenarioAllowedBuildingBoothAndJugglerSchool = 11,
    ScenarioAllowedBuildingChariotMaker = 42,
    ScenarioAllowedBuildingCharioteersFort = 39,
    ScenarioAllowedBuildingCourthouse = 24,
    ScenarioAllowedBuildingDentist = 19,
    ScenarioAllowedBuildingDock = 10,
    ScenarioAllowedBuildingFerryLanding = 29,
    ScenarioAllowedBuildingFestivalSquare = 15,
    ScenarioAllowedBuildingGardens = 30,
    ScenarioAllowedBuildingGatehouses = 35,
    ScenarioAllowedBuildingGoldMine = 2,
    ScenarioAllowedBuildingGranary = 7,
    ScenarioAllowedBuildingInfantryFort = 37,
    ScenarioAllowedBuildingIrrigationDitch = 4,
    ScenarioAllowedBuildingLibrary = 17,
    ScenarioAllowedBuildingLowBridge = 28,
    ScenarioAllowedBuildingMansions = 26,
    ScenarioAllowedBuildingMilitaryAcademy = 40,
    ScenarioAllowedBuildingMortuary = 22,
    ScenarioAllowedBuildingPalaces = 25,
    ScenarioAllowedBuildingPavilionAndDanceSchool = 13,
    ScenarioAllowedBuildingPhysician = 21,
    ScenarioAllowedBuildingPlaza = 31,
    ScenarioAllowedBuildingRecruiter = 36,
    ScenarioAllowedBuildingRoadblock = 27,
    ScenarioAllowedBuildingScribalSchool = 16,
    ScenarioAllowedBuildingSenetHouse = 14,
    ScenarioAllowedBuildingShipyardIndustry = 5,
    ScenarioAllowedBuildingStatues = 32,
    ScenarioAllowedBuildingStorageYard = 9,
    ScenarioAllowedBuildingTaxCollector = 23,
    ScenarioAllowedBuildingTowers = 34,
    ScenarioAllowedBuildingTransportWharf = 44,
    ScenarioAllowedBuildingWalls = 33,
    ScenarioAllowedBuildingWarshipWharf = 43,
    ScenarioAllowedBuildingWaterLift = 3,
    ScenarioAllowedBuildingWaterSupply = 18,
    ScenarioAllowedBuildingWeaponsmith = 41,
    ScenarioAllowedBuildingWorkCamp = 6,
    ScenarioAllowedBuildingZoo = 45,
    TutorialCount = 26,
    TutorialLastMission = 10,
};

enum BuildingMenuPlayerRankLimit {
    BuildingMenuLowerRankMinimum = 0,
    BuildingMenuLowerRankMaximum = 5,
    BuildingMenuMiddleRankMinimum = 6,
    BuildingMenuMiddleRankMaximum = 7,
    BuildingMenuUpperRankMinimum = 8,
    BuildingMenuUpperRankMaximum = 10,
};

static bool ScenarioAllows(PharaohBuildMenuContext *c, int32_t slot)
{
    return c->allowed_buildings[slot] != 0;
}

static bool PlayerRankIsBetween(PharaohBuildMenuContext *c, int32_t minimum, int32_t maximum)
{
    const int32_t rank = c->supply->players[*c->supply->current_player].initial_rank;
    return rank >= minimum && rank <= maximum;
}

static bool EmpireCanProduceAny(PharaohBuildMenuContext *c, const int32_t *resources, int32_t count)
{
    int32_t index;

    for (index = 0; index < count; ++index) {
        if (pharaoh_empire_can_produce_resource(c->supply, resources[index]) != 0) {
            return true;
        }
    }
    return false;
}

static bool BuildingMenuBasicAvailability(PharaohBuildMenuContext *c, int32_t type)
{
    if (type >= BuildingHouseCrudeHut && type <= BuildingHousePalatialEstate) {
        return true;
    }

    switch (type) {
    case BuildingClearLand:
    case BuildingRoad:
    case BuildingArchitectPost:
    case BuildingFirehouse:
    case BuildingPoliceStation:
    case BuildingWell:
    case BuildingMenuReserved137:
        return true;
    case BuildingWaterSupply:
        return ScenarioAllows(c, ScenarioAllowedBuildingWaterSupply);
    case BuildingHuntingLodge:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceGamemeat) != 0;
    case BuildingGranary:
        return ScenarioAllows(c, ScenarioAllowedBuildingGranary);
    case BuildingScribalSchool:
        return ScenarioAllows(c, ScenarioAllowedBuildingScribalSchool);
    case BuildingLibrary:
        return ScenarioAllows(c, ScenarioAllowedBuildingLibrary);
    case BuildingPhysician:
        return ScenarioAllows(c, ScenarioAllowedBuildingPhysician);
    case BuildingApothecary:
        return ScenarioAllows(c, ScenarioAllowedBuildingApothecary);
    case BuildingMortuary:
        return ScenarioAllows(c, ScenarioAllowedBuildingMortuary);
    case BuildingCourthouse:
        return ScenarioAllows(c, ScenarioAllowedBuildingCourthouse);
    case BuildingFerryLanding:
        return ScenarioAllows(c, ScenarioAllowedBuildingFerryLanding);
    case BuildingGoldMine:
        return ScenarioAllows(c, ScenarioAllowedBuildingGoldMine);
    case BuildingBooth:
    case BuildingJugglerSchool:
        return ScenarioAllows(c, ScenarioAllowedBuildingBoothAndJugglerSchool);
    case BuildingFestivalSquare:
        return ScenarioAllows(c, ScenarioAllowedBuildingFestivalSquare);
    case BuildingFigFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceFigs) != 0;
    case BuildingFlaxFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceFlax) != 0;
    case BuildingHennaFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceHenna) != 0;
    case BuildingBarleyFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceBarley) != 0;
    case BuildingClayPit:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceClay) != 0;
    case BuildingPotter:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceClay) != 0;
    case BuildingGardens:
        return ScenarioAllows(c, ScenarioAllowedBuildingGardens);
    case BuildingBrewery:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceBeer) != 0;
    case BuildingRoadblock:
        return ScenarioAllows(c, ScenarioAllowedBuildingRoadblock);
    case BuildingLowBridge:
        return ScenarioAllows(c, ScenarioAllowedBuildingLowBridge);
    case BuildingTaxCollector:
        return ScenarioAllows(c, ScenarioAllowedBuildingTaxCollector);
    case BuildingPapyrusMaker:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceReeds) != 0;
    case BuildingLampMaker:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceOil) != 0 &&
               pharaoh_empire_can_produce_resource_potentially(c->supply, ResourcePottery) != 0;
    case BuildingPaintMaker:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceHenna) != 0;
    case BuildingReedGatherer:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceReeds) != 0;
    case BuildingBazaar:
        return ScenarioAllows(c, ScenarioAllowedBuildingBazaar);
    case BuildingStorageYard:
        return ScenarioAllows(c, ScenarioAllowedBuildingStorageYard);
    case BuildingWorkCamp:
        return ScenarioAllows(c, ScenarioAllowedBuildingWorkCamp);
    case BuildingWeaver:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceFlax) != 0;
    case BuildingTempleOsiris:
    case BuildingShrineOsiris:
        return c->god_status[0] != 0;
    case BuildingTempleBast:
    case BuildingShrineBast:
        return c->god_status[4] != 0;
    case BuildingCarpentersGuild:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceTimber) != 0;
    case BuildingBricklayersGuild:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceBricks) != 0 ||
               pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceBricks) != 0;
    case BuildingShipwright:
        return ScenarioAllows(c, ScenarioAllowedBuildingShipyardIndustry) ||
               ScenarioAllows(c, ScenarioAllowedBuildingWarshipWharf) ||
               ScenarioAllows(c, ScenarioAllowedBuildingTransportWharf);
    case BuildingDentist:
        return ScenarioAllows(c, ScenarioAllowedBuildingDentist);
    case BuildingVillagePalaceUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingPalaces) &&
               PlayerRankIsBetween(c, BuildingMenuLowerRankMinimum, BuildingMenuLowerRankMaximum);
    default:
        return false;
    }
}

static bool BuildingMenuAggregateAvailability(PharaohBuildMenuContext *c, int32_t type, bool tutorialOk)
{
    static const int32_t farmResources[] = {
        ResourceBarley, ResourceGrain, ResourceLettuce, ResourceChickpeas,
        ResourceFlax, ResourcePomegranates, ResourceFigs, ResourceHenna,
    };
    static const int32_t rawMaterialResources[] = {
        ResourceReeds, ResourceClay, ResourceTimber, ResourceStone,
        ResourceLimestone, ResourceSandstone, ResourceCopper, ResourceGems,
    };

    if (tutorialOk && type == BuildingMenuConstructionGuilds) {
        return pharaoh_monument_guild_is_required(c->monuments, BuildingStonemasonsGuild) != 0 ||
               pharaoh_monument_guild_is_required(c->monuments, BuildingBricklayersGuild) != 0 ||
               pharaoh_monument_guild_is_required(c->monuments, BuildingCarpentersGuild) != 0;
    }

    switch (type) {
    case BuildingMenuFarms:
        return EmpireCanProduceAny(c, farmResources, (int32_t)(sizeof(farmResources) / sizeof(farmResources[0])));
    case BuildingMenuRawMaterials:
        return EmpireCanProduceAny(c, rawMaterialResources,
                                   (int32_t)(sizeof(rawMaterialResources) / sizeof(rawMaterialResources[0]))) ||
               ScenarioAllows(c, ScenarioAllowedBuildingGoldMine);
    case BuildingTempleComplexOsiris:
    case BuildingTempleComplexRa:
    case BuildingTempleComplexPtah:
    case BuildingTempleComplexSeth:
    case BuildingTempleComplexBast:
        return c->complexes_allowed[type - BuildingTempleComplexOsiris] != 0;
    case BuildingMenuTempleComplexes:
        return c->complexes_allowed[0] != 0 ||
               c->complexes_allowed[1] != 0 ||
               c->complexes_allowed[2] != 0 ||
               c->complexes_allowed[3] != 0 ||
               c->complexes_allowed[4] != 0;
    case BuildingTempleRa:
    case BuildingTemplePtah:
    case BuildingTempleSeth:
        return c->god_status[type - BuildingTempleOsiris] != 0;
    case BuildingMenuTemples:
    case BuildingMenuShrines:
        return c->god_status[0] != 0 || c->god_status[1] != 0 ||
               c->god_status[2] != 0 || c->god_status[3] != 0 ||
               c->god_status[4] != 0;
    case BuildingShrineRa:
    case BuildingShrinePtah:
    case BuildingShrineSeth:
        return c->god_status[type - BuildingShrineOsiris] != 0;
    case BuildingSmallStatue:
    case BuildingMediumStatue:
    case BuildingLargeStatue:
        return ScenarioAllows(c, ScenarioAllowedBuildingStatues);
    case BuildingMenuBeautification:
        return ScenarioAllows(c, ScenarioAllowedBuildingGardens) ||
               ScenarioAllows(c, ScenarioAllowedBuildingPlaza) ||
               ScenarioAllows(c, ScenarioAllowedBuildingStatues);
    case BuildingPlaza:
        return ScenarioAllows(c, ScenarioAllowedBuildingPlaza);
    case BuildingMenuWaterCrossings:
        return ScenarioAllows(c, ScenarioAllowedBuildingLowBridge) ||
               ScenarioAllows(c, ScenarioAllowedBuildingFerryLanding);
    default:
        return false;
    }
}

static bool BuildingMenuBrickworksAvailable(PharaohBuildMenuContext *c)
{
    if (*c->campaign_mode == 0 && c->settings->mission_id < 7) {
        return false;
    }

    return (((pharaoh_empire_can_produce_resource(c->supply, ResourceClay) != 0 ||
              pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceClay) != 0) &&
             pharaoh_empire_can_produce_resource(c->supply, ResourceGrain) != 0) ||
            ((pharaoh_empire_can_produce_resource(c->supply, ResourceClay) != 0 ||
              pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceClay) != 0) &&
             can_import(c, ResourceStraw) != 0) ||
            (pharaoh_empire_can_produce_resource(c->supply, ResourceGrain) != 0 &&
             can_import(c, ResourceClay) != 0) ||
            (can_import(c, ResourceStraw) != 0 &&
             can_import(c, ResourceClay) != 0));
}

static bool BuildingMenuSpecificAvailability(PharaohBuildMenuContext *c, int32_t type, bool current)
{
    switch (type) {
    case BuildingGrainFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceGrain) != 0;
    case BuildingCattleRanch:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceMeat) != 0;
    case BuildingIrrigationDitch:
        return ScenarioAllows(c, ScenarioAllowedBuildingIrrigationDitch);
    case BuildingTownPalaceUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingPalaces) &&
               PlayerRankIsBetween(c, BuildingMenuMiddleRankMinimum, BuildingMenuMiddleRankMaximum);
    case BuildingCityPalace:
        return ScenarioAllows(c, ScenarioAllowedBuildingPalaces) &&
               PlayerRankIsBetween(c, BuildingMenuUpperRankMinimum, BuildingMenuUpperRankMaximum);
    case BuildingPersonalMansion:
        return ScenarioAllows(c, ScenarioAllowedBuildingMansions) &&
               PlayerRankIsBetween(c, BuildingMenuLowerRankMinimum, BuildingMenuLowerRankMaximum);
    case BuildingFamilyMansion:
        return ScenarioAllows(c, ScenarioAllowedBuildingMansions) &&
               PlayerRankIsBetween(c, BuildingMenuMiddleRankMinimum, BuildingMenuMiddleRankMaximum);
    case BuildingDynastyMansion:
        return ScenarioAllows(c, ScenarioAllowedBuildingMansions) &&
               PlayerRankIsBetween(c, BuildingMenuUpperRankMinimum, BuildingMenuUpperRankMaximum);
    case BuildingMenuWaterLift:
        return ScenarioAllows(c, ScenarioAllowedBuildingWaterLift);
    case BuildingBandstand:
    case BuildingConservatory:
        return ScenarioAllows(c, ScenarioAllowedBuildingBandstandAndConservatory);
    case BuildingPavilion:
    case BuildingDanceSchool:
        return ScenarioAllows(c, ScenarioAllowedBuildingPavilionAndDanceSchool);
    case BuildingSenetHouse:
        return ScenarioAllows(c, ScenarioAllowedBuildingSenetHouse);
    case BuildingZoo:
        return ScenarioAllows(c, ScenarioAllowedBuildingZoo);
    case BuildingDock:
    case BuildingDockUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingDock);
    case BuildingFishingWharf:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceFish) != 0 &&
               ScenarioAllows(c, ScenarioAllowedBuildingShipyardIndustry);
    case BuildingTransportWharf:
        return ScenarioAllows(c, ScenarioAllowedBuildingTransportWharf);
    case BuildingWarshipWharf:
        return ScenarioAllows(c, ScenarioAllowedBuildingWarshipWharf);
    case BuildingWall:
        return ScenarioAllows(c, ScenarioAllowedBuildingWalls);
    case BuildingTower:
        return ScenarioAllows(c, ScenarioAllowedBuildingTowers);
    case BuildingGatehouseUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingGatehouses);
    case BuildingFort:
        return ScenarioAllows(c, ScenarioAllowedBuildingInfantryFort) ||
               ScenarioAllows(c, ScenarioAllowedBuildingArchersFort) ||
               ScenarioAllows(c, ScenarioAllowedBuildingCharioteersFort);
    case BuildingMilitaryAcademy:
        return ScenarioAllows(c, ScenarioAllowedBuildingMilitaryAcademy);
    case BuildingFortCharioteers:
        return ScenarioAllows(c, ScenarioAllowedBuildingCharioteersFort);
    case BuildingFortArchers:
        return ScenarioAllows(c, ScenarioAllowedBuildingArchersFort);
    case BuildingFortInfantry:
        return ScenarioAllows(c, ScenarioAllowedBuildingInfantryFort);
    case BuildingRecruiter:
        return ScenarioAllows(c, ScenarioAllowedBuildingRecruiter);
    case BuildingBazaarUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingBazaar);
    case BuildingGranaryUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingGranary);
    case BuildingStorageYardUp:
        return ScenarioAllows(c, ScenarioAllowedBuildingStorageYard);
    case BuildingMenuDefenses:
        return ScenarioAllows(c, ScenarioAllowedBuildingWalls) ||
               ScenarioAllows(c, ScenarioAllowedBuildingTowers) ||
               ScenarioAllows(c, ScenarioAllowedBuildingGatehouses);
    case BuildingLettuceFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceLettuce) != 0;
    case BuildingChickpeasFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceChickpeas) != 0;
    case BuildingPomegranatesFarm:
        return pharaoh_empire_can_produce_resource(c->supply, ResourcePomegranates) != 0;
    case BuildingWeaponsmith:
        return ScenarioAllows(c, ScenarioAllowedBuildingWeaponsmith);
    case BuildingGemstoneMine:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceGems) != 0;
    case BuildingCopperMine:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceCopper) != 0;
    case BuildingWoodCutter:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceTimber) != 0;
    case BuildingStoneQuarry:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceStone) != 0;
    case BuildingSandstoneQuarry:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceSandstone) != 0;
    case BuildingLimestoneQuarry:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceLimestone) != 0;
    case BuildingGraniteQuarry:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceGranite) != 0;
    case BuildingStonemasonsGuild:
        return pharaoh_empire_can_produce_resource(c->supply, ResourceStone) != 0;
    case BuildingChariotMaker:
        return ScenarioAllows(c, ScenarioAllowedBuildingChariotMaker);
    case BuildingBrickworks:
        return BuildingMenuBrickworksAvailable(c);
    case BuildingTempleGod5:
    case BuildingTempleGod6:
    case BuildingTempleGod7:
    case BuildingTempleGod8:
    case BuildingTempleGod9:
    case BuildingTempleComplexGod5:
    case BuildingTempleComplexGod6:
    case BuildingTempleComplexGod7:
    case BuildingTempleComplexGod8:
    case BuildingTempleComplexGod9:
    case BuildingShrineGod5:
    case BuildingShrineGod6:
    case BuildingShrineGod7:
    case BuildingShrineGod8:
    case BuildingShrineGod9:
        return false;
    case BuildingTempleComplexAltar:
    case BuildingTempleComplexOracle:
        return true;
    case BuildingJeweler:
        return pharaoh_empire_can_produce_resource_potentially(c->supply, ResourceGems) != 0 &&
               (c->settings->mission_id != 5 || *c->campaign_mode != 0);
    default:
        return current;
    }
}

static bool TutorialOverridesBuilding(PharaohBuildMenuContext *c, int32_t type, bool available)
{
    int32_t tutorialIndex;

    if (c->settings->mission_id > TutorialLastMission || *c->campaign_mode != 0) {
        return available;
    }

    for (tutorialIndex = 0; tutorialIndex < TutorialCount; ++tutorialIndex) {
        const PharaohTutorialRule *entry = &c->tutorial_rules[tutorialIndex];
        int32_t buildingIndex;

        if (entry->mission_threshold != c->settings->mission_id || entry->building_type_count <= 0) {
            continue;
        }
        for (buildingIndex = 0; buildingIndex < entry->building_type_count; ++buildingIndex) {
            if (entry->building_types[buildingIndex] == type) {
                available = c->tutorial_flags[tutorialIndex] != 0;
            }
        }
    }
    return available;
}

void pharaoh_build_menu_update(PharaohBuildMenuContext *c)
{
    int32_t group;

    for (group = 0; group < BuildMenuGroups; ++group) {
        int32_t row;

        for (row = 0; row < BuildMenuItems; ++row) {
            const int32_t index = row + group * BuildMenuItems;
            const int32_t type = (int32_t)c->types[index];
            bool available;

            if (*c->cheat_allow_all == 1 || *c->editor_active == 1) {
                c->enabled[index] = 1;
                continue;
            }

            available = BuildingMenuBasicAvailability(c, type);
            if (BuildingMenuAggregateAvailability(c, type, (uint8_t)tutorial_ok(c) != 0)) {
                available = true;
            }
            available = BuildingMenuSpecificAvailability(c, type, available);
            if (*c->campaign_mode == 0 && c->settings->mission_id == 0 && type == BuildingPoliceStation) {
                available = false;
            }
            available = TutorialOverridesBuilding(c, type, available);
            c->enabled[index] = available ? 1 : 0;
        }
    }
}

/* Native provenance: Pharaoh.exe VA 00404350. */

/* Native00404120, reused complete Unit00037 C with typed owners. */
int32_t pharaoh_build_menu_count_items(PharaohBuildMenuContext *c)
{
    const int32_t category = *c->selected_category;
    const PharaohScenarioPlayerRuntime *p =
        &c->supply->players[*c->supply->current_player];
    int32_t count = 0;
    for (int32_t row = 0; row < PHARAOH_BUILD_MENU_ITEMS; ++row) {
        const int32_t index = category * PHARAOH_BUILD_MENU_ITEMS + row;
        const int32_t kind = c->types[index];
        uint8_t enabled = 0;
        if (kind <= 0) return count;
        switch (category) {
        case 6:
            if (kind == 0x30) {
                if (tutorial_ok(c)) {
                    const int32_t have = pharaoh_native_three_nonzero_count_004f5020(
                        p->monument_definition_ids);
                    if (have < pharaoh_scenario_goal_text_count(c->goal_text_ids))
                        enabled = 1;
                }
            } else if (kind == 0x61 && p->temple_complex_enabled != 0) {
                const PharaohBuildingTypeRecord *buildings = c->monuments->demand->complex->buildings;
                if (!pharaoh_temple_upgrade_present(buildings,
                        p->temple_complex_building_id, 1)) enabled = 1;
                else enabled = !pharaoh_temple_upgrade_present(buildings,
                    p->temple_complex_building_id, 2);
            } else enabled = c->enabled[index] != 0;
            break;
        case 10:
            if (kind == 4) {
                if (tutorial_ok(c))
                    enabled = pharaoh_monument_guild_is_required(c->monuments, 0xb3) ||
                        pharaoh_monument_guild_is_required(c->monuments, 0xb2) ||
                        pharaoh_monument_guild_is_required(c->monuments, 0xb1);
            } else enabled = c->enabled[index] != 0;
            break;
        case 0x16:
            if (tutorial_ok(c))
                enabled = pharaoh_monument_guild_is_required(c->monuments, kind) != 0;
            break;
        case 0x1c:
            if (tutorial_ok(c))
                enabled = c->goal_text_ids[kind - 1] != 0 &&
                    p->monument_definition_ids[kind - 1] == 0;
            break;
        case 0x24:
            enabled = !pharaoh_temple_upgrade_present(c->monuments->demand->complex->buildings,
                p->temple_complex_building_id, (uint32_t)row + 1u);
            break;
        default:
            enabled = c->enabled[index] != 0;
            break;
        }
        if (enabled) ++count;
    }
    return count;
}

/* Native004051A0 writes bytes, leaving all adjacent table metadata intact. */
void pharaoh_sidebar_update_build_buttons(PharaohBuildMenuContext *c)
{
    for (int32_t index = 0; index < 12; ++index) {
        for (int32_t table = 0; table < 2; ++table) {
            PharaohSidebarBuildButton *b = &c->sidebar_buttons[table][index];
            b->enabled = 1;
            *c->selected_category = b->category;
            if (pharaoh_build_menu_count_items(c) < 1) b->enabled = 0;
        }
    }
}
