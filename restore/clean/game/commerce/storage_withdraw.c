#include "pharaoh/commerce/storage_withdraw.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,grid_offset)==12 &&
    offsetof(PharaohBuildingTypeRecord,building_type)==16 &&
    offsetof(PharaohBuildingTypeRecord,subtype)==18 &&
    offsetof(PharaohBuildingTypeRecord,next_storage_building_id)==58 &&
    offsetof(PharaohBuildingTypeRecord,storage_link_amount)==60,
    "native storage record fields");

int32_t pharaoh_storage_withdraw(PharaohStorageWithdrawContext *c,
    int32_t building_id,int32_t resource,int32_t amount)
{
    int32_t remaining=amount;
    if(amount<=0) return 0;
    const int16_t type=c->buildings[building_id].building_type;
    if(type!=72 && type!=193) return amount;
    int more;
    do {
        int32_t node=building_id,chosen=0,best=401;
        for(int32_t i=0;i<8;++i){
            node=c->buildings[node].next_storage_building_id;
            if(node<=0) continue;
            const PharaohBuildingTypeRecord *const b=&c->buildings[node];
            if(b->subtype!=resource) continue;
            const int32_t stock=b->storage_link_amount;
            if(stock>0 && stock<best){best=stock;chosen=node;}
        }
        if(chosen==0) return remaining;
        PharaohBuildingTypeRecord *const b=&c->buildings[chosen];
        const int32_t stock=b->storage_link_amount;
        more=0;
        if(stock<=remaining){
            remaining-=stock;
            b->storage_link_amount=0;
            b->subtype=0;
            c->withdrawn[resource]+=(uint32_t)stock;
            c->stored[resource]-=(uint32_t)stock;
            more=remaining>0;
        }else{
            b->storage_link_amount=(int16_t)(stock-remaining);
            c->withdrawn[resource]+=(uint32_t)remaining;
            c->stored[resource]-=(uint32_t)remaining;
            remaining=0;
        }
        uint32_t image;
        if(b->storage_link_amount>0){
            image=resource<31?*c->pile_image_first+(uint32_t)resource*4-4:
                *c->extended_pile_image_first+(uint32_t)resource*4-124;
            image+=(uint32_t)pharaoh_storage_image_level(b->storage_link_amount);
        }else image=*c->empty_image;
        c->tile_images[(uint32_t)b->grid_offset]=image;
    }while(more);
    return remaining;
}

int32_t pharaoh_storage_withdraw_city(PharaohStorageWithdrawContext *c,
    int32_t resource,int32_t amount)
{
    int32_t cursor=*c->city_cursor,remaining=amount;
    for(int32_t i=1;i<4000;++i){
        if(remaining<=0) break;
        if(++cursor>=4000) cursor=1;
        const PharaohBuildingTypeRecord *const b=&c->buildings[cursor];
        if(b->state!=1 || (b->building_type!=72 && b->building_type!=193))
            continue;
        *c->city_cursor=(int16_t)cursor;
        remaining=pharaoh_storage_withdraw(c,cursor,resource,remaining);
    }
    return amount-remaining;
}
