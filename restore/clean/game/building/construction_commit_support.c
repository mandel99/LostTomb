#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_fill.h"

/* Complete MapCommitResourceGate C reuse, native004E2B10. */
void pharaoh_construction_warning_road_access(const PharaohConstructionAccessWarnings *c,
    int32_t tool,int32_t x,int32_t y,int32_t size)
{
    if(*c->editor_active==1)return;
    switch(tool){
    case 41:case 42:case 43:case 92:case 57:case 58:case 170:case 171:
    case 200:case 201:case 202:case 56:case 10:case 40:case 44:case 45:
    case 211:case 212:case 183:case 210:case 227:case 225:case 230:case 228:
    case 222:case 207:case 208:case 213:case 229:case 234:case 236:case 235:return;
    }
    if(pharaoh_map_has_road_access(c->roads,x,y,size))return;
    if((tool==72||tool==193)&&pharaoh_map_has_road_access(c->roads,x,y,3))return;
    if(tool>=65&&tool<=69)return;
    if(tool>=140&&tool<=149&&pharaoh_map_has_road_access_nearby(c->roads,x,y))return;
    if(*c->problem_messages>=3)return;
    pharaoh_city_warning_post(c->warnings,tool>=140&&tool<=149?207:11);
}

/* Complete MapCommitWarnings C reuse, native004E3280. */
void pharaoh_construction_warning_tower(const PharaohConstructionAccessWarnings *c,
    int32_t tool,int32_t x,int32_t y,int32_t layout)
{
    if(*c->editor_active==1)return;
    if(*c->warning_shown)return;
    if((tool==59||tool==172||tool==173)&&
        !pharaoh_map_pattern_has_mask_4000(c->map->terrain,c->map->origin,x,y,c->patterns,layout))
        pharaoh_city_warning_post(c->warnings,41);
}

/* Complete MapCommitState C reuse, native00485830. End-X is exclusive;
 * unlike painting/smoothing, this leaf does not change the shared cursor. */
void pharaoh_map_restore_free_tile_images(const PharaohMapSurfaceContext *c,
    const uint32_t *backup_images)
{
    uint32_t height=(uint32_t)c->map->height,row,first,last,tile,count;
    if((int32_t)height<=0)return;
    for(row=0;(int32_t)row<(int32_t)height;++row){
        first=(uint32_t)c->row_x_start[row];last=(uint32_t)c->row_x_end[row];
        tile=(uint32_t)c->row_start_offset[row];
        if((int32_t)first<(int32_t)last){
            count=last-first;
            do{
                if(!c->building_ids[tile])c->image_ids[tile]=backup_images[tile];
                ++tile;--count;
            }while(count);
        }
    }
}

/* Complete MapCommitWarnings C reuse, native00486020. Real post/capture
 * operations retain native ordering; the captured state is not re-read. */
void pharaoh_construction_ferry_landing_placed(uint32_t *ferry_state,
    const int32_t *building,PharaohCityWarningContext *warnings,PharaohMapBuildingUndo *undo)
{
    uint32_t mode=*ferry_state;
    if(!mode){*ferry_state=1;pharaoh_city_warning_post(warnings,67);}
    else{*ferry_state=0;if(mode==3)(void)pharaoh_map_building_undo_capture(undo,*building);}
}

void pharaoh_map_undo_release_reservations(PharaohMapBuildingUndo *undo,
    PharaohBuildingTypeRecord *live)
{
    _Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
        offsetof(PharaohBuildingTypeRecord,render_tint_bf)==0xbf,
        "native reservation record fields");
    const int32_t count = (int32_t)*undo->count;
    for (uint32_t i = 0; (int32_t)i < count; ++i) {
        const int32_t id = undo->ids[i];
        if (id != 0) {
            PharaohBuildingTypeRecord *b = &live[id];
            if (b->state == 6) b->state = 1;
            b->render_tint_bf = 0;
        }
    }
    *undo->count = 0;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)undo->backup, 0x21d68, 0);
    pharaoh_memory_fill_bytes((uint32_t *)(void *)undo->ids, 0x41a, 0);
}
