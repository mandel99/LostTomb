#include "sound_initialization.h"
#include "media_bindings.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
static RestoreSound sound;
static uint32_t handles[3]={0x100,0x104,0x108},files[4],status[3];
static RestoreSoundSample samples[3];
static RestoreSoundCache cache[3];
static int32_t request[5]={42,1,90,64,1};
static unsigned char payload[32],old_payload[32];
static int checks,starts,reads,frees,sets,inits,loops,pans,volumes,reverbs,fail_read;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sound sample failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static unsigned int channel(uintptr_t handle) {REQUIRE(handle>=0x100&&handle<=0x108&&(handle&3)==0);return (unsigned int)(handle-0x100)/4;}
int _AIL_sample_status_4(uintptr_t handle) {return (int)status[channel(handle)];}
int _AIL_mem_free_lock_4(uintptr_t data) {REQUIRE(data==(uintptr_t)old_payload);++frees;return 0;}
int _AIL_file_read_8(uintptr_t name,uintptr_t target) {REQUIRE(!strcmp((const char *)name,"second.wav")&&target==0);++reads;return fail_read?0:(int)(uintptr_t)payload;}
int _AIL_init_sample_4(uintptr_t handle) {channel(handle);++inits;return 0;}
int _AIL_set_sample_file_12(uintptr_t handle,uintptr_t data,uintptr_t block)
{channel(handle);REQUIRE(data==(uintptr_t)payload&&block==0);++sets;return 1;}
int _AIL_set_sample_loop_count_8(uintptr_t handle,uintptr_t count) {channel(handle);REQUIRE(count==1);++loops;return 0;}
int _AIL_set_sample_pan_8(uintptr_t handle,uintptr_t pan) {channel(handle);REQUIRE(pan==64);++pans;return 0;}
int _AIL_set_sample_volume_8(uintptr_t handle,uintptr_t volume) {channel(handle);REQUIRE(volume==90);++volumes;return 0;}
int _AIL_set_sample_reverb_16(uintptr_t handle,uintptr_t a,uintptr_t b,uintptr_t c)
{channel(handle);REQUIRE((sound.reserved168&&a==0x3f800000&&b==0x40000000&&c==0x40400000)||(!sound.reserved168&&!a&&!b&&!c));++reverbs;return 0;}
int _AIL_start_sample_4(uintptr_t handle) {status[channel(handle)]=4;++starts;return 0;}
static void prepare(void)
{
    int i;memset(&sound,0,sizeof(sound));memset(samples,0,sizeof(samples));memset(cache,0,sizeof(cache));
    starts=reads=frees=sets=inits=loops=pans=volumes=reverbs=fail_read=0;
    sound.files.count=2;sound.files.entries=(uint32_t)(uintptr_t)files;sound.sample_count=3;
    files[0]=(uint32_t)(uintptr_t)"first.wav";files[1]=0;files[2]=(uint32_t)(uintptr_t)"second.wav";files[3]=0;
    sound.handles=(uint32_t)(uintptr_t)handles;sound.samples=(uint32_t)(uintptr_t)samples;sound.cache=(uint32_t)(uintptr_t)cache;
    sound.factors[0]=1;sound.factors[1]=2;sound.factors[2]=3;
    for(i=0;i<3;++i){status[i]=2;cache[i].file=-1;FUN_0055e5d2(&sound,i);}
    request[0]=42;request[1]=1;request[2]=90;request[3]=64;request[4]=1;
}
int main(void)
{
    int reverb,reuse;
    for(reverb=0;reverb<2;++reverb)for(reuse=0;reuse<2;++reuse) {
        prepare();sound.reserved168=reverb;
        if(reuse){cache[0].file=0;cache[0].data=(uint32_t)(uintptr_t)old_payload;}
        REQUIRE(FUN_0055f0e8(&sound,request)==0);
        REQUIRE(starts==1&&reads==1&&sets==1&&inits==1&&frees==reuse&&loops==1&&pans==1&&volumes==1&&reverbs==1);
        REQUIRE(!memcmp(samples[0].values,request,sizeof(request))&&samples[0].values[6]==0);
        REQUIRE(cache[0].file==1&&cache[0].data==(uint32_t)(uintptr_t)payload);
        REQUIRE(FUN_0055f0e8(&sound,request)==-1&&starts==1);
        FUN_0055ecf3((uintptr_t)&sound);REQUIRE(cache[0].state==1);
    }
    prepare();cache[1].file=1;cache[1].data=(uint32_t)(uintptr_t)payload;
    REQUIRE(FUN_0055f0e8(&sound,request)==0&&reads==0&&inits==0&&sets==1&&starts==1&&samples[0].values[6]==1);
    prepare();fail_read=1;
    REQUIRE(FUN_0055f0e8(&sound,request)==-1&&reads==1&&starts==0&&sets==0);
    prepare();status[0]=status[1]=status[2]=4;samples[0].values[6]=samples[1].values[6]=samples[2].values[6]=0;
    REQUIRE(FUN_0055f0e8(&sound,request)==-1&&reads==0&&starts==0);
    prepare();sound.disabled=1;
    REQUIRE(FUN_0055f0e8(&sound,request)==-1&&reads==0);
    prepare();request[1]=-1;REQUIRE(FUN_0055f0e8(&sound,request)==-1&&reads==0);
    request[1]=2;REQUIRE(FUN_0055f0e8(&sound,request)==-1&&reads==0);
    printf("Sound samples: %d checks; playback, reverb bits, duplicate suppression, cache reuse/refcounts, eviction, failed reads and invalid file IDs.\n",checks);
    return 0;
}
