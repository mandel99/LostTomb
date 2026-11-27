#ifndef PHARAOH_RUNTIME_CRT_IEEE_RAISE_H
#define PHARAOH_RUNTIME_CRT_IEEE_RAISE_H

#include <stdint.h>

/* Native caller-owned record. Reserved bits retain the actual owned bytes;
 * neither the dispatcher nor this entry zeroes the entire scratch record. */
typedef struct PharaohCrtIeeeRecord {
    uint8_t bytes[88];
} PharaohCrtIeeeRecord;

/* Whole00567F58. Uses the real Win32 RaiseException path. A continuing
 * handler may update the record, control word and floating result. */
void pharaoh_crt_ieee_raise(PharaohCrtIeeeRecord *record, uint32_t *control,
    uint32_t exceptions, uint32_t operation, const uint64_t *operand,
    uint64_t *result);

#endif
