#include <stdint.h>
#include "pharaoh/runtime/city_view.h"
#include "pharaoh/building/construction_site_checks.h"
#include "pharaoh/building/construction_basic_warnings.h"
#include "pharaoh/rendering/sprite_raw_blit.h"
#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/simulation/finance_calendar.h"
#define PTR(t,a) ((t *)(uintptr_t)(uint32_t)(a))
#define I32(a) (*PTR(const int32_t,a))
typedef struct FixtureCase {uint32_t op;int32_t tool,x,y,size,extra;} FixtureCase;
typedef uint8_t (*TerrainRule)(uint32_t);
static const TerrainRule terrain_rules[]={pharaoh_construction_terrain_rule_00405b40,
 pharaoh_construction_terrain_rule_00405b60,pharaoh_construction_terrain_rule_00405b80};
static uint32_t actual_region_rule(uint32_t terrain,void *selector)
{return terrain_rules[(uint32_t)(uintptr_t)selector](terrain);}
typedef void (*BasicWarning)(const PharaohConstructionBasicWarnings *,int32_t);
static const BasicWarning basic_warnings[]={pharaoh_construction_warning_workers,
 pharaoh_construction_warning_granary,pharaoh_construction_warning_booth,
 pharaoh_construction_warning_bandstand,pharaoh_construction_warning_pavilion,
 pharaoh_construction_warning_fort,pharaoh_construction_warning_recruiter};
/* Insert after FixtureCase/PTR/I32, before fixture_run. Do not initialize
 * native backing data here: the runner seeds both native and C identically.
 * Supported replay domain: preview !=0, rejected-site commit requests,
 * direct SiteValid, and SelectSlot primary/secondary30/31/33.
 * Successful commit is also wired for dry0/road40 terrain, zero transient
 * pointers, no pending pyramid queue, valid even view0..6 and native venue
 * definitions. Allocation failure requires the real warning-disabled gate
 * (E38EC1==0 or E38DDA==0); no font/renderer is substituted. No replacement
 * endpoint or callback is installed. See the companion contract. */
#include "pharaoh/building/entertainment_construction.h"

/* Shared native/C out arguments for op35; watch these eight bytes only,
 * rather than derived C-context storage in20100000+. */
#define ENTERTAINMENT_OUT_X 0x200f0100u
#define ENTERTAINMENT_OUT_Y 0x200f0104u

