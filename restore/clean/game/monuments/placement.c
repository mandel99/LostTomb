#include "pharaoh/monuments/placement.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohMonumentImagePart)==52 &&
    offsetof(PharaohMonumentImagePart,kind)==12 && sizeof(PharaohMapTileAreaOffset)==8,
    "native monument descriptor and square-offset strides");

void pharaoh_monument_placement_bind_surface(PharaohMonumentPlacementContext *c,
    const PharaohMapSurfaceContext *surface,const int32_t *world_x,const int32_t *world_y)
{
    c->geometry=surface->geometry;c->terrain=surface->map->terrain;
    c->origin=&surface->map->origin;c->current_tile=surface->current_tile;
    c->footprints=surface->footprints;c->world_x=world_x;c->world_y=world_y;
}

/* Actual native004E5310,00401440,00547DE0,004E6620 predicates. */
uint8_t pharaoh_monument_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part)
{
    (void)self;(void)part;
    return (terrain&0xfeffd76fu)==0;
}
uint8_t pharaoh_abu_simbel_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part)
{
    uint32_t bits=terrain&0xfeffd76fu;
    return self->parts[part].kind==9?bits==0:bits==0x20000002u;
}
uint8_t pharaoh_royal_tomb_terrain_allowed(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part)
{
    uint32_t bits=terrain&0xfeffd76fu;
    return (uint32_t)part==*self->special_part?bits==0:bits==0x20000002u;
}
uint8_t pharaoh_lighthouse_tile_is_free(const PharaohMonumentPlacementObject *self,
    uint32_t terrain,int32_t part)
{
    uint32_t bits=terrain&0xfeffd76fu;
    (void)self;
    return part==1?bits==0:bits==2;
}

/* Complete shared area algorithm reused from GeneralPlacement. Live globals
 * and part size are reloaded across actual calls, including failed adjacency. */
static uint8_t check_area(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,const PharaohMonumentImagePart *piece,
    uint32_t index,uint32_t x,uint32_t y,uint8_t accepted)
{
    uint32_t row=0,col,tile,terrain;
    *c->current_tile=(uint32_t)*c->origin+y*228u+x;
    while((int32_t)row<piece->size&&accepted){
        col=0;
        while((int32_t)col<piece->size&&accepted){
            if(pharaoh_map_grid_size_reaches_outside(c->geometry,
                (int32_t)((uint32_t)*c->world_x+col+x),
                (int32_t)((uint32_t)*c->world_y+row+y),3))accepted=0;
            else{
                tile=*c->current_tile+c->footprints[row][col].grid_offset;
                terrain=c->terrain[tile];
                accepted=self->predicate(self,terrain,(int32_t)index);
            }
            ++col;
        }
        ++row;
    }
    return accepted;
}

/* Native004E5330; the original count/index upper EAX is not logical AL. */
uint8_t pharaoh_monument_placement_check(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell,
    const PharaohMonumentImagePart *pieces,int32_t count)
{
    int32_t delta=(int32_t)(cell-(uint32_t)*c->origin);
    uint32_t x=(uint32_t)(delta%228),y=(uint32_t)(delta/228),index=0;
    uint8_t accepted=1;
    if(count<=0)return 1;
    do{
        if(!accepted)break;
        accepted=check_area(c,self,pieces,index,(uint32_t)pieces->dx+x,
            (uint32_t)pieces->dy+y,accepted);
        ++index;++pieces;
    }while((int32_t)index<count);
    return accepted;
}

/* Native005463E0. All four real virtual tests execute, including after a hit.
 * A failed special-piece adjacency still writes that piece's current tile. */
uint8_t pharaoh_royal_tomb_placement_check(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell,
    const PharaohMonumentImagePart *pieces,int32_t count)
{
    static const int32_t neighbors[4]={-228,228,-1,1};
    int32_t delta=(int32_t)(cell-(uint32_t)*c->origin);
    uint32_t x=(uint32_t)(delta%228),y=(uint32_t)(delta/228),index=0,px,py;
    uint8_t accepted=1;
    if(count<=0)return 1;
    do{
        if(!accepted)break;
        px=(uint32_t)pieces->dx+x;py=(uint32_t)pieces->dy+y;
        if(index==*self->special_part){
            uint32_t side,center,terrain;
            uint8_t adjacent=0,last=0;
            for(side=0;side<4;++side){
                center=(uint32_t)*c->origin+py*228u+px;
                terrain=c->terrain[center+(uint32_t)neighbors[side]];
                last=self->predicate(self,terrain,(int32_t)index);
                if(side<3&&last)adjacent=1;
            }
            if(!last&&!adjacent)accepted=0;
        }
        accepted=check_area(c,self,pieces,index,px,py,accepted);
        ++index;++pieces;
    }while((int32_t)index<count);
    return accepted;
}

/* Native00401400; reuse the real shared C query. */
uint8_t pharaoh_abu_simbel_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_monument_placement_check(c,self,cell,self->parts,16);
}

/* Native00401430: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_abu_simbel_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xe6,16,progress);
}

/* Native0040D850; reuse the real shared C query. */
uint8_t pharaoh_library_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_monument_placement_check(c,self,cell,self->parts,25);
}

/* Native0040D880: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_library_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xe1,25,progress);
}

/* Native00416F70; reuse the real shared C query. */
uint8_t pharaoh_caesareum_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_monument_placement_check(c,self,cell,self->parts,9);
}

/* Native00416FA0: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_caesareum_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xe3,9,progress);
}

/* Native004E65F0; reuse the real shared C query. */
uint8_t pharaoh_lighthouse_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_monument_placement_check(c,self,cell,self->parts,2);
}

/* Native004E6650: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_lighthouse_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xe4,2,progress);
}

/* Native005456C0; reuse the real shared C query. */
uint8_t pharaoh_small_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_royal_tomb_placement_check(c,self,cell,self->parts,167);
}

/* Native00545700: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_small_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xe5,167,progress);
}

/* Native0054F3F0; reuse the real shared C query. */
uint8_t pharaoh_grand_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_royal_tomb_placement_check(c,self,cell,self->parts,513);
}

/* Native0054F430: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_grand_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xec,513,progress);
}

/* Native00555E00; reuse the real shared C query. */
uint8_t pharaoh_large_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_royal_tomb_placement_check(c,self,cell,self->parts,437);
}

/* Native00555E40: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_large_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xeb,437,progress);
}

/* Native00558B60; reuse the real shared C query. */
uint8_t pharaoh_medium_royal_tomb_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentPlacementObject *self,uint32_t cell)
{
    return pharaoh_royal_tomb_placement_check(c,self,cell,self->parts,158);
}

/* Native00558BA0: native scan count is the fixed part count, not4000. */
uint8_t pharaoh_medium_royal_tomb_has_incomplete(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohMonumentExistingProgress progress)
{
    return pharaoh_monument_object_has_incomplete(object,buildings,0xea,158,progress);
}
