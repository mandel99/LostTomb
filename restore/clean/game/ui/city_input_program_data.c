#include "pharaoh/ui/city_input_program_data.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohImageButtonRecord)==36 &&
    offsetof(PharaohImageButtonRecord,reserved_0e)==14 &&
    offsetof(PharaohImageButtonRecord,enabled)==24 &&
    offsetof(PharaohImageButtonRecord,reserved_1c)==28,
    "Native image-button program row");

#define ROW(x,y,w,h,t,g,i,cb,help,p1,p2) \
    {x,y,w,h,t,g,i,{0,0,cb,0,help,0},1,0,0,0,{0,0,0,0,0,0,0,0}}

static const PharaohImageButtonRecord initial_compass[1]={
    ROW(0,0,36,21,4,379,0,176,158,128,247)
};
static const PharaohImageButtonRecord initial_expanded_extra[2]={
    ROW(128,0,33,20,4,379,7,16,157,128,247),
    ROW(8,-5,117,25,4,362,0,64,157,32,115)
};
static const PharaohImageButtonRecord initial_collapsed_expand[1]={
    ROW(9,0,33,20,4,379,10,16,157,128,247)
};
static const PharaohImageButtonRecord initial_expanded_build[16]={
    ROW(9,251,34,48,2,405,0,112,61,32,115),
    ROW(9,300,34,48,2,405,4,112,61,32,115),
    ROW(9,351,34,48,2,405,8,112,61,32,115),
    ROW(46,251,34,48,2,405,12,112,61,32,115),
    ROW(46,300,34,48,2,405,16,112,61,32,115),
    ROW(46,351,34,48,2,405,20,112,61,32,115),
    ROW(86,251,34,48,2,405,24,112,61,32,115),
    ROW(86,300,34,48,2,405,28,112,61,32,115),
    ROW(86,351,34,48,2,405,32,112,61,32,115),
    ROW(125,251,34,48,2,405,36,112,61,32,115),
    ROW(125,300,34,48,2,405,40,112,61,32,115),
    ROW(125,351,34,48,2,405,44,112,61,32,115),
    ROW(9,404,35,45,4,405,48,16,96,128,247),
    ROW(46,404,38,45,4,405,52,32,96,32,115),
    ROW(86,404,28,45,2,405,56,48,96,128,247),
    ROW(116,404,43,45,4,405,60,144,131,128,247)
};
static const PharaohImageButtonRecord initial_collapsed_build[12]={
    ROW(9,21,27,35,2,377,0,112,61,32,115),
    ROW(9,57,27,34,2,377,4,112,61,32,115),
    ROW(9,92,27,36,2,377,8,112,61,32,115),
    ROW(9,129,27,33,2,377,12,112,61,32,115),
    ROW(9,163,27,34,2,377,16,112,61,32,115),
    ROW(9,198,27,34,2,377,20,112,61,32,115),
    ROW(9,233,27,32,2,377,24,112,61,32,115),
    ROW(9,266,27,35,2,377,28,112,61,32,115),
    ROW(9,302,27,34,2,377,32,112,61,32,115),
    ROW(9,338,27,35,2,377,36,112,61,32,115),
    ROW(9,374,27,31,2,377,40,112,61,32,115),
    ROW(9,406,27,39,2,377,44,112,61,32,115)
};
static const PharaohImageButtonRecord initial_expanded_top[2]={
    ROW(16,143,60,36,4,387,0,32,181,32,115),
    ROW(90,143,62,36,4,387,4,144,181,32,115)
};
static const PharaohImageButtonRecord initial_editor_full[15]={
    ROW(7,123,71,23,4,406,45,0,0,0,0),
    ROW(84,123,71,23,4,406,45,0,0,0,0),
    ROW(13,267,39,26,2,406,0,0,0,0,0),
    ROW(63,267,39,26,2,406,3,0,0,0,0),
    ROW(113,267,39,26,2,406,6,0,0,0,0),
    ROW(13,303,39,26,2,406,18,0,0,0,0),
    ROW(63,303,39,26,2,406,30,0,0,0,0),
    ROW(113,303,39,26,2,406,15,0,0,0,0),
    ROW(13,339,39,26,2,406,33,0,0,0,0),
    ROW(63,339,39,26,2,406,39,0,0,0,0),
    ROW(113,339,39,26,2,406,42,0,0,0,0),
    ROW(13,375,39,26,2,406,27,0,0,0,0),
    ROW(63,375,39,26,4,406,24,0,0,0,0),
    ROW(113,375,39,26,2,712,0,0,0,0,0),
    {0}
};
#undef ROW

/* The compact ROW initializer above preserves the already-qualified bytes,
 * including the little-endian callback DWORDs split across reserved_0e.
 * Parameters differ from callback bytes and are installed explicitly. */
