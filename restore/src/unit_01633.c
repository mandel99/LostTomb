#include "rich_message_text.h"
int32_t FUN_004c8480(const char *text)
{
    const uint8_t *p=(const uint8_t *)text;int32_t width=0,iterations=1;uint8_t nonspace=0,c;
    RT_U(0xe20760)=0;
    do {
        c=*p++;if(!c)return width;
        if(c=='@') {
            c=*p++;
            if(!nonspace) {
                if(c=='P'||c=='L'){RT_U(0xe20760)+=2;return 0;}
                if(c=='G') {
                    RT_U(0xe20760)+=2;c=*p++;
                    while(c>='0'&&c<='9'){RT_U(0xe20760)+=1;c=*p++;}
                    return 0;
                }
                do {RT_U(0xe20760)+=1;if(c<'0'||c>'9')break;c=*p++;}while(1);
            }
        }
        if(c==' '){if(nonspace)return width;width=rt_add(width,4);}
        else if(c>' '){width=rt_add(width,FUN_004cc020(c,RT_I(0xd3b168)));nonspace=1;}
        RT_U(0xe20760)+=1;iterations=rt_add(iterations,1);
    }while(iterations<2000);
    return width;
}
