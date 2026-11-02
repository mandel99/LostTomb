#include "compression.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define INPUT ((uint8_t *)0x30000000u)
#define OUTPUT ((uint8_t *)0x30140000u)
#define STAGE ((uint8_t *)0x30280000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static uint32_t input_size,position,read_limit,errors[8],error_count,requests[8],request_count;
static char hex[2400001],encoded[2400065];
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Compression failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
void FUN_004ce6e0(const char *message,int first,int second)
{REQUIRE(error_count<8&&!first&&!second);errors[error_count++]=(uint32_t)message;}
int FUN_005660ed(uint32_t file,void *out,uint32_t count)
{
    uint32_t copied=input_size-position;REQUIRE(file==37&&request_count<8);requests[request_count++]=count;
    if(copied>count)copied=count;if(copied>read_limit)copied=read_limit;
    if(copied)memcpy(out,INPUT+position,copied);position+=copied;return copied;
}
static uint32_t run(uint32_t op,uint32_t capacity,uint32_t limit,uint32_t *crc)
{
    REQUIRE(op<2&&capacity<=1200000);position=error_count=request_count=0;read_limit=limit;
    memset(OUTPUT,0xa5,capacity+32);memset(STAGE,0x33,1200000);
    U(0xe92e1c)=(uint32_t)STAGE;U(0x1265188)=0x12345678;U(0x126518c)=0x33333333;U(0x1265190)=0x55555555;U(0x1265194)=0x77777777;
    *crc=0xa5a5a5a5;
    return op?FUN_0055bcf0(37,OUTPUT,capacity):FUN_0055c000(crc,INPUT,input_size,OUTPUT,capacity);
}
static unsigned digit(char c){return c<='9'?(unsigned)(c-'0'):(unsigned)(c-'a'+10);}
static void output(uint32_t result,uint32_t crc,uint32_t capacity)
{
    uint32_t i;static const char digits[]="0123456789abcdef";
    for(i=0;i<capacity+32;++i){encoded[i*2]=digits[OUTPUT[i]>>4];encoded[i*2+1]=digits[OUTPUT[i]&15];}encoded[(capacity+32)*2]=0;
    printf("{\"result\":%u,\"crc\":%u,\"position\":%u,\"errors\":[",result,crc,position);
    for(i=0;i<error_count;++i)printf("%s%u",i?",":"",errors[i]);printf("],\"requests\":[");
    for(i=0;i<request_count;++i)printf("%s%u",i?",":"",requests[i]);
    printf("],\"globals\":[%u,%u,%u,%u],\"output\":\"%s\"}\n",U(0x1265188),U(0x126518c),U(0x1265190),U(0x1265194),encoded);fflush(stdout);
}
/* Two exact original55BF10 encoder results, not just a self round-trip.
   The second crosses the4KB input window boundary. */
