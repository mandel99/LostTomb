#include "upper_raster.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define SRC 0x31000000u
#define DST 0x31100000u
#define CAPACITY 0x20000u
static uint32_t skips[4][2],skip_count,checks;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Upper RLE failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
char *upper_skip_impl(char *pixels);
char *FUN_00410f20(char *pixels)
{
    char *result;CHECK(skip_count<4);skips[skip_count][0]=(uint32_t)(uintptr_t)pixels;
    result=upper_skip_impl(pixels);skips[skip_count++][1]=(uint32_t)(uintptr_t)result;return result;
}
static void prepare(const uint32_t p[12])
{
    int width=(int16_t)p[1],skip=(int32_t)p[5];unsigned row,col,cursor=0,rows=p[2]+(skip>0?(unsigned)skip:0)+1;
    DWORD previous;
    CHECK(VirtualProtect((void *)SRC,CAPACITY,PAGE_READWRITE,&previous));memset((void *)SRC,0xa5,CAPACITY);
    for(row=0;width>0&&row<rows;++row){
        col=0;if(p[10]==20){B(SRC+cursor++)=255;B(SRC+cursor++)=0;}
        while(col<(unsigned)width){
            unsigned run=p[10]>=1&&p[10]<=16?p[10]:1+(row+col+p[11])%16;
            int transparent=p[10]==18||((p[10]==17||p[10]==19)&&(row+col+p[11])%3==0);
            unsigned i;if(p[10]==18)run=255;if(run>(unsigned)width-col)run=(unsigned)width-col;
            CHECK(cursor+run*2+2<CAPACITY);B(SRC+cursor++)=(uint8_t)(transparent?255:run);
            if(transparent)B(SRC+cursor++)=(uint8_t)run;
            else for(i=0;i<run;++i){uint16_t value=(uint16_t)((row*(unsigned)width+col+i)*37u+p[11]);memcpy((void *)(uintptr_t)(SRC+cursor),&value,2);cursor+=2;}
            col+=run;
        }
    }
    CHECK(VirtualProtect((void *)SRC,CAPACITY,PAGE_READONLY,&previous));
    for(row=0;row<CAPACITY;++row)B(DST+row)=(uint8_t)((row*13u+(row>>7)*7u)^0xc7u);
    memset((void *)0xd3b520u,0x5a,224);W(0xd3b5be)=(uint16_t)p[1];U(0xd3b5d4)=p[2];
    U(0xd3b5e0)=p[3];U(0xd3b5dc)=p[4];U(0xd3b538)=p[5];U(0xd3b540)=p[6]*2u;U(0xd3b544)=p[7];
    U(0xe92e18)=DST;U(0xe38d94)=p[8];U(0xd3b53c)=p[8]-p[1]*2u;W(0xd3b51c)=(uint16_t)p[9];skip_count=0;
}
static uint32_t invoke(unsigned mode,uint16_t mask)
{
    void (*draw)(void)=NULL;const void *pixels=(void *)SRC;
    uint32_t result,sp_before,sp_after,bp_before,bp_after,bx_saved,si_saved,di_saved;
    switch(mode){
    case 0:draw=(void (*)(void))FUN_00410230;break;case 1:draw=(void (*)(void))FUN_004105c0;break;
    case 2:draw=(void (*)(void))FUN_00410660;break;case 3:draw=(void (*)(void))FUN_00410700;break;
    case 4:draw=(void (*)(void))FUN_004109d0;break;case 5:draw=(void (*)(void))FUN_00410a70;break;
    case 6:draw=(void (*)(void))FUN_00410f20;break;default:CHECK(0);
    case 10:draw=(void (*)(void))FUN_00410b10;break;case 11:draw=(void (*)(void))FUN_00410de0;break;
    case 12:draw=(void (*)(void))FUN_00410e80;break;
    }
    __asm {
        mov sp_before,esp
        mov bp_before,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        movzx eax,mask
        push eax
        push pixels
        call draw
        add esp,8
        mov result,eax
        mov sp_after,esp
        mov bp_after,ebp
        mov bx_saved,ebx
        mov si_saved,esi
        mov di_saved,edi
    }
    CHECK(sp_before==sp_after&&bp_before==bp_after);
    CHECK(bx_saved==0x11223344u&&si_saved==0x33445566u&&di_saved==0x44556677u);
    return mode==1||mode==2||mode==6||mode==11||mode==12?result:0;
}
static uint32_t run(const uint32_t p[12])
{
    uint32_t result;prepare(p);
    if(p[0]<7||p[0]>=10)return invoke(p[0],(uint16_t)p[9]);
    result=invoke(p[0]-7,(uint16_t)p[9]);CHECK(result==0);invoke(p[0]-4,(uint16_t)p[9]);return 0;
}
static void oracle(void)
{
    uint32_t p[12],result,ready=0x55524c45;unsigned i;
    _setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
    while(1){for(i=0;i<12;++i)if(scanf("%u",&p[i])!=1)return;result=run(p);
        fwrite(&result,4,1,stdout);fwrite(&skip_count,4,1,stdout);fwrite((void *)0xd3b520u,224,1,stdout);
        fwrite((void *)DST,CAPACITY,1,stdout);fwrite(skips,8,skip_count,stdout);fflush(stdout);}
}
int main(int argc,char **argv)
{
    uint32_t p[12]={2,58,3,300,8,2,0,28,2048,0xf863,17,123},result;
    CHECK(restore_data_open());CHECK(VirtualAlloc((void *)SRC,CAPACITY,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)SRC);
    CHECK(VirtualAlloc((void *)DST,CAPACITY,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)DST);
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){oracle();return 0;}
    result=run(p);CHECK(result==0&&skip_count==1&&skips[0][1]>SRC);
    p[0]=8;p[6]=30;p[7]=28;CHECK(run(p)==0&&skip_count==2&&skips[0][1]==skips[1][1]);
    p[0]=6;result=run(p);CHECK(result==skips[0][1]&&result>SRC);
    p[5]=0;CHECK(run(p)==SRC);p[5]=2;p[1]=0;CHECK(run(p)==SRC);
    p[1]=58;p[6]=30;p[7]=28;p[9]=0x1234;p[10]=1;
    p[0]=12;CHECK(run(p)==0&&W(DST+8*2048+300*2)==0x1234);
    p[0]=11;CHECK(run(p)==0&&W(DST+8*2048+300*2)==0x1234);
    p[0]=10;CHECK(run(p)==0&&W(DST+8*2048+300*2)==0x1234);
    printf("Upper RLE: %u checks; full framebuffer/pitch/coordinates, vertical skip pointer, paired copy/mask, ABI passed.\n",checks);
    return 0;
}
