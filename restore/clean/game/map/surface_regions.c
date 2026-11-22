#include "surface_refresh_internal.h"

void pharaoh_map_refresh_paved_region(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn,uint32_t clear)
{
 uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn;
 uint32_t swap,width,height,tail,cell,rows,columns,x,y,record,variation,image;uint8_t Access,sprite;
 if(((int32_t)(left))>((int32_t)(right))){swap=left;left=right;right=swap;}if(((int32_t)(top))>((int32_t)(bottom))){swap=top;top=bottom;bottom=swap;}
 if(((int32_t)(left))<0)left=0;if(((int32_t)(top))<0)top=0;
 width=(c->map->width);if(((int32_t)(right))>=((int32_t)(width)))right=width-1u;
 height=(c->map->height);if(((int32_t)(bottom))>=((int32_t)(height)))bottom=height-1u;
 if(clear){
  tail=width-right+left-1u;cell=(c->map->origin)+top*228u+left;
  if(((int32_t)(top))<=((int32_t)(bottom))){
   rows=bottom-top+1u;
   do{
    if(((int32_t)(left))<=((int32_t)(right))){
     columns=right-left+1u;
     do{
      if((uint8_t)c->map->terrain[cell]&64u){
       Access=c->property_flags[cell];
       if(Access&128u){
        record=(uint32_t)(int32_t)(int16_t)c->building_ids[cell];
        if(!record||c->buildings[record].building_type!=138u){
         c->image_ids[cell]=0;c->property_flags[cell]=(uint8_t)(Access&0xf0u);
         sprite=c->edge_flags[cell];c->edge_flags[cell]=(uint8_t)(sprite|64u);
        }
       }
      }
      ++cell;--columns;
     }while(columns);
     /* Native reloads width here but retains its original first-pass tail. */
     width=(c->map->width);
    }
    cell+=(*c->border_size)+tail;--rows;
   }while(rows);
  }
 }
 tail=width-right+left-1u;cell=(c->map->origin)+top*228u+left;y=top;
 if(((int32_t)(y))>((int32_t)(bottom)))return;
 do{
  x=left;
  if(((int32_t)(x))<=((int32_t)(right)))do{
   if(((uint8_t)c->map->terrain[cell]&64u)&&!c->image_ids[cell]&&(c->property_flags[cell]&128u)){
    variation=c->random[cell]&1u;
    if(pharaoh_map_plaza_square_fits(c->map->terrain,c->property_flags,c->image_ids,(int32_t)cell)){image=c->groups[0x1b5].first_image+6u+variation;pharaoh_map_place_surface_square(c,0,((int32_t)(x)),((int32_t)(y)),2,2,image,64);}
    else{
     image=c->groups[0x1b5].first_image;if((x&1u)&&(y&1u))++image;else if((x&1u)!=(y&1u))image+=2u;
     c->image_ids[cell]=image;c->edge_flags[cell]=(uint8_t)(c->edge_flags[cell]|64u);
    }
   }
   ++x;++cell;
  }while(((int32_t)(x))<=((int32_t)(right)));
  cell+=(*c->border_size)+tail;++y;
 }while(((int32_t)(y))<=((int32_t)(bottom)));
}

