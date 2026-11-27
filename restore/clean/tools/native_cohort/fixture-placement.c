#include <stdint.h>
#include "pharaoh/monuments/placement.h"
#include "pharaoh/monuments/royal_tomb_queries.h"
#define PTR(type,n) ((type *)(uintptr_t)(uint32_t)(n))
#define I32(n) (*PTR(const int32_t,n))
typedef struct FixtureCase { uint32_t operation,selector,self,cell,pieces,count,terrain,part; } FixtureCase;
typedef uint8_t (*Check)(const PharaohMonumentPlacementContext *,const PharaohMonumentPlacementObject *,uint32_t);
typedef uint8_t (*Existing)(void *,const PharaohBuildingTypeRecord *,PharaohMonumentExistingProgress);
typedef struct Entry {Check check;Existing existing;uint32_t offset,predicate;} Entry;
static const Entry entries[]={
 {pharaoh_abu_simbel_can_place,pharaoh_abu_simbel_has_incomplete,0x4c,1},
 {pharaoh_library_can_place,pharaoh_library_has_incomplete,0x4c,0},
 {pharaoh_caesareum_can_place,pharaoh_caesareum_has_incomplete,0x4c,0},
 {pharaoh_lighthouse_can_place,pharaoh_lighthouse_has_incomplete,0x4c,3},
 {pharaoh_small_royal_tomb_can_place,pharaoh_small_royal_tomb_has_incomplete,0x1e5c,2},
 {pharaoh_grand_royal_tomb_can_place,pharaoh_grand_royal_tomb_has_incomplete,0x5c88,2},
 {pharaoh_large_royal_tomb_can_place,pharaoh_large_royal_tomb_has_incomplete,0x4ee0,2},
 {pharaoh_medium_royal_tomb_can_place,pharaoh_medium_royal_tomb_has_incomplete,0x1cbc,2}
};
static const PharaohMonumentPlacementPredicate predicates[]={pharaoh_monument_tile_is_free,
 pharaoh_abu_simbel_tile_is_free,pharaoh_royal_tomb_terrain_allowed,pharaoh_lighthouse_tile_is_free};
/* Actual base progress implementation, not an emulation endpoint substitute. */
static int32_t base_progress(void *self,int32_t id,int32_t mode)
{
 if(mode!=0)__debugbreak();
 return pharaoh_monument_no_progress(self,id,(int32_t *)0);
}
__declspec(dllexport) uint32_t __cdecl fixture_run(const FixtureCase *f)
{
 PharaohMapGridGeometry geometry;
 PharaohMonumentPlacementContext c;
 PharaohMonumentPlacementObject object;
 uint32_t predicate=f->selector;
 geometry.width=I32(0xbca2d0);geometry.height=I32(0xbca2d4);
 geometry.diamond_x_min=I32(0xbca2e8);geometry.diamond_y_min=I32(0xbca2ec);
 geometry.diamond_x_max=I32(0xbca2f0);geometry.diamond_y_max=I32(0xbca2f4);
 geometry.diamond_x_center=I32(0xbca2f8);geometry.diamond_y_center=I32(0xbca2fc);
 c.geometry=&geometry;c.terrain=PTR(const uint32_t,0xaf598c);
 c.origin=PTR(const int32_t,0xbca2d8);c.world_x=PTR(const int32_t,0xbca2e8);c.world_y=PTR(const int32_t,0xbca2ec);
 c.current_tile=PTR(uint32_t,0xbc9768);c.footprints=(const PharaohMapTileAreaOffset (*)[6])(uintptr_t)0x5cc8c0;
 object.parts=PTR(const PharaohMonumentImagePart,f->pieces);
 object.special_part=PTR(const uint32_t,f->self+0x50u);
 if(f->operation==6||f->operation==7){
  if(f->selector>=8)__debugbreak();
  predicate=entries[f->selector].predicate;
  object.parts=PTR(const PharaohMonumentImagePart,f->self+entries[f->selector].offset);
 }
 if(f->operation==5)predicate=2;
 if(predicate>=4)__debugbreak();
 object.predicate=predicates[predicate];
 switch(f->operation){
 case 0:return object.predicate(&object,f->terrain,(int32_t)f->part);
 case 4:return pharaoh_monument_placement_check(&c,&object,f->cell,PTR(const PharaohMonumentImagePart,f->pieces),(int32_t)f->count);
 case 5:return pharaoh_royal_tomb_placement_check(&c,&object,f->cell,PTR(const PharaohMonumentImagePart,f->pieces),(int32_t)f->count);
 case 6:return entries[f->selector].check(&c,&object,f->cell);
 case 7:return entries[f->selector].existing(PTR(void,f->self),PTR(const PharaohBuildingTypeRecord,0x7999d8),base_progress);
 case 8:return pharaoh_monument_object_has_incomplete(PTR(void,f->self),PTR(const PharaohBuildingTypeRecord,0x7999d8),(int32_t)f->terrain,(int32_t)f->count,base_progress);
 default:__debugbreak();return 0;
 }
}
