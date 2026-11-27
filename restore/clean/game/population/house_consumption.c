#include "pharaoh/population/house_consumption.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohHouseEconomyRecord) == 264,
    "native building stride");
_Static_assert(offsetof(PharaohHouseEconomyRecord, size) == 3,
    "house size BYTE");
_Static_assert(offsetof(PharaohHouseEconomyRecord, level) == 0x12,
    "house model signed WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord, beer) == 0x64,
    "beer signed WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord, pottery) == 0x66,
    "pottery signed WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord, linen) == 0x68,
    "linen signed WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord, jewelry) == 0x6a,
    "jewelry signed WORD");
_Static_assert(offsetof(PharaohHouseModel, pottery) == 0x2c &&
    offsetof(PharaohHouseModel, linen) == 0x30 &&
    offsetof(PharaohHouseModel, jewelry) == 0x34 &&
    offsetof(PharaohHouseModel, beer) == 0x38,
    "native house goods model DWORDs");

static int32_t signed_word32(uint32_t bits)
{
    return bits <= INT32_MAX ? (int32_t)bits :
        (int32_t)(bits & INT32_MAX) - INT32_MAX - 1;
}

static int32_t house_quantity(int32_t quantity, uint8_t size)
{
    uint32_t bits = (uint32_t)quantity;
    if (size > 1) {
        bits <<= 2;
    }
    return signed_word32(bits);
}

static int32_t daily_house_good_use(int32_t quantity, const int32_t extra[3],
    int32_t beer, int32_t jewelry)
{
    const int32_t remainder = quantity % 4;
    if (remainder == 0) {
        return quantity / 4;
    }
    if (remainder > 0) {
        return extra[remainder - 1];
    }
    /* Native negative indices alias the already scaled beer and jewelry
     * locals, then saved EBX. 451C50 zeroes EBX and its only 4528B0 call
     * preserves that value through both 454B40 call sites. */
    if (remainder == -1) {
        return beer;
    }
    if (remainder == -2) {
        return jewelry;
    }
    return 0;
}

static void consume_inventory(volatile int16_t *inventory, int32_t amount)
{
    if (amount > 0) {
        const int32_t stock = *inventory;
        *inventory = stock < amount ? 0 : (int16_t)(stock - amount);
    }
}

void pharaoh_house_consume_goods(const PharaohHouseConsumptionContext *c,
    int32_t building)
{
    static const int32_t extra_use[4][3] = {
        {1, 1, 0}, {0, 0, 1}, {0, 1, 1}, {0, 0, 1}
    };
    volatile PharaohHouseEconomyRecord *house = &c->buildings[building];
    const int16_t level = house->level;
    const uint8_t size = house->size;
    const volatile PharaohHouseModel *model = &c->models[level];
    const int32_t pottery = house_quantity(model->pottery, size);
    const int32_t linen = house_quantity(model->linen, size);
    const int32_t jewelry = house_quantity(model->jewelry, size);
    const int32_t beer = house_quantity(model->beer, size);
    const int32_t phase = *c->day % 4;
    const int32_t *extra = extra_use[phase];
    const int32_t beer_use = daily_house_good_use(beer, extra, beer, jewelry);
    const int32_t pottery_use = daily_house_good_use(pottery, extra, beer, jewelry);
    const int32_t linen_use = daily_house_good_use(linen, extra, beer, jewelry);
    const int32_t jewelry_use = daily_house_good_use(jewelry, extra, beer, jewelry);

    consume_inventory(&house->beer, beer_use);
    consume_inventory(&house->pottery, pottery_use);
    consume_inventory(&house->linen, linen_use);
    consume_inventory(&house->jewelry, jewelry_use);
}
