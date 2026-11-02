#include "pyramid_parts.h"
#include "editor_brush_validation.h"

/* Whole4EE4A0: cache root kind and rotation, reload the active count family
   between parts, and pass all three image-selection arguments. */
void restore_pyramid_refresh_all_parts(int32_t root)
{
    uint32_t r=restore_pyramid_record(root),kind=PY_B(r+PY_LAYOUT_KIND),current=(uint32_t)root,limit;
    int32_t rotation=PY_I(PY_U(0xbca2e0u))/2,relative;
    for(;;){
        limit=PY_U((PY_U(0x5e4ef4u)==5u?0x56f7ecu:0x56f7d8u)+kind*4u)+(uint32_t)root;
        if(PY_I(current)>=PY_I(limit))break;
        restore_pyramid_update_piece_image(PY_I(current),rotation,PY_I(kind));++current;
    }
    if(FUN_004ecea0(PY_I(kind))){
        relative=PY_I(PY_U(0x5f5d08u+kind*4u)+PY_U(r+PY_CELL)-PY_U(0xbca2d8u));
        FUN_004ed360(relative%228,relative/228,4,0);
    }
}
void FUN_004ee4a0(int32_t root){restore_pyramid_refresh_all_parts(root);}
