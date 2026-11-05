#include "placement_preview.h"
#include "placement_predicate_entries.h"
#include <stdlib.h>
uint8_t restore_placement_empty_mask(uint32_t terrain)
{return (terrain&0xfeffd76fu)==0;}
uint8_t restore_placement_piece_mask(uint32_t self,uint32_t terrain,int32_t piece)
{
 uint32_t bits=terrain&0xfeffd76fu;
 return PV_I(self+0x58u+(uint32_t)piece*52u)==9?bits==0:bits==0x20000002u;
}
uint8_t restore_placement_selected_mask(uint32_t self,uint32_t terrain,int32_t piece)
{
 uint32_t bits=terrain&0xfeffd76fu;
 return piece==PV_I(self+0x50u)?bits==0:bits==0x20000002u;
}
uint8_t restore_placement_two_piece_mask(uint32_t terrain,int32_t piece)
{
 uint32_t bits=terrain&0xfeffd76fu;
 return piece==1?bits==0:bits==2;
}
uint8_t restore_placement_predicate(void *self,uint32_t terrain,int32_t piece)
{
 uint32_t object=(uint32_t)(uintptr_t)self,target=PV_U(PV_U(object)+16u);
 switch(target){
 case 0x4e5310:return restore_placement_empty_mask(terrain);
 case 0x401440:return restore_placement_piece_mask(object,terrain,piece);
 case 0x547de0:return restore_placement_selected_mask(object,terrain,piece);
 case 0x4e6620:return restore_placement_two_piece_mask(terrain,piece);
 default:fprintf(stderr,"Unreconstructed placement virtual predicate %08lx.\n",(unsigned long)target);abort();
 }
}

/* Only verified native constructor vtable and selected-piece stores are retired here.
   Seed fields are supplied separately; startup geometry fills derived fields. */
void restore_placement_preview_initialize_classes(void)
{
    PV_U(0x6fd5d8)=0x56f48c;PV_U(0x6de650)=0x56f438;PV_U(0x6fd368)=0x56f4dc;PV_U(0x6f9320)=0x570160;
    PV_U(0x6f5648)=0x570268;PV_U(0x6de9e0)=0x570228;PV_U(0x6e9188)=0x5701e8;PV_U(0x6de548)=0x56f6e8;
    PV_U(0x6f9320+0x50)=0xa6;PV_U(0x6f5648+0x50)=0xa;PV_U(0x6de9e0+0x50)=0x1b4;PV_U(0x6e9188+0x50)=0x1f;
}

static int32_t rule_00405b40(uint32_t terrain){return (terrain&0x20300002u)==0x100002u;}
static int32_t rule_00405b60(uint32_t terrain){return (terrain&0x20300002u)==2;}
static int32_t rule_00405b80(uint32_t terrain){return (terrain&2)==2;}
code *restore_placement_rule(uint32_t token)
{
    switch(token){case 0x405b40:return (code *)rule_00405b40;case 0x405b60:return (code *)rule_00405b60;case 0x405b80:return (code *)rule_00405b80;}
    fprintf(stderr,"Unreconstructed or truncated placement rule %08lx.\n",(unsigned long)token);abort();
}
