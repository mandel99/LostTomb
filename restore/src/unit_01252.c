#include <stdint.h>
#define DAT_0096e6b0 (*(uint32_t *)0x0096e6b0u)
#define DAT_0096e6b4 (*(uint32_t *)0x0096e6b4u)
#define DAT_00978d98 (*(uint8_t *)0x00978d98u)
#define DAT_00978d9a (*(uint8_t *)0x00978d9au)
#define DAT_00a43eac (*(int8_t *)0x00a43eacu)
#define DAT_00af598c (*(uint32_t *)0x00af598cu)
#define DAT_00b350dc (*(int16_t *)0x00b350dcu)
#define DAT_00bc9804 (*(uint32_t *)0x00bc9804u)
#define DAT_00bc9b94 (*(int32_t *)0x00bc9b94u)
#define DAT_00bc9f24 (*(int32_t *)0x00bc9f24u)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bcd602 (*(uint8_t *)0x00bcd602u)
#define _DAT_00d3b4e0 (*(int32_t *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(int32_t *)0x00d3b4e4u)
/* Existing C neighbor count is in 0..8 and has a char return facade. */
extern char FUN_00477690(uint32_t,int32_t);
extern int32_t FUN_005190a0(uint32_t,uint32_t,uint32_t *,int32_t);

/* Native 486810: collect eligible tiles in 3999-entry batches, select one,
   and publish its signed map coordinates. Preserve the genuine child calls. */
uint32_t FUN_00486810(uint32_t request_a,uint32_t request_b,uint32_t reserved,
                     uint32_t tile_mask,int32_t min_x,int32_t min_y,
                     int32_t max_x,int32_t max_y)
{
    uint8_t rejected_actor_kind;
    int32_t row,count=0,selection;
    (void)reserved; /* No native read of the third argument. */
    if(tile_mask==0x40000u){
        if(DAT_00978d9a==0)return 0;
        rejected_actor_kind=0x5a;
    }else{
        if(DAT_00978d98==0)return 0;
        rejected_actor_kind=0x4b;
    }
    for(row=min_y;row<max_y;++row){
        uint32_t tile;
        int32_t x,right;
        if(min_x<(&DAT_00bc9f24)[(uint32_t)row]){
            tile=(&DAT_00bc9804)[(uint32_t)row];
            x=(&DAT_00bc9f24)[(uint32_t)row];
        }else{
            tile=DAT_00bca2d8+(uint32_t)row*228u+(uint32_t)min_x;
            x=min_x;
        }
        right=(&DAT_00bc9b94)[(uint32_t)row];
        if(max_x<=right)right=max_x;
        for(;x<right;++x,++tile){
            uint32_t flags=(&DAT_00af598c)[tile];
            if((flags&0x80000u)!=0)continue;
            if((tile_mask&flags)!=0){
                int32_t actor;
                if(tile_mask==0x40000u && FUN_00477690(0x40000u,(int32_t)tile)!=8)
                    continue;
                /* Native 4868C2/4868FB use a signed WORD at tile*2,
                   followed by a BYTE actor field at signedID*388. */
                actor=(&DAT_00b350dc)[tile];
                if((&DAT_00bcd602)[(uint32_t)actor*388u]==rejected_actor_kind)
                    continue;
                if((&DAT_00a43eac)[tile]==-1){
                    (&DAT_0096e6b4)[(uint32_t)count]=tile;
                    ++count;
                }
            }
            if(count==3999){
                selection=FUN_005190a0(request_a,request_b,&DAT_0096e6b4,3999);
                if(selection!=0)goto selected;
                count=0;
            }
        }
    }
    if(count!=0){
        selection=FUN_005190a0(request_a,request_b,&DAT_0096e6b4,count);
        if(selection!=0)goto selected;
    }
    if(tile_mask==0x40000u)DAT_00978d9a=0;
    else DAT_00978d98=0;
    return 0;
selected:
    {
        /* The selected index is one-based. Native IDIV treats the wrapped
           tile-origin difference as signed32; negative remainders survive. */
        int32_t delta=(int32_t)((&DAT_0096e6b0)[(uint32_t)selection]-DAT_00bca2d8);
        _DAT_00d3b4e4=delta%228;
        _DAT_00d3b4e0=delta/228;
        return 1;
    }
}
