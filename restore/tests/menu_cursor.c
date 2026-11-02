#include "menu_cursor.h"
#include "menu_cursor_case.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Cursor case failed line%d\n",__LINE__);ExitProcess(2);}}while(0)
static uint32_t p[CURSOR_FIELDS],events[2][23],used,result;
static HCURSOR WINAPI set(HCURSOR cursor)
{
    uint32_t i,*event;CHECK(used<2);event=events[used++];event[0]=0x56f1bc;
    event[1]=1;event[2]=(uint32_t)(uintptr_t)cursor;event[3]=U(0xe390dc);
    for(i=0;i<19;++i)event[4+i]=U(0xe39090+i*4);
    if(p[CURSOR_MUTATIONS]&1)U(0xe390dc)=p[CURSOR_MUTATED];
    if(p[CURSOR_MUTATIONS]&2)for(i=0;i<19;++i)U(0xe39090+i*4)=p[CURSOR_MUTATED]+i*0x1020304u;
    return (HCURSOR)(uintptr_t)p[CURSOR_RETURN];
}
static const RestoreMenuCursorApi api={set};
const RestoreMenuCursorApi *restore_menu_cursor_platform(void){return &api;}
static void run(void)
{
    uint32_t i;for(i=0;i<144;++i)B(0xe39070+i)=(uint8_t)(p[CURSOR_SEED]+i*37u);
    for(i=0;i<19;++i)U(0xe39090+i*4)=p[CURSOR_HANDLES+i];
    used=result=0;memset(events,0,sizeof(events));
    if(p[CURSOR_OP]==0)FUN_004cf460(p[CURSOR_SELECTOR]);
    else result=(uint32_t)(uintptr_t)restore_menu_cursor_select(p[CURSOR_SELECTOR]);
    if(p[CURSOR_OP]==2)result=(uint32_t)(uintptr_t)restore_menu_cursor_select(p[CURSOR_SELECTOR2]);
}
static void output(void)
{
    uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<144;++i)printf("%02x",B(0xe39070+i));
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<23;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;CHECK(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        printf("{\"fields\":%d,\"handle_bytes\":%u}\n",CURSOR_FIELDS,(unsigned)sizeof(HCURSOR));fflush(stdout);
        for(;;){for(i=0;i<CURSOR_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}
    }else{
        memset(p,0,sizeof(p));p[CURSOR_OP]=1;p[CURSOR_SELECTOR]=0x10000001u;p[CURSOR_RETURN]=0x89abcdef;
        p[CURSOR_HANDLES+18]=0xa1b2c3d4;run();CHECK(result==0x89abcdef&&used==1&&events[0][2]==0xa1b2c3d4&&U(0xe390dc)==0xa1b2c3d4);
        p[CURSOR_SELECTOR]=18;p[CURSOR_HANDLES]=0x87654321;run();CHECK(events[0][2]==0x87654321);puts("Cursor DWORD selector, handle, state and API return passed.");
    }
done:restore_data_close();return 0;
}
