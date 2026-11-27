#include "pharaoh/ui/construction_finish.h"
#include "pharaoh/core/static_objects.h"

PharaohPercentageScaleStatus pharaoh_construction_finish(
    const PharaohConstructionFinishContext *c)
{
    const PharaohConstructionInputContext *input=c->input;
    const PharaohConstructionCommitContext *commit=c->commit;
    if(*commit->basic->editor_active==1)
        pharaoh_map_routing_initialize_water(commit->routing);

    int32_t tool=*input->tool;
    if(tool==123 || tool==124 || tool==129) {
        pharaoh_editor_elevation_smooth_all(c->editor);
        *input->drag_started=0;
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    if(*input->drag_started==0)return PHARAOH_PERCENTAGE_SCALE_VALUE;

    const uint32_t selected=*c->selection_tile;
    *input->drag_started=0;
    if(selected==0) {
        if(*input->byte_605798==0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
        *c->selection_tile=*c->fallback_tile;
    }
    if(tool>0) {
        /* Native pushes4, but the proven33C0C3 leaf consumes no arguments. */
        (void)pharaoh_native_return_zero_0047d350();
        tool=*input->tool;
    }
    const int32_t y2=*input->end_y,y1=*input->start_y;
    const int32_t x2=*input->end_x,x1=*input->start_x;
    const int32_t orientation=*c->orientation;
    const uint32_t player=*commit->allocation->current_player;
    return pharaoh_construction_commit(commit,player,orientation,x1,y1,x2,y2,tool);
}
