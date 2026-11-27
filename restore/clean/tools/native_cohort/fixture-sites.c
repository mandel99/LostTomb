#include <stdint.h>
#include "pharaoh/building/construction_site_checks.h"
#define PTR(t,a) ((t *)(uintptr_t)(uint32_t)(a))
#define I32(a) (*PTR(const int32_t,a))
typedef struct FixtureCase {uint32_t op;int32_t tool,x,y,size,extra;} FixtureCase;
__declspec(dllexport) uint32_t __cdecl fixture_run(const FixtureCase *f)
{
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
 calendar.scenario_start_year=PTR(const int16_t,0x7848d8);calendar.current_year=PTR(const int32_t,0x78d7bc);calendar.current_month=PTR(const int32_t,0x78d7c0);
 if(f->op==6)return pharaoh_construction_area_has_entry_point(&points,f->x,f->y,f->size,f->extra);
 if(f->op==7){
  const PharaohScenarioEvent *previous=f->x<0?(const PharaohScenarioEvent *)0:&PTR(PharaohScenarioEvent,0x785d74)[f->x];
  return (uint32_t)(uintptr_t)pharaoh_scenario_invasion_next(PTR(PharaohScenarioEvent,0x785d74),I32(0x785d70),previous,f->y!=0,PTR(uint8_t,0x78a634),PTR(int32_t,0x5d9a40));
 }
 if(f->op==8)return pharaoh_event_is_future_result(&PTR(PharaohScenarioEvent,0x785d74)[f->x],&calendar);
 if(f->op==9)return pharaoh_event_is_running(&PTR(PharaohScenarioEvent,0x785d74)[f->x],&calendar);
 if(f->op!=5){__debugbreak();return 0;}
 warning.enabled=PTR(const uint8_t,0xe38ec1);warning.construction_enabled=PTR(const uint8_t,0xe38dda);
 warning.editor_active=PTR(const uint32_t,0x605a3c);warning.orientation=PTR(const uint32_t,0xbca2e0);
 warning.time=PTR(const uint32_t,0xe92e0c);warning.state=PTR(uint32_t,0xf53f34);warning.redraw=PTR(uint32_t,0xe94e20);
 warning.warnings=PTR(PharaohCityWarning,0xe94e28);warning.selection=(PharaohTextSelection *)0;
 warning.font=(const PharaohBitmapFont *)0;warning.glyph_map=(const uint8_t *)0;warning.current_glyph=(uint32_t *)0;
 military.editor_active=warning.editor_active;military.invasion_warning_shown=PTR(const uint32_t,0xbcb9b4);
 military.events=PTR(PharaohScenarioEvent,0x785d74);military.event_count=PTR(const int32_t,0x785d70);
 military.current_player=PTR(const int32_t,0xeaac28);military.invasion_search_flag=PTR(uint8_t,0x78a634);
 military.current_event_index=PTR(int32_t,0x5d9a40);military.calendar=&calendar;
 military.players=PTR(const PharaohScenarioPlayerRuntime,0xea1870);military.warnings=&warning;
 pharaoh_construction_warning_military(&military,f->tool);return 0;
}
