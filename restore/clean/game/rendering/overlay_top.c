#include "pharaoh/rendering/overlay_top.h"
#include "pharaoh/rendering/upper_sprite_draw.h"
#include "pharaoh/runtime/platform_measure_speed.h"
static int32_t wrap_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t wrap_sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int16_t animation_frames(PharaohOverlayTopContext *c,int32_t image){return pharaoh_sprite_image_record(c->top->draw->images,*c->top->draw->image_count,image)->animation_frames;}
static void draw_image_top(PharaohOverlayTopContext *c,int32_t image,int32_t x,int32_t y){PharaohImageDrawContext *d=c->top->draw;const PharaohUpperSpriteDrawContext top={d->images,d->image_pixels,*d->image_count,d->clip,d->surface_pixels,d->pitch_bytes,*d->tint,&d->state->source.pixels};(void)pharaoh_image_draw_isometric_top(&top,image,x,y);}
static int32_t footprint_offset(PharaohOverlayTopContext *c,int32_t size,int32_t *dx,int32_t *dy){
 const PharaohCityGroundScan *s=c->top->ground->scan;
 if((uint32_t)size>5u)return 0;
 if(size==0){*dx=0;*dy=0;return 1;}
 if(size<5){*dx=(int32_t)(s->tile_x[4-size]+(uint32_t)size);*dy=(int32_t)s->tile_y[4-size];return 1;}
 *dx=(int32_t)(s->tile_x[0]+s->tile_x[3]+5u);*dy=(int32_t)(s->tile_y[0]+s->tile_y[3]);return 1;
}
static void draw_storage_roof(PharaohOverlayTopContext *c,int32_t image,int32_t x,int32_t y){(void)pharaoh_image_draw(c->top->draw,image,wrap_add(x,-4),wrap_add(y,-42));}
enum OverlayTopWalk {
    TopRowsBefore = 8,
    TopRowsExtra = 18,
    TopColsBefore = 4,
    TopColsExtra = 6,
    TopHalfTilesAbove = 9,
    TopTileGapX = 2,
    TopNoHighlight = 9999,
};

enum OverlayTopTile {
    TopSizeMask = 0xF,
    TombGroundRaisedFirst = 48,
    TombGroundRaisedEnd = 72,
    TombGroundRaise = 150,
    VenueTopTerrain = 0x40u | 0x20u,
};

static int32_t RowStartX(PharaohOverlayTopContext *c,int32_t odd)
{
 uint32_t x=c->top->ground->scan->start_x;
 if(odd)x-=c->top->ground->scan->tile_x[3]+1u;
 return (int32_t)(x-(2u+c->top->ground->scan->tile_x[2])*4u);
}

static int32_t IsTomb(PharaohOverlayTopContext *c,int32_t type)
{
    (void)c;
    return type == 229 || type == 234 || type == 235 || type == 236 ||
        type == 230;
}

static int32_t IsVenue(PharaohOverlayTopContext *c,int32_t type)
{
    (void)c;
    return type == 33 || type == 30 || type == 31;
}

static int32_t DrawRaisedTombGround(PharaohOverlayTopContext *c,int32_t x, int32_t y)
{
    (*c->top->ground->current_image) = c->top->ground->images[c->top->ground->scan->tile];
    if ((*c->top->ground->current_image) < pharaoh_sprite_group_first_image(c->top->animations->groups,743) + TombGroundRaisedFirst || (*c->top->ground->current_image) >= pharaoh_sprite_group_first_image(c->top->animations->groups,743) + TombGroundRaisedEnd) {
        return 0;
    }
    pharaoh_image_draw(c->top->draw,(*c->top->ground->current_image), x, wrap_sub(y,TombGroundRaise));
    draw_image_top(c,(*c->top->ground->current_image), x, wrap_sub(y,TombGroundRaise));
    return 1;
}

static void DrawTopBySize(PharaohOverlayTopContext *c,int32_t x, int32_t y, int32_t largerAtTile)
{
    int32_t dx;
    int32_t dy;
    if (footprint_offset(c,c->top->ground->footprints[c->top->ground->scan->tile] & TopSizeMask, &dx, &dy)) {
        draw_image_top(c,(*c->top->ground->current_image), wrap_add(x,dx),wrap_sub(y,dy));
    } else if (largerAtTile) {
        draw_image_top(c,(*c->top->ground->current_image), x, y);
    }
}

static void DrawWalkers(PharaohOverlayTopContext *c,int32_t oddRow, int32_t y)
{
    int32_t x = RowStartX(c,oddRow);
    int32_t row = c->top->ground->scan->row;
    int32_t col = c->top->ground->scan->camera_column - TopColsBefore;
    for (int32_t colNumber = 0; colNumber <= (int32_t)(c->top->ground->scan->visible_columns + TopColsExtra); colNumber++) {
        if (col >= 0 && col < 231) {
            c->top->ground->scan->tile = c->top->ground->cells[(row * 231) + col];
            if (c->top->ground->scan->tile < 268369920) {
                int32_t walkerId = c->top->ground->walker_ids[c->top->ground->scan->tile];
                c->top->ground->scan->column = col;
                while (walkerId > 0) {
                    const PharaohWalkerQueryRecord *f = (&c->top->ground->walkers[walkerId]);
                    if (f->is_ghost == 0 && f->monument_action_120 == 0) {
                        c->top->operations->draw_walker(c->top->operations->user_data,walkerId, x, y, TopNoHighlight);
                        col = c->top->ground->scan->column;
                        row = c->top->ground->scan->row;
                    }
                    walkerId = f->next_walker_id_on_same_tile;
                }
            }
        }
        col++;
        x = (int32_t)((uint32_t)x + TopTileGapX + c->top->ground->scan->tile_x[2]);
    }
}

