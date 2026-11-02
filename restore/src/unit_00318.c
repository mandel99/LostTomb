#include "animation_state.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern uint32_t FUN_00562348(void);
void FUN_0041fdb0(void *record)
{
    void *in_ECX=record;
    uintptr_t self=(uintptr_t)record,table,entries,candidate;
    int32_t kind,index,state,frame,maximum,rows,row,choices,selected;
    int32_t linked=H(self+18);
    if(linked && B(0xbcd630u+(uint32_t)linked*388u)!=B(self+20)) {
        H(self+18)=0;B(self+20)=0;H(self+16)=3;return;
    }
    state=H(self+16);
    if(state==2||state==3)return;
    kind=I(self);if(kind<0||kind>=253)return;
    index=I(self+4);table=0x5be9b8u+(uint32_t)kind*40u;
    if(index<0||index>=I(table+20))return;
    entries=U(table+16);
    U(self+8)+=B(self+21)?0xffffffffu:1u;
    frame=I(self+8);maximum=I(entries+(uint32_t)index*48u+12);
    if(frame>=0&&frame<=maximum)return;
    if(state==1) {
        if(frame>maximum)I(self+8)=maximum;
        else if(frame<0)I(self+8)=0;
        return;
    }
    rows=I(table+28);entries=U(table+24);
    for(row=0;row<rows;++row) {
        candidate=entries+(uint32_t)row*40u;
        if(I(candidate)==index && B(candidate+4)==B(self+21)) {
            for(choices=0;choices<4;++choices)
                if(I(candidate+8u+(uint32_t)choices*8u)<0)break;
            if(!choices)goto fallback;
            selected=(int32_t)FUN_00562348()%choices;
            candidate=U(table+24)+(uint32_t)row*40u+(uint32_t)selected*8u+8u;
            index=I(candidate);I(self+4)=index;
            B(self+21)=B(U(table+24)+(uint32_t)row*40u+(uint32_t)selected*8u+12u);
            if(index<0||index>=I(table+20))goto fallback;
            U(self+8)=B(self+21)?U(U(table+16)+(uint32_t)index*48u+12u):0;
            return;
        }
    }
fallback:
  FUN_0041fca0(in_ECX,3);
}
