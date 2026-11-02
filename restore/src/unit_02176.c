#include "pyramid_parts.h"
#include "building_allocator.h"
#include "building_create_model.h"
#include "editor_brush_validation.h"

/* Whole4F5F10: create the kind215 entrance associated with a kind183
   pyramid root. Model size is cached before allocation; owner is reloaded
   after the allocation/update callbacks, exactly as in the native routine. */
void restore_pyramid_create_entrance(int32_t building)
{
    uint32_t r=restore_pyramid_record(building),root,kind,tile,model_size,owner;
    int32_t x,y,created;
    if(PY_W(r+PY_BUILDING_KIND)!=183u)return;
    root=(uint32_t)building-(uint32_t)(int32_t)(int16_t)PY_W(r+PY_PIECE_INDEX);r=restore_pyramid_record(PY_I(root));
    kind=PY_B(r+PY_LAYOUT_KIND);
    tile=PY_U(r+PY_CELL)+PY_U((PY_U(0x5e4ef4u)==5u?0x5700e8u:0x5700d4u)+kind*4u);
    x=PY_I((uint32_t)(PY_I(tile)%228)-PY_U(0xbca2e8u));y=PY_I((uint32_t)(PY_I(tile)/228)-PY_U(0xbca2ecu));
    model_size=PY_U(0x5d17a8u);owner=PY_U(0xeaac28u);
    created=restore_building_allocate_0046a400(owner,215,x,y,0,0);(void)FUN_00485100(created);
    restore_placement_create_00470990(PY_U(0xeaac28u),215,created,x,y,PY_I(model_size),0,0);
    PY_W(restore_pyramid_record(created)+PY_PIECE_INDEX)=(uint16_t)root;
}
void FUN_004f5f10(int32_t building){restore_pyramid_create_entrance(building);}