static void DrawOverlayColumn(PharaohOverlayTopContext *c,int32_t building,int32_t x,int32_t y)
{
 const uint32_t mode=*c->top->ground->overlay;
 if(mode==20){pharaoh_actor_walking_no_operation();return;}
 switch(mode){
 case 4:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
 case 17:case 18:case 19:case 21:case 22:case 23:case 24:case 25:case 26:
 case 29:case 31:case 43:case 44:case 45:case 48:case 50:
  c->operations->draw_column(c->operations->user_data,mode,building,x,y);break;
 default:break;
 }
}

static int32_t TopShownWithoutBuilding(PharaohOverlayTopContext *c,uint32_t terrain)
{
    return (terrain & 0x40000000u) == 0 && ((terrain & 0x80u) == 0 || (*c->top->ground->overlay) != 45) &&
        ((terrain & 0xc04000u) == 0 || (*c->top->ground->overlay) == 49);
}

static void DrawBuildingTop(PharaohOverlayTopContext *c,int32_t tomb, int32_t x, int32_t y)
{
    if ((c->top->ground->property_flags[c->top->ground->scan->tile] & 64) == 0) {
        return;
    }
    if ((*c->top->ground->overlay) == 45 && !tomb) {
        c->operations->draw_water_top(c->operations->user_data,x, y);
    }
    if (DrawRaisedTombGround(c,x, y)) {
        return;
    }
    if ((*c->top->ground->overlay) == 51 || (*c->top->ground->alternate_overlay) != 0) {
        c->operations->draw_tomb_top(c->operations->user_data,x, y);
        return;
    }
    if ((*c->top->ground->overlay) == 30) {
        return;
    }
    int32_t BuildingId = c->top->ground->building_ids[c->top->ground->scan->tile];
    uint32_t terrain = c->top->ground->terrain[c->top->ground->scan->tile];
    const PharaohBuildingTypeRecord *b = (&c->top->ground->buildings[BuildingId]);
    if (IsTomb(c,b->building_type)) {
        DrawTopBySize(c,x, y, 0);
        return;
    }
    if ((terrain & 0x8u) == 0) {
        if (TopShownWithoutBuilding(c,terrain)) {
            DrawTopBySize(c,x, y, 0);
        }
        return;
    }
    if (BuildingId == 0) {
        return;
    }
    if (c->operations->shows_building(c->operations->user_data,BuildingId) == 0) {
        int32_t type = b->building_type;
        if (!IsVenue(c,type) || (c->top->ground->terrain[c->top->ground->scan->tile] & VenueTopTerrain) == 0) {
            if (type != 209) {
                DrawOverlayColumn(c,BuildingId, x, y);
            }
            return;
        }
    }
    (*c->top->ground->current_image) = c->top->ground->images[c->top->ground->scan->tile];
    DrawTopBySize(c,x, y, 1);
}

static int32_t DrawOwnOverlayTile(PharaohOverlayTopContext *c,int32_t x,int32_t y)
{
 const uint32_t mode=*c->top->ground->overlay;
 static const int32_t resource[10]={1,3,6,2,4,5,13,19,17,15};
 static const int32_t slot[10]={0,2,5,1,3,4,9,11,10,8};
 if(mode==27){c->operations->draw_desirability_top(c->operations->user_data,x,y);DrawRaisedTombGround(c,x,y);return 1;}
 if(mode==28){c->operations->draw_fertility_top(c->operations->user_data,x,y);return 1;}
 if(mode>=33 && mode<=42){c->operations->draw_resource(c->operations->user_data,x,y,resource[mode-33],slot[mode-33]);return 1;}
 return 0;
}

static void DrawTops(PharaohOverlayTopContext *c,int32_t oddRow, int32_t y)
{
    int32_t x = RowStartX(c,oddRow);
    c->top->ground->scan->column = c->top->ground->scan->camera_column - TopColsBefore;
    for (int32_t col = 0; col <= (int32_t)(c->top->ground->scan->visible_columns + TopColsExtra); col++, x = (int32_t)((uint32_t)x + TopTileGapX + c->top->ground->scan->tile_x[2]), c->top->ground->scan->column++) {
        if (c->top->ground->scan->column < 0 || c->top->ground->scan->column >= 231) {
            continue;
        }
        c->top->ground->scan->tile = c->top->ground->cells[(c->top->ground->scan->row * 231) + c->top->ground->scan->column];
        if (c->top->ground->scan->tile >= 268369920) {
            continue;
        }
        int32_t tomb = IsTomb(c,(&c->top->ground->buildings[c->top->ground->building_ids[c->top->ground->scan->tile]])->building_type);
        if ((c->top->ground->terrain[c->top->ground->scan->tile] & (0x4u | 0x40u)) == (0x4u | 0x40u) && c->top->sprites[c->top->ground->scan->tile] != 0) {
            pharaoh_image_draw(c->top->draw,(c->top->sprites[c->top->ground->scan->tile] - 1) + pharaoh_sprite_group_first_image(c->top->animations->groups,560), x, (int32_t)((uint32_t)y-c->top->ground->scan->tile_y[2]));
            continue;
        }
        if (!tomb && DrawOwnOverlayTile(c,x, y)) {
            continue;
        }
        DrawBuildingTop(c,tomb, x, y);
    }
}

