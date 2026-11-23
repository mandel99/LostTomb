#include "pharaoh/rendering/city_object_layers.h"
#include <stddef.h>
_Static_assert(offsetof(PharaohWalkerQueryRecord,high_layer_077)==0x77,"native high-layer byte");
static int32_t signed_bits(uint32_t value){return (int32_t)value;}
void pharaoh_city_draw_high_walkers(PharaohCityObjectLayerContext *c,uint32_t selection)
{
    PharaohCityGroundContext *const g=c->ground;
    uint32_t counter,row,y,rows=0,parity=0,x,column,columns,width,tile,remaining;
    int32_t actor;
    if((*c->cycle_enabled)==1){counter=(*c->cycle_counter)+1u;(*c->cycle_counter)=counter;if(signed_bits(counter)>=5)(*c->cycle_counter)=0;}
    counter=(g->scan->tile_y[3]);row=(g->scan->camera_row);y=(g->scan->start_y)-counter*9u;
    counter=(g->scan->visible_rows)+14u;row-=8u;(g->scan->row)=row;
    if(signed_bits(counter)<=0)return;
    do{
        if(signed_bits(row)>=0&&signed_bits(row)<457){
            x=(g->scan->start_x);if(parity)x=x-(g->scan->tile_x[3])-1u;
            x+=(0xfffffffeu-(g->scan->tile_x[2]))*4u;
            column=(g->scan->camera_column)-4u;columns=0;width=(g->scan->visible_columns)+6u;(g->scan->column)=column;
            if(signed_bits(width)>=0){
                do{
                    if(signed_bits(column)>=0&&signed_bits(column)<231){
                        tile=g->cells[row*231u+column];(g->scan->tile)=tile;
                        if(signed_bits(tile)<0x0fff0000){
                            actor=g->walker_ids[tile];
                            while(actor>0){
                                if(g->walkers[actor].high_layer_077!=0){c->operations->draw_walker(c->operations->user_data,actor,(int32_t)x,(int32_t)y,selection);row=(g->scan->row);column=(g->scan->column);}
                                actor=g->walkers[actor].next_walker_id_on_same_tile;
                            }
                        }
                    }
                    x+=(g->scan->tile_x[2])+2u;++columns;++column;width=(g->scan->visible_columns)+6u;(g->scan->column)=column;
                }while(signed_bits(columns)<=signed_bits(width));
            }
            /* The native second traversal retains a local column, writes the
               global cursor only at its ends, and captures its length once. */
            x=(g->scan->start_x);if(parity)x=x-(g->scan->tile_x[3])-1u;
            x+=(0xfffffffeu-(g->scan->tile_x[2]))*4u;
            column=(g->scan->camera_column)-4u;width=(g->scan->visible_columns)+6u;(g->scan->column)=column;
            if(signed_bits(width)>=0){
                remaining=width+1u;
                do{
                    if(signed_bits(column)>=0&&signed_bits(column)<231){
                        counter=(*g->overlay);
                        if(counter==0||counter==51||(*g->alternate_overlay)!=0)
                            (g->scan->tile)=g->cells[row*231u+column];
                    }
                    ++column;--remaining;x+=(g->scan->tile_x[2])+2u;
                }while(remaining!=0);
                (g->scan->column)=column;
            }
        }
        parity=!parity;y+=(g->scan->tile_y[3]);++rows;++row;(g->scan->row)=row;
    }while(signed_bits(rows)<signed_bits((g->scan->visible_rows)+14u));
}
void pharaoh_city_draw_grid_values(PharaohCityObjectLayerContext *c)
{
    PharaohCityGroundContext *const g=c->ground;
    uint32_t row,width,x,y,tile,column,count,rows=0,alternate=0;
    if(!(*c->debug_grid_a)&&!(*c->debug_grid_b)&&!(*c->debug_grid_c))return;
    y=(g->scan->start_y)-(g->scan->tile_y[3]);row=(g->scan->camera_row);(g->scan->row)=row;
    if(signed_bits((g->scan->visible_rows))<=0)return;
    width=(g->scan->visible_columns);
    do{
        x=(g->scan->start_x);
        if(alternate)x-=(g->scan->tile_x[3])+1u;
        alternate=!alternate;count=0;(g->scan->column)=(g->scan->camera_column);
        if(signed_bits(width)>=0)do{
            column=(g->scan->column);
            tile=g->cells[row*231u+column];(g->scan->tile)=tile;
            if(signed_bits(tile)<0x0fff0000){
                c->operations->draw_grid_value(c->operations->user_data,signed_bits(x),signed_bits(y));row=(g->scan->row);width=(g->scan->visible_columns);
            }
            column=(g->scan->column);++count;x+=(g->scan->tile_x[2])+2u;(g->scan->column)=column+1u;
        }while(signed_bits(count)<=signed_bits(width));
        y+=(g->scan->tile_y[3]);++rows;++row;(g->scan->row)=row;
    }while(signed_bits(rows)<signed_bits((g->scan->visible_rows)));
}

static void high(void *data,int32_t selection){pharaoh_city_draw_high_walkers(data,(uint32_t)selection);}
static void grid(void *data){pharaoh_city_draw_grid_values(data);}
void pharaoh_city_render_bind_object_layers(PharaohCityRenderOperations *ops,
 PharaohCityObjectLayerContext *c)
{
 ops->draw_high_walkers=high;ops->draw_grid_values=grid;
 ops->pass_user_data[PHARAOH_CITY_PASS_HIGH_WALKERS]=c;
 ops->pass_user_data[PHARAOH_CITY_PASS_GRID_VALUES]=c;
}
