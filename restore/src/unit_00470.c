#include "ground_render.h"

static void profile_terrain(uint32_t flags)
{
    uint32_t counter;
    if((flags&4u)!=0&&(flags&0x80000u)==0)counter=0xf55b78;
    else if(flags&2u)counter=0xf55b74;
    else if(flags&0x800u)counter=0xf55b6c;
    else if(flags&0x50000u)counter=0xf55b70;
    else if(flags&0x11u)counter=0xf55b7c;
    else if(flags&0x80u)counter=0xf55b6c;
    else counter=0xf55b68;
    GR_U(counter)=GR_U(counter)+1u;
}
static void normal_tile(uint32_t tile,uint32_t x,uint32_t y,uint32_t alternate,uint32_t column_index,uint32_t row_index,uint32_t middle_column,uint32_t middle_row,uint8_t boundary)
{
    int32_t building=(int16_t)GR_W(0xb4e6fcu+tile*2u);
    uint32_t image=GR_U(0xb7482cu+tile*4u),flags=GR_U(0xaf598cu+tile*4u),foot;
    uint32_t offset,classification,base,step,span,last,index,first,second,px,py;
    int central=GR_S(column_index)>GR_S(middle_column-3u)&&GR_S(column_index)<GR_S(middle_column+3u)&&GR_S(row_index)>GR_S(middle_row-3u)&&GR_S(row_index)<GR_S(middle_row+3u);
    GR_U(0xd3b5c0)=image;
    foot=GR_B(0xb285ccu+tile);
    if(building!=0){
        offset=(uint32_t)building*264u;
        if(GR_B(0x799a97u+offset)!=0)GR_W(0x70e120)=0xf863;
        if(central){
            classification=FUN_0053b830((uint32_t)(int32_t)(int16_t)GR_W(0x7999e8u+offset));
            if(classification==1)GR_U(0xf55b54)=GR_U(0xf55b54)+1u;
            else{
                classification=FUN_0053b830((uint32_t)(int32_t)(int16_t)GR_W(0x7999e8u+offset));
                if(classification==2)GR_U(0xf55b58)=GR_U(0xf55b58)+1u;
                else if(FUN_0053b830((uint32_t)(int32_t)(int16_t)GR_W(0x7999e8u+offset))==4)GR_U(0xf55b60)=GR_U(0xf55b60)+1u;
            }
            tile=GR_U(0x70e488);image=GR_U(0xd3b5c0);
        }
    }else if(central)profile_terrain(flags);
    GR_U(0xf55b84)=GR_U(0xf55b84)+1u;
    if(flags&0x20u){
        GR_W(0x7999e8)=39;FUN_0041d410(0,2);
        tile=GR_U(0x70e488);image=GR_U(0xd3b5c0);
    }
    if(foot&0x10u){image=GR_U(0xf56bdc);GR_U(0xd3b5c0)=image;}
    foot&=15u;
    if(foot==0){
        if((flags&4u)!=0&&GR_U(0x70e3c4)!=0){
            if(flags&0x4000000u){base=boundary?GR_U(0xf57224):GR_U(0xf56d24);step=15;span=90;}
            else{base=GR_U(0xf56bd4);step=1;span=6;}
            last=base-1u+step*6u;
            if(GR_S(image)<=GR_S(last)&&GR_S(image)>=GR_S(base)){
                image+=step;GR_U(0xd3b5c0)=image;
                if(GR_S(image)>GR_S(last)){image-=span;GR_U(0xd3b5c0)=image;}
                GR_U(0xb7482cu+tile*4u)=image;
            }
        }
        base=GR_U(0xf572ec);
        if(GR_S(image)<GR_S(base+48u)||GR_S(image)>=GR_S(base+72u)){
            FUN_0053e590(image,x,y);tile=GR_U(0x70e488);image=GR_U(0xd3b5c0);
        }
        if((GR_U(0xaf598cu+tile*4u)&0xeefed77fu)==0){
            index=GR_U(0x70e47c)*231u+GR_U(0x70e480);
            if(alternate){first=GR_U(0x70e0f4u+index*4u);second=GR_U(0x70e0f8u+index*4u);}
            else{first=GR_U(0x70e0f8u+index*4u);second=GR_U(0x70e0fcu+index*4u);}
            if(ground_neighbor_occludes(first)||ground_neighbor_occludes(second))FUN_0053f250(image,x,y);
        }
    }else if(ground_footprint(foot,x,y,&px,&py))FUN_0053e590(image,px,py);
    GR_W(0x70e120)=0;
}

void FUN_00430ef0(void)
{
    uint32_t y,ystep,width_bound,height_bound,middle_column,middle_row,row,column,x,alternate=0,rows=0,columns,tile;
    uint8_t boundary;
    FUN_0053b670();
    ystep=GR_U(0x70e44c);width_bound=GR_U(0x70e464)+6u;y=GR_U(0x70e48c)-ystep*9u;
    middle_column=(uint32_t)(GR_S(width_bound)/2);height_bound=GR_U(0x70e460)+14u;middle_row=(uint32_t)(GR_S(height_bound)/2);
    row=GR_U(0x70e474)-8u;GR_U(0x70e47c)=row;
    if(GR_S(height_bound)<=0)return;
    do{
        x=GR_U(0x70e490);if(alternate)x=x-GR_U(0x70e45c)-1u;
        alternate=!alternate;x+=(0xfffffffeu-GR_U(0x70e458))*4u;
        column=GR_U(0x70e478)-4u;GR_U(0x70e480)=column;columns=0;
        if(GR_S(width_bound)>=0){
            do{
                if(GR_S(column)>=0&&GR_S(column)<231&&GR_S(row)>=0&&GR_S(row)<457){
                    tile=GR_U(0x70e494u+(row*231u+column)*4u);GR_U(0x70e488)=tile;
                    boundary=(uint8_t)restore_ground_boundary_at(column,row);
                    tile=GR_U(0x70e488);
                    if(tile==GR_U(0x6059d0)){GR_U(0x70e3ec)=x;GR_U(0x70e3e8)=y;}
                    if(GR_S(tile)>=0x0fff0000){
                        GR_U(0xd3b5c0)=GR_U(0xe38e54)==1?0:tile-0x0fff0000u;
                        FUN_0053e590(GR_U(0xd3b5c0),x,y);
                    }else if(GR_B(0xb67d1cu+tile)&0x40u){
                        normal_tile(tile,x,y,alternate,columns,rows,middle_column,middle_row,boundary);
                    }
                    row=GR_U(0x70e47c);column=GR_U(0x70e480);
                }
                ++columns;++column;x+=GR_U(0x70e458)+2u;width_bound=GR_U(0x70e464)+6u;GR_U(0x70e480)=column;
            }while(GR_S(columns)<=GR_S(width_bound));
            ystep=GR_U(0x70e44c);
        }
        y+=ystep;++rows;++row;GR_U(0x70e47c)=row;
    }while(GR_S(rows)<GR_S(GR_U(0x70e460)+14u));
}
