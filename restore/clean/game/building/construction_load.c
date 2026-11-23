#include "pharaoh/building/construction_load.h"
#include "pharaoh/building/venue_images.h"
#include "pharaoh/commerce/storage_queries.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/map/construction_region_images.h"

/* Complete owner of native00470990, reused from ConstructionLoad56.c.
 * Building byte/word aliases below retain their native overlay locations. */
_Static_assert(offsetof(PharaohBuildingTypeRecord,runtime_value_038)==0x38 &&
    offsetof(PharaohBuildingTypeRecord,storage_link_amount)==0x3c &&
    offsetof(PharaohBuildingTypeRecord,render_warning_087)==0x87 &&
    offsetof(PharaohBuildingTypeRecord,reserved_aa)==0xaa &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_ca)==0xca,
    "native construction record aliases");

static void *support_data(const PharaohConstructionLoadOperations *ops)
{return ops->support_user_data != NULL ? ops->support_user_data : ops->user_data;}

static int32_t construction_signed(uint32_t value)
{return value<=INT32_MAX?(int32_t)value:(int32_t)((int64_t)value-INT64_C(4294967296));}
static int32_t construction_add(int32_t value,int32_t delta)
{return construction_signed((uint32_t)value+(uint32_t)delta);}
static PharaohBuildingTypeRecord *construction_building(PharaohConstructionLoadContext *c,int32_t id)
{return &c->surface->buildings[id];}
static uint32_t construction_image(PharaohConstructionLoadContext *c,int32_t group)
{return c->surface->groups[group].first_image;}

