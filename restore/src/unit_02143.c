#include "pyramid_parts.h"
#include "editor_brush_validation.h"

/* Whole4F3C90: pointer/count descriptors own eight-byte (piece,cell) pairs.
   Descriptor pointer and count are reloaded after each image callback. */
void restore_pyramid_mark_special_parts(int32_t root)
{
    uint32_t r=restore_pyramid_record(root),kind=PY_B(r+PY_LAYOUT_KIND),descriptor,i=0,part,record;
    int32_t rotation=PY_I(PY_U(0xbca2e0u))/2,relative;
    descriptor=(PY_U(0x5e4ef4u)==5u?0x5f6d58u:0x5f6cd0u)+kind*8u;
    while(PY_I(i)<PY_I(PY_U(descriptor+4u))){
        part=(uint32_t)root+PY_U(PY_U(descriptor)+i*8u);record=restore_pyramid_record(PY_I(part));
        PY_B(record+PY_RENDER_PHASE)=3;PY_B(record+PY_CELL_PHASE_MASK)=0;
        restore_pyramid_update_piece_image(PY_I(part),rotation,PY_I(kind));++i;
    }
    if(FUN_004ecea0(PY_I(kind))){
        relative=PY_I(PY_U(0x5f5d08u+kind*4u)+PY_U(r+PY_CELL)-PY_U(0xbca2d8u));
        FUN_004ed360(relative%228,relative/228,2,0);
    }
}
void FUN_004f3c90(int32_t root){restore_pyramid_mark_special_parts(root);}
