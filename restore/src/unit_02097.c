#include "pyramid_parts.h"
#include "building_allocator.h"
#include "building_support.h"
#include "editor_brush_validation.h"

static void initialize_piece(int32_t building,uint32_t kind,uint32_t mode,uint32_t piece)
{
    uint32_t r=restore_pyramid_record(building),entry=piece+kind*(mode==5u?36u:144u);
    PY_B(r+PY_LAYOUT_KIND)=(uint8_t)kind;PY_B(r+PY_RENDER_PHASE)=2;
    PY_B(r+PY_SPRITE_STEP)=0;PY_W(r+PY_ASSIGNED_WORKER)=0;PY_B(r+PY_LAYER_INDEX)=0;
    PY_B(r+PY_TEMPLATE_HEIGHT)=PY_B((mode==5u?0x5e657cu:0x5e4efcu)+entry*8u);
    PY_B(r+PY_WORKER_POSITION_A)=0;PY_B(r+PY_WORKER_POSITION_B)=0;PY_B(r+PY_CELL_PHASE_MASK)=15;
    PY_W(r+PY_WORKER_X_A)=0;PY_W(r+PY_WORKER_Y_A)=0;PY_W(r+PY_WORKER_X_B)=0;PY_W(r+PY_WORKER_Y_B)=0;
    PY_U(r+PY_WORKER_IMAGE)=0;PY_W(r+PY_PIECE_INDEX)=(uint16_t)piece;PY_B(r+PY_SIZE)=2;
}

/* Whole4EE780. The fourth native argument is unused. Capture root mode/kind,
   then reload them after callbacks for child records and loop bounds. */
void restore_pyramid_create_parts(uint32_t owner,int32_t x,int32_t y,uint32_t unused,int32_t building)
{
    uint32_t r=restore_pyramid_record(building),kind=PY_U(0x5e6970u),mode=PY_U(0x5e4ef4u),amount=0;
    uint32_t previous=(uint32_t)building,current,entry=0,delta,root;int32_t piece=1,count;
    (void)unused;PY_W(r+PY_PREVIOUS)=0;
    initialize_piece(building,kind,mode,0);
    if(mode==1u)amount=PY_U(0x5f5c68u+kind*4u);
    else if(mode==2u||mode==4u)amount=PY_U(0x5f5c40u+kind*4u);
    else if(mode==3u)amount=PY_U(0x5f5c54u+kind*4u);
    else if(mode==5u)amount=PY_U(0x5f5c7cu+kind*4u);
    PY_W(r+PY_CONSTRUCTION_AMOUNT)=(uint16_t)amount;
    restore_pyramid_stamp_footprint(building);(void)FUN_004f4f80(building);
    for(;;){
        mode=PY_U(0x5e4ef4u);kind=PY_U(0x5e6970u);
        count=PY_I(PY_U((mode==5u?0x56f7ecu:0x56f7d8u)+kind*4u));
        if(piece>=count)break;
        delta=(mode==5u?0x56fd18u:0x56f898u)+entry;
        current=(uint32_t)restore_building_allocate_0046a400(owner,183,
            PY_I((uint32_t)x+PY_U(delta)),PY_I((uint32_t)y+PY_U(delta+4u)),0,PY_I(previous+1u));
        (void)FUN_00485100(PY_I(current));
        r=restore_pyramid_record(PY_I(current));
        mode=PY_U(0x5e4ef4u);
        PY_W(r+PY_PREVIOUS)=(uint16_t)previous;PY_W(restore_pyramid_record(PY_I(previous))+PY_NEXT)=(uint16_t)current;
        kind=PY_U(0x5e6970u);PY_W(r+PY_NEXT)=0;
        initialize_piece(PY_I(current),kind,mode,(uint32_t)piece);
        previous=current;restore_pyramid_stamp_footprint(PY_I(current));++piece;entry+=8u;
    }
    root=previous-(uint32_t)(int32_t)(int16_t)PY_W(restore_pyramid_record(PY_I(previous))+PY_PIECE_INDEX);
    restore_pyramid_mark_special_parts(PY_I(root));restore_pyramid_create_entrance(PY_I(root));
}
void FUN_004ee780(uint32_t owner,int32_t x,int32_t y,uint32_t unused,int32_t building)
{restore_pyramid_create_parts(owner,x,y,unused,building);}