enum ConstructionNativeKind {
    BuildingAbuSimbel = 230,
    BuildingAlexandriaLibrary = 225,
    BuildingApothecary = 46,
    BuildingArchitectPost = 81,
    BuildingArtisansGuild = 231,
    BuildingBandstand = 30,
    BuildingBarleyFarm = 100,
    BuildingBazaar = 70,
    BuildingBazaarUp = 190,
    BuildingBooth = 31,
    BuildingBrewery = 110,
    BuildingBricklayersGuild = 178,
    BuildingBrickworks = 204,
    BuildingBullfightSchool = 37,
    BuildingBurningRuin = 99,
    BuildingCaesareum = 227,
    BuildingCarpentersGuild = 177,
    BuildingCattleRanch = 194,
    BuildingCauseway = 219,
    BuildingChariotMaker = 205,
    BuildingChickpeasFarm = 105,
    BuildingCityPalace = 189,
    BuildingClayPit = 109,
    BuildingConservatory = 34,
    BuildingCopperMine = 217,
    BuildingCourthouse = 184,
    BuildingDanceSchool = 35,
    BuildingDentist = 49,
    BuildingDistributionCenter = 50,
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
    BuildingFortGround = 54,
    BuildingFortInfantry = 45,
    BuildingGatehouse = 171,
    BuildingGatehouseUp = 202,
    BuildingGemstoneMine = 162,
    BuildingGoldMine = 161,
    BuildingGrainFarm = 102,
    BuildingGranary = 71,
    BuildingGranaryUp = 191,
    BuildingGrandRoyalTomb = 236,
    BuildingGraniteQuarry = 216,
    BuildingHennaFarm = 224,
    BuildingHouseCommonManor = 24,
    BuildingHouseCommonResidence = 20,
    BuildingHouseCommonShanty = 13,
    BuildingHouseCrudeHut = 10,
    BuildingHouseElegantManor = 26,
    BuildingHouseElegantResidence = 22,
    BuildingHouseFancyResidence = 23,
    BuildingHouseMeagerShanty = 12,
    BuildingHouseModestApartment = 18,
    BuildingHouseModestEstate = 28,
    BuildingHouseModestHomestead = 16,
    BuildingHouseOrdinaryCottage = 15,
    BuildingHousePalatialEstate = 29,
    BuildingHouseRoughCottage = 14,
    BuildingHouseSpaciousApartment = 19,
    BuildingHouseSpaciousHomestead = 17,
    BuildingHouseSpaciousManor = 25,
    BuildingHouseSpaciousResidence = 21,
    BuildingHouseStatelyManor = 27,
    BuildingHouseSturdyHut = 11,
    BuildingHuntingLodge = 115,
    BuildingJeweler = 113,
    BuildingJugglerSchool = 36,
    BuildingLampMaker = 232,
    BuildingLargeObelisk = 208,
    BuildingLargeRoyalTomb = 235,
    BuildingLargeStatue = 43,
    BuildingLettuceFarm = 103,
    BuildingLibrary = 53,
    BuildingLighthouse = 228,
    BuildingLimestoneQuarry = 107,
    BuildingMausoleum = 222,
    BuildingMediumRoyalTomb = 234,
    BuildingMediumStatue = 42,
    BuildingMilitaryAcademy = 94,
    BuildingMilitaryAcademy2 = 185,
    BuildingMilitaryAcademy3 = 186,
    BuildingMissionPost = 80,
    BuildingMortuary = 47,
    BuildingMortuaryTemple = 218,
    BuildingNativeCrops = 93,
    BuildingNativeHut = 88,
    BuildingNativeMeeting = 89,
    BuildingPaintMaker = 233,
    BuildingPapyrusMaker = 203,
    BuildingPavilion = 33,
    BuildingPersonalMansion = 77,
    BuildingPhysician = 206,
    BuildingPoliceStation = 55,
    BuildingPomegranatesFarm = 104,
    BuildingPotter = 114,
    BuildingPyramid = 183,
    BuildingPyramidEntryTile = 215,
    BuildingRecruiter = 95,
    BuildingReedGatherer = 195,
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
    BuildingSmallObelisk = 207,
    BuildingSmallRoyalTomb = 229,
    BuildingSmallStatue = 41,
    BuildingSphinx = 210,
    BuildingStateRubble = 4,
    BuildingStoneQuarry = 106,
    BuildingStonemasonsGuild = 179,
    BuildingStorageRoom = 73,
    BuildingStorageYard = 72,
    BuildingStorageYardUp = 193,
    BuildingSunTemple = 213,
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
    BuildingTownPalace = 85,
    BuildingTownPalaceUp = 188,
    BuildingTransportWharf = 181,
    BuildingValleyTemple = 220,
    BuildingVillagePalace = 84,
    BuildingVillagePalaceUp = 187,
    BuildingWarshipWharf = 182,
    BuildingWaterLift = 90,
    BuildingWaterSupply = 180,
    BuildingWeaponsmith = 112,
    BuildingWeaver = 111,
    BuildingWell = 92,
    BuildingWoodCutter = 108,
    BuildingWorkCamp = 199,
    BuildingZoo = 226,
    ImageGroupApothecary = 337,
    ImageGroupArchitectPost = 350,
    ImageGroupArtisansGuild = 741,
    ImageGroupBazaar = 291,
    ImageGroupBooth = 383,
    ImageGroupBrewery = 385,
    ImageGroupBricklayersGuild = 326,
    ImageGroupBrickworks = 393,
    ImageGroupCarpentersGuild = 360,
    ImageGroupCauseway = 761,
    ImageGroupChariotMaker = 389,
    ImageGroupCityPalace = 287,
    ImageGroupClayPit = 309,
    ImageGroupConservatory = 320,
    ImageGroupCopperMine = 465,
    ImageGroupCourthouse = 331,
    ImageGroupDanceSchool = 321,
    ImageGroupDentist = 336,
    ImageGroupDock = 546,
    ImageGroupDockBlood = 729,
    ImageGroupDynastyMansion = 356,
    ImageGroupFamilyMansion = 355,
    ImageGroupFerryLanding = 520,
    ImageGroupFerryLandingBlood = 728,
    ImageGroupFirehouse = 347,
    ImageGroupFishingWharf = 515,
    ImageGroupFishingWharfBlood = 727,
    ImageGroupFort = 335,
    ImageGroupGemstoneMine = 457,
    ImageGroupGoldMine = 454,
    ImageGroupGrainCrops = 325,
    ImageGroupGranary = 368,
    ImageGroupHuntingLodge = 445,
    ImageGroupJeweler = 388,
    ImageGroupJugglerSchool = 315,
    ImageGroupLampMaker = 736,
    ImageGroupLargeStatueVariants = 745,
    ImageGroupLibrary = 312,
    ImageGroupMediumStatueVariants = 746,
    ImageGroupMilitaryAcademy = 442,
    ImageGroupMissionPost = 453,
    ImageGroupMortuary = 444,
    ImageGroupMortuaryTemple = 760,
    ImageGroupNativeHut = 452,
    ImageGroupPaintMaker = 737,
    ImageGroupPapyrusMaker = 313,
    ImageGroupPavilion = 317,
    ImageGroupPersonalMansion = 354,
    ImageGroupPhysician = 339,
    ImageGroupPoliceStation = 333,
    ImageGroupPotter = 394,
    ImageGroupPyramidEntryTile = 497,
    ImageGroupRecruiter = 435,
    ImageGroupRoadblock = 367,
    ImageGroupRuinSmallHouse = 532,
    ImageGroupRuins = 533,
    ImageGroupSandstoneQuarry = 466,
    ImageGroupScribalSchool = 311,
    ImageGroupSenetHouse = 286,
    ImageGroupShipwright = 523,
    ImageGroupShipwrightBlood = 723,
    ImageGroupSmallStatueVariants = 747,
    ImageGroupSphinxEndA = 564,
    ImageGroupSphinxEndB = 566,
    ImageGroupSphinxMiddle = 565,
    ImageGroupStoneQuarry = 456,
    ImageGroupStonemasonsGuild = 357,
    ImageGroupStorageYard = 351,
    ImageGroupStorageYardGoods = 353,
    ImageGroupStorageYardGoodsLate = 714,
    ImageGroupStorageYardSpace = 352,
    ImageGroupTaxCollector = 332,
    ImageGroupTempleBast = 345,
    ImageGroupTempleOsiris = 294,
    ImageGroupTemplePtah = 289,
    ImageGroupTempleRa = 288,
    ImageGroupTempleSeth = 290,
    ImageGroupTower = 404,
    ImageGroupTownPalace = 308,
    ImageGroupTransportWharf = 514,
    ImageGroupTransportWharfBlood = 725,
    ImageGroupValleyTemple = 762,
    ImageGroupVillagePalace = 316,
    ImageGroupWarshipWharf = 525,
    ImageGroupWarshipWharfBlood = 724,
    ImageGroupWaterSupply = 338,
    ImageGroupWeaponsmith = 392,
    ImageGroupWeaver = 391,
    ImageGroupWell = 292,
    ImageGroupWoodCutter = 334,
    ImageGroupWorkCamp = 346,
    ImageGroupZoo = 716,
    ResourceGranite = 26,
    ResourceLimestone = 25,
    TerrainBuilding = 8,
    TerrainCanal = 256,
    TerrainGatehouse = 32768,
    TerrainRoad = 64,
    TerrainWallKinds = 12599296,
    WalkerArcher = 11,
    WalkerCharioteer = 13,
    WalkerInfantry = 12,
};

