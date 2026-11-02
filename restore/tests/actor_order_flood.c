#include "actor_order_flood.h"
#include "actor_order_flood_case.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Flood case failed line%d\n",__LINE__);ExitProcess(2);}}while(0)
static const uint32_t full_regions[][2]={{0xbca2c0,48},{0xeadff0,0xcb40},{0xed4130,0xcb30},{0xeed750,0x658a0}};
static uint32_t p[F_FIELDS],events[2][6],used,result;
static void carve(uint32_t x,uint32_t y,uint32_t depth)
{
    uint32_t cell=y*228u+x,step=p[F_WEIGHT]>>(depth/2u),i;
    CHECK(x>0&&x<227&&y>0&&y<227);B(0xed4140u+cell)=0xfe;
    if(depth>=p[F_WIDTH]||!step)return;
    for(i=1;i<=step;++i){
        if(depth&1u){B(0xed4140u+(y-i)*228u+x)=0xfe;B(0xed4140u+(y+i)*228u+x)=0xfe;}
        else{B(0xed4140u+y*228u+x-i)=0xfe;B(0xed4140u+y*228u+x+i)=0xfe;}
    }
    if(depth&1u){carve(x,y-step,depth+1u);carve(x,y+step,depth+1u);}
    else{carve(x-step,y,depth+1u);carve(x+step,y,depth+1u);}
}
static void region(uint32_t i,uint32_t *address,uint32_t *size)
{if(p[F_OP]){CHECK(i<2);*address=i?0xeed750:0xeae000;*size=i?96:32;}else{*address=full_regions[i][0];*size=full_regions[i][1];}}
void restore_flood_trace(uint32_t destination,uint32_t bytes,uint32_t value)
{uint32_t *e;CHECK(used<2);e=events[used++];e[0]=destination;e[1]=bytes;e[2]=value;e[3]=U(0xeae004);e[4]=U(0xeae008);e[5]=U(0xeae00c);}
static void setup(void)
{
    uint32_t i,j,address,size,x,y,w=p[F_WIDTH],h=p[F_HEIGHT],start=p[F_BASE]+p[F_Y]*228u+p[F_X];
    for(i=0;i<(p[F_OP]?2u:4u);++i){region(i,&address,&size);for(j=0;j<size;++j)B(address+j)=(uint8_t)(p[F_SEED]+j*37u+i*11u);}
    if(p[F_OP]){used=result=0;return;}
    U(0xbca2d8)=p[F_BASE];
    memset((void *)0xed4140u,0xfd,51984);
    if(p[F_STYLE]>=7){
        carve(p[F_X],p[F_Y],0);
        if(p[F_STYLE]==8)for(y=1;y<227;++y)for(x=1;x<227;++x){uint32_t cell=y*228u+x,hash=x*1103515245u+y*12345u+p[F_SEED];if(B(0xed4140u+cell)==0xfe&&((hash>>16)%100u)>=p[F_HEIGHT])B(0xed4140u+cell)=0;}
    }else for(y=1;y+1<h;++y)for(x=1;x+1<w;++x){uint32_t hash=x*1103515245u+y*12345u+p[F_SEED];uint8_t value=0;
        switch(p[F_STYLE]){
        case 0:value=(uint8_t)p[F_CLASS];break;
        case 1:value=0;break;
        case 2:value=((x+y)&1)?0xfe:0;break;
        case 3:value=(x&1)?0xfe:0;break;
        case 4:value=((hash>>16)%100u)<p[F_WEIGHT]?0xfe:0;break;
        case 5:value=((hash>>16)%100u)<p[F_WEIGHT]?0xff:0;break;
        case 6:value=(x%8u==0&&y%8u!=1)?0xfd:((x+y)%3u==0?0xfe:0);break;
        default:CHECK(0);
        }
        B(0xed4140u+y*228u+x)=value;
    }
    CHECK(start<51984);if(p[F_STYLE]!=0)B(0xed4140u+start)=(uint8_t)p[F_CLASS];
    used=result=0;memset(events,0,sizeof(events));
}
static void run(void)
{setup();if(p[F_OP])FUN_0040f8f0((uint32_t *)(uintptr_t)(0xeed760u+p[F_ALIGN]),p[F_FILL_BYTES],(uint8_t)p[F_FILL_VALUE]);else result=FUN_0051ce90((int32_t)p[F_X],(int32_t)p[F_Y]);}
static void metrics(uint32_t *visited,uint32_t *retries)
{uint32_t i;*visited=*retries=0;if(p[F_OP])return;for(i=0;i<51984;++i){uint32_t n=B(0xeae010u+i);if(U(0xeed760u+i*4u))++*visited;if(n>4)n=4;*retries+=n;}}
static void output(void)
{
    uint32_t i,j,address,size,visited,retries;metrics(&visited,&retries);printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<(p[F_OP]?2u:4u);++i){region(i,&address,&size);for(j=0;j<size;++j)printf("%02x",B(address+j));}
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<6;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
    printf("],\"metrics\":[%u,%u,%u,%u]}\n",visited,retries,p[F_OP]?0:U(0xeae008),p[F_OP]?0:U(0xeae00c));fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        printf("{\"fields\":%d}\n",F_FIELDS);fflush(stdout);for(;;){for(i=0;i<F_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}
    }else if(argc==2&&!strcmp(argv[1],"--profile")){
        memset(p,0,sizeof(p));p[F_X]=p[F_Y]=114;p[F_WIDTH]=12;p[F_CLASS]=0xfe;p[F_STYLE]=8;
        for(i=0;i<10000;++i){uint32_t visited,retries,t,d,mask=0;p[F_WEIGHT]=i&1u?48u:32u;p[F_HEIGHT]=80u+(i%21u);p[F_SEED]=i*1234567u;run();metrics(&visited,&retries);
            if(visited+retries>=51984u){
                t=U(0xf203a0u+51983u*4u);d=U(0xeed760u+t*4u)-(B(0xed4140u+t)==0xfe?5u:1u);
                if(U(0xeed760u+(t+228u)*4u)==d)mask|=1u;
                if(U(0xeed760u+(t-1u)*4u)==d)mask|=2u;
                if(U(0xeed760u+(t-228u)*4u)==d)mask|=4u;
                if(U(0xeed760u+(t+1u)*4u)==d)mask|=8u;
                printf("%u %u %u %u %u %u %u %u %u %u\n",i,p[F_WEIGHT],p[F_HEIGHT],visited,retries,U(0xeae008),U(0xeae00c),t,B(0xeae010u+t),mask);
            }
        }fflush(stdout);
    }else{
        memset(p,0,sizeof(p));p[F_STYLE]=1;p[F_X]=p[F_Y]=2;p[F_WIDTH]=p[F_HEIGHT]=5;run();CHECK(result==1&&U(0xeed760u+(2u*228u+2u)*4u)==1&&U(0xeae008)==9&&U(0xeae00c)==9);puts("Closed flood DWORD/BYTE fields and actual clears passed.");
    }
done:restore_data_close();return 0;
}
