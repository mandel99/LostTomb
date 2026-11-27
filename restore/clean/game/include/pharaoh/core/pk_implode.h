#ifndef PHARAOH_CORE_PK_IMPLODE_H
#define PHARAOH_CORE_PK_IMPLODE_H
#include "pharaoh/core/compression_tables.h"

void pharaoh_pk_implode_output_bits(PharaohPkImplodeState *state,
    uint32_t bit_count,uint32_t value);
void pharaoh_pk_implode_encode(PharaohPkImplodeState *state);

/* Whole0055C130. Borrow genuine C callbacks and a Native-sized, initially
 * zeroed workspace. Mode/dictionary remain live inputs in Native order.
 * Results:0 encoded,1 invalid dictionary,2 invalid literal mode. */
uint32_t pharaoh_pk_implode(PharaohPkReadFunction read,
    PharaohPkWriteFunction write,PharaohPkImplodeState *state,
    void *user_data,const uint32_t *mode,const uint32_t *dictionary);
#endif
