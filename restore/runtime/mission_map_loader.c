#include "mission_map_loader.h"
#include "map_commit_selection.h" /* Reuse whole 4F44E0 and 53D930. */
#include "mission_loaders.h"
#include "monument_rating.h"
#include "sprite_libraries.h"
#include <io.h>
#include <string.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

extern void FUN_004ce2d0(void);

/* All native entries are cdecl/RET 0; the asset providers have no
   incoming receiver. Only the genuine sprite-manager method receives ECX. */
static void load_asset(uint32_t name,int32_t slot)
{
    (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,
                     (const char *)(uintptr_t)name,slot);
}

int32_t FUN_0053d8e0(void)
{
    uint32_t address;
    for(address=0x784de0u;address<0x784de4u;address+=2u){
        int32_t id=S16(address);
        if(id!=0){
            uint32_t record=0x5d19c8u+((uint32_t)id<<4);
            if(W(record)==2u&&S16(record+2u)<5)return S16(record+2u);
        }
    }
    return 1;
}

/* Native MOVSX WORD selector and WORD clamp, then fourteen five-column
   DWORD copies. The selected index is retained across the sprite load. */
void FUN_004bc720(void)
{
    int32_t selected=S16(0x7848fcu);
    uint32_t source,destination;
    if(selected>=12){selected=11;W(0x7848fcu)=11;}
    load_asset(U(0x5def84u+(uint32_t)selected*4u),2);
    source=0x7083e4u+(uint32_t)selected*280u;
    for(destination=0x709a34u;destination<0x709a6cu;destination+=4u){
        U(destination+0x38u)=U(source-0x38u);
        U(destination)=U(source);
        U(destination+0x70u)=U(source+0x38u);
        U(destination+0xaf0u)=U(source+0x70u);
        U(destination+0xab8u)=U(source+0xa8u);
        source+=4u;
    }
}

static void load_shared_dummies(void)
{
    load_asset(0x579134u,0x12);
    load_asset(0x579124u,0x13);
    load_asset(0x57922cu,0x14);
    load_asset(0x57922cu,0x15);
    load_asset(0x57922cu,0x16);
    load_asset(0x5f75f8u,0x17);
    load_asset(0x579110u,0x18);
}

/* Complete 250-instruction provider, including the five-entry 4F4918 jump
   table. Every name is its original immutable data address. */
void FUN_004f4520(void)
{
    uint8_t phase=B(0x784cbcu);
    if(phase<=1u){
        int32_t kind;
        uint32_t name;
        B(0x784cbcu)=1;
        kind=FUN_004f44e0();
        U(0x5e4ef4u)=(uint32_t)kind;
        switch(kind){
        case 1:name=0x5f763cu;break;
        case 2:name=0x5f7628u;break;
        case 3:name=0x5f7618u;break;
        case 5:name=0x5f7610u;break;
        default:name=0x5f7608u;break;
        }
        load_asset(name,0x11);
        load_shared_dummies();
        load_asset(0x579100u,0x19);
        load_asset(0x5790e0u,0x1b);
        load_asset(0x5790d0u,0x1c);
        load_asset(0x5790c0u,0x1d);
        load_asset(0x5790f0u,0x1a);
        return;
    }
    if(phase==2u){
        int8_t mission;
        load_asset(0x579144u,0x11);
        load_shared_dummies();
        load_asset(0x5790f0u,0x1a);
        mission=(int8_t)B(0xe38ea8u);
        load_asset(mission>=39&&mission<=41?0x5f75ecu:0x5f75e0u,0x19);
        phase=B(0xe38ea8u);
        load_asset(phase==40u||phase==41u?0x5f75ccu:0x5f75d8u,0x1b);
        load_asset(B(0xe38ea8u)==41u?0x5f75c0u:0x5f75b4u,0x1c);
        load_asset(0x5f75acu,0x1d);
        return;
    }
    if(phase==4u){
        load_asset(0x5760b0u,0x1a);
        load_asset(0x579144u,0x11);
        load_shared_dummies();
        load_asset(0x579100u,0x19);
        load_asset(0x5790d0u,0x1c);
        load_asset(0x5790e0u,0x1b);
        load_asset(0x5790c0u,0x1d);
        return;
    }
    load_asset(0x579144u,0x11);
    load_asset(0x579100u,0x19);
    load_asset(0x5790e0u,0x1b);
    load_asset(0x5790d0u,0x1c);
    load_asset(0x5790c0u,0x1d);
    load_asset(0x5f75a0u,0x12);
    load_asset(0x5e4934u,0x13);
    load_asset(0x5762fcu,0x14);
    load_asset(0x576338u,0x15);
    load_asset(0x5762b4u,0x16);
    load_asset(0x576284u,0x17);
    load_asset(0x5f7590u,0x18);
    load_asset(0x5790f0u,0x1a);
}

/* Complete 81-instruction provider. The native comparison is case-sensitive
   and the two cached query results are full DWORD stores. */
void FUN_0053d970(void)
{
    int32_t kind,variant;
    if(strcmp((const char *)0x784914u,(const char *)0x601840u)==0)kind=4;
    else kind=FUN_0053d8e0();
    U(0xf55bacu)=(uint32_t)kind;
    if(kind==2){load_asset(0x6017e0u,9);return;}
    if(kind==3){load_asset(0x6017f0u,9);return;}
    if(kind!=4){load_asset(0x5791a8u,9);return;}
    variant=FUN_0053d930();
    U(0xe94e18u)=(uint32_t)variant;
    switch(variant){
    case 1:load_asset(0x601804u,9);return;
    case 2:load_asset(0x601818u,9);return;
    default:load_asset(0x60182cu,9);return;
    }
}

/* Complete 38-instruction parent. Real CRT I/O is the external boundary;
   the actual chunk loader and all three asset providers execute normally. */
uint32_t FUN_004d5820(const char *path,int mode)
{
    int descriptor;
    FUN_004ce2d0();
    FUN_004bd330(RESTORE_LOAD_SETTINGS);
    descriptor=_open(path,0x8000);
    if(descriptor==-1)return 1;
    FUN_004d4ae0(descriptor);
    (void)_close(descriptor);
    U(0x784f10u)=(uint32_t)S16(0x7848dcu);
    if(mode==0){FUN_004f4520();FUN_0053d970();FUN_004bc720();}
    if(U(0x605a3cu)==1u)U(0x784c2cu)=(uint32_t)restore_monument_rating_int32(1);
    return 0;
}