static void install_parameters(PharaohCityInputProgramData *d)
{
    static const uint32_t categories[12]={1,3,2,4,10,12,8,6,7,5,9,11};
    static const uint32_t editor[13]={1,0,1,2,6,10,5,17,13,14,24,8,32};
    for(int32_t i=0;i<12;++i){
        memcpy(d->expanded_build[i].reserved_1c,&categories[i],4);
        memcpy(d->expanded_build[i].reserved_1c+4,&(uint32_t){110},4);
        memcpy(d->collapsed_build[i].reserved_1c,&categories[i],4);
        memcpy(d->collapsed_build[i].reserved_1c+4,&(uint32_t){110},4);
    }
    memcpy(d->expanded_build[13].reserved_1c+4,&(uint32_t){34},4);
    memcpy(d->expanded_top[0].reserved_1c+4,&(uint32_t){14},4);
    memcpy(d->expanded_top[1].reserved_1c+4,&(uint32_t){32},4);
    for(int32_t i=1;i<14;++i)
        memcpy(d->editor_full[i].reserved_1c,&editor[i-1],4);
}

static void install_exact_tokens(PharaohCityInputProgramData *d)
{
#define TOKEN(row,left,right) do{uint32_t l=(left),r=(right);memcpy((row).reserved_0e+2,&l,4);memcpy((row).reserved_0e+6,&r,4);}while(0)
    TOKEN(d->compass[0],0x00409eb0u,0x0051f780u);
    TOKEN(d->expanded_extra[0],0x00409d10u,0x0051f780u);
    TOKEN(d->expanded_extra[1],0x00409d40u,0x00407320u);
    TOKEN(d->collapsed_expand[0],0x00409d10u,0x0051f780u);
    for(int32_t i=0;i<12;++i){TOKEN(d->expanded_build[i],0x00403d70u,0x00407320u);TOKEN(d->collapsed_build[i],0x00403d70u,0x00407320u);}
    TOKEN(d->expanded_build[12],0x00406010u,0x0051f780u);
    TOKEN(d->expanded_build[13],0x00406020u,0x00407320u);
    TOKEN(d->expanded_build[14],0x00406030u,0x0051f780u);
    TOKEN(d->expanded_build[15],0x00418390u,0x0051f780u);
    TOKEN(d->expanded_top[0],0x0040b520u,0x00407320u);
    TOKEN(d->expanded_top[1],0x0040b590u,0x00407320u);
    TOKEN(d->editor_full[0],0x0040b590u,0x0051f780u);
    TOKEN(d->editor_full[1],0x00409cf0u,0x0051f780u);
    for(int32_t i=2;i<14;++i)TOKEN(d->editor_full[i],0x00409ac0u,0x0051f780u);
#undef TOKEN
}

PharaohCityInputProgramStatus pharaoh_city_input_program_data_initialize_once(
    PharaohCityInputProgramData *d,PharaohCityInputActionResolver resolve,void *user)
{
    if(d->initialized)return PHARAOH_CITY_INPUT_PROGRAM_READY;
    PharaohGenericButtonAction left=resolve?resolve(user,0x00406030u):NULL;
    PharaohGenericButtonAction right=resolve?resolve(user,0x0051f780u):NULL;
    if(!left||!right)return PHARAOH_CITY_INPUT_PROGRAM_MISSING_ACTION;
    memcpy(d->compass,initial_compass,sizeof(d->compass));
    memcpy(d->expanded_extra,initial_expanded_extra,sizeof(d->expanded_extra));
    memcpy(d->collapsed_expand,initial_collapsed_expand,sizeof(d->collapsed_expand));
    memcpy(d->expanded_build,initial_expanded_build,sizeof(d->expanded_build));
    memcpy(d->collapsed_build,initial_collapsed_build,sizeof(d->collapsed_build));
    memcpy(d->expanded_top,initial_expanded_top,sizeof(d->expanded_top));
    memcpy(d->editor_full,initial_editor_full,sizeof(d->editor_full));
    install_exact_tokens(d);install_parameters(d);
    d->problem[0]=(PharaohGenericButton){{86,404,114,449},left,right,0,
        PHARAOH_GENERIC_BUTTON_TRIGGER_LEFT_PRESS,1,0};
    d->expanded_extra_trailing=0;d->collapsed_expand_trailing=405;
    d->expanded_build_trailing=0;d->collapsed_build_trailing=0;
    d->expanded_top_trailing=0;
    d->editor_full_trailing=60;d->editor_preview_trailing=2;
    d->initialized=1;
    return PHARAOH_CITY_INPUT_PROGRAM_READY;
}

void pharaoh_city_input_program_data_bind_tables(PharaohCityInputProgramData *d,
    PharaohCityIngameButtonTables *t)
{
    t->expanded_extra=(PharaohCityInputImageTable){d->expanded_extra,&d->expanded_extra_trailing};
    t->collapsed_expand=(PharaohCityInputImageTable){d->collapsed_expand,&d->collapsed_expand_trailing};
    t->expanded_build=(PharaohCityInputImageTable){d->expanded_build,&d->expanded_build_trailing};
    t->collapsed_build=(PharaohCityInputImageTable){d->collapsed_build,&d->collapsed_build_trailing};
    t->expanded_top=(PharaohCityInputImageTable){d->expanded_top,&d->expanded_top_trailing};
    t->problem=d->problem;
    t->editor_full=(PharaohCityInputImageTable){d->editor_full,&d->editor_full_trailing};
    t->editor_preview=(PharaohCityInputImageTable){d->editor_full,&d->editor_preview_trailing};
}
