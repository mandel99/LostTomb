#ifndef PHARAOH_POPULATION_SERVICE_PROVISION_H
#define PHARAOH_POPULATION_SERVICE_PROVISION_H

#include <stdint.h>

/* Actual7999D8 pool; unrelated and overlapping uses retain their storage. */
typedef struct PharaohNativeHouseServiceRecord {
    uint8_t state,reserved_001[4],house_size;
    uint8_t reserved_006[0x10-6];
    int16_t type,subtype;
    uint8_t reserved_014[0x1c-0x14];
    int16_t population;
    uint8_t reserved_01e[0x4e-0x1e];
    uint8_t tax_coverage,reserved_04f[0x6c-0x4f];
    uint8_t juggler,musician,physician,dancer,magistrate,senet,school,library;
    uint8_t reserved_074,dentist,apothecary,water,mortuary,religion[5];
    uint8_t reserved_07e[8],zoo,bast_flag,reserved_088[0xc2-0x88];
    uint8_t missionary_flag,reserved_0c3[0xe4-0xc3];
    int32_t crime,apothecary_health;
    uint8_t reserved_0ec[0xfc-0xec];
    int32_t physician_health;
    uint8_t reserved_100[8];
} PharaohNativeHouseServiceRecord;

/* View beginning70CE0C, advancing by the actual88-byte model stride. */
typedef struct PharaohHouseServiceModel {
    int32_t minimum_crime;
    uint8_t reserved_004[8];
    int32_t tax_metric;
    uint8_t reserved_010[88-16];
} PharaohHouseServiceModel;

typedef struct PharaohHouseServiceContext {
    volatile PharaohNativeHouseServiceRecord *buildings;
    const volatile int16_t *building_grid;
    const volatile PharaohHouseServiceModel *models;
    const volatile int32_t *map_width,*map_height;
    const volatile uint32_t *grid_base;
    volatile uint32_t *row,*column,*cursor;
    const volatile int32_t *water_service_blocked;
    volatile int32_t *maximum_crime,*maximum_tax_metric;
    const struct PharaohHouseEconomyContext *market_context;
} PharaohHouseServiceContext;

int32_t pharaoh_house_service_juggler(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_musician(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_dancer(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_senet(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_zoo(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_school(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_library(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_dentist(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_physician(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_apothecary(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_mortuary(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
uint32_t pharaoh_house_service_apothecary_health(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount);
void pharaoh_house_service_physician_health(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount);
uint32_t pharaoh_house_service_bast(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
void pharaoh_house_service_crime(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t amount);
int32_t pharaoh_house_service_water(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_religion(const PharaohHouseServiceContext *c,int32_t x,int32_t y,int32_t deity);
void pharaoh_house_service_magistrate(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
uint32_t pharaoh_house_service_missionary(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_labor_seeker(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_tax(const PharaohHouseServiceContext *c,int32_t x,int32_t y);
int32_t pharaoh_house_service_market(const PharaohHouseServiceContext *c,int32_t market,int32_t x,int32_t y);

#endif