enum ConstructionRule {
    ConstructionGridStride = 228,
    ConstructionStatueFacingMask = 15,
    ConstructionStatueVariantShift = 4,
    ConstructionStatueDirections = 4,
    ConstructionStockLateFirst = 31,
    ConstructionStockImagesPerResource = 4,
    ConstructionYardCells = 9,
    ConstructionTempleScriptSlot = 10,
    ConstructionRuinRandomMask = 3,
    ConstructionRuinImages = 9,
    ConstructionTowerWallRadius = 10,
    ConstructionNativeHutVariantMask = 1,
    ConstructionHalfTurnVariantMask = 1
};
static void construction_publish(PharaohConstructionLoadContext *c,int32_t id,int32_t x,int32_t y,int32_t size,uint32_t image,uint32_t terrain)
{pharaoh_map_place_surface_square(c->surface,(uint16_t)id,x,y,size,size,image,terrain);}
static void construction_clear_selections(PharaohConstructionLoadContext *c)
{
    c->monument_selections[5]=0;
    c->monument_selections[4]=0;
    c->monument_selections[2]=0;
    c->monument_selections[3]=0;
    c->monument_selections[0]=0;
    c->monument_selections[1]=0;
}
static uint32_t construction_grid_offset(PharaohConstructionLoadContext *c,int32_t x,int32_t y)
{return (uint32_t)c->surface->map->origin+(uint32_t)y*ConstructionGridStride+(uint32_t)x;}
/* Native simple switch arms contain a live image-group load and publication. */
static int construction_base_image(PharaohConstructionLoadContext *c,int32_t kind,uint32_t *image)
{
    switch(kind) {
    case BuildingNativeCrops:
        *image=construction_image(c,ImageGroupGrainCrops);return 1;
    case BuildingPaintMaker:
        *image=construction_image(c,ImageGroupPaintMaker);return 1;
    case BuildingSenetHouse:
    case BuildingBullfightSchool:
        *image=construction_image(c,ImageGroupSenetHouse);return 1;
    case BuildingConservatory:
        *image=construction_image(c,ImageGroupConservatory);return 1;
    case BuildingDanceSchool:
        *image=construction_image(c,ImageGroupDanceSchool);return 1;
    case BuildingJugglerSchool:
        *image=construction_image(c,ImageGroupJugglerSchool);return 1;
    case BuildingApothecary:
        *image=construction_image(c,ImageGroupApothecary);return 1;
    case BuildingMortuary:
        *image=construction_image(c,ImageGroupMortuary);return 1;
    case BuildingDentist:
        *image=construction_image(c,ImageGroupDentist);return 1;
    case BuildingScribalSchool:
        *image=construction_image(c,ImageGroupScribalSchool);return 1;
    case BuildingLibrary:
        *image=construction_image(c,ImageGroupLibrary);return 1;
    case BuildingPoliceStation:
        *image=construction_image(c,ImageGroupPoliceStation);return 1;
    case BuildingTempleOsiris:
    case BuildingTempleGod5:
    case BuildingTempleGod6:
    case BuildingTempleGod7:
    case BuildingTempleGod8:
    case BuildingTempleGod9:
        *image=construction_image(c,ImageGroupTempleOsiris);return 1;
    case BuildingTempleRa:
        *image=construction_image(c,ImageGroupTempleRa);return 1;
    case BuildingTemplePtah:
        *image=construction_image(c,ImageGroupTemplePtah);return 1;
    case BuildingTempleSeth:
        *image=construction_image(c,ImageGroupTempleSeth);return 1;
    case BuildingTempleBast:
        *image=construction_image(c,ImageGroupTempleBast);return 1;
    case BuildingMissionPost:
        *image=construction_image(c,ImageGroupMissionPost);return 1;
    case BuildingArchitectPost:
        *image=construction_image(c,ImageGroupArchitectPost);return 1;
    case BuildingTaxCollector:
        *image=construction_image(c,ImageGroupTaxCollector);return 1;
    case BuildingWell:
        *image=construction_image(c,ImageGroupWell);return 1;
    case BuildingMilitaryAcademy2:
    case BuildingMilitaryAcademy3:
        *image=construction_image(c,ImageGroupMilitaryAcademy);return 1;
    case BuildingStoneQuarry:
        *image=construction_image(c,ImageGroupStoneQuarry);return 1;
    case BuildingWoodCutter:
        *image=construction_image(c,ImageGroupWoodCutter);return 1;
    case BuildingClayPit:
        *image=construction_image(c,ImageGroupClayPit);return 1;
    case BuildingBrewery:
        *image=construction_image(c,ImageGroupBrewery);return 1;
    case BuildingWeaver:
        *image=construction_image(c,ImageGroupWeaver);return 1;
    case BuildingWeaponsmith:
        *image=construction_image(c,ImageGroupWeaponsmith);return 1;
    case BuildingJeweler:
        *image=construction_image(c,ImageGroupJeweler);return 1;
    case BuildingPotter:
        *image=construction_image(c,ImageGroupPotter);return 1;
    case BuildingHuntingLodge:
        *image=construction_image(c,ImageGroupHuntingLodge);return 1;
    case BuildingGoldMine:
        *image=construction_image(c,ImageGroupGoldMine);return 1;
    case BuildingGemstoneMine:
        *image=construction_image(c,ImageGroupGemstoneMine);return 1;
    case BuildingFirehouse:
        *image=construction_image(c,ImageGroupFirehouse);return 1;
    case BuildingCarpentersGuild:
        *image=construction_image(c,ImageGroupCarpentersGuild);return 1;
    case BuildingBricklayersGuild:
        *image=construction_image(c,ImageGroupBricklayersGuild);return 1;
    case BuildingStonemasonsGuild:
        *image=construction_image(c,ImageGroupStonemasonsGuild);return 1;
    case BuildingWaterSupply:
        *image=construction_image(c,ImageGroupWaterSupply);return 1;
    case BuildingCourthouse:
        *image=construction_image(c,ImageGroupCourthouse);return 1;
    case BuildingWorkCamp:
        *image=construction_image(c,ImageGroupWorkCamp);return 1;
    case BuildingPapyrusMaker:
        *image=construction_image(c,ImageGroupPapyrusMaker);return 1;
    case BuildingBrickworks:
        *image=construction_image(c,ImageGroupBrickworks);return 1;
    case BuildingChariotMaker:
        *image=construction_image(c,ImageGroupChariotMaker);return 1;
    case BuildingPhysician:
        *image=construction_image(c,ImageGroupPhysician);return 1;
    case BuildingCopperMine:
        *image=construction_image(c,ImageGroupCopperMine);return 1;
    case BuildingSandstoneQuarry:
        *image=construction_image(c,ImageGroupSandstoneQuarry);return 1;
    case BuildingZoo:
        *image=construction_image(c,ImageGroupZoo);return 1;
    case BuildingArtisansGuild:
        *image=construction_image(c,ImageGroupArtisansGuild);return 1;
    case BuildingLampMaker:
        *image=construction_image(c,ImageGroupLampMaker);return 1;
    default:return 0;
    }
}
static void construction_place_fort(PharaohConstructionLoadContext *c,uint32_t owner,int32_t kind,int32_t id,int32_t x,int32_t y,
    int32_t size,uint32_t creating)
{
    construction_publish(c,id,x,y,size,construction_image(c,ImageGroupFort),TerrainBuilding);
    int32_t ground;
    if (!creating) ground=construction_building(c,id)->next_part_building_id;
    else {
        int32_t formation_id=c->operations->legion_create(c->operations->user_data,id);
        PharaohBuildingTypeRecord *building=construction_building(c,id);
        building->runtime_value_038=0;
        building->formation_id=(int16_t)formation_id;
        int32_t walkerType,savedIndex;
        if (kind==BuildingFortCharioteers) {walkerType=WalkerCharioteer;savedIndex=2;}
        else if (kind==BuildingFortArchers) {walkerType=WalkerArcher;savedIndex=1;}
        else {walkerType=WalkerInfantry;savedIndex=0;}
        building->subtype=(int16_t)walkerType;
        PharaohConstructionFormation *formation=&c->formations[formation_id];
        volatile PharaohConstructionSavedLegion *saved=&c->saved_legions[savedIndex];
        uint8_t savedTroops=saved->troop_count.bytes.low;
        formation->walker_type=(int16_t)walkerType;
        if (savedTroops) {
            formation->legion_id=saved->legion_id.bytes.low;
            c->operations->legion_add_soldiers(c->operations->user_data,(int32_t)(saved->troop_count.value&UINT8_MAX),formation_id,
                construction_add(x,4),construction_add(y,1));
            formation->experience=(int16_t)saved->experience.words.low;
            saved->experience.value=UINT32_MAX;
            saved->troop_count.bytes.low=0;
            saved->legion_id.bytes.low=UINT8_MAX;
        }
        ground=pharaoh_building_allocate(c->allocation,owner,BuildingFortGround,
            construction_add(x,3),construction_add(y,-1),0,0);
        pharaoh_map_building_undo_capture(c->surface->undo,ground);
        PharaohBuildingTypeRecord *created=construction_building(c,ground);
        created->formation_id=(int16_t)formation_id;
        created->runtime_value_038=(int16_t)id;
        building->next_part_building_id=(int16_t)ground;
        created->next_part_building_id=0;
    }
    construction_publish(c,ground,construction_add(x,3),construction_add(y,-1),4,
        construction_image(c,ImageGroupFort)+1,TerrainBuilding);
}
static void construction_place_storage_yard(PharaohConstructionLoadContext *c,uint32_t owner,int32_t id,int32_t x,int32_t y,
    int32_t size,uint32_t creating)
{
    typedef struct {int32_t dx,dy,image_group;} YardCell;
    /* Native471E96/9A initialize the first two zero coordinates, which the
     * decompiler split from its remaining25-DWORD local array. */
    static const YardCell cells[ConstructionYardCells]={
        {0,0,ImageGroupStorageYard},{1,0,ImageGroupStorageYardSpace},
        {2,0,ImageGroupStorageYardSpace},{0,1,ImageGroupStorageYardSpace},
        {1,1,ImageGroupStorageYardSpace},{2,1,ImageGroupStorageYardSpace},
        {0,2,ImageGroupStorageYardSpace},{1,2,ImageGroupStorageYardSpace},
        {2,2,ImageGroupStorageYardSpace}
    };
    int32_t previous=id;
    if (creating) {
        construction_building(c,id)->storage_id=(uint8_t)c->operations->storage_allocate(support_data(c->operations),id);
        construction_building(c,id)->runtime_value_038=0;
    }
    for (int32_t index=0;index<ConstructionYardCells;++index) {
        uint32_t image=construction_image(c,cells[index].image_group);
        if (index>0 && !creating) {
            int32_t resource=construction_building(c,id)->subtype;
            if (resource) {
                if (resource<ConstructionStockLateFirst)
                    image=construction_image(c,ImageGroupStorageYardGoods)+(uint32_t)resource*4u-4u;
                else image=construction_image(c,ImageGroupStorageYardGoodsLate)+
                    (uint32_t)resource*4u-ConstructionStockLateFirst*4u;
                image+=pharaoh_native_return_zero_0047d350();
                image+=(uint32_t)pharaoh_storage_image_level(construction_building(c,id)->storage_link_amount);
            }
        }
        construction_publish(c,id,construction_add(x,cells[index].dx),construction_add(y,cells[index].dy),
            size,image,TerrainBuilding);
        if (index<ConstructionYardCells-1) {
            if (!creating) id=construction_building(c,id)->next_part_building_id;
            else {
                id=pharaoh_building_allocate(c->allocation,owner,BuildingStorageRoom,
                    construction_add(x,cells[index+1].dx),construction_add(y,cells[index+1].dy),0,0);
                pharaoh_map_building_undo_capture(c->surface->undo,id);
                construction_building(c,id)->runtime_value_038=(int16_t)previous;
                construction_building(c,previous)->next_part_building_id=(int16_t)id;
            }
            previous=id;
        }
    }
}
static uint32_t construction_statue_image(PharaohConstructionLoadContext *c,int32_t kind,int32_t id)
{
    const PharaohBuildingTypeRecord *building=construction_building(c,id);
    int32_t facing=(int32_t)(building->reserved_aa[0]&ConstructionStatueFacingMask)-(*c->surface->orientation)/2;
    uint32_t variant=building->reserved_aa[0]>>ConstructionStatueVariantShift;
    --facing;if (facing<0) facing+=ConstructionStatueDirections;
    uint32_t image;
    if (variant<2) image=construction_image(c,c->properties[kind].image_group);
    else {
        int32_t group=kind==BuildingSmallStatue?ImageGroupSmallStatueVariants:
            kind==BuildingMediumStatue?ImageGroupMediumStatueVariants:ImageGroupLargeStatueVariants;
        image=construction_image(c,group)-8u;
    }
    return image+variant*ConstructionStatueDirections+(uint32_t)facing;
}