uint32_t fixture_entertainment(const FixtureCase *f)
{
    PharaohMapTerrainView *map = PTR(PharaohMapTerrainView,0x20100000);
    PharaohMapSurfaceContext *surface = PTR(PharaohMapSurfaceContext,0x20101000);
    PharaohBuildingAllocationContext *allocation = PTR(PharaohBuildingAllocationContext,0x20102000);
    PharaohVenueTileContext *venue = PTR(PharaohVenueTileContext,0x20103000);
    PharaohConstructionUndoContext *backup = PTR(PharaohConstructionUndoContext,0x20104000);
    PharaohEntertainmentConstructionContext *c = PTR(PharaohEntertainmentConstructionContext,0x20105000);
    PharaohMapGridGeometry *geometry = PTR(PharaohMapGridGeometry,0x20106000);
    PharaohMapBuildingUndo *undo = PTR(PharaohMapBuildingUndo,0x20107000);
    PharaohCityWarningContext *warnings = PTR(PharaohCityWarningContext,0x20108000);
    PharaohBuildingAllocationDefinition *definitions = PTR(PharaohBuildingAllocationDefinition,0x20109000);

    *map = (PharaohMapTerrainView){0};
    map->terrain = PTR(uint32_t,0xaf598c);
    map->origin = I32(0xbca2d8);
    map->width = I32(0xbca2d0);
    map->height = I32(0xbca2d4);
    *geometry = (PharaohMapGridGeometry){0};
    geometry->width = map->width;geometry->height = map->height;
    geometry->diamond_x_min = I32(0xbca2e8);geometry->diamond_y_min = I32(0xbca2ec);
    geometry->diamond_x_max = I32(0xbca2f0);geometry->diamond_y_max = I32(0xbca2f4);
    geometry->diamond_x_center = I32(0xbca2f8);geometry->diamond_y_center = I32(0xbca2fc);

    *surface = (PharaohMapSurfaceContext){0};
    surface->map = map;
    surface->geometry = geometry;
    surface->border_size = PTR(const int32_t,0xbca2dc);
    surface->flood_tiles = PTR(PharaohFloodTile *const,0xd05720);
    surface->building_ids = PTR(int16_t,0xb4e6fc);
    surface->buildings = PTR(PharaohBuildingTypeRecord,0x7999d8);
    surface->orientation = PTR(int32_t,0xbca2e0);
    surface->building_orientation = PTR(const uint32_t,0xe94e04);
    surface->groups = PTR(const PharaohSpriteGroupRecord,0xf55bb0);
    surface->image_ids = PTR(uint32_t,0xb7482c);
    surface->property_flags = PTR(uint8_t,0xb285cc);
    surface->edge_flags = PTR(uint8_t,0xb67d1c);
    surface->row_x_start = PTR(const int32_t,0xbc9f24);
    surface->row_x_end = PTR(const int32_t,0xbc9b94);
    surface->row_start_offset = PTR(const int32_t,0xbc9804);
    surface->current_tile = PTR(uint32_t,0xbc9768);
    surface->random = PTR(const uint8_t,0xacf85c);
    surface->footprints = (const PharaohMapTileAreaOffset (*)[6])(uintptr_t)0x5cc8c0;
    surface->placement_failed = PTR(uint32_t,0xbc9734);
    surface->blocked_grid = PTR(const int16_t,0xb350dc);
    surface->pyramid_queue_count = PTR(uint32_t,0xeaaf64);
    surface->pyramid_queue = PTR(uint32_t,0xeaac30);
    surface->pyramid_style = PTR(const uint32_t,0x5e4ef4);
    surface->pyramid_complex_offsets = PTR(const int32_t,0x5f5d08);

    *undo = (PharaohMapBuildingUndo){0};
    undo->live = surface->buildings;
    undo->backup = PTR(PharaohBuildingTypeRecord,0xba7888);
    undo->ids = PTR(int16_t,0xba746c);
    undo->count = PTR(uint32_t,0xbc95f0);
    undo->available = PTR(uint32_t,0xbc9694);
    surface->undo = undo;

    *warnings = (PharaohCityWarningContext){0};
    warnings->enabled = PTR(const uint8_t,0xe38ec1);
    warnings->construction_enabled = PTR(const uint8_t,0xe38dda);
    warnings->editor_active = PTR(const uint32_t,0x605a3c);
    warnings->orientation = PTR(const uint32_t,0xbca2e0);
    warnings->time = PTR(const uint32_t,0xe92e0c);
    warnings->state = PTR(uint32_t,0xf53f34);
    warnings->redraw = PTR(uint32_t,0xe94e20);
    warnings->warnings = PTR(PharaohCityWarning,0xe94e28);

    /* Only the context is normalized. Its values come from the actual native
     *20-byte rows, not synthetic definition tables or replacement children. */
    for (uint32_t i=0;i<237;++i) {
        definitions[i].size = *PTR(const uint8_t,0x5d06dc+i*20u);
        definitions[i].native_04 = *PTR(const uint8_t,0x5d06e0+i*20u);
    }

    *allocation = (PharaohBuildingAllocationContext){0};
    allocation->surface = surface;
    allocation->current_player = PTR(const uint32_t,0xeaac28);
    allocation->players = PTR(const PharaohScenarioPlayerRuntime,0xea1870);
    allocation->definitions = definitions;
    allocation->water_patterns = (const int32_t (*)[24])(uintptr_t)0x5cc9e0;
    allocation->rotation = PTR(const uint32_t,0xbc9714);
    allocation->next_serial = PTR(uint32_t,0x78d7b4);
    allocation->highest_id = PTR(uint32_t,0x78d790);
    allocation->ferry_state = PTR(uint32_t,0xbca310);
    allocation->first_ferry = PTR(uint32_t,0xbc9800);
    allocation->undo.ready = PTR(const int32_t,0xbc968c);
    allocation->undo.mode = PTR(const int32_t,0xbc9694);
    allocation->undo.count = PTR(const int32_t,0xbc95f0);
    allocation->undo.ids = PTR(const int16_t,0xba746c);
    allocation->warnings = warnings;

    *venue = (PharaohVenueTileContext){0};
    venue->cleared = PTR(uint8_t,0xae8e7c);
    *backup = (PharaohConstructionUndoContext){0};
    backup->saved_terrain = PTR(uint32_t,0x9de62c);
    backup->saved_images = PTR(uint32_t,0xa1126c);
    backup->saved_bytes[0] = PTR(uint8_t,0x9d1b1c);
    backup->saved_bytes[1] = PTR(uint8_t,0x9b84fc);
    backup->saved_bytes[2] = PTR(uint8_t,0x9ab9ec);

    *c = (PharaohEntertainmentConstructionContext){0};
    c->allocation = allocation;
    c->venue_tiles = venue;
    c->backup = backup;
    c->players = PTR(PharaohEntertainmentPlayer,0xea1870);
    c->selection_x = PTR(const uint32_t,0xbca304);
    c->selection_y = PTR(const uint32_t,0xbca308);
    c->occupancy = PTR(uint32_t,0x775614);
    c->bandstand_ranks = PTR(const int32_t,0x5d2960);
    c->pavilion_ranks = PTR(const int32_t,0x5d2984);
    c->patterns_by_size[0] = PTR(const uint8_t,0x5d2924);
    c->patterns_by_size[1] = PTR(const uint8_t,0x5d2928);
    c->patterns_by_size[2] = PTR(const uint8_t,0x5d2934);
    c->patterns_by_size[3] = PTR(const uint8_t,0x5d2944);
    c->placed_count = PTR(uint32_t,0xbc971c);

    switch (f->op) {
    case 30:
        return pharaoh_entertainment_build_booth(c,(uint32_t)f->size,f->x,f->y);
    case 31:
        return pharaoh_entertainment_build_bandstand(c,(uint32_t)f->size,f->x,f->y);
    case 32:
        return pharaoh_entertainment_build_pavilion(c,(uint32_t)f->size,f->x,f->y);
    case 33:
        return pharaoh_entertainment_build_festival_square(c,(uint32_t)f->size,f->x,f->y);
    case 34:
        return pharaoh_entertainment_site_valid(c,(PharaohEntertainmentKind)f->tool);
    case 35:
        return pharaoh_entertainment_select_slot(c,f->x,f->y,
            PTR(int32_t,ENTERTAINMENT_OUT_X),PTR(int32_t,ENTERTAINMENT_OUT_Y),
            (PharaohEntertainmentKind)f->tool,(PharaohEntertainmentKind)f->extra);
    default:
        return 0; /* Dispatcher contract: only operations30..35 reach here. */
    }
}

