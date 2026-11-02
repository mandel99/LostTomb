#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int FUN_004e92c0(int,int);
extern int FUN_004e94a0(int);
extern int FUN_004e94d0(int);
extern void FUN_004e9700(void);
extern int FUN_004e9760(int,int);
extern int FUN_004ea280(int);
extern void FUN_004ea740(void);
extern uint32_t FUN_004ea930(void);
extern uint32_t FUN_004ea9a0(int);
extern int FUN_004ea9c0(int);
extern int FUN_004eada0(int);
extern int FUN_004f5020(void);
extern unsigned int FUN_005165b0(void);
extern char *FUN_005169c0(void);
extern char *FUN_00516a20(void);
extern void FUN_00517a20(void);

static int verify_filename_helpers(void)
{
    char *name=(char *)(uintptr_t)0x00e38e74u;
    unsigned char *outputs=(unsigned char *)(uintptr_t)0x00eadda0u;
    char saved_name[32];
    unsigned char saved_outputs[0x208],expected[0x208];
    static const char *names[]={"","A","Khenshoth","abcdefghijklmnopqrstuvwxy123456"};
    unsigned int i;
    int ok=0;
    memcpy(saved_name,name,sizeof(saved_name));
    memcpy(saved_outputs,outputs,sizeof(saved_outputs));
    for(i=0;i<sizeof(names)/sizeof(names[0]);++i) {
        char *returned;
        strcpy(name,names[i]);
        memset(outputs,0xcc,sizeof(saved_outputs));
        returned=((char *(__cdecl *)(void))0x005169c0u)();
        memcpy(expected,outputs,sizeof(expected));
        memset(outputs,0xcc,sizeof(saved_outputs));
        if(FUN_005169c0()!=returned || memcmp(outputs,expected,sizeof(expected))) goto done;
        memset(outputs,0xcc,sizeof(saved_outputs));
        returned=((char *(__cdecl *)(void))0x00516a20u)();
        memcpy(expected,outputs,sizeof(expected));
        memset(outputs,0xcc,sizeof(saved_outputs));
        if(FUN_00516a20()!=returned || memcmp(outputs,expected,sizeof(expected))) goto done;
    }
    ok=1;
done:
    memcpy(name,saved_name,sizeof(saved_name));
    memcpy(outputs,saved_outputs,sizeof(saved_outputs));
    return ok;
}

static int verify_entity_count(void)
{
    enum { bytes=4000*0x108 };
    unsigned char *records=(unsigned char *)(uintptr_t)0x00799ae0u;
    unsigned char *saved=(unsigned char *)malloc(bytes);
    static const int thresholds[]={-200,0,10,11,50,51};
    static const int expected[]={3,2,2,1,1,0};
    static const int slots[]={0,3,3999};
    static const short ages[]={20,-100,60};
    unsigned int i;
    int ok=0;
    if(!saved) return 0;
    memcpy(saved,records,bytes);
    memset(records,0,bytes);
    for(i=0;i<3;++i) {
        unsigned char *record=records+(unsigned int)slots[i]*0x108u;
        record[0]=2;
        record[5]=1;
        *(short *)(record+0x10u)=ages[i];
        *(short *)(record+0x1cu)=-1;
    }
    for(i=0;i<sizeof(thresholds)/sizeof(thresholds[0]);++i) {
        int original=((int (__cdecl *)(int))0x004ea280u)(thresholds[i]);
        if(original!=expected[i] || original!=FUN_004ea280(thresholds[i])) goto done;
    }
    ok=1;
done:
    memcpy(records,saved,bytes);
    free(saved);
    return ok;
}

