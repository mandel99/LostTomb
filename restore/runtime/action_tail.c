#include "pending_message.h"
#include "menu_input.h"
#include <stddef.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern char *FUN_00562240(char *destination,char *source,size_t bytes);
static __declspec(thread) const uint8_t *command_native_payload;

const uint8_t *restore_command_take_native_payload(void)
{
    const uint8_t *payload=command_native_payload;
    command_native_payload=NULL;
    return payload;
}

static int32_t decode_command(const char *encoded,uint8_t decoded[100])
{
    uint8_t character,a,b,filter;uint32_t i,j,record,candidate;
    (void)FUN_00562240((char *)decoded,(char *)encoded,100);character=decoded[0];decoded[99]=0;
    for(i=0;character;++i){decoded[i]=character^3u;character=decoded[i+1u];}
    if(!B(0x7027e0u))return -1;
    /* Native's test of its ESP-based local buffer against NULL is unreachable
       for a valid stack. The100byte array and terminating byte are exact. */
    filter=B(0x578c68u);
    for(i=0,record=0x57c3c8u;i<34;++i,record+=12u){
        if(filter&&!B(record+8u))continue;
        candidate=U(record);j=0;
        for(;;){
            a=decoded[j];b=B(candidate+j);if(a!=b)break;if(!a)return(int32_t)i;
            a=decoded[j+1u];b=B(candidate+j+1u);if(a!=b)break;if(!a)return(int32_t)i;
            j+=2u;
        }
    }
    return 34;
}
int32_t FUN_0041d050(const char *encoded)
{
    uint8_t decoded[100];
    return decode_command(encoded,decoded);
}
static uint8_t invoke_command(int32_t index,const uint8_t *payload)
{
    uint32_t counter;const uint8_t *saved;
    if(index<0||index>=34)return 0;
    counter=0xea5d10u+U(0xeaac28u)*0x49d8u;U(counter)=U(counter)+1u;
    saved=command_native_payload;command_native_payload=payload;
    restore_menu_callback(U(0x57c3ccu+(uint32_t)index*12u));
    command_native_payload=saved;
    return 1;
}
uint8_t FUN_0041d010(int32_t index){return invoke_command(index,NULL);}
void FUN_0041d3f0(void)
{
    int32_t index;uint8_t decoded[100];
    B(0xe38ddau)=B(0x7027e8u);index=decode_command((const char *)(uintptr_t)0x7027ecu,decoded);
    /* With S=41D3F0 entry ESP, native decoder storage is [S-108,S-9].
       The encoded arg remains pushed across CALL41D010. Its nullary CALL
       and the three-argument command wrapper put God entry at S-32;
       the thirty-byte God local block is therefore decoded[44..73].
       Ret/arg writes are above this range, saved registers below it.
       Pass the actual decoded bytes, including suffixes after decoded NUL. */
    (void)invoke_command(index,decoded+44);
}
