#include "sprite_render.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#define SRC ((uint8_t *)0x31000000u)
#define DST ((uint8_t *)0x31100000u)
#define CAPACITY 0x100000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static unsigned int checks,scenarios;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Pixel copy failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint16_t sample(unsigned int i,unsigned int seed) {return i%7==0?0xf81f:(uint16_t)(i*37+seed);}
/* Native 40FED0/40FF50 write the first source pixel at x+width.
   Include high address/coordinate bytes, a width above 255, transparency,
   clipped source padding, full destination guards and source preservation. */
static void mirrored_copy(int clipped,unsigned int width,unsigned int height)
{
    unsigned int row,col,source_stride=width*2u+(clipped?14u:0u);
    const unsigned int x=273,y=263,pitch=1280;
    uint8_t *expected=malloc(CAPACITY),*before=malloc(CAPACITY);
    REQUIRE(expected&&before);
    memset(SRC,0xa5,CAPACITY);memset(DST,0xa5,CAPACITY);memset(expected,0xa5,CAPACITY);
    for(row=0;row<height;++row)for(col=0;col<width;++col){
        uint16_t value=sample(row*width+col,123);
        size_t s=(size_t)row*source_stride+col*2u;
        size_t d=(size_t)(y+row)*pitch+(x+width-col)*2u;
        REQUIRE(s+2<=CAPACITY&&d+2<=CAPACITY);memcpy(SRC+s,&value,2);
        if(value!=0xf81f)memcpy(expected+d,&value,2);
    }
    memcpy(before,SRC,CAPACITY);
    U(0xe92e18)=(uint32_t)(uintptr_t)DST;U(0xe38d94)=pitch;
    U(0xd3b5e0)=x;U(0xd3b5dc)=y;U(0xd3b544)=width;
    U(0xd3b5d4)=height;U(0xd3b53c)=pitch-width*2u;
    U(0xd3b540)=clipped?14u:0u;*(int16_t *)0xd3b5beu=(int16_t)width;
    if(clipped)FUN_0040ff50(SRC);else FUN_0040fed0(SRC);
    REQUIRE(!memcmp(expected,DST,CAPACITY));REQUIRE(!memcmp(before,SRC,CAPACITY));
    free(before);free(expected);++scenarios;
}
static size_t run_fill(const unsigned int p[9],int compare)
{
    unsigned int row,col,i,width=p[1],height=p[2],skip=p[5];
    size_t cursor=0,size=(size_t)p[7]*(p[4]+height)+(size_t)(p[3]+width)*2+64;
    uint8_t *expected=malloc(CAPACITY),*source_before=malloc(CAPACITY);
    uint16_t color=(uint16_t)p[8];
    REQUIRE(expected&&source_before&&size<CAPACITY);
    memset(SRC,0xa5,CAPACITY);memset(DST,0xa5,CAPACITY);memset(expected,0xa5,CAPACITY);
    for(row=0;row<height+skip;++row)for(col=0;col<width;) {
        unsigned int run=1+(row+col)%16;
        int transparent=(row+col)%3==0;
        if(run>width-col)run=width-col;
        REQUIRE(cursor+2+run*2<CAPACITY);
        SRC[cursor++]=transparent?255:(uint8_t)run;
        if(transparent)SRC[cursor++]=(uint8_t)run;
        else for(i=0;i<run;++i) {
            uint16_t value=sample(row*width+col+i,123);
            memcpy(SRC+cursor,&value,2);cursor+=2;
            if(row>=skip) {
                size_t d=(size_t)p[4]*p[7]+p[3]*2+(size_t)(row-skip)*(width*2+p[6])+(col+i)*2;
                REQUIRE(d+2<CAPACITY);memcpy(expected+d,p[0]==3?&color:&value,2);
            }
        }
        col+=run;
    }
    memcpy(source_before,SRC,CAPACITY);
    U(0xe92e18)=(uint32_t)(uintptr_t)DST;U(0xe38d94)=p[7];U(0xd3b5e0)=p[3];U(0xd3b5dc)=p[4];
    U(0xd3b5d4)=height;U(0xd3b538)=skip;U(0xd3b53c)=p[6];
    *(int16_t *)0xd3b5beu=(int16_t)width;memcpy((void *)0xd3b51cu,&color,2);
    if(p[0]==3)FUN_00410b10(SRC);else FUN_00410230(SRC);
    if(compare)REQUIRE(!memcmp(expected,DST,CAPACITY));
    REQUIRE(!memcmp(source_before,SRC,CAPACITY));free(source_before);free(expected);
    ++scenarios;return size;
}
static size_t run(const unsigned int p[9],int compare)
{
    if(p[0]>=3)return run_fill(p,compare);
    unsigned int i,row,col,width=p[1],height=p[2],skip=p[0]==1?0:p[5];
    size_t size=(size_t)p[7]*(p[4]+height)+(size_t)(p[3]+width)*2+64;
    uint8_t *expected=NULL;
    REQUIRE(p[0]<3&&size<CAPACITY&&((size_t)width*2+skip)*height<CAPACITY);
    for(i=0;i<CAPACITY/2;++i) {uint16_t value=sample(i,p[8]);memcpy(SRC+i*2,&value,2);}
    memset(DST,0xa5,CAPACITY);U(0xe92e18)=(uint32_t)(uintptr_t)DST;U(0xe38d94)=p[7];
    U(0xd3b5e0)=p[3];U(0xd3b5dc)=p[4];U(0xd3b544)=width;U(0xd3b5d4)=height;
    *(int16_t *)0xd3b5beu=(int16_t)width;U(0xd3b540)=p[5];U(0xd3b53c)=p[6];
    if(compare) {
        expected=malloc(CAPACITY);REQUIRE(expected);memset(expected,0xa5,CAPACITY);
        for(row=0;row<height;++row)for(col=0;col<width;++col) {
            size_t s=(size_t)row*(width*2+skip)+col*2;
            size_t d=(size_t)p[4]*p[7]+p[3]*2+(size_t)row*(width*2+p[6])+col*2;
            uint16_t value;memcpy(&value,SRC+s,2);
            if(!p[0]||value!=0xf81f)memcpy(expected+d,&value,2);
        }
    }
    if(p[0]==0)FUN_0040fb60(SRC);else if(p[0]==1)FUN_0040ffd0(SRC);else FUN_00410090(SRC);
    if(compare) {REQUIRE(!memcmp(expected,DST,CAPACITY));free(expected);}
    for(i=0;i<CAPACITY/2;++i) {uint16_t value;memcpy(&value,SRC+i*2,2);if(value!=sample(i,p[8])) {REQUIRE(0);break;}}
    ++scenarios;return size;
}
int main(int argc,char **argv)
{
    unsigned int p[9]={0,8,7,2,3,6,1264,1280,123},i,j,k;size_t size;
    unsigned int widths[]={1,2,3,4,7,31,640},heights[]={1,2,7,480};
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc(SRC,CAPACITY,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==SRC);
    REQUIRE(VirtualAlloc(DST,CAPACITY,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==DST);
    if(argc==2&&!strcmp(argv[1],"--mirrored-copy")) {
        mirrored_copy(0,7,3);mirrored_copy(1,7,3);
        mirrored_copy(0,300,3);mirrored_copy(1,300,3);
        mirrored_copy(0,7,0);mirrored_copy(1,0,3);
        printf("Mirrored copies: %u checks, %u focused cases; complete output/source and native pixel bias verified.\n",checks,scenarios);
    }else if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);
        puts("{\"source\":822083584,\"destination\":823132160}");fflush(stdout);
        for(;;) {
            for(i=0;i<9;++i)if(scanf("%u",&p[i])!=1)goto done;
            size=run(p,0);printf("{\"bytes\":%u}\n",(unsigned int)size);fwrite(DST,1,size,stdout);fflush(stdout);
        }
    } else {
        for(i=0;i<5;++i)for(j=0;j<7;++j)for(k=0;k<4;++k) {
            p[0]=i;p[1]=widths[j];p[2]=heights[k];p[6]=p[7]-p[1]*2;run(p,1);
        }
        p[1]=0;p[2]=0;run(p,1);
        printf("Pixel copies: %u checks across %u scenarios; complete destination, row padding, transparency and source preservation verified.\n",checks,scenarios);
    }
done:
    VirtualFree(SRC,0,MEM_RELEASE);VirtualFree(DST,0,MEM_RELEASE);restore_data_close();return 0;
}
