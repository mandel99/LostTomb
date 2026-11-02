#include "map_file_load.h"
static uint32_t terminated_size(const char *text)
{
    uint32_t n=0;do{++n;}while(MFR(text+n-1u,1));return n;
}
static void copy_words(char *destination,const char *source,uint32_t words)
{
    uint32_t i,value;for(i=0;i<words;++i){value=MFR(source+i*4u,4);memcpy(destination+i*4u,&value,4);}
}
static void trim_name(char *name){MFE(0x4ce3a0,name,0,0);FUN_004ce3a0(name);}
void FUN_004099b0(void)
{
    char path[100],*destination,*name=(char *)(uintptr_t)0xe37078u;
    uint32_t selected,prefix_size,name_size,i,mode;
    MFE(0x4099b0,0,0,0);
    selected=MFR(0xd3b198u,4);
    if(selected==0){MFW(0xe38ddau,1,1);MFW(0xe38dcau,1,1);return;}
    if(selected!=1)return;
    trim_name(name);
    MFE(0x4cbea0,0x576274,0xe38aec,3);FUN_004cbea0(0x576274,(char *)(uintptr_t)0xe38aec,3);
    MFE(0x4ce3c0,name,0,0);FUN_004ce3c0(name);
    prefix_size=terminated_size((char *)(uintptr_t)0x57626c);
    copy_words(path,(char *)(uintptr_t)0x57626c,prefix_size>>2);
    for(i=prefix_size&~3u;i<prefix_size;++i)path[i]=(char)MFR(0x57626cu+i,1);
    name_size=terminated_size(name);destination=path+terminated_size(path)-1u;
    copy_words(destination,name,name_size>>2);
    /* Original loads the mode between DWORD and remaining BYTE copies. */
    mode=MFR(0xd3b1c0u,1);
    for(i=name_size&~3u;i<name_size;++i)destination[i]=(char)MFR(name+i,1);
    if(mode==1){
        if(!FUN_004ce3f0(path)){trim_name(name);MFW(0xd3b1b3u,1,100);return;}
        if(MFR(0xd3b1c0u,1)==1){FUN_004de600();goto finish;}
    }
    (void)FUN_004d5520(path);
finish:
    trim_name(name);MFW(0xe38ddau,1,1);MFW(0xe38dcau,1,1);
}
