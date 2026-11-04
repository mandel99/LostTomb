#include "figure_menu_actions.h"
#include "actor_predicate.h"
#include "sound_initialization.h"
#include "sound_streams.h"
#include "menu_full_input_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y);

/* Copy the actual terminating NUL in DWORD groups, then remaining BYTEs.
   The names in the original24-byte voice records fit the32-byte scratch;
   no new truncation, format conversion or fallback filename is introduced. */
static uint32_t terminated_bytes(uint32_t source)
{uint32_t size=0;do{++size;}while(B(source+size-1u));return size;}
static void copy_native_words(uint32_t target,uint32_t source,uint32_t size)
{
    uint32_t index;
    for(index=0;index<(size&~3u);index+=4u)U(target+index)=U(source+index);
    for(;index<size;++index)B(target+index)=B(source+index);
}
/* Whole539A70/61 instructions. Sound-enabled and voice volume are DWORDs;
   the voice gate is a BYTE. Each native volume reload is kept. All three
   original thiscall children receive the actual E92BF8 sound controller. */
uint32_t restore_figure_voice_00539a70(const char *name)
{
    uint32_t prefix=0x5f90a4u,target=0xf55a20u,size,source,first_volume,index;
    if(!U(0xf55b88u)||!B(0xe38e65u))return 0;
    size=terminated_bytes(prefix);copy_native_words(target,prefix,size);
    source=(uint32_t)(uintptr_t)name;size=terminated_bytes(source);
    target+=terminated_bytes(target)-1u;
    copy_native_words(target,source,size&~3u);
    /*539AD4 reads the first volume before539ADF copies the BYTE tail. */
    first_volume=U(0xe38eccu);
    for(index=size&~3u;index<size;++index)B(target+index)=B(source+index);
    FUN_0055f639(RESTORE_SOUND,1,(int32_t)first_volume);
    restore_sound_music_volume_0055f32f(RESTORE_SOUND,(int32_t)U(0xe38eccu));
    return FUN_0055f449(RESTORE_SOUND,(const char *)(uintptr_t)0xf55a20u,1,1);
}
/* Sole native caller53C04A is cdecl1/RET0 and discards ECX/EDX/flags;
   no tail or address-taken references. Retire old implicit-ECX unit02481. */
uint32_t FUN_00539a70(char *name)
{return restore_figure_voice_00539a70(name);}
/* Whole53BFE0/44 instructions. All32 native local bytes are zeroed before
   the exact NUL-inclusive copy; variant is signed, filenames have24B stride. */
uint32_t restore_actor_voice_variant_result(int32_t kind,int32_t variant,uint32_t incoming_eax)
{
    uint32_t words[8],source,size,index;
    if(kind<=0||variant<0)return incoming_eax;
    for(index=0;index<8u;++index)words[index]=0;
    source=kind==108?0x60110cu+(uint32_t)variant*24u:
        0x5fb16cu+((uint32_t)variant-20u+(uint32_t)kind*20u)*24u;
    size=terminated_bytes(source);
    copy_native_words((uint32_t)(uintptr_t)words,source,size);
    return FUN_00539a70((char *)words);
}
void restore_actor_voice_variant(int32_t kind,int32_t variant)
{(void)restore_actor_voice_variant_result(kind,variant,0);}

/*53D280 returns the raw figure-table code. Unlike the selection action,
   secondary-2 is rejected, while secondary0 and values below-4 survive. */
int32_t restore_actor_figure_code(int32_t actor)
{
    uint32_t record=(uint32_t)actor*388u,kind=B(0xbcd602u+record),code;
    int32_t building=(int16_t)W(0xbcd652u+record);
    code=U(0x6012ecu+kind*4u);
    if((int32_t)code>0)return (int32_t)code;
    if(code==0xfffffffcu){
        kind=B(0xbcd603u+record);code=U(0x6012ecu+kind*4u);
        if(code+4u>3u)return (int32_t)code;
        if(code!=0xfffffffdu)return -1;
    }else if(code==0xfffffffeu){
        uint32_t scenario=U(0x5e4c20u+(U(0x784cb0u)&255u)*48u);
        return scenario==68u?210:scenario==69u?209:scenario==70u?208:-1;
    }else if(code!=0xfffffffdu)return -1;
    code=FUN_004c5120(building);
    return code<=4u?(int32_t)(230u+code):-1;
}
static uint32_t figure_sound_pan(int32_t screen_x)
{
    uint8_t raw[10];int32_t width;
    uint32_t numerator_address=0x570150u;uint64_t result;
    width=(int32_t)(U(0xf53f78u)?U(0xf53f60u):U(0xf53f64u));
    /* Native53B58F..53B5B0: preserve inherited x87 PC/RC and both extended
       values across the true truncation boundary, without a float spill.
       FPSW/trap equivalence remains outside the masked-exception contract. */
    __asm {
        fild width
        mov eax,numerator_address
        fdivr dword ptr[eax]
        fild screen_x
        fmul st(0),st(1)
        lea eax,raw
        fstp tbyte ptr[eax]
    }
    result=restore_menu_input_trunc80(raw);
    __asm { fstp st(0) }
    return (uint32_t)result;
}
/* Whole53B4A0/100 instructions. Exact2-target/91-byte offscreen-type
   remap, original signed cue/channel checks, projection and request stores. */