static void construction_place_sphinx(PharaohConstructionLoadContext *c,uint32_t owner,int32_t id,int32_t x,int32_t y,int32_t size,uint32_t creating)
{
    if (creating) {
        c->cities[(*c->current_player)].sphinx_stage=1;
        pharaoh_sphinx_reset_work_spots(c->animations->sphinx,c->animation_scripts,c->animation_script_count,c->random);
        construction_building(c,id)->subtype=((*c->surface->orientation)==0||(*c->surface->orientation)==4)?0:3;
        construction_building(c,id)->runtime_value_038=0;
        int32_t middle=pharaoh_building_allocate(c->allocation,owner,BuildingSphinx,construction_add(x,6),y,0,0);
        pharaoh_map_building_undo_capture(c->surface->undo,middle);
        construction_building(c,middle)->subtype=((*c->surface->orientation)==0||(*c->surface->orientation)==4)?1:4;
        construction_building(c,middle)->runtime_value_038=(int16_t)id;
        construction_building(c,id)->next_part_building_id=(int16_t)middle;
        construction_building(c,middle)->next_part_building_id=0;
        int32_t end=pharaoh_building_allocate(c->allocation,owner,BuildingSphinx,construction_add(x,12),y,0,0);
        pharaoh_map_building_undo_capture(c->surface->undo,end);
        construction_building(c,end)->subtype=((*c->surface->orientation)==0||(*c->surface->orientation)==4)?2:5;
        construction_building(c,end)->runtime_value_038=(int16_t)middle;
        construction_building(c,middle)->next_part_building_id=(int16_t)end;
        construction_building(c,end)->next_part_building_id=0;
        c->operations->sphinx_create(support_data(c->operations),id);
    }
    if (pharaoh_building_follow_link(c->surface->buildings,id)==id) {
        uint32_t image;
        if ((*c->surface->orientation)==0) image=construction_image(c,ImageGroupSphinxEndA)+1;
        else if ((*c->surface->orientation)==4) image=construction_image(c,ImageGroupSphinxEndB)+1;
        else {
            image=construction_image(c,ImageGroupSphinxEndB);
            if ((*c->surface->orientation)!=2) image=construction_image(c,ImageGroupSphinxEndA);
        }
        construction_publish(c,id,x,y,size,image,TerrainBuilding);
        int32_t middle=construction_building(c,id)->next_part_building_id;
        if ((*c->surface->orientation)==0) image=construction_image(c,ImageGroupSphinxMiddle)+1;
        else if ((*c->surface->orientation)==4) image=construction_image(c,ImageGroupSphinxMiddle)+1;
        else if ((*c->surface->orientation)==2||(*c->surface->orientation)==6) image=construction_image(c,ImageGroupSphinxMiddle);
        construction_publish(c,middle,construction_add(x,6),y,size,image,TerrainBuilding);
        uint32_t endImage;
        if ((*c->surface->orientation)==0) endImage=construction_image(c,ImageGroupSphinxEndB)+1;
        else if ((*c->surface->orientation)==4) endImage=construction_image(c,ImageGroupSphinxEndA)+1;
        else {
            endImage=construction_image(c,ImageGroupSphinxEndA);
            if ((*c->surface->orientation)!=2) {
                endImage=image;
                if ((*c->surface->orientation)==6) endImage=construction_image(c,ImageGroupSphinxEndB);
            }
        }
        construction_publish(c,construction_building(c,middle)->next_part_building_id,construction_add(x,12),y,size,endImage,TerrainBuilding);
    }
    pharaoh_sphinx_load_stage_libraries(c->libraries,c->cities[*c->current_player].sphinx_stage,c->sphinx_stage_names,c->sphinx_fixed_library);
}
static void construction_store_palace(PharaohConstructionLoadContext *c,int32_t id,int32_t x,int32_t y)
{
    PharaohConstructionCity *city=&c->cities[(*c->current_player)];
    if (city->palace_grid_offset) return;
    city->palace_building_id=id;
    city->palace_x=(int16_t)x;
    city->palace_y=(int16_t)y;
    city->palace_grid_offset=construction_signed(construction_grid_offset(c,x,y));
}
static void construction_publish_waterfront(PharaohConstructionLoadContext *c,int32_t kind,int32_t id,int32_t x,int32_t y)
{
    if (kind==BuildingFishingWharf||kind==BuildingTransportWharf||kind==BuildingFerryLanding)
        c->operations->water_determine_orientation_size2(c->operations->user_data,x,y,0,0);
    else pharaoh_map_water_determine_orientation_size3(c->water_orientation_map,x,y,0);
    construction_building(c,id)->reserved_aa[0]=(uint8_t)*c->water_orientation;
    uint32_t blood=(uint8_t)pharaoh_map_sweep_has_passed(*c->surface->sweep_countdown,*c->surface->sweep_start,x),image;
    switch (kind) {
    case BuildingShipwright:image=construction_image(c,blood?ImageGroupShipwrightBlood:ImageGroupShipwright);break;
    case BuildingDock:case BuildingDockUp:image=construction_image(c,blood?ImageGroupDockBlood:ImageGroupDock);break;
    case BuildingFishingWharf:image=construction_image(c,blood?ImageGroupFishingWharfBlood:ImageGroupFishingWharf);break;
    case BuildingTransportWharf:image=construction_image(c,blood?ImageGroupTransportWharfBlood:ImageGroupTransportWharf);break;
    case BuildingWarshipWharf:image=construction_image(c,blood?ImageGroupWarshipWharfBlood:ImageGroupWarshipWharf);break;
    default:image=construction_image(c,blood?ImageGroupFerryLandingBlood:ImageGroupFerryLanding);break;
    }
    image+=(*c->water_image_offset);
    if (kind==BuildingFishingWharf||kind==BuildingTransportWharf||kind==BuildingFerryLanding)
        pharaoh_map_building_tiles_add_water_size2(c->water_footprint,(int16_t)id,x,y,image);
    else pharaoh_map_building_tiles_add_water_size3(c->water_footprint,(int16_t)id,x,y,image);
}

