#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void FUN_004c45e0(void);
static int selected[20],count,phase,current;
static void require(int condition) {if(!condition) abort();}
static unsigned char *record(int id) {return (unsigned char *)(0x007999d8u+id*264u);}
static void populate(int id,int age,int score)
{
    unsigned char *r=record(id);
    r[0]=1; r[3]=3;
    *(short *)(r+6)=(short)(-id);
    *(short *)(r+8)=(short)(2*id);
    *(short *)(r+16)=(short)age;
    *(short *)(r+18)=(short)score;
}
void FUN_0046d100(int id,int flag)
{
    require(phase==0&&flag==0&&count<20);
    require(*(unsigned int *)0x00e950e0u==0);
    selected[count++]=current=id;
    phase=1;
}
void FUN_004695d0(int id,int flag) {require(phase==1&&id==current&&flag==1); phase=2;}
void FUN_0053b090(int effect) {require(phase==2&&effect==17); phase=3;}
void FUN_00516f90(int x1,int y1,int x2,int y2,int flag)
{
    require(phase==3&&flag==1);
    require(x1==-current-3&&x2==-current+3&&y1==2*current-3&&y2==2*current+3);
    phase=4;
}
void FUN_005173a0(int x1,int y1,int x2,int y2)
{
    require(phase==4&&x1==-current-3&&x2==-current+3&&y1==2*current-3&&y2==2*current+3);
    phase=0;
}
static void reset(void)
{
    memset(record(0),0,4001u*264u);
    *(unsigned int *)0x00e950e0u=0x89abcdefu;
    count=phase=0;
}
int main(void)
{
    int i;
    if(!restore_data_open()) return 1;
    reset();
    FUN_004c45e0(); require(count==0&&phase==0);
    /* Age limits are inclusive 10..29; invalid active flags are excluded. */
    populate(1,9,100); populate(2,10,-32768); populate(3,29,32767); populate(4,30,100);
    populate(5,20,100); record(5)[0]=2;
    FUN_004c45e0(); require(count==2&&selected[0]==2&&selected[1]==3&&phase==0);
    reset();
    /* Fewer than 20 matches keep scan order, including the final valid record. */
    populate(10,20,50); populate(100,20,-50); populate(3999,20,0); populate(4000,20,100);
    FUN_004c45e0(); require(count==3&&selected[0]==10&&selected[1]==100&&selected[2]==3999);
    reset();
    for(i=1;i<=20;++i) populate(i,20,21-i);
    FUN_004c45e0(); require(count==20);
    for(i=0;i<20;++i) require(selected[i]==20-i);
    reset();
    for(i=1;i<=20;++i) populate(i,20,i);
    populate(21,20,100);
    FUN_004c45e0(); require(count==20&&selected[0]==21);
    for(i=1;i<20;++i) require(selected[i]==i);
    restore_data_close();
    puts("Record selection: scan boundaries, signed keys, 20-slot insertion and exact effect coordinates verified.");
    return 0;
}
