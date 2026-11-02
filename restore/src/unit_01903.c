#include "mausoleum.h"
#include "city_footprint_validation.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4DFD90: reposition one part and its builder/carrier. Preserve the
   actor type/state BYTE tests, signed WORD related actor and post-callback
   reloads; route reset and movement restart have distinct native branches. */
void FUN_004dfd90(int32_t building)
{
    int32_t geometry[13],x,y;
    uint32_t offset=(uint32_t)building*264u,worker,actor,type,state,tile,related;
    restore_city_geometry_004ded50(geometry,(int32_t)(int16_t)W(0x7999eau+offset),
        I(U(0xbca2e0u)),I(U(0x799a48u+offset)));
    x=I(U(0x799a40u+offset)+(uint32_t)geometry[0]);y=I(U(0x799a44u+offset)+(uint32_t)geometry[1]);
    W(0x7999deu+offset)=(uint16_t)x;W(0x7999e0u+offset)=(uint16_t)y;
    U(0x7999e4u+offset)=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    (void)FUN_004e0c50(building,&x,&y);
    W(0x7999feu+offset)=(uint16_t)x;W(0x799a00u+offset)=(uint16_t)y;
    worker=U(0x799a34u+offset);
    if(!worker)goto refresh;
    actor=worker*388u;type=B(0xbcd602u+actor);state=B(0xbcd630u+actor);
    if((type==79u&&state==9u)||(type==81u&&state==9u)||(type==85u&&state==27u)){
        W(0xbcd61cu+actor)=W(0x7999feu+offset);W(0xbcd61eu+actor)=(uint16_t)y;
        U(0xbcd620u+actor)=U(0x7999e4u+offset);
        goto retarget;
    }
    if((type==79u&&state==10u)||(type==85u&&state==28u)||(type==81u&&(state==10u||state==11u))){
        FUN_004b5420(I(worker));
        W(0xbcd60cu+actor)=W(0x7999deu+offset);W(0xbcd60eu+actor)=W(0x7999e0u+offset);
        tile=U(0x7999e4u+offset);U(0xbcd618u+actor)=tile;
        if(type==81u)U(0xbcd620u+actor)=tile;
        FUN_004b52f0(I(worker));
        if(type!=81u||B(0xbcd630u+actor)!=10u)goto refresh;
        related=(uint32_t)(int32_t)(int16_t)W(0xbcd660u+U(0xd056f8u)*388u);
        if(!related)goto refresh;
        actor=related*388u;
        if(B(0xbcd602u+actor)!=85u||B(0xbcd630u+actor)!=26u)goto refresh;
        worker=related;
        W(0xbcd61cu+actor)=W(0x7999feu+offset);W(0xbcd61eu+actor)=W(0x799a00u+offset);
        U(0xbcd620u+actor)=U(0x7999e4u+offset);
retarget:
        FUN_004b3250(I(worker));W(0xbcd63cu+actor)=0;
    }
refresh:
    FUN_004dfbb0(building);
}
