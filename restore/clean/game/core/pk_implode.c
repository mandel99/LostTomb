#include "pharaoh/core/pk_implode.h"
#include "pharaoh/core/pk_explode.h"
#include "pharaoh/core/pk_explode_data.h"
#include "pharaoh/core/memory_strings.h"

/* Whole0055C990. Native producers emit at most16 bits per request. */
void pharaoh_pk_implode_output_bits(PharaohPkImplodeState *state,
    uint32_t bit_count,uint32_t value)
{
    if(bit_count>8u) {
        bit_count-=8u;
        pharaoh_pk_implode_output_bits(state,8,value);
        value>>=8;
    }
    const uint8_t shift=(uint8_t)state->output_bits;
    const uint32_t total=state->output_bits+bit_count;
    uint8_t *out=&state->output[state->output_bytes];
    *out|=(uint8_t)(value<<(shift&31u));
    state->output_bits=total;
    if(total<9u) {
        state->output_bits&=7u;
        if(state->output_bits==0)++state->output_bytes;
    } else {
        const uint32_t position=state->output_bytes;
        state->output_bytes=position+1u;
        state->output[position+1u]=(uint8_t)(value>>((8u-shift)&31u));
        state->output_bits&=7u;
    }
    if(state->output_bytes>0x7ffu)pharaoh_compression_flush_full_output(state);
}

static void emit_literal(PharaohPkImplodeState *state,uint8_t value)
{
    pharaoh_pk_implode_output_bits(state,state->character_bits[value],
        state->character_codes[value]);
}
static void finish_stream(PharaohPkImplodeState *state)
{
    pharaoh_pk_implode_output_bits(state,state->character_bits[773],
        state->character_codes[773]);
    if(state->output_bits!=0)++state->output_bytes;
    state->write(state->output,&state->output_bytes,state->user_data);
}

/* Whole0055C2D0. Preserve read batching, lookahead and lazy-match policy. */
void pharaoh_pk_implode_encode(PharaohPkImplodeState *state)
{
    uint8_t *work=(uint8_t *)(void *)state+offsetof(PharaohPkImplodeState,work);
    uint32_t phase=0;
    uint8_t eof=0;
    state->output[0]=(uint8_t)state->compression_type;
    uint8_t *current=work+state->dictionary_bytes+516u;
    state->output[1]=(uint8_t)state->dictionary_bits;
    state->output_bytes=2;
    pharaoh_compression_fill_bytes((uint32_t *)(void *)(state->output+2),0,0x800);
    state->output_bits=0;
    for(;;) {
        uint32_t remaining=0x1000,total=0;
        do {
            uint32_t requested=remaining;
            const uint32_t got=state->read(work+state->dictionary_bytes+
                516u+total,&requested,state->user_data);
            if(got==0) {
                if(total==0 && phase==0){finish_stream(state);return;}
                eof=1;break;
            }
            total+=got;remaining-=got;
        } while(remaining!=0);
        const uint32_t dictionary=state->dictionary_bytes;
        uint8_t *end=work+dictionary+total;
        if(eof)end+=516;
        if(phase==0) {
            pharaoh_compression_build_pair_index(state,current,end+1);
            ++phase;
            if(state->dictionary_bytes!=0x1000u)++phase;
        } else if(phase==1) {
            pharaoh_compression_build_pair_index(state,current+516-dictionary,end+1);
            ++phase;
        } else {
            pharaoh_compression_build_pair_index(state,current-dictionary,end+1);
        }
        while(current<end) {
            uint32_t length=(uint32_t)pharaoh_compression_find_longest_match(state,current);
choose_token:
            if(length==0 || (length==2 && state->distance>0xffu))goto literal;
            if(eof && end<current+length) {
                length=(uint32_t)(end-current);
                if(length<2 || (length==2 && state->distance>=0x100u))goto literal;
                goto match;
            }
            if(length>7 || end<=current+1)goto match;
            {
                const uint32_t distance=state->distance;
                const uint32_t next=(uint32_t)pharaoh_compression_find_longest_match(state,current+1);
                if(next<=length || (next<=length+1u && distance<0x81u)) {
                    state->distance=distance;goto match;
                }
                emit_literal(state,*current++);
                length=next;goto choose_token;
            }
match:
            pharaoh_pk_implode_output_bits(state,state->character_bits[length+254u],
                state->character_codes[length+254u]);
            if(length==2) {
                const uint32_t group=state->distance>>2;
                pharaoh_pk_implode_output_bits(state,state->distance_bits[group],state->distance_codes[group]);
                current+=2;
                pharaoh_pk_implode_output_bits(state,2,state->distance&3u);
            } else {
                const uint32_t group=state->distance>>((uint8_t)state->dictionary_bits&31u);
                pharaoh_pk_implode_output_bits(state,state->distance_bits[group],state->distance_codes[group]);
                current+=length;
                pharaoh_pk_implode_output_bits(state,state->dictionary_bits,state->dictionary_mask&state->distance);
            }
            continue;
literal:
            emit_literal(state,*current++);
        }
        if(eof){finish_stream(state);return;}
        current-=0x1000;
        pharaoh_memory_copy_dwords_then_bytes(work,work+0x1000,
            state->dictionary_bytes+516u);
    }
}

/* Whole0055C130; seven pinned encoder tables equal existing decoder data. */
uint32_t pharaoh_pk_implode(PharaohPkReadFunction read,
    PharaohPkWriteFunction write,PharaohPkImplodeState *state,
    void *user_data,const uint32_t *mode,const uint32_t *dictionary)
{
    const PharaohPkExplodeTables *tables=&pharaoh_pk_explode_tables;
    state->read=read;state->write=write;
    state->dictionary_bytes=*dictionary;
    state->compression_type=*mode;
    state->user_data=user_data;
    state->dictionary_bits=4;state->dictionary_mask=15;
    switch(*dictionary) {
    case 0x1000:++state->dictionary_bits;state->dictionary_mask|=0x20u;
        /* fall through */
    case 0x800:++state->dictionary_bits;state->dictionary_mask|=0x10u;
        /* fall through */
    case 0x400:break;
    default:return 1;
    }
    uint32_t index=0;
    if(*mode==0) {
        for(;index<256u;++index) {
            state->character_bits[index]=9;
            state->character_codes[index]=(uint16_t)(index*2u);
        }
    } else if(*mode==1) {
        for(;index<256u;++index) {
            state->character_bits[index]=(uint8_t)(tables->length_bit_count[index]+1u);
            state->character_codes[index]=(uint16_t)(tables->length_code[index]*2u);
        }
    } else return 2;
    for(uint32_t group=0;group<16u;++group) {
        const uint32_t count=1u<<(tables->literal_extra_bit_count[group]&31u);
        for(uint32_t item=0;item<count;++item) {
            const uint32_t shift=(tables->literal_bit_count[group]+1u)&31u;
            uint16_t code=(uint16_t)((uint32_t)(uint16_t)item<<shift);
            state->character_bits[index]=(uint8_t)(tables->literal_extra_bit_count[group]+
                tables->literal_bit_count[group]+1u);
            code=(uint16_t)(code|((uint16_t)tables->literal_code_start[group]*2u)|1u);
            state->character_codes[index++]=code;
        }
    }
    pharaoh_memory_copy_dwords_then_bytes(state->distance_codes,tables->distance_code_start,64);
    pharaoh_memory_copy_dwords_then_bytes(state->distance_bits,tables->distance_extra_bit_count,64);
    pharaoh_pk_implode_encode(state);
    return 0;
}
