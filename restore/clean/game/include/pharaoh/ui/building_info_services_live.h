#ifndef PHARAOH_UI_BUILDING_INFO_SERVICES_LIVE_H
#define PHARAOH_UI_BUILDING_INFO_SERVICES_LIVE_H

#include "pharaoh/ui/building_info_draw_live.h"

/* Borrowed actual EA1870 player records, stride49D8. These are concrete
 * overlapping views of saved scene fields, not a parallel player state. */
typedef struct PharaohInfoServicesPlayerRecord {
    uint8_t reserved_0000[0x8];
    int32_t tax_percentage;                 /* EA1878, DWORD */
    uint8_t reserved_000c[0x2790-0xc];
    int32_t palace_building_id;             /* EA4000, DWORD */
    uint8_t reserved_2794[0x2c84-0x2794];
    int32_t stolen_this_year;               /* EA44F4, DWORD */
    uint8_t reserved_2c88[0x2c98-0x2c88];
    int32_t salary;                        /* EA4508, DWORD */
    int32_t salary_rank;                   /* EA450C, DWORD */
    uint8_t reserved_2ca0[0x438c-0x2ca0];
    int32_t months_since_festival;          /* EA5BFC, DWORD */
    uint8_t reserved_4390[0x4486-0x4390];
    uint16_t palace_placed;                 /* EA5CF6, WORD zero test */
    uint8_t reserved_4488[4];
    int16_t palace_robberies;               /* EA5CFC, MOVSX WORD */
    uint8_t reserved_448e[0x44f8-0x448e];
    int32_t festival_planned_size;          /* EA5D68, DWORD */
    int32_t festival_planned_months;        /* EA5D6C, DWORD */
    uint8_t reserved_4500[0x4528-0x4500];
    int32_t festival_effect_first;          /* EA5D98, DWORD zero test */
    int32_t festival_effect_second;         /* EA5D9C, DWORD zero test */
    uint8_t reserved_4530[0x49d8-0x4530];
} PharaohInfoServicesPlayerRecord;

typedef struct PharaohInfoServicesLiveContext {
    const PharaohStorageInfoDrawContext *drawing;
    const PharaohInfoServicesPlayerRecord *players;
    const uint32_t *current_player; /* actual EAAC28 */
    const int32_t *month;           /* actual 78D7C0 */
    const PharaohInfoSquareDrawContext *square_buttons;
    const PharaohSquareButtonRecord *tax_arrows; /* actual5C8D38, two24B rows */
    const char *space_suffix;       /* actual57C048, bytes20 00 */
    const char *percent_suffix;     /* actual5E03FC, percent sign and NUL */
} PharaohInfoServicesLiveContext;

/* Complete real rendering providers. The nested draw context borrows the
 * actual224-byte info state,264-byte buildings, shared text selection/cursor,
 * font views, initialized layout and actual image/clip/framebuffer state.
 * The Native callers consume no EAX result from these drawing procedures. */
void pharaoh_info_services_draw_bandstand(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_booth(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_senet_house(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_zoo(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_pavilion(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_conservatory(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_dance_school(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_juggler_school(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_scribal_school(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_library(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_apothecary(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_mortuary(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_physician(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_dentist(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_police_station(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_firehouse(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_architect_post(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_mansion_foreground(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_mansion(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_palace(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_courthouse(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_tax_collector(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_mission_post(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_native_hut(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_native_meeting(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_native_crops(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_oracle(const PharaohInfoServicesLiveContext *);
void pharaoh_info_services_draw_temple(const PharaohInfoServicesLiveContext *,int32_t god);
void pharaoh_info_services_draw_festival_square(const PharaohInfoServicesLiveContext *);

#endif
