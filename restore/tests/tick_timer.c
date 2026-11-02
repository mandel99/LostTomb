#include "simulation_tick.h"
#include "graphics_session.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
static DWORD current_tick;
static DWORD WINAPI tick(void) {return current_tick;}
static RestoreGraphicsApi platform;
const RestoreGraphicsApi *restore_graphics_platform(void) {return &platform;}
static unsigned int checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Tick timer failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t run(const uint32_t p[10])
{
    unsigned int i;current_tick=p[0];GFX_U(0xe94db8)=p[1];GFX_U(0xe38e6c)=p[2];
    GFX_B(0xe38e60)=(uint8_t)p[3];GFX_B(0xe38e61)=0xa5;GFX_B(0xe38dda)=(uint8_t)p[4];
    GFX_U(0xd3a6c4)=p[5];GFX_U(0x6059e4)=p[6];GFX_U(0xe38dcc)=p[7];
    REQUIRE(p[4]<89);GFX_U(0x5e1c50+p[4]*4)=p[8];
    for(i=0;i<10;++i)GFX_U(0x5e1db4+i*4)=p[9];
    return FUN_004ddcb0();
}
int main(int argc,char **argv)
{
    uint32_t p[10]={1000,900,90,0,0,0,0,0,0,50},result;unsigned int i;
    REQUIRE(restore_data_open());platform.ticks=tick;
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":2}");fflush(stdout);
        for(;;) {
            for(i=0;i<10;++i)if(scanf("%u",&p[i])!=1)goto done;
            result=run(p);printf("{\"result\":%u,\"last\":%u}\n",result,GFX_U(0xe94db8));fflush(stdout);
        }
    } else {
        REQUIRE(run(p)==10000&&GFX_U(0xe94db8)==1000&&GFX_B(0xe38e61)==0xa5);
        p[1]=990;REQUIRE(run(p)==0&&GFX_U(0xe94db8)==990);
        p[0]=10;p[1]=0xfffffff0u;p[9]=20;REQUIRE(run(p)==10000);
        p[0]=1;p[1]=2;REQUIRE(run(p)==10000);
        p[2]=0;REQUIRE(run(p)==0);p[2]=90;
        p[6]=0x100;REQUIRE(run(p)==0);p[6]=0;
        p[7]=0x100;REQUIRE(run(p)==0);p[7]=0;
        p[3]=1;REQUIRE(run(p)==0);p[3]=0;
        p[4]=0x12;p[5]=0;REQUIRE(run(p)==0);p[5]=1;REQUIRE(run(p)==10000);
        p[4]=0x44;p[8]=1;REQUIRE(run(p)==0);
        printf("Tick timer: %u checks; full clock/speed, wraparound, mode table and pause gates.\n",checks);
    }
done:restore_data_close();return 0;
}
