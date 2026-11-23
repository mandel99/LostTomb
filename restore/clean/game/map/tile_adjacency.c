#include "pharaoh/map/tile_adjacency.h"

static const int32_t neighbor[8] = {-228,-227,1,229,228,227,-1,-229};

/* Native0047DB00; native slots are clockwise from north. */
void pharaoh_map_adjacent_terrain_any(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask)
{
    for (int32_t i=0; i<8; ++i) out[i]=(terrain[tile+neighbor[i]] & mask)!=0;
}

/* Native0047DBC0. */
void pharaoh_map_adjacent_terrain_all(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask)
{
    for (int32_t i=0; i<8; ++i) out[i]=(terrain[tile+neighbor[i]] & mask)==mask;
}

/* Native0047DC80. */
void pharaoh_map_adjacent_terrain_include_exclude(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t include, uint32_t exclude)
{
    for (int32_t i=0; i<8; ++i) {
        const uint32_t flags=terrain[tile+neighbor[i]];
        out[i]=(flags & include)!=0 && (flags & exclude)==0;
    }
}

/* Native0047DDD0; the processor masks the shift to five bits. */
void pharaoh_map_adjacent_terrain_add_bits(uint32_t out[8],
    const uint32_t *terrain, int32_t tile, uint32_t mask, uint32_t shift)
{
    shift &= 31u;
    for (int32_t i=0; i<8; ++i)
        out[i] |= (uint32_t)((terrain[tile+neighbor[i]] & mask)!=0) << shift;
}

/* Native0047DEE0. Keep the original transient1->0 store order. */
void pharaoh_map_adjacent_road_flags(uint32_t out[8],
    const uint32_t *terrain, const uint8_t *property_flags, int32_t tile)
{
    for (int32_t i=0; i<8; ++i) {
        const int32_t at=tile+neighbor[i];
        if ((uint8_t)terrain[at] & 2u) {
            const uint8_t properties=property_flags[at];
            out[i]=1;
            if (!(properties & 128u)) out[i]=0;
        } else out[i]=0;
    }
}

/* Native0047E000: raw0x8000 values, rather than normalized Boolean slots. */
uint32_t pharaoh_map_adjacent_floodplain_cardinals(uint32_t out[8],
    const uint32_t *terrain, int32_t tile)
{
    uint32_t any=0;
    for (int32_t i=7; i>=1; i-=2) out[i]=0;
    for (int32_t i=0; i<8; i+=2) {
        out[i]=terrain[tile+neighbor[i]] & 0x8000u;
        any |= out[i];
    }
    return any!=0;
}

/* Native0047E750 preserves every existing slot DWORD before addition. */
void pharaoh_map_adjacent_add_canal_flags(uint32_t out[8],
    const uint32_t *terrain, int32_t tile)
{
    for (int32_t i=0; i<8; ++i)
        out[i] += (terrain[tile+neighbor[i]] >> 14) & 1u;
}

/* Native0047CAE0. Empty dimensions retain the native success path. */
int32_t pharaoh_map_terrain_rectangle_fits(const PharaohMapTileArea *c,
    int32_t x, int32_t y, int32_t width, int32_t height,
    uint32_t flags, uint32_t image_plus_one)
{
    const PharaohMapTerrainView *m=c->map;
    if (x<0 || y<0) return 0;
    if ((int32_t)((uint32_t)x+(uint32_t)width-1u)>=m->width) return 0;
    if ((int32_t)((uint32_t)y+(uint32_t)height-1u)>=m->height) return 0;
    const uint32_t base=(uint32_t)m->origin+(uint32_t)y*228u+(uint32_t)x;
    *c->current_tile=base;
    for (int32_t row=0; row<height; ++row) {
        for (int32_t col=0; col<width; ++col) {
            const uint32_t tile=base+c->offsets[row][col].grid_offset;
            if ((m->terrain[tile] & 0xeefed77fu)!=flags) return 0;
            if (image_plus_one && c->image_ids[tile]!=image_plus_one-1u) return 0;
        }
    }
    return 1;
}

/* Native004781A0. Native00477690 clears EAX first, so byte promotion is exact. */
int32_t pharaoh_map_terrain_least_connected_neighbor(
    const uint32_t *terrain, int32_t tile, uint32_t mask)
{
    static const int32_t delta[8]={-228,1,228,-1,-227,229,227,-229};
    if (pharaoh_map_terrain_mask_has_open_corner(terrain,tile,mask,1)) return 0;
    int32_t best=0,result=0;
    for (int32_t i=0; i<8; ++i) {
        const int32_t at=tile+delta[i];
        const int32_t score=8-pharaoh_map_terrain_count_neighbours_with(terrain,at,mask);
        if ((terrain[at] & mask) && score>best) {best=score;result=delta[i];}
    }
    return result;
}

/* Pinned native table005DBCC8; verified with live Ghidra memory. */
const int32_t pharaoh_royal_tomb_rock_offsets[14][4] = {
    {12,15,18,21},
    {15,18,21,12},
    {18,21,12,15},
    {21,12,15,18},
    {24,30,36,42},
    {30,36,42,24},
    {36,42,24,30},
    {42,24,30,36},
    {0,3,6,9},
    {3,6,9,0},
    {6,9,0,3},
    {9,0,3,6},
    {72,75,72,75},
    {75,72,75,72},
};

static int32_t rock_variant(const PharaohMapRockVariantContext *c,
    uint32_t tile, int32_t row, uint32_t modulus)
{
    return (int32_t)((uint32_t)pharaoh_royal_tomb_rock_offsets[row][c->orientation/2]+
        (uint32_t)c->random[tile]%modulus);
}

