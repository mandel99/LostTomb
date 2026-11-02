#include "dropdown_actions.h"
#include "dropdown_menu.h"
#include "menu_input.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Dropdown action failure line %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0x605780,128},{0x6de500,32},{0x784b50,0x2a0},{0xe38dc0,0x160},{0xf53f00,0x90},{0x705700,64},{0xd3b180,48},{0x6f2000,64}};
static const uint32_t tokens[]={0x402bc0,0x402be0,0x402e30,0x402ee0,0x4032c0,0x406070,0x4060e0,0x406100,0x406140,0x406170};
static void (*const functions[])(void)={FUN_00402bc0,FUN_00402be0,FUN_00402e30,FUN_00402ee0,FUN_004032c0,FUN_00406070,FUN_004060e0,FUN_00406100,FUN_00406140,FUN_00406170};
static uint32_t p[24],writes[128][3],used,result;
void restore_dropdown_action_write(uint32_t address,uint32_t width,uint32_t value)
{
    CHECK(used<128);writes[used][0]=address;writes[used][1]=width;
    writes[used++][2]=width==1?value&255u:width==2?value&65535u:value;
    if(width==1)DM_B(address)=(uint8_t)value;
    else if(width==2)DM_W(address)=(uint16_t)value;
    else{CHECK(width==4);DM_U(address)=value;}
}
static void prepare(void)
{
    uint32_t i,j;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)
        for(j=0;j<regions[i][1];++j)DM_B(regions[i][0]+j)=(uint8_t)(p[2]+j*37u+i*11u);
    DM_B(0xe38dda)=(uint8_t)p[3];DM_B(0xe38dca)=(uint8_t)p[4];
    DM_U(0xe38e64)=p[5];DM_B(0xe38e63)=(uint8_t)p[6];
    DM_U(0xe38ec8)=p[7];DM_U(0xe38ecc)=p[8];DM_U(0xe38ec0)=p[9];
    DM_U(0xe38ec4)=p[10];DM_U(0xe38ed0)=p[11];DM_U(0xe38ef0)=p[12];
    DM_U(0xe38e6c)=p[13];DM_U(0xe38e70)=p[14];
    DM_U(0xf53f18)=p[15];DM_U(0xf53f1c)=p[16];DM_U(0xf53f20)=p[17];DM_U(0xf53f4c)=p[18];
    DM_U(0x705728)=p[19];DM_U(0x705724)=0x6f2000;DM_U(0x70572c)=1;DM_U(0x705730)=1;
    DM_U(0x6f2008)=0x6f2020;DM_W(0x6f200c)=1;
    DM_U(0x6f2024)=tokens[p[0]];DM_U(0x6f202c)=p[20];DM_B(0xe38e03)=1;
    used=result=0;
}
static void run(void)
{
    CHECK(p[0]<10);prepare();
    switch(p[1]){
    case 0:functions[p[0]]();break;
    case 1:CHECK(restore_dropdown_action(tokens[p[0]])==1);break;
    case 2:restore_menu_callback(tokens[p[0]]);break;
    case 3:restore_dropdown_callback(tokens[p[0]]);break;
    case 4:result=(uint32_t)FUN_00425f20((RestoreDropdownBar *)0x6f2000,1);break;
    default:CHECK(0);
    }
}
static void output(void)
{
    static const char hex[]="0123456789abcdef";uint32_t i,j;
    printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=DM_B(regions[i][0]+j);putchar(hex[b>>4]);putchar(hex[b&15]);}
    printf("\",\"writes\":[");
    for(i=0;i<used;++i)printf("%s[%u,%u,%u]",i?",":"",writes[i][0],writes[i][1],writes[i][2]);
    puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    CHECK(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        uint32_t i;puts("{\"fields\":24}");fflush(stdout);
        for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}
    }else{
        p[1]=4;p[2]=73;p[3]=3;p[19]=1;run();
        CHECK(DM_B(0xe38dda)==10&&DM_U(0xf53f18)==0&&DM_B(0xe38dca)==1);
        CHECK(result==1&&DM_U(0x705724)==0&&DM_U(0x70572c)==0);
        p[0]=4;p[13]=0xabcdef12;p[14]=0x87654321;run();
        CHECK(DM_U(0x6057a0)==p[13]&&DM_U(0x60579c)==p[14]&&DM_B(0xe38dda)==12);
        p[0]=9;run();CHECK(DM_U(0x784bc0)==UINT32_MAX&&DM_U(0x784bfc)==UINT32_MAX);
        CHECK(restore_dropdown_action(0xdeadbeef)==0);
        puts("Dropdown actions: real click/dispatcher/settings snapshot/reset range checks passed.");
    }
done:restore_data_close();return 0;
}
