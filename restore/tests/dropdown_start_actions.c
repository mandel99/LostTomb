#include "restore_types.h"
#include "dropdown_start_actions.h"
#include "dropdown_menu.h"
#include "menu_input.h"
#include "file_list.h"
#include "sound_streams.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Start menu failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0x6057b0,0x2b0},{0x6fdc90,0x90},{0x7027a0,48},{0x705700,64},{0x78d790,64},{0xbc9680,48},{0xd3b180,96},{0xe20980,48},{0xe36ee0,48},{0xe38dc0,256},{0xe94df0,48},{0xe92be0,0x240},{0xf53f20,0x70},{0xf55b70,48},{0x6f2000,64},{0x6f3000,0x400}};
static uint32_t p[32],used,result,events[64][16];
static const char *names[]={"Alpha.map","Beta.extra.map",".hidden.map","no_extension","Gamma.MAP","zeta.map","  spaced.name.map","Last."};
static const char *profiles[]={"Restore","Alpha.Beta","","City"};
extern void FUN_004d3c40(void);
extern void restore_start_selector_body(uint32_t,char);
extern uint32_t restore_start_stream_body(RestoreSound *,const char *,int,uint32_t);
extern int FUN_00562370(const char *,const char *);
static uint32_t hash(const char *text){uint32_t v=2166136261u;while(*text)v=(v^(uint8_t)*text++)*16777619u;return v;}
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{
    uint32_t *e;CHECK(used<64);e=events[used++];memset(e,0,64);e[0]=a;e[1]=n;if(n)memcpy(e+2,v,n*4);
    e[8]=DM_B(0xe38dda);e[9]=DM_B(0xe38dca);e[10]=DM_U(0x605a3c);e[11]=DM_B(0xe38e64);e[12]=DM_U(0xf55b88);e[13]=RESTORE_SOUND->streams[0];e[14]=DM_U(0x6057d0);e[15]=DM_U(0xd3b198);
}
static void family(uint32_t token){event(token,0,NULL);if(p[16]&1)DM_B(0xe38dda)=(uint8_t)p[17];if(p[16]&2)DM_B(0xe38e64)=(uint8_t)p[18];if(p[16]&4)DM_U(0xf55b88)=p[19];}
void restore_start_family(void){family(0x516bb0);}
void restore_start_choose(void){family(0x516990);}
void restore_start_campaign(void){family(0x4cb6a0);}
int restore_start_close_stream(uintptr_t stream){uint32_t a[]={(uint32_t)stream};event(0x56f2d0,1,a);if(p[16]&8)RESTORE_SOUND->digital=p[20];return 1;}
int restore_start_open_stream(uintptr_t digital,uintptr_t name,uintptr_t size){uint32_t a[]={(uint32_t)digital,(uint32_t)name,(uint32_t)size};event(0x56f290,3,a);if(p[16]&16){RESTORE_SOUND->volumes[0]=(int32_t)p[21];RESTORE_SOUND->volumes[2]=(int32_t)(p[21]^0x8000);RESTORE_SOUND->volumes[3]=(int32_t)(p[21]^0xffff);}return (int)p[13];}
int restore_start_stream_loop(uintptr_t stream,uintptr_t loops){uint32_t a[]={(uint32_t)stream,(uint32_t)loops};event(0x56f300,2,a);return 1;}
int restore_start_stream_volume(uintptr_t stream,uintptr_t volume){uint32_t a[]={(uint32_t)stream,(uint32_t)volume};event(0x56f29c,2,a);return 1;}
int restore_start_stream_start(uintptr_t stream){uint32_t a[]={(uint32_t)stream};event(0x56f304,1,a);return 1;}
uint8_t restore_start_enumerate(RestoreFileList *self,const char *pattern,char flags)
{
    int32_t count=(int32_t)p[14];uint32_t i,a[]={(uint32_t)(uintptr_t)self,hash(pattern),(uint8_t)flags,(uint32_t)((uintptr_t)pattern==0x576260u?0x576260u:0)};
    CHECK(self==RESTORE_SAVE_FILES&&count<=8);event(0x41c740,4,a);
    self->offsets=(uint32_t *)0x6f3000u;self->count=count;self->capacity=8;self->strings=(char *)0x6f3100u;self->string_capacity=512;self->used=count>0?count*64:0;
    for(i=0;count>0&&i<(uint32_t)count;++i){self->offsets[i]=i*64;strcpy(self->strings+i*64,names[i]);}
    if(p[16]&32){DM_B(0xe38dda)=(uint8_t)p[17];DM_U(0x6059f4)=0xabcdef01;}
    return (uint8_t)p[28];
}
void restore_start_sort(RestoreFileList *self,RestoreFileCompare compare){uint32_t a[]={(uint32_t)(uintptr_t)self,0x562370};CHECK(self==RESTORE_SAVE_FILES&&compare==FUN_00562370);event(0x41ca10,2,a);}
undefined4 *restore_start_profile(void){event(0x5169c0,0,NULL);return (undefined4 *)0x6f3340u;}
static void prepare(void)
{
    uint32_t i,j,token=p[0]?0x409580:0x4023a0;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)DM_B(regions[i][0]+j)=(uint8_t)(p[2]+j*37u+i*11u);
    DM_U(0x605a3c)=p[3];DM_B(0x7027b4)=(uint8_t)p[4];DM_B(0xe38dda)=(uint8_t)p[5];DM_B(0xe38dca)=(uint8_t)p[6];DM_B(0xe38e64)=(uint8_t)p[7];DM_U(0xf55b88)=p[8];
    RESTORE_SOUND->disabled=p[9];RESTORE_SOUND->digital=p[11];
    for(i=0;i<3;++i)RESTORE_SOUND->streams[i]=p[10];
    RESTORE_SOUND->volumes[0]=(int32_t)p[12];RESTORE_SOUND->volumes[1]=(int32_t)(p[12]^1);RESTORE_SOUND->volumes[2]=(int32_t)(p[12]^0x8000);RESTORE_SOUND->volumes[3]=(int32_t)(p[12]^0xffff);
    DM_U(0x705728)=p[22];DM_U(0x705724)=0x6f2000;DM_U(0x70572c)=1;DM_U(0x705730)=1;
    DM_U(0x6f2008)=0x6f2020;DM_W(0x6f200c)=1;DM_U(0x6f2024)=token;DM_U(0x6f202c)=p[23];DM_B(0xe38e03)=1;
    CHECK(p[15]<4);strcpy((char *)0x6f3340u,profiles[p[15]]);used=result=0;
}
static void run(void)
{
    uint32_t token=p[0]?0x409580:0x4023a0;prepare();
    if(p[0]==2){FUN_004d3c40();return;}
    if(p[0]==3){restore_start_selector_body(0x576260u,(char)p[27]);return;}
    if(p[0]==4){result=restore_start_stream_body(RESTORE_SOUND,(const char *)0x5761acu,(int)p[25],p[26]);return;}
    CHECK(p[0]<2);
    switch(p[1]){case 0:if(p[0])FUN_00409580();else FUN_004023a0();break;case 1:CHECK(restore_dropdown_start_action(token)==1);break;case 2:restore_menu_callback(token);break;case 3:restore_dropdown_callback(token);break;case 4:result=(uint32_t)FUN_00425f20((RestoreDropdownBar *)0x6f2000u,1);break;default:CHECK(0);}
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=DM_B(regions[i][0]+j);putchar(hex[b>>4]);putchar(hex[b&15]);}
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<16;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    CHECK(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){uint32_t i;puts("{\"fields\":32}");fflush(stdout);for(;;){for(i=0;i<32;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
    else{
        p[1]=4;p[3]=1;p[22]=1;run();CHECK(DM_B(0xe38dda)==0&&DM_B(0xe38dca)==1&&DM_U(0x78d7b0)==0&&DM_U(0x6fdcc0)==UINT32_MAX);
        p[0]=1;p[14]=4;run();CHECK(DM_B(0xe38dda)==16&&DM_B(0xd3b1c0)==1&&DM_U(0x6059f8)==4&&!strcmp((char *)0x6f3140u,"Beta")&&!strcmp((char *)0x6f3180u,""));
        CHECK(restore_dropdown_start_action(0xdeadbeef)==0);puts("New game/map selector: real reset, stream, file-name trim and actual click routing passed.");
    }
done:restore_data_close();return 0;
}