void pharaoh_building_construction_add_to_map(PharaohConstructionLoadContext *c,uint32_t owner,int32_t kind,int32_t id,
    int32_t x,int32_t y,int32_t size,uint32_t creating,uint32_t mode)
{
    /* Native normalizes the generic fort from the record before dispatch. */
    if (kind==BuildingFort) {
        int16_t subtype=construction_building(c,id)->subtype;
        kind=subtype==WalkerCharioteer?BuildingFortCharioteers:
            subtype==WalkerArcher?BuildingFortArchers:BuildingFortInfantry;
    }
    uint32_t image;
    if (construction_base_image(c,kind,&image)) {construction_publish(c,id,x,y,size,image,TerrainBuilding);return;}
    switch(kind) {
    case BuildingHouseCrudeHut:case BuildingHouseSturdyHut:case BuildingHouseMeagerShanty:
    case BuildingHouseCommonShanty:case BuildingHouseRoughCottage:case BuildingHouseOrdinaryCottage:
    case BuildingHouseModestHomestead:case BuildingHouseSpaciousHomestead:case BuildingHouseModestApartment:
    case BuildingHouseSpaciousApartment:case BuildingHouseCommonResidence:case BuildingHouseSpaciousResidence:
    case BuildingHouseElegantResidence:case BuildingHouseFancyResidence:case BuildingHouseCommonManor:
    case BuildingHouseSpaciousManor:case BuildingHouseElegantManor:case BuildingHouseStatelyManor:
    case BuildingHouseModestEstate:case BuildingHousePalatialEstate:
        if (!construction_building(c,id)->render_warning_087&&!construction_building(c,id)->house_population)
            pharaoh_house_change_to_vacant_lots(c->allocation,c->building_tiles,id);
        else pharaoh_house_change_type_image(c->surface,c->house_images,id,(int16_t)kind);
        return;
    case BuildingBandstand:
        if (construction_building(c,construction_building(c,id)->next_part_building_id)->building_type==BuildingBandstand)
            (void)pharaoh_map_publish_venue_adjacent_images(c->surface,id,x,y);
        if (pharaoh_building_follow_link(c->surface->buildings,id)!=id||!construction_building(c,id)->venue_has_plaza) return;
        pharaoh_map_add_venue_plaza_tiles(c->venue_tiles,(uint32_t)construction_building(c,id)->venue_grid_offset,BuildingBandstand,(uint16_t)id);
        return;
    case BuildingBooth:case BuildingPavilion:
        construction_publish(c,id,x,y,kind==BuildingPavilion?2:size,
            construction_image(c,kind==BuildingPavilion?ImageGroupPavilion:ImageGroupBooth),TerrainBuilding);
        if (pharaoh_building_follow_link(c->surface->buildings,id)!=id||!construction_building(c,id)->venue_has_plaza) return;
        pharaoh_map_add_venue_plaza_tiles(c->venue_tiles,(uint32_t)construction_building(c,id)->venue_grid_offset,kind,(uint16_t)id);
        return;
    case BuildingFortCharioteers:case BuildingFortArchers:case BuildingFortInfantry:
        construction_place_fort(c,owner,kind,id,x,y,size,creating);return;
    case BuildingSmallStatue:case BuildingMediumStatue:case BuildingLargeStatue:
        construction_publish(c,id,x,y,size,construction_statue_image(c,kind,id),TerrainBuilding);return;
    case BuildingDistributionCenter: {
        c->cities[(*c->current_player)].distribution_center_placed=1;
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupRecruiter),TerrainBuilding);
        PharaohConstructionCity *city=&c->cities[(*c->current_player)];
        if (city->distribution_center_grid_offset) return;
        city->distribution_center_building_id=id;
        city->distribution_center_x=(int16_t)x;city->distribution_center_y=(int16_t)y;
        city->distribution_center_grid_offset=construction_signed(construction_grid_offset(c,x,y));return;
    }
    case BuildingTempleComplexOsiris:case BuildingTempleComplexRa:case BuildingTempleComplexPtah:
    case BuildingTempleComplexSeth:case BuildingTempleComplexBast:case BuildingTempleComplexGod5:
    case BuildingTempleComplexGod6:case BuildingTempleComplexGod7:case BuildingTempleComplexGod8:case BuildingTempleComplexGod9: {
        uint32_t god=pharaoh_building_group_index(c->surface->buildings,id);
        (void)pharaoh_sprite_library_load_file(c->libraries,c->temple_scripts[god],ConstructionTempleScriptSlot);
        c->operations->temple_complex_create(c->operations->user_data,id,x,y,owner,kind,creating);return;
    }
    case BuildingBazaar:case BuildingBazaarUp:case BuildingGranary:case BuildingGranaryUp:
        if (creating) {
            pharaoh_city_resource_determine_available(c->resources);
            construction_building(c,id)->storage_id=(uint8_t)c->operations->storage_allocate(support_data(c->operations),id);
        }
        image=construction_image(c,(kind==BuildingBazaar||kind==BuildingBazaarUp)?ImageGroupBazaar:ImageGroupGranary);
        construction_publish(c,id,x,y,size,image,TerrainBuilding);return;
    case BuildingStorageYard:case BuildingStorageYardUp:
        construction_place_storage_yard(c,owner,id,x,y,size,creating);return;
    case BuildingShipwright:case BuildingDock:case BuildingDockUp:case BuildingFishingWharf:
    case BuildingTransportWharf:case BuildingWarshipWharf:
        if (kind==BuildingDock||kind==BuildingDockUp) {
            pharaoh_map_water_determine_orientation_size3(c->water_orientation_map,x,y,0);
            if (creating) ++(*c->dock_count);
            construction_building(c,id)->reserved_aa[0]=(uint8_t)*c->water_orientation;
            uint32_t blood=(uint8_t)pharaoh_map_sweep_has_passed(*c->surface->sweep_countdown,*c->surface->sweep_start,x);
            image=construction_image(c,blood?ImageGroupDockBlood:ImageGroupDock)+(*c->water_image_offset);
            pharaoh_map_building_tiles_add_water_size3(c->water_footprint,(int16_t)id,x,y,image);
        } else construction_publish_waterfront(c,kind,id,x,y);
        if (kind==BuildingWarshipWharf&&creating) ++(*c->warship_wharf_count);
        return;
    case BuildingPersonalMansion:
        c->cities[(*c->current_player)].mansion_placed=1;
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupPersonalMansion),TerrainBuilding);return;
    case BuildingFamilyMansion:
        c->cities[(*c->current_player)].mansion_placed=1;
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupFamilyMansion),TerrainBuilding);return;
    case BuildingDynastyMansion:
        c->cities[(*c->current_player)].mansion_placed=1;
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupDynastyMansion),TerrainBuilding);return;
    case BuildingVillagePalace:case BuildingTownPalace:case BuildingVillagePalaceUp:
    case BuildingTownPalaceUp:case BuildingCityPalace:
        c->cities[(*c->current_player)].palace_placed=1;
        image=construction_image(c,kind==BuildingCityPalace?ImageGroupCityPalace:
            kind==BuildingTownPalaceUp?ImageGroupTownPalace:ImageGroupVillagePalace);
        construction_publish(c,id,x,y,size,image,TerrainBuilding);construction_store_palace(c,id,x,y);return;
    case BuildingNativeHut:
        image=construction_image(c,ImageGroupNativeHut);
        image+=pharaoh_crt_random_next(c->random)&ConstructionNativeHutVariantMask;
        construction_publish(c,id,x,y,size,image,TerrainBuilding);return;
    case BuildingNativeMeeting:
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupNativeHut)+2,TerrainBuilding);return;
    case BuildingWaterLift:
        if (creating) {
            c->operations->water_determine_orientation_size2(c->operations->user_data,x,y,0,1);
            construction_building(c,id)->reserved_aa[0]=(uint8_t)*c->water_orientation;
        }
        pharaoh_map_update_water_lift_image(c->surface,c->water_footprint,c->water_lift_images,id);return;
    case BuildingMilitaryAcademy:
        c->cities[(*c->current_player)].military_academy_placed=1;
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupMilitaryAcademy),TerrainBuilding);return;
    case BuildingRecruiter: {
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupRecruiter),TerrainBuilding);
        PharaohConstructionCity *city=&c->cities[(*c->current_player)];
        if (city->recruiter_grid_offset) return;
        city->recruiter_building_id=id;city->recruiter_x=(int16_t)x;city->recruiter_y=(int16_t)y;
        city->recruiter_grid_offset=construction_signed(construction_grid_offset(c,x,y));return;
    }
    case BuildingBurningRuin: {
        const PharaohBuildingTypeRecord *building=construction_building(c,id);
        if (building->state==BuildingStateRubble) {
            c->operations->clear_building_rectangle(c->operations->user_data,id,building->x,building->y,building->size,building->size);return;
        }
        uint32_t random=c->surface->random[(uint32_t)building->grid_offset]&ConstructionRuinRandomMask;
        if (building->house_size&&building->subtype<=1) image=construction_image(c,ImageGroupRuinSmallHouse);
        else image=construction_image(c,ImageGroupRuins)+random*ConstructionRuinImages;
        construction_publish(c,id,building->x,building->y,1,image,TerrainBuilding);return;
    }
    case BuildingBarleyFarm:case BuildingFlaxFarm:case BuildingGrainFarm:case BuildingLettuceFarm:
    case BuildingPomegranatesFarm:case BuildingChickpeasFarm:case BuildingCattleRanch:
    case BuildingReedGatherer:case BuildingFigFarm:case BuildingHennaFarm:
        c->operations->farm_update_image(c->operations->user_data,id,0);return;
    case BuildingLimestoneQuarry:case BuildingGraniteQuarry:
        image=construction_image(c,(int32_t)pharaoh_quarry_image_group(kind==BuildingLimestoneQuarry?ResourceLimestone:ResourceGranite));
        construction_publish(c,id,x,y,size,image,TerrainBuilding);return;
    case BuildingFerryLanding:
        if (creating) {
            construction_building(c,id)->runtime_value_ca=(uint8_t)c->operations->ferry_queue_allocate(support_data(c->operations),(uint32_t)id);
            (void)pharaoh_ring_queue_initialize(&c->ferry_queues[construction_building(c,id)->runtime_value_ca].queue);
            if ((*c->ferry_state)==0) construction_building(c,id)->linked_ferry_building_id=0;
        }
        construction_publish_waterfront(c,kind,id,x,y);
        if ((*c->ferry_state)==1) {pharaoh_map_building_undo_capture(c->surface->undo,(*c->first_ferry));pharaoh_map_building_undo_capture(c->surface->undo,id);}
        if ((*c->ferry_state)==2) pharaoh_map_building_undo_capture(c->surface->undo,id);
        return;
    case BuildingRoadblock:
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupRoadblock),
            (c->surface->map->terrain[construction_grid_offset(c,x,y)]&TerrainCanal)|TerrainBuilding|TerrainRoad);return;
    case BuildingShrineOsiris:case BuildingShrineRa:case BuildingShrinePtah:case BuildingShrineSeth:
    case BuildingShrineBast:case BuildingShrineGod5:case BuildingShrineGod6:case BuildingShrineGod7:
    case BuildingShrineGod8:case BuildingShrineGod9:
        construction_publish(c,id,x,y,size,construction_image(c,c->properties[kind].image_group),TerrainBuilding);return;
    case BuildingGatehouse:case BuildingGatehouseUp:
        pharaoh_gatehouse_update_map(c->temple_images,id);return;
    case BuildingTower:
        pharaoh_map_terrain_and_area(c->surface->map,x,y,2,0,~(uint32_t)TerrainWallKinds,c->terrain_cursor);
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupTower),TerrainBuilding|TerrainGatehouse);
        pharaoh_map_refresh_wall_region(c->surface,x,y,ConstructionTowerWallRadius);return;
    case BuildingPyramid:
        if (creating) c->operations->pyramid_create_parts(c->operations->user_data,owner,x,y,(uint32_t)size,id);
        if (!construction_building(c,id)->runtime_value_038) pharaoh_pyramid_refresh_part_images(c->pyramid_images,id);
        if (creating) (void)pharaoh_construction_cancel(c->cancel,c->surface->buildings);return;
    case BuildingSmallObelisk:case BuildingLargeObelisk:
        if (creating) c->operations->obelisk_create(support_data(c->operations),id);
        c->operations->obelisk_update_image(support_data(c->operations),id,x,y);return;
    case BuildingFestivalSquare:
        pharaoh_map_add_venue_plaza_tiles(c->venue_tiles,(uint32_t)construction_building(c,id)->venue_grid_offset,BuildingFestivalSquare,(uint16_t)id);
        c->cities[(*c->current_player)].festival_square_placed=1;return;
    case BuildingSphinx:construction_place_sphinx(c,owner,id,x,y,size,creating);return;
    case BuildingTempleComplexAltar:case BuildingTempleComplexOracle: {
        int32_t cachedPlayer=(int32_t)(*c->current_player);
        if (creating) {
            int32_t complex=c->cities[(*c->current_player)].temple_complex_building_id;
            construction_building(c,complex)->temple_upgrade+=(uint8_t)(kind==BuildingTempleComplexAltar?1:2);
        }
        pharaoh_temple_complex_update_map(c->temple_images,c->cities[cachedPlayer].temple_complex_building_id,0);return;
    }
    case BuildingSunTemple:
        if (!creating) {pharaoh_sun_temple_update_map(c->sun_temple_images,(uint32_t)id);return;}
        c->operations->sun_temple_create(c->operations->user_data,x,y,(uint32_t)id);pharaoh_sun_temple_reset_work_spots(c->animations->sun_temple,c->animation_scripts,c->animation_script_count,c->random);construction_clear_selections(c);return;
    case BuildingPyramidEntryTile:
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupPyramidEntryTile),TerrainBuilding|TerrainRoad);return;
    case BuildingMortuaryTemple:
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupMortuaryTemple)+((uint32_t)((*c->surface->orientation)/2)&ConstructionHalfTurnVariantMask),TerrainBuilding);return;
    case BuildingCauseway:
        construction_publish(c,id,x,y,size,construction_image(c,ImageGroupCauseway)+((uint32_t)((*c->surface->orientation)/2)&ConstructionHalfTurnVariantMask),TerrainBuilding);return;
    case BuildingValleyTemple:
        pharaoh_map_building_tiles_add_water_size2(c->water_footprint,(int16_t)id,x,y,construction_image(c,ImageGroupValleyTemple)+(uint32_t)((*c->surface->orientation)/2));return;
    case BuildingMausoleum:
        if (!creating) {pharaoh_mausoleum_update_map(c->mausoleum_images,id);return;}
        c->operations->mausoleum_place(c->operations->user_data,x,y,id,(uint8_t)mode);construction_clear_selections(c);return;
    case BuildingAlexandriaLibrary:
        if (!creating) {pharaoh_library_update_map(c->monument_images[PHARAOH_ROTATED_LIBRARY],id);return;}
        c->operations->library_place(c->operations->user_data,x,y,id,(uint8_t)mode!=0);
        pharaoh_library_reset_work_spots(c->animations->library,c->animation_scripts,c->animation_script_count,c->random);construction_clear_selections(c);return;
    case BuildingCaesareum:
        if (!creating) {pharaoh_monument_update_image_parts(c->monument_images[PHARAOH_ROTATED_CAESAREUM],id);return;}
        c->operations->caesareum_place(c->operations->user_data,x,y,id,false);
        pharaoh_caesareum_reset_work_spots(c->animations->caesareum,c->animation_scripts,c->animation_script_count,c->random);(void)pharaoh_construction_cancel(c->cancel,c->surface->buildings);return;
    case BuildingLighthouse:
        if (!creating) {pharaoh_lighthouse_update_map(c->monument_images[PHARAOH_ROTATED_LIGHTHOUSE],id);return;}
        c->operations->lighthouse_place(c->operations->user_data,x,y,id,(uint8_t)mode!=0);
        pharaoh_lighthouse_reset_work_spots(c->animations->lighthouse,c->animation_scripts,c->animation_script_count,c->random);construction_clear_selections(c);return;
    case BuildingAbuSimbel:
        if (!creating) {pharaoh_monument_update_image_parts(c->monument_images[PHARAOH_ROTATED_ABU_SIMBEL],id);return;}
        c->operations->abu_simbel_place(c->operations->user_data,x,y,id,(uint8_t)mode!=0);
        pharaoh_abu_simbel_reset_work_spots(c->animations->abu_simbel,c->animation_scripts,c->animation_script_count,c->random);construction_clear_selections(c);return;
    case BuildingSmallRoyalTomb:case BuildingMediumRoyalTomb:case BuildingLargeRoyalTomb:case BuildingGrandRoyalTomb: {
        PharaohMonumentImageContext *object=kind==BuildingSmallRoyalTomb?c->monument_images[PHARAOH_ROTATED_SMALL_TOMB]:
            kind==BuildingMediumRoyalTomb?c->monument_images[PHARAOH_ROTATED_MEDIUM_TOMB]:
            kind==BuildingLargeRoyalTomb?c->monument_images[PHARAOH_ROTATED_LARGE_TOMB]:c->monument_images[PHARAOH_ROTATED_GRAND_TOMB];
        if (!creating) {pharaoh_royal_tomb_update_map(object,id);return;}
        bool skip=(uint8_t)mode!=0;
        if (kind==BuildingSmallRoyalTomb) c->operations->small_tomb_place(c->operations->user_data,x,y,id,skip);
        else if (kind==BuildingMediumRoyalTomb) c->operations->medium_tomb_place(c->operations->user_data,x,y,id,skip);
        else if (kind==BuildingLargeRoyalTomb) c->operations->large_tomb_place(c->operations->user_data,x,y,id,skip);
        else c->operations->grand_tomb_place(c->operations->user_data,x,y,id,false);
        pharaoh_royal_tomb_reset_work_spots(c->animations->royal_tomb,c->animation_scripts,c->animation_script_count,c->random);
        if (kind==BuildingGrandRoyalTomb) (void)pharaoh_construction_cancel(c->cancel,c->surface->buildings);
        else c->monument_selections[5]=0;
        return;
    }
    default:return;
    }
}
