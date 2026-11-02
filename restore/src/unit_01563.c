#include "simulation_tick.h"
#include "flood_simulation.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
extern void FUN_00489510(void);
static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b) {return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t mul(int32_t a,int32_t b) {return (int32_t)((uint32_t)a*(uint32_t)b);}
static int32_t trunc32(double value) {return (int32_t)(uint32_t)(int64_t)value;}
void FUN_004be770(uintptr_t self)
{
    int32_t season,width,start,end,span,years,subday,total,previous,cycle,before,after;
    double period,absolute;
    if (!*(uint8_t *)(self+0x32fbc)) {
        I(self+0x32fb4)=6;I(self+0x32c54)=0;return;
    }
    if (I(0x78d7cc)==1 && I(self+0x32fb4)!=2) {
        int32_t target=I(self+0x32c5c),current=I(self+0x32c54);
        if (current>target) {current=sub(current,5);I(self+0x32c54)=current<=target?target:current;}
        else if (current<target) {current=add(current,5);I(self+0x32c54)=current>=target?target:current;}
    }
    period=*(double *)0xd05718u;
    years=sub(I(0x78d7bc),*(int16_t *)0x7848d8u);
    season=I(self+0x32c4c);width=I(self+0x32fb8);
    start=add(add(mul(season,105)/100,trunc32((double)years*period)),15);
    end=add(add(I(self+0x32c50),mul(width,2)),start);
    span=trunc32(((double)I(self+0x32c60)*(double)width)*0.01);
    subday=add(mul(add(mul(I(0x78d7c0),16),I(0x78d7c4)),51),I(0x78d7cc));
    total=add(trunc32((double)years*9792.0),subday);
    absolute=(double)total;
    previous=trunc32((absolute-1.0)/25.0);cycle=trunc32(absolute/25.0);
    if (period*0.5<(double)sub(start,cycle)) {
        int32_t year_cycles=trunc32(period);start=sub(start,year_cycles);end=sub(end,year_cycles);
    }
    I(self+0x32fb0)=season/30;before=sub(start,cycle);after=sub(cycle,end);
    if (cycle>=start && cycle<end) {
        int32_t row;
        if (cycle<add(start,span)) {I(self+0x32fb4)=2;row=add(before,30);}
        else if (cycle<sub(end,span)) {I(self+0x32fb4)=3;row=sub(30,span);}
        else {I(self+0x32fb4)=4;row=add(after,31);}
        row=sub(row,1);I(self+0x32fac)=row<0?0:row>30?30:row;
    } else {
        I(self+0x32fac)=30;
        I(self+0x32fb4)=before>=0&&before<28?1:after>=0&&after<28?5:6;
    }
    if (cycle!=previous) {
        if (cycle<start || cycle>=end) {
            if (before==49) FUN_00489510();
            else if (before==1) FUN_004be490(self);
            else if (after==1) FUN_004be580(self);
        } else if (cycle<add(start,span) || cycle>=sub(end,span)) {
            *(uint8_t *)0xeaac2cu=1;
        } else if (cycle==add(start,width)) {
            uint32_t row;for(row=0;row<30;++row)I(self+0x32c68+row*28u)=0;
        }
    }
    if (cycle>=sub(end,width)) {
        int32_t elapsed=add(width,after),progress,limit=mul(width,60),selected;
        if (elapsed<0) elapsed=add(elapsed,trunc32(period));
        progress=add(trunc32(absolute/(25.0*0.16666666666666666))%6,mul(elapsed,6));
        selected=progress<=limit?progress:limit;
        if (progress<limit && selected!=I(0x5df760)) {
            int32_t offset=selected%width,maximum=sub(width,1),row;
            if (offset>=maximum) offset=maximum;
            I(0x5df760)=selected;row=sub(I(self+0x32fac),offset);
            if (row>=0 && row<I(self+0x32fac))
                FUN_004be0d0((RestoreWorldRegion *)(self+0x32c64+(uint32_t)row*28u));
        }
        I(0x5df75c)=progress;
    }
    {
        int32_t percent=mul(total%25,100)/25,phase=I(self+0x32fb4);
        if (phase==2 || phase==4) {
            FUN_004bd850((RestoreWorldRegion *)(self+0x32c64+U(self+0x32fac)*28u),phase==2,percent,0);
            I(0xbc9694)=0;
        }
    }
}