/* Complete typed reuse of MenuDragRoadVariant, native00475FD0. */
int32_t pharaoh_map_royal_tomb_rock_variant(PharaohMapRockVariantContext *c,
    uint32_t tile)
{
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    uint32_t snapshot[8],count=0;
    uint32_t *s=c->adjacent;
    for (int32_t i=0;i<16;++i) s[i]=0;
    *c->current_tile=tile;
    pharaoh_map_adjacent_terrain_all(s,c->terrain,(int32_t)tile,0x20000002u);
    for (int32_t i=0;i<8;++i) snapshot[i]=s[i];
    pharaoh_map_adjacent_terrain_all(s,c->terrain,(int32_t)tile,0x80000u);
    for (int32_t i=0;i<8;++i) s[i]=s[i] ? 0 : snapshot[i];
    for (int32_t i=0;i<8;++i) {
        const uint32_t at=tile+(uint32_t)delta[i];
        const int32_t id=c->building_ids[at];
        if (id<=0) continue;
        const PharaohBuildingTypeRecord *b=&c->buildings[id];
        if (b->building_type==230) {s[i]=1;continue;}
        if (b->building_type!=229 && b->building_type!=234 &&
            b->building_type!=235 && b->building_type!=236) continue;
        /* Native DWORD+68 is shared with the origin_x record overlay. */
        if (b->origin_x==18) continue;
        const int32_t root=(int32_t)((uint32_t)id-(uint32_t)(int32_t)b->subtype);
        if (c->buildings[root].monument_stage==1) s[i]=1;
    }
    for (int32_t i=0;i<8;++i) if (s[i]==1) ++count;
    if (count==8) return (int32_t)((uint32_t)c->random[tile]%24u+48u);
    if (count==7) {
        if (!s[1]) return rock_variant(c,tile,0,3);
        if (!s[7]) return rock_variant(c,tile,1,3);
        if (!s[5]) return rock_variant(c,tile,2,3);
        if (!s[3]) return rock_variant(c,tile,3,3);
    } else if (count==6) {
        if (!s[7] && !s[3]) return rock_variant(c,tile,12,3);
        if (!s[1] && !s[5]) return rock_variant(c,tile,13,3);
    }
    if (s[6] && s[7] && s[0] && s[1] && s[2] && count<=7)
        return rock_variant(c,tile,4,6);
    if (s[4] && s[5] && s[6] && s[7] && s[0] && count<=7)
        return rock_variant(c,tile,5,6);
    if (s[2] && s[3] && s[4] && s[5] && s[6] && count<=7)
        return rock_variant(c,tile,6,6);
    if (s[0] && s[1] && s[2] && s[3] && s[4] && count<=7)
        return rock_variant(c,tile,7,6);
    if (s[0] && s[1] && s[2]) return rock_variant(c,tile,8,3);
    if (s[6] && s[7] && s[0]) return rock_variant(c,tile,9,3);
    if (s[4] && s[5] && s[6]) return rock_variant(c,tile,10,3);
    if (s[2] && s[3] && s[4]) return rock_variant(c,tile,11,3);
    return -1;
}

/* Unchanged complete C child leaves00477690 and00477EB0. */
uint8_t pharaoh_map_terrain_count_neighbours_with(
    const uint32_t *terrain, int32_t offset, uint32_t mask)
{
    static const int32_t delta[8] = {
        -229, -228, -227, -1, 1, 227, 228, 229
    };
    int32_t index;
    uint8_t count = 0;
    for (index = 0; index < 8; ++index)
        count += (terrain[offset + delta[index]] & mask) != 0;
    return count;
}

uint8_t pharaoh_map_terrain_mask_has_open_corner(
    const uint32_t *terrain, int32_t tile, uint32_t mask,
    uint8_t check_opposite_diagonals)
{
    const uint32_t northwest = terrain[tile - 229];
    const uint32_t north = terrain[tile - 228];
    const uint32_t northeast = terrain[tile - 227];
    const uint32_t west = terrain[tile - 1];
    const uint32_t east = terrain[tile + 1];
    const uint32_t southwest = terrain[tile + 227];
    const uint32_t south = terrain[tile + 228];
    const uint32_t southeast = terrain[tile + 229];

    if (check_opposite_diagonals != 0) {
        if ((mask & northwest & southeast) != 0) return 0;
        if ((mask & northeast & southwest) != 0) return 0;
    }
    if ((mask & (northwest | west | north)) == 0) return 1;
    if ((mask & (northeast | east | north)) == 0) return 1;
    if ((mask & (southwest | south | west)) == 0) return 1;
    return (mask & (southeast | south | east)) == 0;
}

void pharaoh_map_adjacent_flags_invert(uint32_t flags[8])
{
    int32_t index;
    for (index = 0; index < 8; ++index) flags[index] = flags[index] == 0;
}

void pharaoh_map_adjacent_flags_normalize(uint32_t flags[8])
{
    int32_t index;
    for (index = 0; index < 8; ++index) flags[index] = flags[index] != 0;
}

int32_t pharaoh_map_adjacent_flags_count(const uint32_t flags[8])
{
    int32_t index, count = 0;
    for (index = 0; index < 8; ++index) if (flags[index] != 0) ++count;
    return count;
}

/* Native004775F0; all eight neighbors, include any and exclude all. */
int32_t pharaoh_map_terrain_count_neighbours_matching(
    const uint32_t *terrain, int32_t offset,
    uint32_t required_mask, uint32_t forbidden_mask)
{
    static const int32_t delta[8] = {
        -228, -227, 1, 229, 228, 227, -1, -229
    };
    int32_t direction;
    int32_t count = 0;
    for (direction = 0; direction < 8; ++direction) {
        const uint32_t value = terrain[offset + delta[direction]];
        if ((value & required_mask) != 0 &&
            (value & forbidden_mask) == 0) {
            ++count;
        }
    }
    return count;
}
