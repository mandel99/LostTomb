#include "message_panel_calls.h"
#include <stdlib.h>
#define I MP_I
#define U MP_U
#define W MP_W
#define B MP_B
#define R MP_R
#define E MP_E
void FUN_004c6c70(void)
{
    char body_buffer[1000],title_buffer[200],caption_buffer[200];
    const char *body=restore_message_text(R->body),*title=restore_message_text(R->title);
    uint8_t media_kind=(uint8_t)*restore_message_text(R->media);
    int32_t image,width,height,position,amount,resource,cost,plural,x,y,font;uint32_t external;
    if(B(0xd3b184)) {
        if(E->body_template>=0){FUN_00448330(E->body_template,body_buffer,1000);body=body_buffer;}
        if(E->title_template>=0){FUN_00448330(E->title_template,title_buffer,200);title=title_buffer;}
        if(E->caption_template>=0)FUN_00448330(E->caption_template,caption_buffer,200);
    }
    I(0xd3b168)=I(0xf56f9c);I(0xd3b164)=I(0xf56fa4);
    I(0xd3b15c)=I(0xe38da4)+28;I(0xd3b160)=I(0xe38da8);
    if(!U(0xd3b140)){FUN_004dddc0();if(B(0xe38dca)==1)FUN_0051e990();}
    FUN_00536820(I(0xd3b160),I(0xd3b15c),30,28);
    FUN_004cd680(I(0xd3b160)+7,I(0xd3b15c)+9,466,239,0);FUN_004c8040();
    if(media_kind=='@') {
        image=I(0xf55c34);image+=FUN_00561c96(restore_message_text(R->media)+1)-1;
        if(FUN_0053e530(image,&width,&height))FUN_0053e590(image,(466-width)/2+8+I(0xd3b160),(239-height)/2+8+I(0xd3b15c));
    } else FUN_00538dc0(restore_message_text(R->media),I(0xd3b160)+8,I(0xd3b15c)+8,0,B(0xe38dda));
    FUN_00536a30(I(0xd3b160)+8,I(0xd3b15c)+245,29,10);
    FUN_004cc720(title,I(0xd3b160)+8,I(0xd3b15c)+414,464,I(0xf56f8c),0);
    I(0xe20778)=0;FUN_004ccbb0(25,I(0xd3b174),I(0xd3b160)+16,I(0xd3b15c)+257,I(0xf56f9c),0);
    FUN_004cccb0(0,I(0xd3b178),I(0xe20778)+18+I(0xd3b160),I(0xd3b15c)+257,I(0xf56f9c),0);
    if(R->type==2&&R->subtype==1&&I(0xd3b124)==251)
        FUN_004ccc30(8,0,I(0xd3b170),I(0xe20778)+90+I(0xd3b160),I(0xd3b15c)+257,I(0xf56f9c),0);
    else {
        const char *family;
        FUN_004ccbb0(63,5,I(0xe20778)+90+I(0xd3b160),I(0xd3b15c)+257,I(0xf56f9c),0);
        x=I(0xe20778)+90+I(0xd3b160);y=I(0xd3b15c)+257;font=I(0xf56f9c);
        family=FUN_00447840();FUN_004cc770(family,x,y,font,0);
    }
    position=I(0xd3b144)-I(0xd3b15c)+I(0xd3b150);height=R->rows-position/16;
    I(0xe20774)=0;I(0xd3b148)=height-1;I(0xd3b13c)=height-2;I(0xd3b14c)=R->columns-4;
    FUN_004c8070(body,I(0xd3b160)+16,I(0xd3b15c)+277,480,0,0);
    if(R->type==2&&R->subtype==2) {
        /* The native subtype invariant supplies a valid external record. */
        external=(I(0xd3b170)<0||I(0xd3b170)>=I(0x785d70))?0:0x785d74u+U(0xd3b170)*124u;
        amount=E->quantity;resource=E->resource;cost=E->cost;
        if(!mp_unscaled_resource(resource))amount=(int32_t)((uint32_t)amount*100u);
        FUN_004cce00(amount,64,(const char *)0x57c048u,I(0xd3b160)+8,I(0xd3b15c)+384,I(0xf56f9c),0);
        FUN_0053e590(I(0xf571fc)+(resource==36?21:resource),I(0xd3b160)+70,I(0xd3b15c)+379);
        plural=mp_resource_plural(resource,amount)?54:0;
        FUN_004ccbb0(23,resource+plural,I(0xd3b160)+100,I(0xd3b15c)+384,I(0xf56f9c),0);
        /* The original faults at[NULL+48] when its record invariant fails. */
        if(!external) {
            fprintf(stderr,"Message panel 004c6c70: invalid external record index %ld (count %ld).\n",(long)I(0xd3b170),(long)I(0x785d70));
            abort();
        }
        if(W(external+48)<2) {
            I(0xe20778)=0;FUN_004ccc30(8,4,cost,I(0xd3b160)+250,I(0xd3b15c)+384,I(0xf56f9c),0);
            FUN_004ccbb0(12,2,I(0xe20778)+250+I(0xd3b160),I(0xd3b15c)+384,I(0xf56f9c),0);
        }
    }
    FUN_004c7340();
}