void pharaoh_map_set_fault_image(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn,int32_t mode)
{
    uint32_t dy=(c->geometry->diamond_y_min),dx=(c->geometry->diamond_x_min),x=(uint32_t)xIn,y=(uint32_t)yIn,offset,cell,flags,Access,sprite;
    uint32_t left,top,columns=3,rows=3,width,height,endrow,endcol,col,row,frame,image,variant;
    if(!(uint8_t)pharaoh_map_grid_is_inside(c->geometry,(int32_t)(dx+x),(int32_t)(dy+y)))return;
    offset=(c->map->origin);cell=offset+y*228u+x;flags=c->map->terrain[cell];Access=c->property_flags[cell];
    if(mode==1){c->map->terrain[cell]=flags|2u;Access|=128u;}
    else{c->map->terrain[cell]=flags&0xdfcffffdu;Access&=127u;}
    c->property_flags[cell]=(uint8_t)Access;
    left=x-1u;top=y-1u;
    if((int32_t)left<0){columns=left+3u;left=0;}
    else{width=(c->map->width);if((int32_t)(left+3u)>(int32_t)width)columns=width-left;}
    if((int32_t)top<0){rows=top+3u;top=0;}
    else{height=(c->map->height);if((int32_t)(top+3u)>(int32_t)height)rows=height-top;}
    endrow=top+rows;(*c->scan_y)=top;cell=offset+top*228u+left;(*c->current_tile)=cell;
    if((int32_t)top>=(int32_t)endrow)return;
    endcol=left+columns;
    do{
        col=left;(*c->scan_x)=col;
        if((int32_t)col<(int32_t)endcol)do{
            if(((uint8_t)c->map->terrain[cell]&2u)&&(c->property_flags[cell]&128u)){
                pharaoh_map_adjacent_road_flags(c->adjacent,c->map->terrain,c->property_flags,(int32_t)*c->current_tile);pharaoh_map_adjacent_flags_normalize(c->adjacent);
                if(surface_match(c,c->fault_context,17,-1,0)){
                    frame=(c->selection->frame);image=c->groups[0x1f7].first_image;variant=(c->selection->orientation_offset);image+=frame+variant;cell=(*c->current_tile);
                }else{cell=(*c->current_tile);image=c->groups[0x1f7].first_image;}
                c->image_ids[cell]=image;Access=c->property_flags[cell];sprite=c->edge_flags[cell];
                c->property_flags[cell]=(uint8_t)(Access&0xf0u);c->edge_flags[cell]=(uint8_t)(sprite|64u);
            }
            col=(*c->scan_x)+1u;++cell;(*c->scan_x)=col;(*c->current_tile)=cell;
        }while((int32_t)col<(int32_t)endcol);
        row=(*c->scan_y)+1u;(*c->scan_y)=row;cell+=228u-columns;row=(*c->scan_y);(*c->current_tile)=cell;
    }while((int32_t)row<(int32_t)endrow);
}

void pharaoh_map_refresh_faults(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn)
{
    uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn,width,height,cell,stride,row,col;
    if(leftIn<0)left=0;if(topIn<0)top=0;
    width=(c->map->width);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(c->map->height);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=(c->map->origin);stride=width-right+left-1u;cell+=top*228u+left;row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{if(((uint8_t)c->map->terrain[cell]&2u)&&(c->property_flags[cell]&128u))pharaoh_map_set_fault_image(c,(int32_t)col,(int32_t)row,1);++cell;++col;}while((int32_t)col<=(int32_t)right);
        }
        cell+=(*c->border_size)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}

void pharaoh_map_set_rubble_image(PharaohMapSurfaceContext *c,int32_t xIn,int32_t yIn)
{
    uint32_t dy=(c->geometry->diamond_y_min),dx=(c->geometry->diamond_x_min),x=(uint32_t)xIn,y=(uint32_t)yIn,cell,base,Access,variation,flags,sprite;
    if(!(uint8_t)pharaoh_map_grid_is_inside(c->geometry,(int32_t)(dx+x),(int32_t)(dy+y)))return;
    cell=(c->map->origin)+y*228u+x;base=c->groups[0x213].first_image;Access=c->property_flags[cell];
    (*c->current_tile)=cell;variation=c->random[cell];c->property_flags[cell]=(uint8_t)(Access&0xf0u);
    flags=c->map->terrain[cell];c->canal_water[cell]=0;c->image_ids[cell]=base+(variation&7u);
    sprite=c->edge_flags[cell];c->edge_flags[cell]=(uint8_t)(sprite|64u);c->map->terrain[cell]=flags|0x1000u;
}

void pharaoh_map_refresh_rubble(PharaohMapSurfaceContext *c,int32_t leftIn,int32_t topIn,int32_t rightIn,int32_t bottomIn)
{
    uint32_t left=(uint32_t)leftIn,top=(uint32_t)topIn,right=(uint32_t)rightIn,bottom=(uint32_t)bottomIn,width,height,cell,step,row,col,flags;
    if(leftIn<0)left=0;if(topIn<0)top=0;
    width=(c->map->width);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=(c->map->height);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=(c->map->origin)+top*228u+left;step=width-right+left-1u;
    if((int32_t)top>(int32_t)bottom)return;
    row=top;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{
                flags=c->map->terrain[cell];
                if((flags&0x1000u)&&!(flags&0x700u)&&!(flags&0x68u))pharaoh_map_set_rubble_image(c,(int32_t)col,(int32_t)row);
                ++col;++cell;
            }while((int32_t)col<=(int32_t)right);
        }
        cell+=(*c->border_size)+step;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