static void DrawAnimation(PharaohOverlayTopContext *c,int32_t BuildingId, int32_t imageId, int32_t x, int32_t y)
{
    const PharaohBuildingTypeRecord *b = (&c->top->ground->buildings[BuildingId]);
    if (b->building_type == 71 || b->building_type == 191) {
        c->top->operations->draw_granary_stores(c->top->operations->user_data,BuildingId, c->top->ground->scan->tile, x, y);
        return;
    }
    int32_t frame = c->top->operations->animation_offset(c->top->operations->user_data,imageId, c->top->ground->scan->tile);
    if (frame <= 0) {
        return;
    }
    int16_t frames = animation_frames(c,imageId < (*c->top->draw->image_count) ? imageId : 0);
    if (frames < frame) {
        frame = frames;
    }
    int32_t size = c->top->ground->footprints[c->top->ground->scan->tile] & TopSizeMask;
    if (c->top->operations->is_floodplain_farm(c->top->operations->user_data,BuildingId) == 0) {
        pharaoh_city_top_draw_type_animation(c->top,imageId,(uint32_t)frame,BuildingId,(uint32_t)size,x,y);
    }
}

static void DrawAnimations(PharaohOverlayTopContext *c,int32_t oddRow, int32_t y)
{
    int32_t x = RowStartX(c,oddRow);
    c->top->ground->scan->column = c->top->ground->scan->camera_column - TopColsBefore;
    for (int32_t col = 0; col <= (int32_t)(c->top->ground->scan->visible_columns + TopColsExtra); col++, x = (int32_t)((uint32_t)x + TopTileGapX + c->top->ground->scan->tile_x[2]), c->top->ground->scan->column++) {
        if (c->top->ground->scan->column < 0 || c->top->ground->scan->column >= 231) {
            continue;
        }
        c->top->ground->scan->tile = c->top->ground->cells[(c->top->ground->scan->row * 231) + c->top->ground->scan->column];
        if (c->top->ground->scan->tile >= 268369920) {
            continue;
        }
        int32_t BuildingId = c->top->ground->building_ids[c->top->ground->scan->tile];
        int32_t shown = BuildingId != 0 && c->operations->shows_building(c->operations->user_data,BuildingId) != 0;
        int32_t imageId = c->top->ground->images[c->top->ground->scan->tile];
        if (animation_frames(c,imageId < (*c->top->draw->image_count) ? imageId : 0) == 0 || !shown) {
            continue;
        }
        uint8_t edge = c->top->ground->property_flags[c->top->ground->scan->tile];
        int32_t type = (&c->top->ground->buildings[BuildingId])->building_type;
        if (type == 72 || type == 193) {
            draw_storage_roof(c,pharaoh_sprite_group_first_image(c->top->animations->groups,351) + 17, x, y);
        }
        if ((edge & 64) != 0) {
            DrawAnimation(c,BuildingId, imageId, x, y);
            c->top->operations->draw_building_details(c->top->operations->user_data,BuildingId, x, y);
        }
    }
}

void pharaoh_city_draw_overlay_top(PharaohOverlayTopContext *c)
{
    int32_t y = c->top->ground->scan->start_y - (c->top->ground->scan->tile_y[3] * TopHalfTilesAbove);
    int32_t oddRow = 0;
    c->top->ground->scan->row = c->top->ground->scan->camera_row - TopRowsBefore;
    for (int32_t row = 0; row < (int32_t)(c->top->ground->scan->visible_rows + TopRowsExtra); row++) {
        if ((int32_t)c->top->ground->scan->row >= 0 && (int32_t)c->top->ground->scan->row < 457) {
            DrawWalkers(c,oddRow, y);
            DrawTops(c,oddRow, y);
            DrawAnimations(c,oddRow, y);
        }
        oddRow = !oddRow;
        y = (int32_t)((uint32_t)y+c->top->ground->scan->tile_y[3]);
        c->top->ground->scan->row++;
    }
}
static void overlay_step(void *data){pharaoh_city_draw_overlay_top(data);}
void pharaoh_city_render_bind_overlay_top(PharaohCityRenderOperations *ops,PharaohOverlayTopContext *c){ops->draw_overlay_top=overlay_step;ops->pass_user_data[PHARAOH_CITY_PASS_OVERLAY_TOP]=c;}
