#include "city_history.h"
#include <stdio.h>
#include <string.h>

static unsigned char storage[0x49d8 + 2];
static unsigned char *city = storage + 1; /* Exercise an unaligned caller-owned record. */
static void put32(unsigned int offset, int32_t value)
{
    memcpy(city + offset, &value, sizeof(value));
}
static int32_t get32(unsigned int offset)
{
    int32_t value;
    memcpy(&value, city + offset, sizeof(value));
    return value;
}
#define CHECK(expr) do { if (!(expr)) { fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#expr); return 1; } } while (0)

int main(void)
{
    unsigned int i;
    int16_t bins[] = {-32768,32767,-100,100,1,-1,0,17,-5,9,2,3,4,5,-7,8,-9,10,-11,12,-13};
    memset(storage, 0, sizeof(storage));
    storage[0] = storage[sizeof(storage)-1] = 0xa5;
    memcpy(city + 0x3e34, bins, sizeof(bins));
    CHECK(restore_city_sum_bins(city,0,10) == 20);
    CHECK(restore_city_sum_bins(city,0,14) == 34);
    CHECK(restore_city_sum_bins(city,14,7) == -10);
    CHECK(restore_city_sum_bins(city,0,0) == 0);
    for (i=1; i<=10000; ++i) {
        put32(0x1894,(int32_t)i * 101);
        restore_city_history_append(city);
        CHECK(get32(0x3e2c) == (int32_t)(i % 2400));
        CHECK(get32(0x3e30) == (int32_t)i);
        CHECK(restore_city_history_get(city,1,0) == (int32_t)i * 101);
        if (i >= 2400) {
            CHECK(restore_city_history_get(city,2400,0) == (int32_t)(i-2399) * 101);
            CHECK(restore_city_history_get(city,2400,2399) == (int32_t)i * 101);
        }
    }
    put32(0x3e30,INT32_MAX);
    restore_city_history_append(city);
    CHECK(get32(0x3e30) == INT32_MIN);
    CHECK(storage[0] == 0xa5 && storage[sizeof(storage)-1] == 0xa5);
    puts("PASS: standalone C city history, 10000 updates, wraparound, signed bins and unaligned storage.");
    return 0;
}
