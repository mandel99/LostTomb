#include "menu_window_cursor_consumer.h"
#include "menu_window_cursor_consumer_case.h"
#include "menu_cursor.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Actor order case failed line%d\n",__LINE__);ExitProcess(2);}}while(0)
typedef struct Region{uint32_t address,size;}Region;
static Region regions[24];static uint32_t used_regions,p[P_FIELDS],events[16][32],used_events,result;
static uint32_t original_id(void){return (int32_t)p[P_ID]>0?p[P_ID]:0;}
static uint32_t tile(uint32_t x,uint32_t y,uint32_t base){return base+y*228u+x;}
static uint32_t initial_tile(void){return tile(p[P_TARGET_X],p[P_TARGET_Y],p[P_BASE]);}
static uint32_t alternate_tile(void){return tile(p[P_ALT_X],p[P_ALT_Y],p[P_ALT_BASE]);}
static void add(uint32_t address,uint32_t size){CHECK(used_regions<24);regions[used_regions].address=address;regions[used_regions++].size=size;}
static int compare(const void *left,const void *right)
{uint32_t a=((const Region *)left)->address,b=((const Region *)right)->address;return a<b?-1:a>b?1:0;}
static void make_regions(void)
{
    uint32_t i,n=0,ids[]={original_id(),10,11,12,512},tiles[]={initial_tile(),alternate_tile()};used_regions=0;
    add(0xbca2b0,64);add(0xd056e0,64);add(0xd04e40,128);add(0xe38dc0,192);add(0xe39070,144);
    for(i=0;i<5;++i)add(0xbcd5f8u+ids[i]*388u-32u,452);
    for(i=0;i<2;++i){add(0xb350dcu+tiles[i]*2u-16u,34);add(0xaf598cu+tiles[i]*4u-16u,36);}
    qsort(regions,used_regions,sizeof(regions[0]),compare);
    for(i=0;i<used_regions;++i){
        if(n&&regions[i].address<=regions[n-1].address+regions[n-1].size){uint32_t end=regions[i].address+regions[i].size;if(end>regions[n-1].address+regions[n-1].size)regions[n-1].size=end-regions[n-1].address;}
        else regions[n++]=regions[i];
    }used_regions=n;
}
void restore_consumer_trace(uint32_t address,uint32_t count,const uint32_t *args)
{
    RestoreOrderActor *actor=restore_order_actor(original_id());uint32_t *e;CHECK(used_events<16&&count<=6);e=events[used_events++];memset(e,0,128);e[0]=address;e[1]=count;if(count)memcpy(e+2,args,count*4);
    e[8]=B(0xe38dda);e[9]=B(0xe38dca);e[10]=U(0xd05700);e[11]=U(0xd05708);
    e[12]=U(0xbca2c8);e[13]=U(0xbca2cc);e[14]=U(0xbca2d8);e[15]=actor->state;e[16]=(uint32_t)(int32_t)actor->route;
    e[17]=(uint32_t)(int32_t)actor->x;e[18]=(uint32_t)(int32_t)actor->y;e[19]=(uint32_t)(int32_t)actor->target_x;e[20]=(uint32_t)(int32_t)actor->target_y;
    e[21]=actor->action_tag;e[22]=actor->order_mode;e[23]=actor->can_special;e[24]=U(0xe390dc);e[25]=U(0xe390c4);e[26]=U(0xe390c8);e[28]=B(0xe38e60);e[29]=U(0xbca2c4);e[31]=p[P_SEED];
}
int32_t FUN_0040c180(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y)
{
    RestoreOrderActor *actor=restore_order_actor(original_id());uint32_t args[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty,0xabc00001,0xabc00002};CHECK(out_x!=out_y);restore_consumer_trace(0x40c180,6,args);*out_x=(int32_t)p[P_OUT_X];*out_y=(int32_t)p[P_OUT_Y];
    if(p[P_MUTATIONS]&1)U(0xd05700)=512;
    if(p[P_MUTATIONS]&2){actor->x=(int16_t)p[P_ALT_ACTOR_X];actor->y=(int16_t)p[P_ALT_ACTOR_Y];}
    if(p[P_MUTATIONS]&4){U(0xbca2c8)=p[P_ALT_X];U(0xbca2cc)=p[P_ALT_Y];U(0xbca2d8)=p[P_ALT_BASE];}
    if(p[P_MUTATIONS]&8){U(0xe390dc)=p[P_CURSOR_VALUE];U(0xd05708)=0xabcdef00;}
    return (int32_t)p[P_LINE_RETURN];
}
int32_t FUN_0040bda0(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y)
{
    uint32_t args[]={(uint32_t)x,(uint32_t)y,(uint32_t)tx,(uint32_t)ty,0xabc00003,0xabc00004};CHECK(out_x!=out_y);restore_consumer_trace(0x40bda0,6,args);*out_x=(int32_t)p[P_OTHER_X];*out_y=(int32_t)p[P_OTHER_Y];
    if(p[P_MUTATIONS]&32)restore_order_actor(original_id())->state=0xed;
    return (int32_t)p[P_OTHER_RETURN];
}
uint32_t FUN_004b6400(int32_t x,int32_t y,int32_t group,uint32_t flags)
{
    RestoreOrderActor *actor=restore_order_actor(original_id());uint32_t args[]={(uint32_t)x,(uint32_t)y,(uint32_t)group,flags};restore_consumer_trace(0x4b6400,4,args);
    if(p[P_MUTATIONS]&16){actor->target_x=-1234;actor->target_y=2345;actor->action_tag=0xbc;}
    return p[P_ACTION_RETURN];
}
static HCURSOR WINAPI set_cursor(HCURSOR cursor)
{uint32_t args[]={(uint32_t)(uintptr_t)cursor};restore_consumer_trace(0x56f1bc,1,args);return (HCURSOR)(uintptr_t)p[P_CURSOR_RETURN];}
static const RestoreMenuCursorApi cursor_api={set_cursor};
const RestoreMenuCursorApi *restore_menu_cursor_platform(void){return &cursor_api;}
static void setup(void)
{
    RestoreOrderActor *actor;uint32_t i,j,tiles[]={initial_tile(),alternate_tile()},head=p[P_CHAIN]?10:0;make_regions();
    for(i=0;i<used_regions;++i)for(j=0;j<regions[i].size;++j)B(regions[i].address+j)=(uint8_t)(p[P_SEED]+j*37u+i*11u);
    U(0xd05700)=p[P_ID];U(0xd05708)=p[P_ACTIVE];U(0xbca2c4)=p[P_GATE];U(0xbca2c8)=p[P_TARGET_X];U(0xbca2cc)=p[P_TARGET_Y];U(0xbca2d8)=p[P_BASE];
    B(0xe38e60)=(uint8_t)p[P_BLOCK];B(0xe38dda)=31;B(0xe38dca)=0;
    for(i=0;i<19;++i)U(0xe39090+i*4)=0xa000005au+i*0x10000u;
    U(0xe390c4)=p[P_CURSOR_C4];U(0xe390c8)=p[P_CURSOR_MATCH]==3?p[P_CURSOR_C4]:p[P_CURSOR_C8];
    U(0xe390dc)=p[P_CURSOR_MATCH]==1?U(0xe390c8):p[P_CURSOR_MATCH]==2||p[P_CURSOR_MATCH]==3?U(0xe390c4):p[P_CURSOR_VALUE];
    actor=restore_order_actor(original_id());actor->state=(uint8_t)p[P_STATE];actor->x=(int16_t)p[P_ACTOR_X];actor->y=(int16_t)p[P_ACTOR_Y];actor->route=(int16_t)p[P_ROUTE];actor->can_special=(uint8_t)p[P_CAN_SPECIAL];
    if((int16_t)p[P_ROUTE]>0)W(0xd04e58u+(uint32_t)(int32_t)(int16_t)p[P_ROUTE]*2u)=(uint16_t)(p[P_ID]+(p[P_OWNER_MATCH]?0u:1u));
    for(i=0;i<3;++i){actor=restore_order_actor(10+i);actor->kind=(uint8_t)p[P_KIND];actor->tag=(int16_t)p[P_TAG];actor->next=0;}
    if(p[P_CHAIN]==2){restore_order_actor(10)->kind=10;restore_order_actor(10)->next=11;}
    if(p[P_CHAIN]==3){restore_order_actor(10)->kind=15;restore_order_actor(10)->next=11;restore_order_actor(11)->kind=0;restore_order_actor(11)->next=12;}
    for(i=0;i<2;++i){W(0xb350dcu+tiles[i]*2u)=(uint16_t)head;U(0xaf598cu+tiles[i]*4u)=p[P_TILE_FLAGS];}
    used_events=result=0;memset(events,0,sizeof(events));
}
static void run(void)
{
    setup();switch(p[P_OP]){
    case 0:FUN_0040ba40();break;
    case 1:result=(uint32_t)FUN_004b5520(initial_tile());break;
    case 2:result=FUN_0051cd80((int32_t)p[P_DIR_X],(int32_t)p[P_DIR_Y],(int32_t)p[P_DIR_TO_X],(int32_t)p[P_DIR_TO_Y]);break;
    case 3:result=(uint32_t)FUN_004cde60((int32_t)p[P_SCALE_VALUE],(int32_t)p[P_SCALE_DIVISOR]);break;
    case 4:FUN_004cf460(p[P_CURSOR_SELECTOR]);FUN_0040ba40();break;
    default:CHECK(0);
    }
}
static void output(void)
{
    uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<used_regions;++i)for(j=0;j<regions[i].size;++j)printf("%02x",B(regions[i].address+j));
    printf("\",\"events\":[");for(i=0;i<used_events;++i){printf("%s[",i?",":"");for(j=0;j<32;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        printf("{\"fields\":%d,\"actor_bytes\":%u}\n",P_FIELDS,(unsigned)sizeof(RestoreOrderActor));fflush(stdout);
        for(;;){for(i=0;i<P_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}
    }else{
        memset(p,0,sizeof(p));p[P_ID]=256;p[P_GATE]=256;p[P_ACTIVE]=256;p[P_STATE]=1;p[P_CURSOR_MATCH]=1;p[P_CURSOR_C4]=0x1122005a;p[P_CURSOR_C8]=0x3344005a;p[P_CURSOR_VALUE]=0x5566005a;
        p[P_TARGET_X]=5;p[P_TARGET_Y]=2;p[P_BASE]=4096;p[P_ALT_X]=7;p[P_ALT_Y]=3;p[P_ALT_BASE]=4096;p[P_OUT_X]=0x1234abcd;p[P_OUT_Y]=0xfedc3456;
        run();CHECK(B(0xe38dda)==1&&B(0xe38dca)==1&&U(0xd05708)==0&&restore_order_actor(256)->target_x==(int16_t)p[P_OUT_X]);puts("Whole actor-order DWORD cursor/ID gates and WORD target stores passed.");
    }
done:restore_data_close();return 0;
}
