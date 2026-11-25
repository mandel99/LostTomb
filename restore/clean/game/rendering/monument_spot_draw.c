#include "pharaoh/rendering/monument_spot_draw.h"
#include "pharaoh/rendering/monument_scene_data.h"
#include "pharaoh/monuments/stages.h"

static int32_t add_i32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}

static void draw_group(PharaohMonumentSpotGroupDrawContext *c,
    const PharaohMonumentWorkSpotBinding *bindings,int32_t count,
    int32_t origin_x,int32_t origin_y)
{
    for(int32_t i=0;i<count;++i) {
        int32_t x,y,image;
        pharaoh_monument_spot_get_draw_info(c->draw,bindings[i].spot,
            &x,&y,&image);
        if(image!=0)c->operations->draw_image(c->operations->user_data,image,
            add_i32(origin_x,x),add_i32(origin_y,y));
    }
}

/* Complete 22-entry one-callee C translation of Pharaoh.exe 0x0041FFF0. */
void pharaoh_sphinx_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{draw_group(c,c->sphinx,22,x,y);}
/* Complete 13-entry one-callee C translation of Pharaoh.exe 0x00420050. */
void pharaoh_sun_temple_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{draw_group(c,c->sun_temple,13,x,y);}
/* Complete 27-entry translation of Pharaoh.exe 0x004206F0. */
void pharaoh_library_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{
    const int32_t variant=(c->draw->map_orientation/2)&1;
    if(variant!=pharaoh_library_spot_table_variant(
            *c->library_position_variant)) {
        pharaoh_monument_scene_library_positions_set_variant(
            variant,c->library_position_variant);
    }
    draw_group(c,c->library,27,x,y);
}
/* Complete 56-entry one-callee C translation of Pharaoh.exe 0x00420A30. */
void pharaoh_abu_simbel_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{draw_group(c,c->abu_simbel,56,x,y);}
/* Complete 27-entry one-callee C translation of Pharaoh.exe 0x00420D60. */
void pharaoh_caesareum_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{draw_group(c,c->caesareum,27,x,y);}
/* Complete 15-entry one-callee C translation of Pharaoh.exe 0x00421060. */
void pharaoh_lighthouse_draw_work_spots(PharaohMonumentSpotGroupDrawContext *c,
    int32_t x,int32_t y)
{draw_group(c,c->lighthouse,15,x,y);}