static void encode_case(uint32_t size,const uint8_t *expected,uint32_t encoded_size,uint32_t expected_crc)
{
    uint32_t checksum=0,encoded=0,decoded_crc=0;
    error_count=0;U(0x126519c)=0;
    memset(OUTPUT,0xa5,size+32);
    REQUIRE(FUN_0055bf10(&checksum,&encoded,INPUT,OUTPUT,size)==1);
    REQUIRE(!error_count&&checksum==expected_crc&&encoded==encoded_size);
    if(memcmp(OUTPUT,expected,encoded_size)||OUTPUT[encoded_size]!=0xa5){
        uint32_t index;
        fprintf(stderr,"Encoder %u bytes differed from original:",size);
        for(index=0;index<encoded_size;++index)fprintf(stderr,"%02x",OUTPUT[index]);
        fputc('\n',stderr);REQUIRE(0);
    }
    memcpy(STAGE,OUTPUT,encoded_size);memset(OUTPUT,0xa5,size+32);
    {
        uint32_t result=FUN_0055c000(&decoded_crc,STAGE,encoded_size,OUTPUT,size+1u);
        if(result!=1){
            uint32_t index;
            fprintf(stderr,"Decoder %u bytes failed, errors:",size);
            for(index=0;index<error_count;++index)fprintf(stderr," %08x",errors[index]);
            fputs(" output:",stderr);
            for(index=0;index<size&&index<32;++index)fprintf(stderr,"%02x",OUTPUT[index]);
            fputc('\n',stderr);
        }
        REQUIRE(result==1);
    }
    REQUIRE(!error_count&&decoded_crc==expected_crc&&!memcmp(OUTPUT,INPUT,size)&&OUTPUT[size]==0xa5);
    if(size==8192u){
        /* Original55BE50 rejects even the final zero-length flush once
           position equals capacity. Preserve this observed native case;
           one spare byte above permits the same8192-byte decoded output. */
        error_count=0;decoded_crc=0xdeadbeefu;memset(OUTPUT,0xa5,size+32);
        REQUIRE(FUN_0055c000(&decoded_crc,STAGE,encoded_size,OUTPUT,size)==0);
        REQUIRE(error_count==2&&errors[0]==0x603654u&&errors[1]==0x6036b4u);
        REQUIRE(decoded_crc==0xdeadbeefu&&!memcmp(OUTPUT,INPUT,size)&&OUTPUT[size]==0xa5);
    }
}
int main(int argc,char **argv)
{
    uint32_t op,capacity,limit,crc,result,i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc(INPUT,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==INPUT);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":4}");fflush(stdout);
        while(scanf("%u %u %u %2400000s",&op,&capacity,&limit,hex)==4) {
            input_size=!strcmp(hex,"-")?0:(uint32_t)strlen(hex)/2;REQUIRE(input_size<=1200000);
            for(i=0;i<input_size;++i)INPUT[i]=(uint8_t)((digit(hex[i*2])<<4)|digit(hex[i*2+1]));
            result=run(op,capacity,limit,&crc);output(result,crc,capacity);
        }
    } else {
        static const uint8_t sample[]={0,4,0x82,0x24,0x25,0x8f,0x80,0x7f};memcpy(INPUT,sample,sizeof(sample));input_size=sizeof(sample);
        REQUIRE(run(0,13,0xffffffffu,&crc)==1);REQUIRE(!memcmp(OUTPUT,"AIAIAIAIAIAIA",13));
        memset(INPUT,0,4);input_size=4;REQUIRE(run(1,300,0xffffffffu,&crc)==1&&U(0x126518c)==0&&OUTPUT[299]==0&&OUTPUT[300]==0xa5);
        {
            static const uint8_t short_encoded[]={0x00,0x06,0x82,0x24,0x25,0x0f,0x02,0xfe,0x01};
            static const uint8_t window_encoded[]={
                0x00,0x06,0x00,0x04,0x10,0x30,0x80,0x40,0x01,0x03,0x07,0x10,0x24,0x50,0xb0,0x80,
                0x41,0x03,0x07,0x0f,0x20,0x44,0x90,0x30,0x81,0x42,0x05,0x8b,0x00,0xfe,0xad,0x00,
                0x7e,0x0f,0x0a,0xe0,0xd7,0x94,0x00,0x7e,0xfe,0x16,0xc0,0x4f,0xaf,0x02,0xf8,0xf1,
                0x66,0x00,0x3f,0x3c,0x18,0xc0,0x0f,0xc1,0x06,0xf0,0x03,0xf4,0x01,0xfc,0x00,0x7d,
                0x00,0x3f,0x40,0x1f,0xc0,0x0f,0xd0,0x07,0xf0,0x03,0xf4,0x01,0xfc,0x00,0x7d,0x00,
                0x3f,0x40,0x1f,0x50,0xba,0x15,0xf0,0x0f};
            memcpy(INPUT,"AIAIAIAIAIAIA",13);
            encode_case(13,short_encoded,sizeof(short_encoded),0x506b8690u);
            for(i=0;i<8192u;++i)INPUT[i]=(uint8_t)(i%23u);
            encode_case(8192,window_encoded,sizeof(window_encoded),0x680196fau);
        }
        puts("Compression: recovered callbacks, exact output bounds, CRC32, DCL decoder and legacy block envelope.");
    }
    VirtualFree(INPUT,0,MEM_RELEASE);restore_data_close();return 0;
}
