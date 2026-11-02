#include "tool_selector.h"
static void finish(void)
{
    TS_U(0x6059e4)=0;TS_U(0x6059dc)=0;TS_U(0x6059e0)=0;TS_U(0x6059d4)=0;TS_U(0x6059d8)=0;TS_B(0xe38dca)=1;
}
static void select_pair(uint32_t category,uint32_t mode)
{TS_U(0x605a30)=category;TS_U(0x6059e8)=mode;}
void FUN_00405200(void)
{
    int32_t category,selection,mode;
    if(TS_I(0xbca310)==1)FUN_00486060(TS_I(0xbc9800));
    if(TS_I(0xbca310)==2)TS_U(0xbca310)=0;
    category=TS_I(0x7056ec);
    TS_U(0x6059b4)=0;TS_U(0x6059b8)=0;TS_U(0x6059bc)=0;TS_U(0x6059c4)=0;TS_U(0x6059c8)=0;TS_U(0x6059cc)=0;TS_U(0x6059c0)=0;TS_U(0x605a48)=0;
    TS_U(0xbc9710)=TS_U(0xe92e0c);TS_U(0xbc9714)=0;TS_U(0xbca300)=0;TS_U(0xbca2c4)=0;TS_B(0x605a5c)=0;
    selection=TS_I(0xd3b198);
    switch(category) {
    case 0xe:TS_B(0xe38dda)=1;TS_U(0x605a34)=(uint32_t)selection-1u;finish();return;
    case 0x25:TS_B(0xe38dda)=1;TS_W(0x784b94)=(uint16_t)((uint32_t)selection-1u);finish();return;
    case 0x1b:
        switch(selection){case 1:select_pair(2,0x76);break;case 2:select_pair(0x1a,0x86);break;case 3:select_pair(0x1d,0xc5);break;case 4:select_pair(0x1b,8);break;}
        TS_B(0xe38dda)=1;finish();return;
    case 0xf:
        TS_B(0xe38dda)=1;TS_U(0x605a2c)=selection;
        if(selection==1)select_pair(0xf,0x7f);else if(selection==2)select_pair(0x10,0x80);finish();return;
    case 0x12:
        TS_B(0xe38dda)=1;if(selection==1)select_pair(0x12,0x82);else if(selection==2)select_pair(0x13,0x83);finish();return;
    case 0x13:
        TS_B(0xe38dda)=1;if(selection==1)select_pair(0x15,0x58);else if(selection==2)select_pair(0x16,0x59);else if(selection==3)select_pair(0x17,0x5d);finish();return;
    case 0x1a:
        if(selection<=8){select_pair(0x18,0x84);TS_U(0x605a24)=selection;}
        else if(selection<=12){select_pair(0x19,0x85);TS_U(0x605a1c)=(uint32_t)selection-8u;}
        else if(selection<=16){select_pair(0x1c,0x8b);TS_U(0x605a1c)=(uint32_t)selection-12u;}
        else {select_pair(0x1f,0xd6);TS_U(0x605a20)=(uint32_t)selection-16u;}
        TS_B(0xe38dda)=1;finish();return;
    case 0x11:select_pair(0xd,0x7e);TS_U(0x605a28)=selection;TS_B(0xe38dda)=1;finish();return;
    case 0x10:
        TS_B(0xe38dda)=1;if(selection==1)select_pair(0xb,0x7b);else if(selection==2)select_pair(0xc,0x7c);else if(selection==3)select_pair(9,0x81);finish();return;
    case 0:
        TS_B(0xe38dda)=1;mode=TS_I(0x5ce3b8u+(uint32_t)selection*4u);TS_I(0x6059e8)=mode;TS_U(0x605a30)=mode==0xc6?0x1e:mode==0xdf?0x21:5;finish();return;
    case 0x1c: {
        uint32_t record=0x5d19c8u+(uint32_t)(int32_t)TS_S(0x784ddeu+(uint32_t)selection*2u)*16u;
        int16_t subtype;
        if(TS_W(record)==1){mode=0xb7;TS_I(0x6059e8)=mode;TS_I(0x5e6970)=TS_S(record+4);goto ordinary_flags;}
        subtype=TS_S(record+2);
        if(subtype==1) {
            uint16_t saved=TS_W(0xea5baau+TS_U(0xeaac28)*0x49d8u);
            TS_U(0x6059e8)=0xd2;TS_W(0xea5baau+TS_U(0xeaac28)*0x49d8u)=1;FUN_0053d850();
            TS_W(0xea5baau+TS_U(0xeaac28)*0x49d8u)=saved;mode=TS_I(0x6059e8);goto classify;
        }
        switch(subtype){case 3:mode=0xd5;break;case 6:mode=0xe1;break;case 14:mode=0xe6;break;case 7:mode=0xe3;break;case 10:mode=0xe5;break;case 11:mode=0xea;break;case 12:mode=0xeb;break;case 13:mode=0xec;break;case 5:mode=0xe4;break;case 4:mode=0xde;break;
        case 2:mode=(int32_t)((uint32_t)(int32_t)TS_S(record+4)+0xcfu);TS_I(0x576144)=selection;TS_I(0x6059e8)=mode;goto classify;
        default:mode=TS_I(0x6059e8);goto classify;}
        TS_I(0x6059e8)=mode;goto ordinary_flags;
    }
    default:mode=TS_I(0x5ce3b8u+((uint32_t)selection+(uint32_t)category*30u)*4u);TS_I(0x6059e8)=mode;break;
    }
classify:
    {
        int32_t submenu=-1;
        switch(mode){case 2:submenu=0x14;break;case 3:submenu=0x15;break;case 4:submenu=0x16;break;case 0x30:submenu=0x1c;break;case 0x34:submenu=0x22;break;case 0x5b:submenu=0x23;break;case 0xae:submenu=0x1e;break;case 0xaf:submenu=0x1f;break;case 0xb0:submenu=0x20;break;case 0x60:submenu=0x17;break;case 0x61:submenu=TS_W(0xea5ce8u+TS_U(0xeaac28)*0x49d8u)?0x24:0x18;break;case 0x96:submenu=0x1d;break;case 0x39:submenu=0x19;break;}
        if(submenu!=-1){int32_t count;TS_I(0x7056ec)=submenu;count=FUN_00404120();TS_I(0x7056f0)=count;TS_U(0x7056e8)=TS_U(0x5cf63cu+(uint32_t)count*8u);TS_U(0x7056e4)=TS_U(0x5cf640u+(uint32_t)count*8u);TS_U(0x6059e8)=0;finish();return;}
    }
    switch(mode) {
    case 0x66:case 0x64:case 0x67:case 0x68:case 0x65:case 0xe0:case 0x69:case 0xc4:TS_U(0x6059cc)=1;goto selected;
    case 0xc2:TS_U(0x6059cc)=0;goto selected;
    case 0x6a:TS_U(0x605a48)=0x405b80;goto selected;
    case 0xdd:case 0x6b:case 0xd8:case 0xa2:TS_U(0x605a48)=0x405b60;goto selected;
    case 0xa1:case 0xd9:TS_U(0x605a48)=0x405b40;goto selected;
    case 0x6d:TS_U(0x6059c4)=1;goto selected;
    case 0x3a:case 0xaa:case 0xab:case 0xc8:case 0xc9:case 0xca:TS_U(0xbc9714)=1;goto selected;
    }
ordinary_flags:
    if(mode>=0x8c&&mode<=0x95)TS_U(0xbc9714)=1;
    else switch(mode){case 0x38:TS_U(0xbc9714)=1;break;case 0x3b:case 0xac:case 0xad:TS_U(0x6059bc)=1;break;case 0x8a:TS_U(0x6059b8)=1;break;case 0x5c:case 0xb4:case 0x4d:case 0x4e:case 0x4f:case 0xbb:case 0xbc:case 0xbd:TS_U(0x6059b4)=1;break;
    default:if(mode>=0x41&&mode<=0x45)TS_U(0xbc9714)=TS_U(0xbca2e0);else if(mode>=0x29&&mode<=0x2b)TS_U(0xbc9714)=0;break;}
selected:TS_B(0xe38dda)=1;finish();
}
