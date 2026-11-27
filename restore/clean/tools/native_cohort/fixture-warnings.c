#include <stdint.h>
#include "pharaoh/building/construction_resource_warnings.h"
#include "pharaoh/building/construction_commit_support.h"
#define PTR(t,a) ((t *)(uintptr_t)(uint32_t)(a))
#define I32(a) (*PTR(const int32_t,a))
typedef struct FixtureCase {uint32_t operation;int32_t tool,x,y,size,extra;} FixtureCase;
typedef void (*Warning)(const PharaohConstructionResourceWarnings *,int32_t);
static const Warning entries[]={
 pharaoh_construction_warning_brewery,
 pharaoh_construction_warning_weaver,
 pharaoh_construction_warning_jeweler,
 pharaoh_construction_warning_potter,
 pharaoh_construction_warning_papyrus,
 pharaoh_construction_warning_paint,
 pharaoh_construction_warning_cattle,
 pharaoh_construction_warning_carpenters,
 pharaoh_construction_warning_sandstone,
 pharaoh_construction_warning_bricks,
 pharaoh_construction_warning_lamps,
 pharaoh_construction_warning_senet,
 pharaoh_construction_warning_zoo,
 pharaoh_construction_warning_papyrus_users,
 pharaoh_construction_warning_mortuary,
 pharaoh_construction_warning_artisans,
 pharaoh_construction_warning_pyramid,
 pharaoh_construction_warning_ptolemaic,
 pharaoh_construction_warning_venue_schools,
};
__declspec(dllexport) uint32_t __cdecl fixture_run(const FixtureCase *f)
{
 PharaohCityWarningContext w={0};
 PharaohEmpireSupplyContext supply;
 PharaohConstructionResourceWarnings c;
 PharaohMapTerrainView map;
 PharaohRoadAccessContext road;
 PharaohConstructionAccessWarnings a;
 PharaohMapSurfaceContext surface={0};
 PharaohMapBuildingUndo undo;
 uint32_t answer=0;
 w.enabled=PTR(const uint8_t,0xe38ec1);w.construction_enabled=PTR(const uint8_t,0xe38dda);
 w.editor_active=PTR(const uint32_t,0x605a3c);w.orientation=PTR(const uint32_t,0xbca2e0);
 w.time=PTR(const uint32_t,0xe92e0c);w.state=PTR(uint32_t,0xf53f34);
 w.redraw=PTR(uint32_t,0xe94e20);w.warnings=PTR(PharaohCityWarning,0xe94e28);
 supply.cities=PTR(const PharaohEmpireCityRecord,0x77de00);
 supply.route_limits=PTR(const PharaohEmpireTradeAmounts,0x77646c);
 supply.allowed_buildings=PTR(const int16_t,0x784ce0);
 supply.players=PTR(PharaohScenarioPlayerRuntime,0xea1870);
 supply.current_player=PTR(const int32_t,0xeaac28);
 c.editor_active=w.editor_active;c.industry_active=PTR(const int32_t,0x78d64c);
 c.industry_total=PTR(const int32_t,0x78d6dc);c.building_totals=PTR(const int32_t,0x705af0);
 c.supply=&supply;c.objects=PTR(const PharaohEmpireObjectRecord,0x77f748);
 c.warnings=&w;c.definitions=PTR(const PharaohMonumentDefinition,0x5d19c8);
 c.goal_definition_ids=PTR(const int16_t,0x784de0);
 if(f->operation<19){entries[f->operation](&c,f->tool);return 0;}
 map.terrain=PTR(uint32_t,0xaf598c);map.origin=I32(0xbca2d8);
 map.width=I32(0xbca2d0);map.height=I32(0xbca2d4);
 road.grid_start=map.origin;road.terrain=map.terrain;road.building_ids=PTR(const int16_t,0xb4e6fc);
 road.buildings=PTR(const PharaohBuildingTypeRecord,0x7999d8);road.regions=PTR(const uint8_t,0xa76aec);
 road.ranks=PTR(const PharaohRoadNetworkRank,0xea5f1c+0x49d8u*(uint32_t)*supply.current_player);
 road.ring_offsets=PTR(const int32_t,0x5cc9e0);road.ring_xy=PTR(const int32_t,0x5cce98);
 road.result_x=I32(0xd3b4e4);road.result_y=I32(0xd3b4e0);
 a.editor_active=w.editor_active;a.warning_shown=w.redraw;a.problem_messages=PTR(const int32_t,0xe950d4);
 a.warnings=&w;a.roads=&road;a.map=&map;a.patterns=(const int32_t (*)[24])(uintptr_t)0x5cc9e0;
 undo.live=road.buildings;undo.backup=PTR(PharaohBuildingTypeRecord,0xba7888);
 undo.ids=PTR(int16_t,0xba746c);undo.count=PTR(uint32_t,0xbc95f0);undo.available=PTR(uint32_t,0xbc9694);
 switch(f->operation){
 case 19:pharaoh_construction_warning_road_access(&a,f->tool,f->x,f->y,f->size);break;
 case 20:pharaoh_construction_warning_tower(&a,f->tool,f->x,f->y,f->size);break;
 case 21:surface.map=&map;surface.row_x_start=PTR(const int32_t,0xbc9f24);
 surface.row_x_end=PTR(const int32_t,0xbc9b94);surface.row_start_offset=PTR(const int32_t,0xbc9804);
 surface.building_ids=PTR(int16_t,0xb4e6fc);surface.image_ids=PTR(uint32_t,0xb7482c);
 pharaoh_map_restore_free_tile_images(&surface,PTR(const uint32_t,0xa1126c));break;
 case 22:pharaoh_construction_ferry_landing_placed(PTR(uint32_t,0xbca310),PTR(const int32_t,0xbc9800),&w,&undo);break;
 case 23:answer=pharaoh_empire_our_city_sells_resource(c.objects,(uint32_t)f->tool);break;
 case 24:answer=pharaoh_empire_object_city_sells_resource(c.objects,f->x,(uint32_t)f->tool);break;
 case 25:answer=(uint32_t)pharaoh_map_pattern_has_mask_4000(map.terrain,map.origin,f->x,f->y,a.patterns,f->size);break;
 default:__debugbreak();break;
 }
 *PTR(int32_t,0xd3b4e4)=road.result_x;*PTR(int32_t,0xd3b4e0)=road.result_y;
 return answer;
}