int restore_verify_city_history(FILE *log)
{
    enum { span=0xa8e8 };
    unsigned char *memory=(unsigned char *)(uintptr_t)0x00ea0000u;
    unsigned char *saved=(unsigned char *)malloc(span);
    unsigned char *expected=(unsigned char *)malloc(0x49d8);
    uint32_t *selector=(uint32_t *)(uintptr_t)0x00eaac28u;
    uint32_t saved_selector=*selector, random=0x27a08135u;
    int32_t saved_requirements[72];
    int32_t *requirements=(int32_t *)(uintptr_t)0x00784de8u;
    unsigned int s,i,j;
    int ok=0;
    if (!saved || !expected) goto done;
    memcpy(saved,memory,span);
    memcpy(saved_requirements,requirements,sizeof(saved_requirements));
    for (s=0;s<2;++s) {
        unsigned char *city=memory+s*0x49d8u;
        *selector=s;
        for (i=0;i<100;++i) ((int16_t *)(city+0x3e34u))[i]=(int16_t)(i*739u-32768u);
        for (i=0;i<10;++i) {
            if (((int (__cdecl *)(int,int))0x004e92c0u)((int)s,(int)i)!=FUN_004e92c0((int)s,(int)i)) goto restore;
        }
        if (((int (__cdecl *)(int))0x004e94a0u)((int)s)!=FUN_004e94a0((int)s)) goto restore;
        if (((int (__cdecl *)(int))0x004e94d0u)((int)s)!=FUN_004e94d0((int)s)) goto restore;
        for (i=0;i<6;++i) {
            static const int cursors[]={0,1,2398,2399,17,1234};
            static const int totals[]={0,1,2399,2400,10000,2147483647};
            *(int *)(city+0x3e2cu)=cursors[i];
            *(int *)(city+0x3e30u)=totals[i];
            *(int *)(city+0x1894u)=0x12340000+(int)i;
            memcpy(expected,city,0x49d8);
            ((void (__cdecl *)(void))0x004e9700u)();
            {
                unsigned char actual[0x49d8];
                memcpy(actual,city,sizeof(actual));
                memcpy(city,expected,sizeof(actual));
                FUN_004e9700();
                if (memcmp(city,actual,sizeof(actual))) goto restore;
            }
            for (j=0;j<2400;++j) ((int *)(city+0x18acu))[j]=(int)(j*371u+19u);
            for (j=0;j<48;++j) {
                int distance=(int)(j*59u), index=(int)(j*47u);
                if (((int (__cdecl *)(int,int))0x004e9760u)(distance,index)!=FUN_004e9760(distance,index)) goto restore;
            }
        }
        for (i=0;i<2;++i) {
            uint32_t before=0x80000000u, expected_value;
            *(uint32_t *)(city+0x5d40u)=i;
            *(uint32_t *)(city+0x5d44u)=before;
            ((void (__cdecl *)(void))0x004ea740u)();
            expected_value=*(uint32_t *)(city+0x5d44u);
            *(uint32_t *)(city+0x5d44u)=before;
            FUN_004ea740();
            if (*(uint32_t *)(city+0x5d44u)!=expected_value) goto restore;
        }
        for (i=0;i<8;++i) {
            for (j=0;j<3;++j) ((int32_t *)(city+0x5fa0u))[j]=(i&(1u<<j)) ? -(int32_t)(j+1) : 0;
            if(((int (__cdecl *)(void))0x004f5020u)()!=FUN_004f5020()) goto restore;
        }
    }
    for (i=0;i<4096;++i) {
        int input;
        random=random*1664525u+1013904223u;
        input=(i<1024) ? (int)i-512 : (int)random;
        if (((uint32_t (__cdecl *)(int))0x004ea9a0u)(input)!=FUN_004ea9a0(input)) goto restore;
        if (((int (__cdecl *)(int))0x004ea9c0u)(input)!=FUN_004ea9c0(input)) goto restore;
        if (((int (__cdecl *)(int))0x004eada0u)(input)!=FUN_004eada0(input)) goto restore;
    }
    for (i=0;i<40;++i) {
        memset(requirements,0,sizeof(saved_requirements));
        if (i<36) requirements[i]=1;
        if (((uint32_t (__cdecl *)(void))0x004ea930u)()!=FUN_004ea930()) goto restore;
    }
    for (i=0;i<4;++i) {
        static const int ages[]={-32768,0,20,32767};
        if (((int (__cdecl *)(int))0x004ea280u)(ages[i])!=FUN_004ea280(ages[i])) goto restore;
    }
    if(((unsigned int (__cdecl *)(void))0x005165b0u)()!=FUN_005165b0()) goto restore;
    if(!verify_filename_helpers()) goto restore;
    if(!verify_entity_count()) goto restore;
    {
        unsigned char *map=(unsigned char *)(uintptr_t)0x00ee0c50u;
        unsigned char *saved_map=(unsigned char *)malloc(0xcb10u);
        if(!saved_map) goto restore;
        memcpy(saved_map,map,0xcb10u);
        ((void (__cdecl *)(void))0x00517a20u)();
        for(i=0;i<0xcb10u;++i) if(map[i]!=0xffu) break;
        memcpy(map,saved_map,0xcb10u);
        FUN_00517a20();
        for(j=0;j<0xcb10u;++j) if(map[j]!=0xffu) break;
        memcpy(map,saved_map,0xcb10u);
        free(saved_map);
        if(i!=0xcb10u || j!=0xcb10u) goto restore;
    }
    ok=1;
restore:
    memcpy(memory,saved,span);
    memcpy(requirements,saved_requirements,sizeof(saved_requirements));
    *selector=saved_selector;
done:
    free(expected);
    free(saved);
    fprintf(log,"%s: city history and predicates match original x86 (two cities, ring boundaries, 12288 full-EAX predicate comparisons).\n",ok?"PASS":"FAIL");
    return ok;
}
