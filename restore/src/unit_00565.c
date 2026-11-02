#include "placement_preview.h"
#define I PV_I
#define U PV_U
#define W PV_W
#define B PV_B
static int32_t treasury(void){return I(0xea187cu+U(0xeaac28)*0x49d8u);}
static uint32_t terrain(int32_t tile){return U(0xaf598cu+(uint32_t)tile*4u);}
static uint8_t live_object(int32_t tile){uint32_t object=U(0xd05720u+(uint32_t)tile*4u);return object&&B(object+24)==0;}
static void draw_valid(int32_t image,int32_t x,int32_t y){W(0x70e120)=0x1fe3;FUN_0053e590(image,x,y);FUN_0053f250(image,x,y);W(0x70e120)=0;}
static void road(void)
{
    int32_t tile=I(0xbca300),image=I(0xf56c94),x,y,base;uint32_t bits;uint8_t invalid=0;
    FUN_004f0ba0(I(0xbca304),I(0xbca308),&x,&y);y=pv_add(y,I(0x70e44c));bits=terrain(tile);
    if((bits&0x10000)||live_object(tile)) {
        image=I(0xf56c34);
        if(!FUN_0051c510(tile))invalid=1;
        else image=pv_add(image,FUN_0051c5f0(tile,pv_sub(I(0xb7482cu+(uint32_t)tile*4u),image)));
    }
    bits&=0xeefed77f;
    if(bits&0x100) {
        image=I(0xf56b84);if(!FUN_0051c390(tile))invalid=1;else image=pv_add(image,(int32_t)FUN_0051c460(tile));
    } else if(bits)invalid=1;
    else {
        image=(B(0xb285ccu+(uint32_t)tile)&0x80)?I(0xf56c44):I(0xf56c94);
        if(!FUN_0047ed30(I(0xbca300),0x40)&&FUN_0047ed00(I(0xbca300),0x40))image=pv_add(image,1);
    }
    if(treasury()<=-5000)invalid=1;
    if(FUN_0046f9a0(pv_add(I(0xbca2e8),I(0xbca304)),pv_add(I(0xbca2ec),I(0xbca308)),3))invalid=1;
    base=pv_sub(y,I(0x70e44c));
    if(invalid)FUN_0053f4f0(I(0xf56be4),x,base,0xfffff863u);else draw_valid(image,x,base);
}
static void well(void)
{
    int32_t x,y,screen_y,tile,image;uint8_t invalid=0;
    FUN_004f0ba0(I(0xbca304),I(0xbca308),&x,&y);y=pv_add(y,I(0x70e44c));
    if(!I(0x6059e4)) {
        tile=pv_add(pv_add(I(0xbca2d8),pv_mul(I(0xbca308),228)),I(0xbca304));I(0xbc9768)=tile;
        if(terrain(tile)&0x40)invalid=FUN_0051c630(tile)==0;
        else invalid=(terrain(tile)&0xeefed77f)!=0||live_object(tile);
    } else if(!I(0xbc9724))invalid=1;
    if(treasury()<=-5000)invalid=1;
    if(FUN_0046f9a0(pv_add(I(0xbca2e8),I(0xbca304)),pv_add(I(0xbca2ec),I(0xbca308)),3))invalid=1;
    screen_y=pv_sub(y,I(0x70e44c));
    if(invalid){FUN_0053f4f0(I(0xf56be4),x,screen_y,0xfffff863u);return;}
    W(0x70e120)=0x1fe3;U(0xbc9768)=U(0xbca300);FUN_0047e870();FUN_00484be0(0x5cc678,16,-1,0);tile=I(0xbc9768);
    if(terrain(tile)&0x40) {
        if(!I(0xbc9744))I(0xbc9748)=((uint8_t)~(uint8_t)U(0xaf55fcu+(uint32_t)tile*4u)>>6)&1;
        if(B(0xb285ccu+(uint32_t)tile)&0x80)image=pv_sub(I(0xb7482cu+(uint32_t)tile*4u),I(0xf56b84));
        else {FUN_0047b920();image=pv_add(I(0xbc9748),48);}
    } else image=pv_add(I(0xbc9748),48);
    FUN_0053e590(pv_add(I(0xf56b84),image),x,screen_y);W(0x70e120)=0;
}
static void queue_draw(void)
{
    uint32_t row=0x70e140u+U(0x70e13c)*16u;U(row)=U(0xd3b580);U(row+4)=U(0xd3b57c);U(row+8)=U(0xd3b578);U(0x70e13c)+=1;
}
static void step(int32_t direction,int32_t *x,int32_t *y)
{
    switch(direction){case 0:*x=pv_add(*x,I(0x70e45c));*y=pv_sub(*y,I(0x70e44c));break;case 2:*x=pv_add(*x,I(0x70e45c));*y=pv_add(*y,I(0x70e44c));break;case 4:*x=pv_sub(*x,I(0x70e45c));*y=pv_add(*y,I(0x70e44c));break;case 6:*x=pv_sub(*x,I(0x70e45c));*y=pv_sub(*y,I(0x70e44c));break;}
}
static void bridge(void)
{
    int32_t x,y,direction,length,index,image,peak=5;uint8_t invalid;
    FUN_004f0ba0(I(0xbca304),I(0xbca308),&x,&y);y=pv_add(y,I(0x70e44c));FUN_0047f200(I(0xbca304),I(0xbca308),I(0x6059e8)==0x52?0:1);
    direction=pv_sub(I(0xbc96f4),I(0xbca2e0));if(direction<0)direction=pv_add(direction,8);
    length=I(0xbc96f8);invalid=(I(0x6059e8)==0x53&&length<5)||!I(0xbc96fc);
    if(treasury()<=-5000||invalid) {
        int32_t remaining;y=pv_sub(y,I(0x70e44c));FUN_0053f4f0(I(0xf56be4),x,y,length<1?0xfffff863u:0x1fe3u);
        if(I(0xbc96f8)<2)return;
        remaining=pv_sub(I(0xbc96f8),1);do{step(direction,&x,&y);remaining=pv_sub(remaining,1);}while(remaining);
        FUN_0053f4f0(I(0xf56be4),x,y,0xfffff863u);return;
    }
    y=pv_sub(pv_sub(y,I(0x70e448)),I(0x70e44c));W(0x70e120)=0x1fe3;U(0x70e13c)=0;
    if(I(0x6059e8)==0x52) {
        image=direction==0?3:direction==2?0:direction==4?5:2;
        FUN_0053f4f0(pv_add(I(0xf56d34),image),x,y,0x1fe3);FUN_0053e590(pv_add(I(0xf56d34),image),x,y);queue_draw();
        for(index=1;index<I(0xbc96f8);index=pv_add(index,1)) {
            step(direction,&x,&y);
            if(index==pv_sub(I(0xbc96f8),1))image=direction==0?5:direction==2?2:direction==4?3:0;
            else image=direction==0||direction==4?4:1;
            FUN_0053e590(pv_add(I(0xf56d34),image),x,y);queue_draw();
        }
    } else {
        int32_t length_now=I(0xbc96f8);
        if(length_now==9||length_now==10)peak=4;else if(length_now==13||length_now==14)peak=6;else if(length_now==15||length_now==16)peak=7;else if(length_now!=11&&length_now!=12)peak=8;
        switch(direction){case 0:FUN_0053e590(pv_add(I(0xf56d34),11),pv_sub(x,3),pv_sub(y,50));break;case 2:FUN_0053e590(pv_add(I(0xf56d34),6),pv_sub(x,1),pv_sub(y,12));break;case 4:FUN_0053e590(pv_add(I(0xf56d34),9),pv_sub(x,30),pv_sub(y,12));break;case 6:FUN_0053e590(pv_add(I(0xf56d34),8),pv_sub(x,23),pv_sub(y,53));break;}
        queue_draw();
        for(index=1;index<I(0xbc96f8);index=pv_add(index,1)) {
            int32_t draw_x=x,draw_y=y;uint8_t draw=1;step(direction,&x,&y);draw_x=x;draw_y=y;
            if(index==1||index==pv_sub(I(0xbc96f8),1))continue;
            if(index==pv_sub(I(0xbc96f8),2)) {
                switch(direction){case 0:image=9;draw_x=pv_add(x,1);draw_y=pv_sub(y,24);break;case 2:image=8;draw_x=pv_add(x,7);draw_y=pv_sub(y,39);break;case 4:image=11;draw_x=pv_sub(x,34);draw_y=pv_sub(y,35);break;case 6:image=6;draw_x=pv_sub(x,29);draw_y=pv_sub(y,22);break;default:draw=0;image=0;break;}
            } else if(index==peak) {
                if(direction==0||direction==4){image=13;draw_y=pv_sub(y,38);}
                else if(direction==2||direction==6){image=12;draw_x=pv_add(x,7);draw_y=pv_sub(y,38);}else{draw=0;image=0;}
            } else {
                if(direction==0||direction==4){image=10;draw_y=pv_sub(y,37);}
                else if(direction==2||direction==6){image=7;draw_x=pv_add(x,7);draw_y=pv_sub(y,38);}else{draw=0;image=0;}
            }
            if(draw)FUN_0053e590(pv_add(I(0xf56d34),image),draw_x,draw_y);queue_draw();
        }
    }
    if((direction==0||direction==6)&&pv_sub(I(0x70e13c),1)>=0) {
        int32_t remaining=I(0x70e13c);uint32_t row=0x70e140u+(uint32_t)pv_sub(remaining,1)*16u;
        do {FUN_0053e590(I(row),I(row+4),I(row+8));row-=16;remaining=pv_sub(remaining,1);}while(remaining);
    }
    W(0x70e120)=0;
}
static void building(void);
void FUN_0043b630(void)
{
    int32_t mode=I(0x6059e8);B(0x6de514)=mode==0xe5||mode==0xea||mode==0xeb||mode==0xec;
    if(!I(0xbca300)||(I(0xe38dcc)&&!B(0x605a5c))||I(0x605994)||mode<=0)return;
    if(I(0x605a3c)==1) {
        switch(mode){case 0x7e:case 0x84:case 0x85:case 0x7f:case 0x80:case 0x77:case 0x82:case 0x83:case 0x8b:case 0xd6:case 0x58:case 0x59:case 0x5d:case 0x81:break;default:return;}
    }
    switch(mode) {
    case 7:case 0x5a:FUN_0043dd70(1,1);return;
    case 5:road();return;case 8:well();return;case 0x52:case 0x53:bridge();return;
    case 0x28:case 0x2c:case 0x2d:FUN_0043d720();return;case 0xd2:FUN_00441490();return;case 0xd5:FUN_00541c00();return;
    case 0xe1:restore_placement_member_adapter(0x40d870,(void *)0x6fd5d8u);return;case 0xe6:restore_placement_member_adapter(0x401420,(void *)0x6de650u);return;
    case 0xe3:restore_placement_member_adapter(0x416f90,(void *)0x6fd368u);return;case 0xe5:restore_placement_member_adapter(0x5456e0,(void *)0x6f9320u);return;
    case 0xea:restore_placement_member_adapter(0x558b80,(void *)0x6f5648u);return;case 0xeb:restore_placement_member_adapter(0x555e20,(void *)0x6de9e0u);return;
    case 0xec:restore_placement_member_adapter(0x54f410,(void *)0x6e9188u);return;case 0xe4:restore_placement_member_adapter(0x4e6610,(void *)0x6de548u);return;
    case 0xde:FUN_004debe0();return;case 0x4a:FUN_0043df10(0x20b);return;case 0xb7:FUN_004edbb0();return;
    case 0x4c:case 0xb5:FUN_0043dd70(1,0);return;case 0xb6:FUN_0043df10(0x20d);return;case 0x4b:FUN_0043df10(0x222);return;case 0xc0:FUN_0043df10(0x20c);return;
    case 0x81:FUN_0043e090();return;case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:FUN_004c0f30(mode);return;
    case 0xd3:case 0xd4:FUN_004c2f50(mode);return;case 0xab:case 0xca:FUN_004bfcd0();return;
    case 0x29:case 0x2a:case 0x2b:FUN_00441f80();return;case 0x88:FUN_0043d300();return;
    case 0x1f:case 0x21:case 0x1e:case 0xd1:FUN_0043f5b0(mode);return;
    }
    building();
}

