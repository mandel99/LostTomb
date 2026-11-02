#include "rich_message_text.h"
void FUN_004c8070(const char *text,int32_t x,int32_t y,int32_t width,int32_t calculate,int32_t flags)
{
    const uint8_t *p=(const uint8_t *)text;int32_t remaining=0,image=0,delay=0,line=0,iterations=0;
    uint8_t more=1,indent=0;
    if(calculate==1)RT_U(0xd3b138)=0;
    for(;;) {
        int32_t used,x_indent,length=0;uint8_t trim=1,skip=0;
        if((!more&&!remaining)||(iterations=rt_add(iterations,1))>=1000)return;
        memset((void *)0xe3439cu,0,200);used=x_indent=indent?50:0;indent=0;
        while((more||remaining)&&used<width) {
            int32_t consumed,n;
            if(remaining){remaining=rt_sub(remaining,1);break;}
            used=rt_add(used,FUN_004c8480((const char *)p));
            if(used>=width){if(!used)more=0;continue;}
            consumed=RT_I(0xe20760);
            for(n=0;n<consumed;n=rt_add(n,1)) {
                uint8_t c=*p++;
                if(c=='@') {
                    if(*p=='P'){indent=1;++p;used=width;break;}
                    if(*p=='L'){++p;used=width;break;}
                    if(*p=='G') {
                        if(length&&calculate==1)RT_U(0xd3b138)+=1;
                        ++p;image=rt_sub(rt_add(RT_I(0xf55c04),FUN_004cc070((const char *)p)),1);
                        c=*p++;while(c>='0'&&c<='9')c=*p++;
                        remaining=rt_add(rt_metric(image,22)/16,2);used=width;if(line>0)delay=1;break;
                    }
                }
                if(!trim||c!=' '){trim=0;RT_B(0xe3439cu+(uint32_t)length)=c;length=rt_add(length,1);}
            }
            if(!*p)more=0;
        }
        RT_U(0xe20780)=1;RT_U(0xe20778)=0;RT_B(0xe20799)=1;
        if(!calculate)skip=line<RT_I(0xd3b130)||line>=rt_add(RT_I(0xd3b13c),RT_I(0xd3b130));
        if(calculate||!skip) {
            FUN_004c8590((const char *)0xe3439cu,rt_add(x,x_indent),y,calculate,flags);
            if(RT_I(0xd3b168)==RT_I(0xf56f84)) {
                uint16_t color=RT_W(0xd3b51c);RT_W(0xd3b51c)=RT_I(0xe92dfc)==2?0xff3a:0x7f9a;
                FUN_004c8590((const char *)0xe3439cu,rt_sub(rt_add(x,x_indent),1),rt_sub(y,1),calculate,flags);
                RT_W(0xd3b51c)=color;
            }
        }
        if(!calculate&&image) {
            if(delay)delay=rt_sub(delay,1);
            else {
                int32_t sy,sx,center=rt_sub(rt_sub((int32_t)(RT_U(0xd3b14c)*16u),rt_metric(image,20)),4);
                remaining=rt_add(rt_metric(image,22)/16,2);
                if(line<rt_add(RT_I(0xd3b13c),RT_I(0xd3b130))) {
                    sy=rt_add(y,8);if(line<RT_I(0xd3b130))sy=rt_add(sy,(int32_t)((uint32_t)rt_sub(line,RT_I(0xd3b130))*16u));
                    sx=rt_sub(rt_add(x,center/2),4);FUN_0053e590(image,sx,sy);
                }
                image=0;
            }
        }
        if(RT_I(0xe20778)>RT_I(0xe2076c))RT_U(0xe2076c)=RT_U(0xe20778);
        line=rt_add(line,1);if(calculate==1)RT_U(0xd3b138)+=1;
        if(!skip){y=rt_add(y,16);RT_U(0xe20774)+=16;}
    }
}