__declspec(dllexport) uint32_t __cdecl fixture_run(const FixtureCase *f)
{
 if(f->op>=30&&f->op<=35)return fixture_entertainment(f);
 if(f->op==29){
  PharaohCityViewState v={0};
  PharaohCityProjectionLookup *p=PTR(PharaohCityProjectionLookup,0x20100000);
  p->projected_minimum_x=I32(0x70e428);p->projected_maximum_x=I32(0x70e420);
  p->projected_minimum_y=I32(0x70e42c);p->projected_maximum_y=I32(0x70e424);
  v.viewport.width_tiles=I32(0x70e464);v.viewport.height_tiles=I32(0x70e460);
  v.camera_x=I32(0x70e478);v.camera_y=I32(0x70e474);
  pharaoh_minimap_calculate_projection_origin(&v,p,f->x,f->y,PTR(int32_t,0x70e3dc),PTR(int32_t,0x70e3d8));return 0;
 }
 if(f->op==36){
  PharaohMapTerrainView region={0};region.terrain=PTR(uint32_t,0xaf598c);
  region.origin=I32(0xbca2d8);region.width=I32(0xbca2d0);region.height=I32(0xbca2d4);
  return pharaoh_map_region_terrain_count_reached(&region,f->x,f->y,f->size,f->extra,
   (uint32_t)f->tool,I32(0x200f0018),PTR(int32_t,0xd3b4e4),PTR(int32_t,0xd3b4e0));
 }
 PharaohMapGridGeometry geo;
 PharaohMapTerrainView map;
 PharaohMonumentPlacementContext placement;
 PharaohMapSurfaceContext surface;
 PharaohConstructionEntryPoints points;
 PharaohTempleComplexSiteContext temple;
 PharaohMausoleumProjectionContext projection;
 PharaohComplexProgressContext progress;
 PharaohEventCalendarView calendar;
 PharaohCityWarningContext warning;
 PharaohConstructionMilitaryWarnings military;
 int32_t ids[13];const int16_t *targets[13];
 geo.width=I32(0xbca2d0);geo.height=I32(0xbca2d4);
 geo.diamond_x_min=I32(0xbca2e8);geo.diamond_y_min=I32(0xbca2ec);
 geo.diamond_x_max=I32(0xbca2f0);geo.diamond_y_max=I32(0xbca2f4);
 geo.diamond_x_center=I32(0xbca2f8);geo.diamond_y_center=I32(0xbca2fc);
 map.terrain=PTR(uint32_t,0xaf598c);map.origin=I32(0xbca2d8);map.width=geo.width;map.height=geo.height;
 placement.geometry=&geo;placement.terrain=map.terrain;placement.origin=PTR(const int32_t,0xbca2d8);
 placement.world_x=PTR(const int32_t,0xbca2e8);placement.world_y=PTR(const int32_t,0xbca2ec);
 placement.current_tile=PTR(uint32_t,0xbc9768);placement.footprints=(const PharaohMapTileAreaOffset (*)[6])(uintptr_t)0x5cc8c0;
 for(int32_t i=0;i<4;++i){
  points.primary_x[i]=PTR(const int16_t,0x784b64)[i];points.primary_y[i]=PTR(const int16_t,0x784b6c)[i];
  points.secondary_x[i]=PTR(const int32_t,0x784cc0)[i];points.secondary_y[i]=PTR(const int32_t,0x784cd0)[i];
 }
 if(f->op==0){
  surface.map=&map;surface.geometry=&geo;surface.current_tile=placement.current_tile;
  surface.footprints=placement.footprints;surface.blocked_grid=PTR(const int16_t,0xb350dc);
  surface.image_ids=PTR(uint32_t,0xb7482c);surface.flood_tiles=PTR(PharaohFloodTile *const,0xd05720);
  return pharaoh_gatehouse_site_fits(&surface,PTR(const PharaohGatehouseImagePart,0x5df810),f->x,f->y,f->size);
 }
 if(f->op==1){
  temple.map=&map;temple.walker_ids=PTR(const int16_t,0xb350dc);temple.entry_points=&points;
  temple.orientation=PTR(const int32_t,0xbca2e0);temple.construction_orientation=PTR(const int32_t,0xbc9714);
  return (uint32_t)pharaoh_temple_complex_can_place(&temple,f->x,f->y);
 }
 if(f->op==2){
  projection.initial_parts=PTR(const PharaohMausoleumPartDescriptor,0x5e2ad8);
  projection.projected_parts=(PharaohMausoleumPartDescriptor *)0;projection.projection_scale_x=0;projection.projection_scale_y=0;
  return pharaoh_mausoleum_can_place(&placement,&projection,PTR(const int32_t,0xbca2e0),(uint32_t)f->extra);
 }
 if(f->op==3)return pharaoh_sun_temple_can_place(&placement,PTR(const PharaohMonumentImagePart,0x601dd8),(uint32_t)f->extra);
 if(f->op==4){
  progress.buildings=PTR(PharaohBuildingTypeRecord,0x7999d8);progress.walkers=PTR(const PharaohWalkerQueryRecord,0xbcd5f8);
  progress.sun_temple_parts=PTR(const PharaohComplexPartDefinition,0x601de4);
  progress.mausoleum_parts=(const PharaohComplexPartDefinition *)0;
  for(int32_t i=0;i<13;++i){ids[i]=I32(0x596084+16*i);targets[i]=PTR(const int16_t,I32(0x596088+16*i));}
  progress.reference_ids=ids;progress.reference_targets=targets;
  return pharaoh_sun_temple_has_completed(&progress);
 }
 if(f->op>=18&&f->op<=20)return terrain_rules[f->op-18]((uint32_t)f->tool);
 if(f->op==21){pharaoh_graphics_and_bytes(PTR(uint32_t,0x20000000u+(uint32_t)f->y),f->x,(uint8_t)f->tool);return 0;}
 if(f->op==22)return (uint32_t)pharaoh_city_buildings_has_treasury(PTR(const PharaohBuildingTypeRecord,0x7999d8),I32(0x78d790));
 if(f->op==23)return (uint32_t)pharaoh_building_type_is_0046fde0_group(f->tool);
 if(f->op==24)return (uint32_t)pharaoh_map_region_matches_predicate(&map,f->x,f->y,f->size,f->extra,
     actual_region_rule,(void *)(uintptr_t)(uint32_t)f->tool,PTR(int32_t,0xd3b4e4),PTR(int32_t,0xd3b4e0));
 if(f->op==25){pharaoh_game_undo_finish_build((uint32_t)I32(0xbc9728),PTR(uint32_t,0xbc9684),PTR(int32_t,0xbc9688),PTR(int32_t,0xbc968c),PTR(uint8_t,0xe38dca));return 0;}
 if(f->op==26)return (uint32_t)pharaoh_walker_enemy_near_building_site(PTR(const PharaohWalkerQueryRecord,0xbcd5f8),f->x,f->y,f->size,f->extra);
 if(f->op==27){
  uint32_t base=0xea1870u+(uint32_t)f->tool*0x49d8u;
  uint8_t active=*PTR(const uint8_t,base+2);int32_t editor=I32(0x605a3c);
  PharaohFinanceLedger ledger;ledger.treasury=I32(base+12);ledger.construction_current_year=(uint32_t)I32(base+0x2c8c);
  pharaoh_finance_process_construction(&ledger,active,editor,f->x);
  if(active==1&&editor!=1){*PTR(int32_t,base+12)=ledger.treasury;*PTR(uint32_t,base+0x2c8c)=ledger.construction_current_year;}
  return 0;
 }
 if(f->op==28)return (uint32_t)pharaoh_building_stores_treasury_funds(PTR(const PharaohBuildingTypeRecord,0x7999d8),f->x);
 calendar.scenario_start_year=PTR(const int16_t,0x7848d8);calendar.current_year=PTR(const int32_t,0x78d7bc);calendar.current_month=PTR(const int32_t,0x78d7c0);
 if(f->op==6)return pharaoh_construction_area_has_entry_point(&points,f->x,f->y,f->size,f->extra);
 if(f->op==7){
  const PharaohScenarioEvent *previous=f->x<0?(const PharaohScenarioEvent *)0:&PTR(PharaohScenarioEvent,0x785d74)[f->x];
  return (uint32_t)(uintptr_t)pharaoh_scenario_invasion_next(PTR(PharaohScenarioEvent,0x785d74),I32(0x785d70),previous,f->y!=0,PTR(uint8_t,0x78a634),PTR(int32_t,0x5d9a40));
 }
 if(f->op==8)return pharaoh_event_is_future_result(&PTR(PharaohScenarioEvent,0x785d74)[f->x],&calendar);
 if(f->op==9)return pharaoh_event_is_running(&PTR(PharaohScenarioEvent,0x785d74)[f->x],&calendar);
 if(f->op!=5&&(f->op<10||f->op>17)){__debugbreak();return 0;}
 warning.enabled=PTR(const uint8_t,0xe38ec1);warning.construction_enabled=PTR(const uint8_t,0xe38dda);
 warning.editor_active=PTR(const uint32_t,0x605a3c);warning.orientation=PTR(const uint32_t,0xbca2e0);
 warning.time=PTR(const uint32_t,0xe92e0c);warning.state=PTR(uint32_t,0xf53f34);warning.redraw=PTR(uint32_t,0xe94e20);
 warning.warnings=PTR(PharaohCityWarning,0xe94e28);warning.selection=(PharaohTextSelection *)0;
 warning.font=(const PharaohBitmapFont *)0;warning.glyph_map=(const uint8_t *)0;warning.current_glyph=(uint32_t *)0;
 if(f->op>=10&&f->op<=17){
  PharaohConstructionBasicWarnings basic;
  basic.editor_active=warning.editor_active;basic.warnings=&warning;basic.map=&map;
  basic.models=PTR(const PharaohBuildingModelRecord,0x705ea4);basic.players=PTR(const PharaohScenarioPlayerRuntime,0xea1870);
  basic.current_player=PTR(const int32_t,0xeaac28);basic.granary_count=PTR(const int32_t,0x705854);
  basic.booth_count=PTR(const int32_t,0x7057cc);basic.bandstand_count=PTR(const int32_t,0x7057c4);
  basic.pavilion_count=PTR(const int32_t,0x7057c8);basic.fort_count=PTR(const int32_t,0x7058b8);
  if(f->op==17)pharaoh_construction_warning_well(&basic,f->tool,f->x,f->y);
  else basic_warnings[f->op-10](&basic,f->tool);
  return 0;
 }
 military.editor_active=warning.editor_active;military.invasion_warning_shown=PTR(const uint32_t,0xbcb9b4);
 military.events=PTR(PharaohScenarioEvent,0x785d74);military.event_count=PTR(const int32_t,0x785d70);
 military.current_player=PTR(const int32_t,0xeaac28);military.invasion_search_flag=PTR(uint8_t,0x78a634);
 military.current_event_index=PTR(int32_t,0x5d9a40);military.calendar=&calendar;
 military.players=PTR(const PharaohScenarioPlayerRuntime,0xea1870);military.warnings=&warning;
 pharaoh_construction_warning_military(&military,f->tool);return 0;
}
