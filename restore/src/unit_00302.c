#include <stdint.h>
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern int FUN_0041e3b0(void);
int FUN_0041e470(void)
{
    int score=I(0x784c30)+(I(0x784c28)+I(0x784c2c)*10+I(0x784c24))*2;
    int count,i,trade=0,gods=0;
    if(I(0x784c58)&&I(0x784c5c)>500) {
        int value=I(0x784c5c);
        score+=value<=2000?20:value<=4000?50:value<=8000?80:100;
    }
    count=FUN_0041e3b0();
    if(count>0)score+=count<=2?40:count<=4?60:count<=10?80:100;
    /* Native reverse scan deliberately visits records count-1 through 1. */
    for(i=I(0x785d70)-1;i>0;--i)
        if(*(uint8_t *)(uintptr_t)(0x785d74u+(uint32_t)i*124u+2)==1)++trade;
    score+=trade<=5?20:trade<=10?40:trade<=20?60:trade<=30?80:100;
    for(i=0;i<5;++i) {
        int value=*(int16_t *)(uintptr_t)(0x7848e2u+(uint32_t)i*2u);
        if(value==1)++gods;else if(value==2)gods+=2;
    }
    if(gods>0)score+=gods<=2?20:gods<=5?50:gods<=8?80:100;
    return score;
}