static void figure_sound(uint32_t actor,int32_t cue,int32_t channel)
{
    uint32_t offset,width,height,sample,mode;int32_t x,y,volume;uint8_t outside;
    if(!U(0xf55b88u)||cue>=33||channel>=5||!B(0xe38e63u))return;
    mode=U(0xf53f78u);width=U(0xf53f60u);if(!mode)width=U(0xf53f64u);
    offset=actor*388u;height=U(0xe38d98u);
    y=(int16_t)W(0xbcd60eu+offset);x=(int16_t)W(0xbcd60cu+offset);
    FUN_004f0ba0(x,y,&x,&y);
    outside=(uint8_t)(x<0||y<0||x>(int32_t)width||y>(int32_t)height);
    if(outside){
        switch(B(0xbcd602u+offset)){
        case 11:case 12:case 13:case 43:case 44:case 45:case 54:case 55:
        case 56:case 77:case 78:case 92:case 93:case 99:case 100:case 101:break;
        default:return;
        }
    }
    sample=U(0xf54120u+((uint32_t)cue*5u+(uint32_t)channel)*4u);
    if((int32_t)sample<0)return;
    U(0xe92db4u)=figure_sound_pan(x);
    volume=(int32_t)U(0xe38ec4u);if(outside)volume/=4;
    U(0xe92db0u)=(uint32_t)volume;U(0xe92dacu)=sample;
    U(0xe92da8u)=UINT32_MAX;U(0xe92db8u)=1;
    (void)FUN_0055f0e8(RESTORE_SOUND,(const int32_t *)(uintptr_t)0xe92da8u);
}
/* Whole53D0B0/130 instructions. DWORD selected actor/image fields, BYTE
   figure kinds and signed WORD building IDs; both native5-way switches.
   Reload the main kind after sound before interpreting image sentinels. */
uint32_t restore_figure_selection_result_0053d0b0(void)
{
    uint32_t actor=U(0xeadd24u+U(0xeadce4u)*4u),offset,kind,image;
    int32_t cue=-1,voice;
    if((int32_t)actor<=0)return actor;offset=actor*388u;kind=B(0xbcd602u+offset);
    switch(kind){case 83:cue=0;break;case 103:cue=1;break;case 104:cue=2;break;
    case 102:cue=3;break;case 69:cue=4;break;case 70:cue=5;break;case 84:cue=6;break;
    case 82:cue=7;break;case 68:cue=8;break;default:break;}
    if(cue>=0)figure_sound(actor,cue,0);
    kind=B(0xbcd602u+offset);image=U(0x6012ecu+kind*4u);U(0xeadce0u)=image;
    if(image==0xfffffffeu){
        kind=U(0x5e4c20u+(U(0x784cb0u)&255u)*48u);
        if(kind==68u)image=210;else if(kind==69u)image=209;else if(kind==70u)image=208;
    }else{
        if(image==0xfffffffdu){
            kind=FUN_004c5120((int16_t)W(0xbcd652u+offset));
            if(kind<=4u)image=230u+kind;else image=U(0xeadce0u);
        }
        if(image==0xfffffffcu){
            kind=B(0xbcd603u+offset);image=U(0x6012ecu+kind*4u);U(0xeadce0u)=image;
            if(image==0xfffffffdu){
                kind=FUN_004c5120((int16_t)W(0xbcd652u+offset));
                if(kind<=4u)image=230u+kind;else image=U(0xeadce0u);
            }
        }
        if(image==UINT32_MAX)return image;
    }
    image-=201u;U(0xeadce0u)=image;if((int32_t)image<0)return image;
    voice=(int8_t)B(0xbcd675u+offset);U(0xeadcdcu)=(uint32_t)voice;
    return restore_actor_voice_variant_result((int32_t)image,voice,image);
}
void restore_figure_selection_0053d0b0(void)
{(void)restore_figure_selection_result_0053d0b0();}
/* Whole406700/5 instructions. */
int restore_figure_menu_action(uint32_t token)
{
    uint32_t selected;
    if(token!=0x406700u)return 0;
    selected=U(0xd3b198u);U(0xeadce4u)=selected;
    restore_figure_selection_0053d0b0();B(0xe38dcau)=1;return 1;
}
