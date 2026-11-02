#include "terrain_redraw.h"
#include <stdint.h>

#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define TERRAIN(i) U(0xaf598cu + (uint32_t)(i) * 4u)
#define IMAGE(i) U(0xb7482cu + (uint32_t)(i) * 4u)
#define BUILDING(i) H(0xb4e6fcu + (uint32_t)(i) * 2u)
#define RECORD(id) (0x7999d8u + (uint32_t)(id) * 264u)
extern void FUN_0047c140(int32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern int32_t FUN_0047c880(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047db00(uint32_t);
extern int32_t FUN_00484f40(void);
extern int32_t FUN_00484be0(void *,int32_t,int32_t,int32_t);
extern int32_t FUN_004f3d70(int32_t);
extern uint32_t FUN_004f3f70(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004f60d0(int32_t);
extern void FUN_004f6130(void);
extern uint32_t FUN_0051c5f0(int32_t,uint32_t);

static const int32_t neighbors[8] = {-228,-227,1,229,228,227,-1,-229};

static int neighbors_blocked(int32_t tile)
{
    uint32_t i;
    for (i = 0; i < 8; ++i)
        if (TERRAIN(tile + neighbors[i]) & 0xeeffd77fu) return 1;
    return 0;
}

/* The original building tests can retain the current graphic even when the
   remaining terrain bits would permit clearing it. All record offsets are bytes. */
static int retain_building(int32_t tile)
{
    int32_t id = BUILDING(tile), type = H(RECORD(id) + 0x10u);
    uint32_t record = RECORD(id);
    if (type == 0xb7) {
        int32_t state = FUN_004f3d70(tile);
        return state >= 11 || (state >= 3 && (uint8_t)FUN_004f3f70(id,tile,state,0));
    }
    if (type == 0xd5 || type == 0xe3 || type == 0xe5 || type == 0xea ||
        type == 0xeb || type == 0xec || type == 0xe4) {
        if (I(record + 0x58u)) return 1;
        return I(RECORD(id - H(record + 0x12u)) + 0x58u) != 0;
    }
    if (type == 0xe6)
        return I(RECORD(id - H(record + 0x12u)) + 0x54u) > 0;
    if (type == 0xe1) {
        int32_t i, minimum = 21, first = id - H(record + 0x12u);
        for (i = 0; i < 25; ++i) {
            int32_t value = I(RECORD(first + i) + 0x58u);
            if (value < minimum) minimum = value;
        }
        return minimum > 0;
    }
    {
        int32_t previous = tile - 20, found = 0;
        while (previous > I(0xbca2d8)) {
            found = BUILDING(previous);
            if (found || !(TERRAIN(previous) & 0x10000000u)) break;
            previous -= 20;
        }
        if (found && H(RECORD(found) + 0x10u) == 0xb7) FUN_004f60d0(found);
    }
    return 0;
}

static void redraw_floodplain(int32_t tile)
{
    uint32_t group, variant, saved = U(0xbc9768), random;
    U(0xbc9768) = (uint32_t)tile;
    FUN_0047db00(0x10000);
    U(0xbc9768) = saved;
    if (FUN_00484f40()) {
        FUN_00484be0((void *)0x5cbfb8u,0x1a,-1,0);
        group = 0x210; variant = U(0xbc9748) + 0x30u;
    } else {
        uint32_t fertility = B(0xa509bcu + tile);
        /* Native constants are exactly 12.0 / 96.0, so the nonnegative
           x87/ftol results are exact integer division by eight. */
        if (fertility <= 100) variant = fertility / 8u;
        else {
            int32_t orientation = I(0xbca2e0);
            variant = (fertility & 8u)
                ? ((fertility - (uint32_t)(orientation / 2)) & 3u) + (fertility & 8u)
                : (fertility - (uint32_t)orientation) & 7u;
            variant |= 0x80u;
        }
        random = B(0xacf85cu + tile);
        if (variant < 11) {
            group = 0x21f; variant += (random % 3u) * 12u;
        } else if (variant >= 0x80) {
            group = 0x231; variant &= 0x7f;
        } else if (!(TERRAIN(tile) & 0x10000000u) && neighbors_blocked(tile)) {
            group = 0x225; variant = random % 12u;
        } else {
            uint32_t i;
            group = 0x226; variant = random % 12u;
            for (i = 0; i < 8; ++i) {
                int32_t n = tile + neighbors[i];
                uint32_t fertility_n = B(0xa509bcu + n), flags = TERRAIN(n);
                if (fertility_n > 100) fertility_n = 100;
                if (fertility_n / 8u != 11 || (flags & 0x10801u) ||
                    (!(flags & 0x10000000u) && neighbors_blocked(n))) {
                    group = 0x229;
                    break;
                }
            }
        }
    }
    if (TERRAIN(tile) & 0x40u) variant = FUN_0051c5f0(tile,variant);
    IMAGE(tile) = U(0xf55bb4u + group * 8u) + variant;
}

void FUN_004767d0(int32_t x0,int32_t y0,int32_t x1,int32_t y1,
                  uint32_t preserve_clear,uint32_t exact_rectangle)
{
    int32_t x,y,tile,start,step;
    if (!(uint8_t)exact_rectangle) {--x0;--y0;++x1;++y1;}
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= I(0xbca2d0)) x1 = I(0xbca2d0) - 1;
    if (y1 >= I(0xbca2d4)) y1 = I(0xbca2d4) - 1;
    start = I(0xbca2d8) + y0 * 228 + x0;
    step = I(0xbca2d0) - x1 - 1 + x0;
    tile = start;
    for (y = y0; y <= y1; ++y) {
        for (x = x0; x <= x1; ++x,++tile) {
            uint32_t flags = TERRAIN(tile), object;
            if (flags & 0x10000000u) {
                flags &= 0xeffffff7u;
                if (retain_building(tile)) continue;
            }
            object = U(0xd05720u + (uint32_t)tile * 4u);
            if (object && !B(object + 0x18u)) {
                if (flags & 8u) continue;
            } else if (flags & 0xeeffd77fu) continue;
            if (flags & 0x80u) {
                if (!(flags & 0x22310803u)) redraw_floodplain(tile);
            } else if (!(uint8_t)preserve_clear) {
                IMAGE(tile) = 0;
                B(0xb285ccu + tile) &= 0xf0;
                B(0xb67d1cu + tile) |= 0x40;
            }
        }
        tile += I(0xbca2dc) + step;
    }
    if (!(uint8_t)preserve_clear) {
        uint32_t phase;
        for (phase = 0; phase < 2; ++phase) {
            tile = start;
            for (y = y0; y <= y1; ++y) {
                for (x = x0; x <= x1; ++x,++tile) {
                    uint32_t flags = TERRAIN(tile), object_flags = 0, image;
                    int32_t building = 0, size = 1;
                    if (flags & 0x10000000u) {
                        flags &= 0xeffffff7u;
                        building = BUILDING(tile); object_flags = 0x10000008u;
                    }
                    if ((flags & 0xeefed77fu) || IMAGE(tile)) continue;
                    if (!phase) {
                        if (B(0xacf85cu + tile) & 0xf0) continue;
                        image = U((B(0xb285ccu + tile) & 0x20) ? 0xf56b74u : 0xf56b8cu)
                              + (B(0xacf85cu + tile) & 7u);
                    } else {
                        uint32_t group = (B(0xb285ccu + tile) & 0x20) ? 0x1f8 : 0x1fb;
                        if (FUN_0047c880(x,y,4,4,0xffff,1,1)) {size = 4;image = 0x2a;}
                        else if (FUN_0047c880(x,y,3,3,0xffff,1,1)) {size = 3;image = 0x18 + (B(0xacf85cu + tile) & 1u) * 9u;}
                        else if (FUN_0047c880(x,y,2,2,0xffff,1,1)) {size = 2;image = 8 + (B(0xacf85cu + tile) & 3u) * 4u;}
                        else image = B(0xacf85cu + tile) & 7u;
                        image += U(0xf55bb4u + group * 8u);
                    }
                    FUN_0047c140(building,x,y,size,size,image,object_flags);
                }
                tile += I(0xbca2dc) + step;
            }
        }
    }
    FUN_004f6130();
}
