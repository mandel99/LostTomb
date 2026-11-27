#ifndef PHARAOH_COMMERCE_STORAGE_WITHDRAW_H
#define PHARAOH_COMMERCE_STORAGE_WITHDRAW_H
#include "pharaoh/building/type_queries.h"
#include "pharaoh/commerce/storage_queries.h"

typedef struct PharaohStorageWithdrawContext {
    PharaohBuildingTypeRecord *buildings; /*7999D8,4000x264*/
    uint32_t *withdrawn,*stored; /*EA60D0/EA6160 + player*49D8, resource DWORDs*/
    uint32_t *tile_images; /*B7482C, genuine map image grid*/
    const uint32_t *pile_image_first,*extended_pile_image_first,*empty_image;
    /* F566BC,F57204,F566B4 */
    int16_t *city_cursor; /*EA3FC2 + player*49D8*/
} PharaohStorageWithdrawContext;

/* Complete0045B570 and0045B4D0. Actual linked storage records and player
   counters must share their native storage; no normalized building copies. */
int32_t pharaoh_storage_withdraw(PharaohStorageWithdrawContext *,
    int32_t building_id,int32_t resource,int32_t amount);
int32_t pharaoh_storage_withdraw_city(PharaohStorageWithdrawContext *,
    int32_t resource,int32_t amount);
#endif
