#include "sidebar_message_actions.h"
#include "display_confirmation.h"
#include "camera_navigation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
/* Actual whole29-instruction forward DWORD-pair/byte copy, native RET0.
   Its existing C preserves the48-byte nonoverlapping record swaps here. */
extern void FUN_0040f820(uint32_t *source,uint32_t *destination,int32_t bytes);
static void copy_record(uint32_t source,uint32_t destination)
{FUN_0040f820((uint32_t *)(uintptr_t)source,(uint32_t *)(uintptr_t)destination,48);}

/* Native4E1640..4E174F:86 instructions. Exactly1000 passes over999
   adjacent pairs; retain the selected signed WORD key through the swaps. */
void restore_sidebar_message_sort_004e1640(void)
{
 uint32_t selected=U(0xe951e4u),pass,cursor,count,index;
 int32_t key=-1;
 if((int32_t)selected>=0)key=S16(0xe95234u+selected*48u);
 for(pass=1000u;pass!=0;--pass){
  cursor=0xe95264u;
  do{
   if(W(cursor-0x32u)==0){
    if(W(cursor-2u)==0)goto next_pair;
   }else{
    int32_t previous=S16(cursor-0x30u);
    int32_t current=S16(cursor);
    if(previous>=current)goto next_pair;
    if(W(cursor-2u)==0)goto next_pair;
   }
   copy_record(cursor-0x3cu,0xe951f8u);
   copy_record(cursor-0x0cu,cursor-0x3cu);
   copy_record(0xe951f8u,cursor-0x0cu);
next_pair:
   cursor+=48u;
  }while(cursor<0xea0db4u);
 }
 count=0;cursor=0xe95232u;
 U(0xe951ecu)=0;
 U(0xe9511cu)=0;
 do{if(W(cursor)!=0)++count;cursor+=48u;}while(cursor<0xea0db2u);
 U(0xe951ecu)=count;
 if((int32_t)count>10)U(0xe9511cu)=count-10u;
 else U(0xe95120u)=0;
 if(key<0)return;
 index=0;cursor=0xe95234u;
 do{
  int32_t candidate=S16(cursor);
  if(candidate==key){U(0xe951e4u)=index;return;}
  cursor+=48u;++index;
 }while(cursor<0xea0db4u);
}
/* Native4E1620..4E1633:4 instructions. */
void restore_sidebar_message_open_004e1620(void)
{
 restore_sidebar_message_sort_004e1640();
 B(0xe38ddau)=0x11;
 B(0xe38dcau)=1;
}
/* Native4B80B0..4B8126:41 instructions, genuine one-DWORD/cdecl RET0.
   Stop at the first matching group even when both coordinates are nonpositive. */
uint32_t restore_sidebar_group_tile_004b80b0(int32_t key)
{
 uint32_t cursor=0xbcba9au,id=1;
 do{
  if(B(cursor-0x42u)==1&&B(cursor)==0&&B(cursor+0x27u)==0&&
     S16(cursor+0x46u)==key){
   uint32_t offset=id*144u;
   uint32_t map=U(0xbca2d8u);
   int32_t y=S16(0xbcb9f8u+offset);
   int32_t x=S16(0xbcb9f6u+offset);
   uint32_t tile=map+(uint32_t)x+(uint32_t)y*228u;
   if(x>0||y>0)return tile;
   return 0;
  }
  cursor+=144u;++id;
 }while(cursor<0xbcd62au);
 return 0;
}
/* Both native selector passes perform this same ordered record/year/type
   query. Type7 uses the actual group-position query and then fallback tile. */
static int eligible(uint32_t cursor,int32_t *category)
{
 int32_t type=S16(cursor),year;
 uint32_t threshold,tile;
 if(type==0)return 0;
 year=S16(cursor-2u);
 threshold=U(0x78d7bcu)-1u;
 if(year<(int32_t)threshold)return 0;
 *category=S16(0xe209bau+(uint32_t)type*80u);
 if(*category==1)return 1;
 if(*category!=7)return 0;
 tile=restore_sidebar_group_tile_004b80b0((int32_t)U(cursor-10u));
 if((int32_t)tile>0)return 1;
 return (int32_t)U(cursor-6u)>0;
}
/* Native4E23E0..4E2527:113 instructions. Deliberately scans999 records,
   while the sort above scans1000. All native index updates/reloads remain. */
void restore_sidebar_message_locate_004e23e0(void)
{
 uint32_t cursor,count,seen,index,tile;
 int32_t category;
 restore_sidebar_message_sort_004e1640();
 seen=0;cursor=0xe95232u;
 U(0xe951d8u)=0;
 do{
  if(eligible(cursor,&category))U(0xe951d8u)=U(0xe951d8u)+1u;
  cursor+=48u;
 }while(cursor<0xea0d82u);
 count=U(0xe951d8u);
 if((int32_t)count<=0){U(0xe951d4u)=0;return;}
 index=U(0xe951d4u);
 if((int32_t)index>=(int32_t)count)U(0xe951d4u)=0;
 cursor=0xe95232u;
 do{
  if(!eligible(cursor,&category))goto next_record;
  index=U(0xe951d4u);
  ++seen;
  if((int32_t)index>=(int32_t)seen)goto next_record;
  tile=U(cursor-6u);
  U(0xe951d4u)=index+1u;
  if(category==7){
   tile=restore_sidebar_group_tile_004b80b0((int32_t)U(cursor-10u));
   if((int32_t)tile<=0){
    tile=U(cursor-6u);
    if((int32_t)tile<=0)goto next_record;
   }
  }
  FUN_00430cd0(tile);
  B(0xe38ddau)=1;
  B(0xe38dcau)=1;
  return;
next_record:
  cursor+=48u;
 }while(cursor<0xea0d82u);
}
/* Native406020:one tail JMP to4E1620, no new argument or register input. */
void restore_sidebar_message_00406020(void)
{restore_sidebar_message_open_004e1620();}
/* Native406030..40606C:14 instructions. Genuine PE import slot56F22C
   is WINMM.dll!timeGetTime, matching the accepted typed clock service. */
void restore_sidebar_message_00406030(void)
{
 uint32_t now,previous,elapsed;
 if(U(0xd3b198u)==0)return;
 now=restore_display_confirmation_clock();
 previous=U(0xe951d0u);
 elapsed=now-previous;
 if((int32_t)elapsed>3000)U(0xe951d4u)=0;
 U(0xe951d0u)=now;
 restore_sidebar_message_locate_004e23e0();
 B(0xe38dcau)=1;
}
int restore_sidebar_message_action(uint32_t token)
{
 switch(token){
 case 0x406020u:restore_sidebar_message_00406020();return 1;
 case 0x406030u:restore_sidebar_message_00406030();return 1;
 default:return 0;
 }
}

/* All five native direct callers discard this routine's volatile outputs.
   The original has no address-of/data references or tail-entry callers.
   Keep one algorithm for sidebar and older monthly/message paths alike. */
void FUN_004e1640(void)
{
 restore_sidebar_message_sort_004e1640();
}
