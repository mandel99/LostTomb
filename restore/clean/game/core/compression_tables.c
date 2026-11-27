#include "pharaoh/core/compression_tables.h"
#include "pharaoh/core/memory_fill.h"

uint32_t pharaoh_compression_crc_update(
    const uint8_t *input, const uint32_t *length, const uint32_t *state,
    const uint32_t table[256])
{
    uint32_t remaining = *length;
    uint32_t crc = *state;
    while (remaining-- != 0u) {
        crc = (crc >> 8) ^ table[(crc ^ *input++) & UINT32_C(0xff)];
    }
    return crc;
}

void pharaoh_decompression_build_decode_table(
    int32_t count, const uint8_t *lengths, const uint8_t *starts,
    uint8_t table[256])
{
    while (--count >= 0) {
        const uint32_t step = UINT32_C(1) << (lengths[count] & 31u);
        uint32_t index = starts[count];
        do {
            table[index] = (uint8_t)count;
            index += step;
        } while (index < 256u);
    }
}

/* Whole0055CA20; reuse the real DWORD-first/trailing-BYTE fill algorithm. */
void pharaoh_compression_fill_bytes(
    uint32_t *destination,uint8_t value,uint32_t byte_count)
{
    pharaoh_memory_fill_bytes(destination,byte_count,value);
}

/* Whole0055CB10. Stable reverse placement preserves equal-pair ordering. */
void pharaoh_compression_build_pair_index(
    PharaohPkImplodeState *state,const uint8_t *begin,const uint8_t *end)
{
    const uint8_t *work=(const uint8_t *)(const void *)state+
        offsetof(PharaohPkImplodeState,work);
    for(uint32_t i=0;i<2304u;i+=2u) {
        state->hash_to_index[i]=0;
        state->hash_to_index[i+1u]=0;
    }
    const uint8_t *p=begin;
    do {
        const uint32_t key=(uint32_t)p[0]*4u+(uint32_t)p[1]*5u;
        ++state->hash_to_index[key];
        ++p;
    } while(p<end);
    uint16_t total=0;
    for(uint32_t i=0;i<2304u;++i) {
        total=(uint16_t)(total+state->hash_to_index[i]);
        state->hash_to_index[i]=total;
    }
    const uint32_t first=(uint32_t)(begin-work);
    uint32_t position=(uint32_t)(end-work);
    do {
        --position;
        const uint32_t key=(uint32_t)work[position]*4u+
            (uint32_t)work[position+1u]*5u;
        const uint16_t slot=(uint16_t)(state->hash_to_index[key]-1u);
        state->hash_to_index[key]=slot;
        state->hash_offsets[slot]=(uint16_t)position;
    } while(position>first);
}

/* Whole0055C650, including the long-match prefix fallback. */
int32_t pharaoh_compression_find_longest_match(
    PharaohPkImplodeState *state,const uint8_t *current)
{
    /* Work lookahead uses the complete Native workspace representation. */
    const uint8_t *work=(const uint8_t *)(const void *)state+
        offsetof(PharaohPkImplodeState,work);
    int32_t best=1;
    const uint32_t key=(uint32_t)current[0]*4u+(uint32_t)current[1]*5u;
    uint16_t *bucket=&state->hash_to_index[key];
    uint32_t index=*bucket;
    const uint32_t lower=(uint32_t)(current-work)-state->dictionary_bytes+1u;
    if(state->hash_offsets[index]<lower) {
        do {++index;} while(state->hash_offsets[index]<lower);
        *bucket=(uint16_t)index;
    }
    const uint8_t *last_candidate=current-1;
    const uint8_t *candidate=work+state->hash_offsets[index];
    if(last_candidate<=candidate)return 0;
    for(;;) {
        if(candidate[best-1]==current[best-1] && candidate[0]==current[0]) {
            const uint8_t *scan=candidate+1;
            const uint8_t *input=current;
            int32_t length=2;
            do {
                ++scan;
                if(*scan!=input[2])break;
                ++length;++input;
            } while(length<516);
            if(best<=length) {
                state->distance=(uint32_t)(current-scan)+(uint32_t)length-1u;
                best=length;
                if(length>10) {
                    if(length==516){--state->distance;return 516;}
                    if(last_candidate<=work+state->hash_offsets[index+1u])
                        return length;
                    int16_t prefix=0,built=1;
                    state->match_next[0]=-1;state->match_next[1]=0;
                    do {
                        if(current[built]==current[prefix] ||
                           (prefix=state->match_next[prefix],prefix==-1)) {
                            ++built;++prefix;state->match_next[built]=prefix;
                        }
                    } while(built<length);
                    const uint8_t *frontier=work+state->hash_offsets[index]+length;
                    for(;;) {
                        length=state->match_next[length];
                        if(length==-1)length=0;
                        do {
                            ++index;candidate=work+state->hash_offsets[index];
                            if(last_candidate<=candidate)return best;
                        } while(candidate+length<frontier);
                        if(candidate[best-2]==current[best-2]) {
                            if(candidate+length!=frontier){length=0;frontier=candidate;}
                        } else {
                            do {
                                ++index;candidate=work+state->hash_offsets[index];
                                if(last_candidate<=candidate)return best;
                            } while(candidate[best-2]!=current[best-2] || current[0]!=candidate[0]);
                            length=2;frontier=candidate+2;
                        }
                        if(current[length]==*frontier) {
                            do {
                                ++length;
                                if(length>515)break;
                                ++frontier;
                            } while(current[length]==*frontier);
                        }
                        if(best<=length) {
                            state->distance=(uint32_t)(current-candidate)-1u;
                            if(best<length) {
                                if(length==516)return 516;
                                do {
                                    if(current[built]==current[prefix] ||
                                       (prefix=state->match_next[prefix],prefix==-1)) {
                                        ++built;++prefix;state->match_next[built]=prefix;
                                    }
                                    best=length;
                                } while(built<length);
                            }
                        }
                    }
                }
            }
        }
        ++index;candidate=work+state->hash_offsets[index];
        if(last_candidate<=candidate)return best<2?0:best;
    }
}

/* Whole0055CA90. The callback may change live output counters. */
void pharaoh_compression_flush_full_output(PharaohPkImplodeState *state)
{
    uint32_t count=0x800;
    uint8_t *output=state->output;
    state->write(output,&count,state->user_data);
    const uint8_t whole=state->output[0x800];
    const uint8_t partial=state->output[state->output_bytes];
    state->output_bytes-=0x800;
    pharaoh_compression_fill_bytes((uint32_t *)(void *)output,0,0x802);
    if(state->output_bytes!=0)output[0]=whole;
    if(state->output_bits!=0)output[state->output_bytes]=partial;
}
