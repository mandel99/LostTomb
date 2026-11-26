#ifndef PHARAOH_UI_DEBUG_BUILDING_TYPES_H
#define PHARAOH_UI_DEBUG_BUILDING_TYPES_H
#include <stddef.h>
#include <stdint.h>

/* Existing native264-byte building views; unrelated union alternatives omitted. */
typedef struct {
    int16_t inventory[12];
    uint8_t juggler;
    uint8_t musician;
    uint8_t physician;
    uint8_t dancer;
    uint8_t courthouse;
    uint8_t senet;
    uint8_t school;
    uint8_t library;
    uint8_t reserved1;
    uint8_t dentist;
    uint8_t apothecary;
    uint8_t waterSupply;
    uint8_t mortuary;
    uint8_t temple[5];
    uint8_t noSpaceToExpand;
    uint8_t foodTypes;
    int8_t entertainment;
    int8_t education;
    int8_t health;
    int8_t numGods;
    uint8_t devolveDelay;
    uint8_t evolveTextId;
    uint8_t zoo;
    uint8_t isSick;
    uint8_t taxBonus;
    uint8_t LegacyPadding1[3];
    int32_t taxIncome;
    uint8_t reserved2[40];
} PharaohDebugBuildingHouse;

typedef struct {
    int16_t reserved1;
    int16_t inventory[12];
    uint8_t reserved2[48];
    int16_t goodDemands[4];
    uint8_t reserved3[6];
    uint8_t fetchInventoryId;
    uint8_t reserved4[7];
    int16_t secondTraderId;
    int16_t secondBuyerId;
} PharaohDebugBuildingMarket;

typedef struct {
    int16_t reserved1;
    int16_t resourceStored[36];
    uint8_t reserved2[24];
    int16_t secondCartId;
} PharaohDebugBuildingGranary;

typedef struct {
    uint8_t reserved1[74];
    int16_t jugglerId;
    int16_t musicianId;
    int16_t dancerId;
    int16_t reserved2;
    uint8_t numShows;
    uint8_t showDaysLeft[3];
    uint8_t reserved3[14];
} PharaohDebugBuildingEntertainment;

typedef struct {
    uint8_t reserved1[74];
    int16_t aqueductLinks[4];
    uint8_t reserved2[18];
} PharaohDebugBuildingWaterLift;

typedef struct {
    uint8_t state;
    uint8_t player;
    uint8_t reservedId;
    uint8_t size;
    uint8_t houseIsMerged;
    uint8_t houseSize;
    int16_t x;
    int16_t y;
    uint8_t padding2[2];
    int32_t gridOffset;
    int16_t type;
    int16_t subtype;
    uint8_t roadNetworkId;
    uint8_t padding3;
    uint16_t createdSequence;
    int16_t housesCovered;
    int16_t percentageHousesCovered;
    int16_t housePopulation;
    int16_t housePopulationRoom;
    int16_t distanceFromEntry;
    int16_t houseHighestPopulation;
    int16_t unreachableTicks;
    int16_t roadAccessX;
    int16_t roadAccessY;
    int16_t walkerId;
    int16_t walkerId2;
    int16_t immigrantWalkerId;
    int16_t walkerId4;
    uint8_t walkerSpawnDelay;
    uint8_t reserved1;
    uint8_t walkerRoamDirection;
    uint8_t hasWaterAccess;
    uint8_t waterDependency;
    uint8_t padding4;
    int16_t prevPartBuildingId;
    int16_t nextPartBuildingId;
    int16_t loadsStored;
    uint8_t reserved2;
    uint8_t hasWellAccess;
    int16_t numWorkers;
    int8_t LaborCategory;
    uint8_t outputResource;
    uint8_t HasRoadAccess;
    uint8_t padding5;
    int16_t damageRisk;
    int16_t fireRisk;
    int16_t fireDuration;
    uint8_t fireProof;
    uint8_t houseWalkerGenerationDelay;
    uint8_t houseTaxCoverage;
    uint8_t padding6;
    int16_t formationId;
    uint8_t padding7[2];
    union {
        PharaohDebugBuildingHouse house;
        PharaohDebugBuildingMarket market;
        PharaohDebugBuildingGranary granary;
        PharaohDebugBuildingEntertainment entertainment;
        PharaohDebugBuildingWaterLift waterLift;
        uint8_t native_storage[100];
    } data;
    int32_t taxIncomeOrStorage;
    uint8_t houseMonthsWithoutFood;
    uint8_t ruinHasPlague;
    int8_t desirability;
    uint8_t isDeleted;
    uint8_t isAdjacentToWater;
    uint8_t storageId;
    union {
        int8_t house;
        uint8_t nativeAnger;
    } sentiment;
    uint8_t showOnProblemOverlay;
    int32_t linkedBuildingId;
    int16_t monumentRemainingBlocks;
    uint8_t queueIndex;
    uint8_t queueSlot;
    int16_t secondInputStock;
    int16_t secondInputResource;
    int32_t venuePlazaGridOffset;
    int32_t venueOrientation;
    int32_t venueHasPlaza;
    int32_t infoOrdersToggle;
    int32_t infoOrdersScroll;
    int32_t crimeRisk;
    int32_t houseMalariaRisk;
    int32_t housePlagueDistance;
    int32_t houseSickMonths;
    int32_t houseWaterDistance;
    int32_t houseMarshDistance;
    int32_t housePlagueRisk;
    int32_t problemFlags;
    int16_t incomingCarts;
    int16_t incomingLoads;
} PharaohDebugBuilding;

_Static_assert(sizeof(PharaohDebugBuilding) == 0x108 &&
    offsetof(PharaohDebugBuilding,data) == 0x54 &&
    offsetof(PharaohDebugBuilding,loadsStored) == 0x3c &&
    offsetof(PharaohDebugBuilding,damageRisk) == 0x46 &&
    offsetof(PharaohDebugBuilding,houseTaxCoverage) == 0x4e &&
    offsetof(PharaohDebugBuildingHouse,juggler) == 0x18 &&
    offsetof(PharaohDebugBuildingHouse,entertainment) == 0x2c &&
    offsetof(PharaohDebugBuildingHouse,zoo) == 0x32 &&
    offsetof(PharaohDebugBuildingEntertainment,numShows) == 0x52 &&
    offsetof(PharaohDebugBuildingWaterLift,aqueductLinks) == 0x4a,
    "native debug building field views");
#endif
