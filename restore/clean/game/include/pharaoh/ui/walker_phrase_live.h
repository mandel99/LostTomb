#ifndef PHARAOH_UI_WALKER_PHRASE_LIVE_H
#define PHARAOH_UI_WALKER_PHRASE_LIVE_H

#include "pharaoh/events/event_queries.h"
#include "pharaoh/city/labor.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/commerce/storage_queries.h"

/* Borrowed native EA187C + player*49D8 profile view. Address labels preserve
 * observed fields without assigning unproved player-facing meanings. */
typedef struct PharaohWalkerPhrasePlayer {
    int32_t native_ea187c,native_ea1880;
    uint8_t reserved_0008[4];
    int32_t native_ea1888;
    uint8_t reserved_0010[8];
    uint32_t native_ea1894;
    uint8_t reserved_001c[0x27fe];
    uint16_t trade_state[36];
    int16_t trade_keep[36],resource_permissions[36];
    uint8_t reserved_28f2[0x21a];
    int32_t native_ea4388;
    uint8_t reserved_2b10[0x10];
    PharaohLaborCategory labor[10];
    uint8_t reserved_2be8[4];
    uint32_t native_ea4468;
    int32_t native_ea446c;
    uint8_t reserved_2bf4[4];
    int32_t native_ea4474;
    uint8_t reserved_2bfc[0x18];
    uint32_t native_ea4490,native_ea4494,native_ea4498,native_ea449c;
    uint8_t reserved_2c24[0x1694];
    uint32_t native_ea5b34;
    uint8_t reserved_42bc[0x10];
    int32_t native_ea5b48;
    uint8_t reserved_42d0[0xa8];
    int32_t native_ea5bf4;
    uint8_t reserved_437c[0x12];
    int8_t native_ea5c0a[5];
    uint8_t reserved_4393[0x1a1];
    uint32_t native_ea5db0;
    uint8_t reserved_4538[0xf0];
    uint32_t native_ea5ea4;
    uint8_t reserved_462c[0x30];
    uint8_t native_ea5ed8;
    uint8_t reserved_465d[0x1b7];
    uint32_t native_ea6090;
    uint8_t reserved_4818[0xcc];
    int32_t resource_stock[36];
    uint8_t reserved_4974[4];
    int16_t native_ea61f4,native_ea61f6,native_ea61f8;
    uint8_t reserved_497e[0x16];
    uint32_t native_ea6210;
    uint8_t reserved_4998[0x40];
} PharaohWalkerPhrasePlayer;

/* Actual77DE00 external-trader records and77AF00 resource-table overlay. */
typedef struct PharaohWalkerPhraseTrader {
    uint8_t reserved_00[4],route,reserved_05;
    uint8_t exports[36],imports[36],reserved_4e[28];
} PharaohWalkerPhraseTrader;
typedef struct PharaohWalkerPhraseResource {
    uint32_t native_77af00,native_77af04;
    uint8_t reserved_08[80];
} PharaohWalkerPhraseResource;

typedef enum PharaohWalkerPhraseStatus {
    PHARAOH_WALKER_PHRASE_VALUE,
    PHARAOH_WALKER_PHRASE_DIVIDE_BY_ZERO,
    PHARAOH_WALKER_PHRASE_DIVIDE_OVERFLOW
} PharaohWalkerPhraseStatus;
typedef struct PharaohWalkerPhraseResult {
    PharaohWalkerPhraseStatus status;
    uint32_t value;
} PharaohWalkerPhraseResult;

typedef struct PharaohWalkerPhraseContext {
    PharaohWalkerQueryRecord *walkers; /* actual388-byte saved walkers */
    PharaohBuildingTypeRecord *buildings; /* actual264-byte buildings */
    volatile PharaohWalkerPhrasePlayer *players; /* actual49D8 profiles */
    const volatile uint32_t *current_player; /*EAAC28*/
    const PharaohBuildingModelRecord *models; /*705EA4,40-byte rows*/
    const PharaohBuildingStorageRecord *storages; /*7900B8,196-byte rows*/
    const PharaohWalkerPhraseTrader *traders; /*77DE00,106-byte rows*/
    const PharaohWalkerPhraseResource *resources; /*77AF00,88-byte rows*/
    const int32_t *route_limits,*route_traded,*export_keep; /*77646C/77592C/775810*/
    PharaohScenarioEvent *events; /*785D74,124-byte records*/
    const volatile int32_t *event_count; /*785D70*/
    uint8_t *invasion_search_flag;
    int32_t *current_event_index;
    const PharaohEventCalendarView *calendar;
    PharaohOccupancyState *occupancy; /* actual grids/objects; buildings view unused */
    const volatile uint32_t *processing_mode; /*D05720+32FB4=D386D4*/
    const volatile uint32_t *native_bcb9b4,*random_second_7; /*BCB9B4/D3B374*/
    const volatile int32_t *native_d38374;
    const volatile uint8_t *native_d386dc;
    volatile uint8_t *weights; /*F55BA4,8 bytes*/
    volatile uint32_t *shared_result,*distance_x,*distance_y; /*EADCDC/D3B4FC/F8*/
} PharaohWalkerPhraseContext;

/* Complete53D3A0: fullDWORD total is consumed directly by53C060's IDIV.
 * all_defaults is the caller's actual BYTE; only the native clear paths write
 * it. Both output-pointer aliases and the two initial DWORD stores survive. */
PharaohWalkerPhraseResult pharaoh_walker_phrase_build_weights(
    const PharaohWalkerPhraseContext *,volatile uint8_t weights[8],
    volatile uint8_t *all_defaults);
PharaohWalkerPhraseResult pharaoh_walker_phrase_choose_category(
    const PharaohWalkerPhraseContext *); /*53C060,fullEAX0..9*/

/* The observed4BF9D0 consumer tests AL; unused upper-register residues are not
 * C inputs or outputs. Occupancy uses actual building X/Y and the existing
 * real region algorithm, avoiding a copied compact XY building pool. */
uint8_t pharaoh_walker_phrase_processing_blocked(const PharaohWalkerPhraseContext *);
uint32_t pharaoh_walker_phrase_trade_can_export(const PharaohWalkerPhraseContext *,
    uint32_t player,int32_t trader,int32_t resource); /*4434E0*/
uint32_t pharaoh_walker_phrase_trade_can_import(const PharaohWalkerPhraseContext *,
    uint32_t player,int32_t trader,int32_t resource); /*4435F0*/
uint32_t pharaoh_walker_phrase_storage_can_export(const PharaohWalkerPhraseContext *,
    int32_t walker,uint32_t player,int32_t building,int32_t trader); /*45CF50*/
uint32_t pharaoh_walker_phrase_storage_can_import(const PharaohWalkerPhraseContext *,
    int32_t walker,uint32_t player,int32_t building,int32_t trader); /*45D180*/

/* Complete53C100 effects. The real4F7E50 caller ignores EAX, so normal return
 * exposes no incidental register value, including the nonpositive-ID arm.
 * Non-VALUE status is the real native IDIV#DE; host must raise immediately.
 * No whole pool copy, general native-memory emulation or endpoint callbacks. */
PharaohWalkerPhraseStatus pharaoh_walker_phrase_update(
    const PharaohWalkerPhraseContext *,int32_t walker_id);

#endif
