#include "message_panel_calls.h"
#define I MP_I
#define U MP_U
#define W MP_W
#define B MP_B
#define R MP_R
#define E MP_E
void FUN_004c5e60(void)
{
    char body_buffer[1000],title_buffer[200],caption_buffer[200];
    const char *body=restore_message_text(R->body),*title=restore_message_text(R->title),*caption=restore_message_text(R->caption);
    int32_t height,position,subtype,value,variant,base,x,y,resource,amount,cost,plural;
    uint32_t external;
    if(B(0xd3b184)) {
        if(E->body_template>=0){FUN_00448330(E->body_template,body_buffer,1000);body=body_buffer;}
        if(E->title_template>=0){FUN_00448330(E->title_template,title_buffer,200);title=title_buffer;}
        if(E->caption_template>=0){FUN_00448330(E->caption_template,caption_buffer,200);caption=caption_buffer;}
    }
    I(0xd3b164)=I(0xf56fac);I(0xd3b168)=I(0xf56f9c);
    I(0xd3b160)=R->x+I(0xe38da8);I(0xd3b15c)=R->y+I(0xe38da4);
    if(!U(0xd3b140)){FUN_004dddc0();if(B(0xe38dca)==1)FUN_0051e990();}
    I(0xd3b144)=R->type?32:48;I(0xd3b154)=I(0xd3b160)+16;
    FUN_00536820(I(0xd3b160),I(0xd3b15c),R->columns,R->rows);
    if(!R->title_x) {
        y=R->subtype==4?R->title_y+I(0xd3b15c):I(0xd3b15c)+14;
        FUN_004cc720(title,I(0xd3b160),y,R->columns*16,I(0xf56fb4),0);I(0xd3b150)=I(0xd3b15c)+48;
    } else {
        FUN_004cc770(title,R->title_x+I(0xd3b160),R->title_y+I(0xd3b15c),I(0xf56fb4),0);I(0xd3b150)=I(0xd3b15c)+32;
    }
    if(R->picture1) {
        if(!I(0xd3b124)) {
            R->picture1_x=16;R->picture1_y=16;
            if(I(0x978d7c)==2) {
                FUN_0053e590(I(0xf5690c),I(0xd3b160)+16,I(0xd3b15c)+16);
                FUN_00536990(R->picture1_x+30+I(0xd3b160),R->picture1_y+40+I(0xd3b15c),2,1);
            } else FUN_0053e590(I(0xf55c7c),I(0xd3b160)+16,I(0xd3b15c)+16);
        } else {
            value=R->picture1<1000?R->picture1-1+I(0xf55c04):R->picture1-1001+I(0xf5731c);
            FUN_0053e590(value,R->picture1_x+I(0xd3b160),R->picture1_y+I(0xd3b15c));
        }
        height=W(0xd3b5bc)+8+I(0xd3b15c)+R->picture1_y;if(I(0xd3b150)<height)I(0xd3b150)=height;
    }
    if(R->picture2) {
        FUN_0053e590(R->picture2-1+I(0xf55c04),R->picture2_x+I(0xd3b160),R->picture2_y+I(0xd3b15c));
        height=W(0xd3b5bc)+8+I(0xd3b15c)+R->picture2_y;if(I(0xd3b150)<height)I(0xd3b150)=height;
    }
    if(R->caption_x) {
        I(0xd3b158)=R->columns*16-R->caption_x-16;I(0xe20774)=0;
        FUN_004cd010(caption,R->caption_x+I(0xd3b160),R->caption_y+I(0xd3b15c),I(0xd3b158),I(0xf56f8c),0);
        height=R->caption_y+I(0xe20774)+I(0xd3b15c);if(I(0xd3b150)<height)I(0xd3b150)=height+4;
    }
    if(!I(0xd3b138)) {
        position=I(0xd3b150)-I(0xd3b15c)+I(0xd3b144);B(0x5c7e10)=1;B(0x5c7e38)=1;
        height=R->rows-position/16;I(0xd3b148)=height-1;I(0xd3b13c)=height-2;I(0xd3b14c)=R->columns-4;
        FUN_004c8070(body,I(0xd3b154)+8,I(0xd3b150)+6,I(0xd3b14c)*16-16,1,0);
        if(I(0xd3b13c)<I(0xd3b138))I(0xd3b134)=I(0xd3b138)-I(0xd3b13c);
        else {I(0xd3b134)=0;B(0x5c7e10)=0;B(0x5c7e38)=0;I(0xd3b14c)=R->columns-2;}
        B(0xe38dca)=1;
    }
    if(I(0xd3b13c)<I(0xd3b138))FUN_00536a30(R->columns*16-46+I(0xd3b160),I(0xd3b150)+26,2,I(0xd3b148)-2);
    if(I(0xd3b180))FUN_004cd9d0(R->media_x+I(0xd3b160),R->media_y+I(0xd3b15c),200,200,0xf800);
    FUN_00536a30(I(0xd3b154),I(0xd3b150),I(0xd3b14c),I(0xd3b148));
    I(0xd3b55c)=I(0xd3b154)+3;I(0xd3b558)=I(0xd3b14c)*16-3+I(0xd3b154);
    I(0xd3b554)=I(0xd3b150)+3;I(0xd3b550)=I(0xd3b148)*16-3+I(0xd3b150);FUN_004c8040();
    if(R->type!=2)FUN_004c8070(body,I(0xd3b154)+8,I(0xd3b150)+6,I(0xd3b14c)*16-16,0,0);
    else {
        if(R->subtype!=4) {
            I(0xe20778)=0;FUN_004ccbb0(25,I(0xd3b174),I(0xd3b154)+10,I(0xd3b150)+6,I(0xf56f9c),0);
            FUN_004cccb0(0,I(0xd3b178),I(0xe20778)+12+I(0xd3b154),I(0xd3b150)+6,I(0xf56f9c),0);
            if(R->subtype==1&&I(0xd3b170)) {
                if(I(0xd3b124)==251)FUN_004ccc30(8,0,I(0xd3b170),I(0xd3b160)+240,I(0xd3b150)+6,I(0xf56f9c),0);
                else FUN_004ccbb0(41,I(0xd3b170),I(0xd3b160)+240,I(0xd3b150)+6,I(0xf56f9c),0);
            } else {
                const char *family;int font;
                FUN_004ccbb0(63,5,I(0xe20778)+80+I(0xd3b154),I(0xd3b150)+6,I(0xf56f9c),0);
                x=I(0xe20778)+80+I(0xd3b154);y=I(0xd3b150)+6;font=I(0xf56f9c);
                family=FUN_00447840();FUN_004cc770(family,x,y,font,0);
            }
        }
        subtype=R->subtype;I(0xe20774)=0;
        if(subtype==1||subtype==7) {
            FUN_004ccbb0(12,1,I(0xd3b160)+100,I(0xd3b150)+44,I(0xf56f9c),0);y=I(0xd3b150)+86;
        } else if(subtype==3) {
            value=I(0xea5db0u+U(0xeaac28)*0x49d8u);
            if(value>=1&&value<=5)FUN_004ccbb0(12,value+2,I(0xd3b160)+64,I(0xd3b150)+44,I(0xf56f9c),0);
            y=I(0xd3b150)+86;
        } else if(subtype==4)y=I(0xd3b150)+6;
        else if(subtype==5||subtype==6) {
            base=I(0xf571fc);x=I(0xd3b160)+64;y=I(0xd3b150)+40;variant=FUN_0047d350(I(0xd3b16c),3);
            FUN_0053e590(variant+base+I(0xd3b16c),x,y);
            if(subtype==5)FUN_004ccbb0(B(0xe39088)?21:195,B(0x77de03u+U(0xd3b170)*106u),I(0xd3b160)+100,I(0xd3b150)+44,I(0xf56f9c),0);
            else FUN_004cce00(I(0xd3b170),64,(const char *)0x5e03ecu,I(0xd3b160)+100,I(0xd3b150)+44,I(0xf56f9c),0);
            y=I(0xd3b150)+86;
        } else y=I(0xd3b150)+56;
        FUN_004c8070(body,I(0xd3b154)+8,y,I(0xd3b14c)*16-16,0,0);
        if(R->subtype==2&&I(0xd3b170)>=0&&I(0xd3b170)<I(0x785d70)) {
            external=0x785d74u+U(0xd3b170)*124u;
            if(external) {
                amount=E->quantity;resource=E->resource;cost=E->cost;
                if(resource!=37) {
                    I(0xe20778)=0;if(!mp_unscaled_resource(resource))amount=(int32_t)((uint32_t)amount*100u);
                    value=I(0xf571fc)+(resource==36?21:resource);
                    FUN_0053e590(value,I(0xd3b154)+8,I(0xe20774)+81+I(0xd3b150));
                    FUN_004cce00(amount,64,(const char *)0x57c048u,I(0xd3b154)+20,I(0xe20774)+86+I(0xd3b150),I(0xf56f9c),0);
                    plural=mp_resource_plural(resource,amount)?54:0;
                    FUN_004ccbb0(23,resource+plural,I(0xe20778)+20+I(0xd3b154),I(0xe20774)+86+I(0xd3b150),I(0xf56f9c),0);
                    if(B(external+2)==1&&W(external+48)<2) {
                        I(0xe20778)=0;FUN_004ccc30(8,4,cost,I(0xd3b154)+220,I(0xe20774)+86+I(0xd3b150),I(0xf56f9c),0);
                        FUN_004ccbb0(12,2,I(0xe20778)+220+I(0xd3b154),I(0xe20774)+86+I(0xd3b150),I(0xf56f9c),0);
                    }
                }
            }
        }
        FUN_004c5ce0();
    }
    I(0xd3b558)=I(0xe38db0);I(0xd3b55c)=0;I(0xd3b554)=0;I(0xd3b550)=I(0xe38dac);FUN_004c72b0();
}