static uint8_t scenario_forbids(void)
{
    uint32_t scenario=U(0xeaac28)*0x49d8u;
    switch(I(0x6059e8)) {
    case 0xbb:case 0xbc:case 0xbd:return W(0xea5cf6u+scenario)!=0;
    case 0x4d:case 0x4e:case 0x4f:return I(0xea620cu+scenario)!=0;
    case 0x5f:return I(0x705c6c)!=0;case 0x32:return I(0xea5e70u+scenario)!=0;
    case 0x5e:return I(0xea6240u+scenario)!=0;default:return 0;
    }
}
static uint8_t individual_terrain_mode(int32_t mode)
{
    switch(mode){case 0x7e:case 0x84:case 0x85:case 0x7f:case 0x80:case 0x77:case 0x82:case 0x83:case 0x8b:case 0xd6:return 1;default:return 0;}
}
static void building(void)
{
    int32_t mode,size,origin_x,origin_y,tile,image,count,orientation,x,y,base_y,n;
    uint8_t all_invalid=0,any_invalid=0,skip_scenario=0,seen_raised=0,seen_ground=0,can_raise;
    uint32_t offset_base,definition;
    if(I(0xbc9714)>0&&restore_placement_key_state(0x52)==(int16_t)0x8001)I(0xbc9714)=2-(I(0xbc9714)!=1);
    tile=I(0xbca300);mode=I(0x6059e8);definition=0x5d06dcu+(uint32_t)mode*20u;
    size=I(definition);image=pv_add(I(0xf55bb4u+U(definition+8)*8u),I(definition+12));if(mode==0x48)size=3;
    origin_x=I(0xbca304);origin_y=I(0xbca308);
    if(I(0xbca2e0)==2)origin_x=pv_add(pv_sub(origin_x,size),1);
    else if(I(0xbca2e0)!=0){if(I(0xbca2e0)==4)origin_x=pv_add(pv_sub(origin_x,size),1);origin_y=pv_add(pv_sub(origin_y,size),1);}
    if(!I(0x605a3c)&&(I(0xbca304)<3||pv_add(pv_sub(I(0xbca304),1),size)>=pv_sub(I(0xbca2d0),3)||I(0xbca308)<3||pv_add(pv_sub(I(0xbca308),1),size)>=pv_sub(I(0xbca2d4),3)))all_invalid=any_invalid=1;
    if(FUN_00482650(origin_x,origin_y,size,1,0x80000))all_invalid=any_invalid=1;
    if(I(0x6059cc)){if(!FUN_00482650(origin_x,origin_y,3,1,0x10800))all_invalid=any_invalid=1;}
    else if(I(0x605a48)){if(!FUN_00482720(origin_x,origin_y,2,1,restore_placement_rule(U(0x605a48))))all_invalid=any_invalid=1;}
    else if(I(0x6059c8)){if(!FUN_00482650(origin_x,origin_y,2,1,1))all_invalid=any_invalid=1;}
    else if(I(0x6059c4)){if(!FUN_00482650(origin_x,origin_y,2,3,0x10004))all_invalid=any_invalid=1;}
    else if(I(0x6059c0)){if(!FUN_00482650(origin_x,origin_y,2,1,0x40000))all_invalid=any_invalid=1;}
    else if(I(0x6059bc)){if(!FUN_004828e0(origin_x,origin_y,2,0,0x4000))all_invalid=any_invalid=1;}
    else if(I(0x6059b8)){if(!FUN_004828e0(origin_x,origin_y,1,0,0x40))all_invalid=any_invalid=1;}
    else if(I(0x6059b4)){if(!FUN_004829b0(origin_x,origin_y,size,0,0x80,I(0x6059b4)))all_invalid=any_invalid=1;}
    else {
        mode=I(0x6059e8);
        if(mode==0x7e||mode==0x7f||mode==0x80||mode==0x82||mode==0x83) {
            if(!FUN_0046f9a0(pv_add(I(0xbca2e8),I(0xbca304)),pv_add(I(0xbca2ec),I(0xbca308)),4))all_invalid=any_invalid=1;
            mode=I(0x6059e8);
            if(mode==0x7e){skip_scenario=1;if(I(0x605a28)>8&&!(terrain(I(0xbca300))&4))all_invalid=any_invalid=1;}
            else if((mode==0x82||mode==0x83)&&!FUN_0047ed60(I(0xbca300)))all_invalid=any_invalid=1;
        } else if(mode==0x84){skip_scenario=1;if(!(terrain(I(0xbca300))&4))all_invalid=any_invalid=1;}
        else if(mode==0x85||mode==0x8b){if(!(uint8_t)FUN_004ec410(terrain(I(0xbca300)),mode==0x8b))all_invalid=any_invalid=1;}
        else if(mode==0xd6){if(!FUN_00477d40(I(0xbca304),I(0xbca308)))all_invalid=any_invalid=1;}
        else if(mode>=0x29&&mode<=0x2b)image=pv_add(image,I(0xbc9714));
    }
    if(!skip_scenario&&scenario_forbids())all_invalid=any_invalid=1;
    FUN_004f0ba0(I(0xbca304),I(0xbca308),&x,&y);base_y=pv_add(y,I(0x70e44c));
    count=I(0x5cfcd4u+(uint32_t)size*4u);orientation=I(0xbca2e0)/2;can_raise=(uint8_t)FUN_0046fde0(I(0x6059e8));offset_base=0x5cfe10u+(uint32_t)orientation*144u;
    for(n=0;n<count;n=pv_add(n,1)) {
        int32_t cell=pv_add(I(offset_base+(uint32_t)n*4u),tile);uint32_t bits,blocked;uint8_t test=1;
        if(live_object(cell)){all_invalid=any_invalid=1;continue;}
        bits=terrain(cell);
        if(bits&0x10000){if(seen_ground||!can_raise)any_invalid=1;seen_raised=1;}
        else {if(seen_raised)any_invalid=1;seen_ground=1;}
        blocked=bits&0xeefed77f;mode=I(0x6059e8);
        if(mode==0x3b||mode==0xac||mode==0xad)blocked&=0xff3fbfff;
        if(mode==0x38||mode==0x26)blocked&=0xffffffbf;
        else if(mode==0x8a)blocked&=8;
        else {
            if(mode==0x7f||mode==0x80||mode==0x77)blocked&=0xffffffbf;
            if(mode==0x7e){if(I(0x605a28)<=8)blocked&=0xffffffbf;else test=0;}
            else {if(mode==0x82||mode==0x83)blocked=0;if(mode==0x84||mode==0x8b||mode==0x85||mode==0xd6)test=0;}
        }
        if(test&&blocked)any_invalid=1;
        if(PV_S(0xb350dcu+(uint32_t)cell*2u))any_invalid=1;
    }
    mode=I(0x6059e8);
    if(mode==0x26) {
        if(!(terrain(pv_add(I(offset_base),tile))&0x40))any_invalid=1;I(0xbc9768)=tile;if(!FUN_0047b920())any_invalid=1;
    }
    if(I(0x6059e8)==0x8a){uint32_t bits=terrain(pv_add(I(offset_base),tile));if(!(bits&0x40)||(bits&0x10000)||B(0xadc36cu+(uint32_t)tile))any_invalid=1;}
    if(treasury()<=-5000)all_invalid=1;
    else if(!any_invalid) {
        int32_t draw_y=pv_sub(base_y,I(0x70e44c));mode=I(0x6059e8);
        if(individual_terrain_mode(mode)){FUN_0053f4f0(I(0xf56be4),x,draw_y,0x1fe3);return;}
        if(mode==0xc3||mode==0xc2) {
            uint32_t source=mode==0xc3?0xf564dcu:0xf56764u;W(0x70e120)=0x1fe3;FUN_0053e590(I(source),x,draw_y);FUN_0053f250(I(source),x,draw_y);W(0x70e120)=0;return;
        }
        if((uint8_t)FUN_00487f80(mode)) {
            W(0x70e120)=0x1fe3;
            if((uint8_t)FUN_00489470(I(0xbca304),I(0xbca308),3,3)){int32_t base=I(0xf56544);image=pv_add(base,FUN_00489050(I(0xbca304),I(0xbca308),3,3)/14);}
            else image=I(0xf56b24);
            FUN_0053e590(image,x,draw_y);FUN_0053f250(image,x,draw_y);W(0x70e120)=0;return;
        }
        mode=I(0x6059e8);
        if(mode==0xcf||mode==0xd0) {
            W(0x70e120)=0x1fe3;switch(I(0x576144)){case 1:image=I(0xf56d54);break;case 2:image=I(0xf56d5c);break;case 3:image=I(0xf56d64);break;default:image=0;break;}
            FUN_0053e590(image,x,draw_y);FUN_0053f250(image,x,draw_y);W(0x70e120)=0;return;
        }
        if(mode==0x48||mode==0xc1) {
            uint32_t pos;W(0x70e120)=0x1fe3;FUN_0053e590(image,x,draw_y);FUN_0053f250(image,x,draw_y);FUN_0053e590(pv_add(I(0xf566ac),17),pv_sub(x,4),pv_sub(draw_y,42));
            for(pos=0x5cfcf8;pos<0x5cfd38;pos+=8) {
                int32_t xx=pv_add(x,I(pos)),yy=pv_add(base_y,pv_sub(I(pos+4),I(0x70e44c)));
                FUN_0053e590(I(0xf566b4),xx,yy);FUN_0053f250(I(0xf566b4),xx,yy);
            }
            W(0x70e120)=0;return;
        }
        if(mode!=0x47&&mode!=0xbf) {
            if(mode==10){W(0x70e120)=0x1fe3;FUN_0053e590(I(0xf5653c),x,draw_y);FUN_0053f250(I(0xf5653c),x,draw_y);W(0x70e120)=0;return;}
            if(mode>10&&mode<0x1e){for(n=0;n<count;n=pv_add(n,1))FUN_0053f4f0(I(0xf56be4),pv_add(x,I(0x5cfcf0u+(uint32_t)n*8u)),pv_add(base_y,pv_sub(I(0x5cfcf4u+(uint32_t)n*8u),I(0x70e44c))),0x1fe3);return;}
            if(mode==0x38)return;
        }
        draw_valid(image,x,draw_y);return;
    }
    for(n=0;n<count;n=pv_add(n,1)) {
        int32_t xx=pv_add(x,I(0x5cfcf0u+(uint32_t)n*8u)),yy=pv_add(base_y,pv_sub(I(0x5cfcf4u+(uint32_t)n*8u),I(0x70e44c))),cell=pv_add(I(offset_base+(uint32_t)n*4u),tile);
        uint32_t bits=terrain(cell),raised=bits&0x10000,blocked,mask=0xfffff863u;
        if(I(0x6059e8)==0x26){I(0xbc9768)=cell;if(!FUN_0047b920())bits=1;}
        blocked=bits&0xeefed77f;mode=I(0x6059e8);if(mode==0x7e||mode==0x7f||mode==0x80||mode==0x77)blocked&=0xffffffbf;
        if(PV_S(0xb350dcu+(uint32_t)cell*2u))blocked=1;
        if(!all_invalid&&!blocked&&!live_object(cell)&&(!raised||can_raise))mask=0x1fe3;
        FUN_0053f4f0(I(0xf56be4),xx,yy,mask);
    }
}
