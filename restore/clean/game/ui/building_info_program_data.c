#include "pharaoh/ui/building_info_program_data.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohInfoNativeGenericButton)==28 &&
    offsetof(PharaohInfoNativeGenericButton,left_token)==8 &&
    offsetof(PharaohInfoNativeGenericButton,right_token)==12 &&
    offsetof(PharaohInfoNativeGenericButton,activated)==17 &&
    offsetof(PharaohInfoNativeGenericButton,trigger)==19 &&
    offsetof(PharaohInfoNativeGenericButton,primary_parameter)==20 &&
    offsetof(PharaohInfoNativeGenericButton,secondary_parameter)==24,
    "native GenericButton28 evidence layout");
_Static_assert(sizeof(PharaohMenuRect)==8 && sizeof(PharaohImageButtonRecord)==36 &&
    offsetof(PharaohImageButtonRecord,reserved_0e)==14 &&
    offsetof(PharaohImageButtonRecord,enabled)==24 &&
    offsetof(PharaohImageButtonRecord,reserved_1c)==28 &&
    sizeof(PharaohSquareButtonRecord)==24 &&
    offsetof(PharaohSquareButtonRecord,callback_token)==8 &&
    offsetof(PharaohSquareButtonRecord,primary_parameter)==16 &&
    offsetof(PharaohSquareButtonRecord,secondary_parameter)==20,
    "native image36/square24 fields");
_Static_assert(PHARAOH_INFO_ROW_COUNT==15,"native info generic row catalogue");

/* Pinned PE bytes supplied by losttomb-info-ui-tables/native-data.json.
 * These are initial program records; runtime values with producers are borrowed. */
const PharaohInfoNativeGenericButton pharaoh_info_program_native_generic[68]={
    /* HOUSE 0x005C5A78 */
    {{0,0,240,20},0x004062d0u,0x0051f780u,0,0,0,1,0,0},
    /* GRANARY 0x005C5A98 */
    {{0,0,272,20},0x004062f0u,0x0051f780u,0,0,0,1,0,0},
    /* BAZAAR 0x005C5AB8 */
    {{0,0,272,20},0x004062f0u,0x0051f780u,0,0,0,1,0,0},
    /* STORAGE 0x005C5AD8 */
    {{0,0,272,20},0x004062f0u,0x0051f780u,0,0,0,1,0,0},
    /* BACK 0x005C5BA8 */
    {{0,0,304,20},0x00406620u,0x0051f780u,0,0,0,1,0,0},
    /* STORAGE_BACK 0x005C5BC8 */
    {{0,20,304,40},0x00406680u,0x0051f780u,0,0,0,1,2,0},
    /* POLICIES 0x005C68C8 */
    {{0,0,165,20},0x00406300u,0x0051f780u,0,0,0,1,1,0},
    {{0,20,165,40},0x00406300u,0x0051f780u,0,0,0,1,2,0},
    {{0,40,165,60},0x00406300u,0x0051f780u,0,0,0,1,3,0},
    {{0,60,165,80},0x00406300u,0x0051f780u,0,0,0,1,4,0},
    {{0,80,165,100},0x00406300u,0x0051f780u,0,0,0,1,5,0},
    {{0,100,165,120},0x00406300u,0x0051f780u,0,0,0,1,6,0},
    {{0,120,165,140},0x00406300u,0x0051f780u,0,0,0,1,7,0},
    {{0,140,165,160},0x00406300u,0x0051f780u,0,0,0,1,8,0},
    {{0,160,165,180},0x00406300u,0x0051f780u,0,0,0,1,9,0},
    {{0,180,165,200},0x00406300u,0x0051f780u,0,0,0,1,10,0},
    {{0,200,165,220},0x00406300u,0x0051f780u,0,0,0,1,11,0},
    {{0,220,165,240},0x00406300u,0x0051f780u,0,0,0,1,12,0},
    {{0,240,165,260},0x00406300u,0x0051f780u,0,0,0,1,13,0},
    {{0,260,165,280},0x00406300u,0x0051f780u,0,0,0,1,14,0},
    {{0,280,165,300},0x00406300u,0x0051f780u,0,0,0,1,15,0},
    {{0,300,165,320},0x00406300u,0x0051f780u,0,0,0,1,16,0},
    {{0,320,165,340},0x00406300u,0x0051f780u,0,0,0,1,17,0},
    {{0,340,165,360},0x00406300u,0x0051f780u,0,0,0,1,18,0},
    {{0,360,165,380},0x00406300u,0x0051f780u,0,0,0,1,19,0},
    {{0,380,165,400},0x00406300u,0x0051f780u,0,0,0,1,20,0},
    {{0,400,165,420},0x00406300u,0x0051f780u,0,0,0,1,21,0},
    {{0,420,165,440},0x00406300u,0x0051f780u,0,0,0,1,22,0},
    {{0,440,165,460},0x00406300u,0x0051f780u,0,0,0,1,23,0},
    {{0,460,165,480},0x00406300u,0x0051f780u,0,0,0,1,24,0},
    {{0,480,165,500},0x00406300u,0x0051f780u,0,0,0,1,25,0},
    {{0,500,165,520},0x00406300u,0x0051f780u,0,0,0,1,26,0},
    {{0,520,165,540},0x00406300u,0x0051f780u,0,0,0,1,27,0},
    {{0,540,165,560},0x00406300u,0x0051f780u,0,0,0,1,28,0},
    {{0,560,165,580},0x00406300u,0x0051f780u,0,0,0,1,29,0},
    {{0,580,165,600},0x00406300u,0x0051f780u,0,0,0,1,30,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    {{0,0,0,0},0x00000000u,0x00000000u,0,0,0,0,0,0},
    /* WALKERS 0x005C5BE8 */
    {{26,46,76,96},0x00406700u,0x0051f780u,0,0,0,1,0,0},
    {{86,46,136,96},0x00406700u,0x0051f780u,0,0,0,1,1,0},
    {{146,46,196,96},0x00406700u,0x0051f780u,0,0,0,1,2,0},
    {{206,46,256,96},0x00406700u,0x0051f780u,0,0,0,1,3,0},
    {{266,46,316,96},0x00406700u,0x0051f780u,0,0,0,1,4,0},
    {{326,46,376,96},0x00406700u,0x0051f780u,0,0,0,1,5,0},
    {{386,46,436,96},0x00406700u,0x0051f780u,0,0,0,1,6,0},
    /* PHRASE 0x005C5CB0 */
    {{0,0,150,25},0x0053d730u,0x0051f780u,0,0,0,1,0,0},
    /* LEGION 0x005C66D8 */
    {{19,139,103,223},0x00406720u,0x0051f780u,0,0,0,1,0,0},
    {{104,139,188,223},0x00406720u,0x0051f780u,0,0,0,1,1,0},
    {{189,139,273,223},0x00406720u,0x0051f780u,0,0,0,1,2,0},
    {{274,139,358,223},0x00406720u,0x0051f780u,0,0,0,1,3,0},
    {{359,139,443,223},0x00406fb0u,0x0051f780u,0,0,0,1,4,0},
    /* LEGION_RETURN 0x005C6768 */
    {{0,0,320,30},0x00407110u,0x0051f780u,0,0,0,1,0,0},
    /* TRANSPORT 0x005C6788 */
    {{19,139,103,223},0x00406b40u,0x0051f780u,0,0,0,1,0,0},
    {{104,139,188,223},0x00406b40u,0x0051f780u,0,0,0,1,1,0},
    {{189,139,273,223},0x00406b40u,0x0051f780u,0,0,0,1,2,0},
    {{274,139,358,223},0x00406b40u,0x0051f780u,0,0,0,1,3,0},
    {{359,139,443,223},0x00406e80u,0x0051f780u,0,0,0,1,4,0},
    /* WARSHIP 0x005C6818 */
    {{19,139,103,223},0x00406c90u,0x0051f780u,0,0,0,1,0,0},
    {{104,139,188,223},0x00406c90u,0x0051f780u,0,0,0,1,1,0},
    {{189,139,273,223},0x00406c90u,0x0051f780u,0,0,0,1,2,0},
    {{274,139,358,223},0x00406c90u,0x0051f780u,0,0,0,1,3,0},
    {{359,139,443,223},0x00406de0u,0x0051f780u,0,0,0,1,4,0},
    /* FESTIVAL 0x005C5448 */
    {{40,61,365,86},0x0040ad30u,0x0051f780u,0,0,0,1,1,0},
    /* SALARY 0x005C5188 */
    {{82,154,382,174},0x0040aa50u,0x0051f780u,0,0,0,1,1,0},
};

const PharaohMenuRect pharaoh_info_program_rectangles[68]={
    {0,0,240,20},
    {0,0,272,20},
    {0,0,272,20},
    {0,0,272,20},
    {0,0,304,20},
    {0,20,304,40},
    {0,0,165,20},
    {0,20,165,40},
    {0,40,165,60},
    {0,60,165,80},
    {0,80,165,100},
    {0,100,165,120},
    {0,120,165,140},
    {0,140,165,160},
    {0,160,165,180},
    {0,180,165,200},
    {0,200,165,220},
    {0,220,165,240},
    {0,240,165,260},
    {0,260,165,280},
    {0,280,165,300},
    {0,300,165,320},
    {0,320,165,340},
    {0,340,165,360},
    {0,360,165,380},
    {0,380,165,400},
    {0,400,165,420},
    {0,420,165,440},
    {0,440,165,460},
    {0,460,165,480},
    {0,480,165,500},
    {0,500,165,520},
    {0,520,165,540},
    {0,540,165,560},
    {0,560,165,580},
    {0,580,165,600},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0},
    {26,46,76,96},
    {86,46,136,96},
    {146,46,196,96},
    {206,46,256,96},
    {266,46,316,96},
    {326,46,376,96},
    {386,46,436,96},
    {0,0,150,25},
    {19,139,103,223},
    {104,139,188,223},
    {189,139,273,223},
    {274,139,358,223},
    {359,139,443,223},
    {0,0,320,30},
    {19,139,103,223},
    {104,139,188,223},
    {189,139,273,223},
    {274,139,358,223},
    {359,139,443,223},
    {19,139,103,223},
    {104,139,188,223},
    {189,139,273,223},
    {274,139,358,223},
    {359,139,443,223},
    {40,61,365,86},
    {82,154,382,174},
};

const PharaohInfoProgramGenericRow pharaoh_info_program_generic_rows[15]={
    {0x005c5a78u,1,0,&pharaoh_info_program_native_generic[0],&pharaoh_info_program_rectangles[0]},
    {0x005c5a98u,1,1,&pharaoh_info_program_native_generic[1],&pharaoh_info_program_rectangles[1]},
    {0x005c5ab8u,1,2,&pharaoh_info_program_native_generic[2],&pharaoh_info_program_rectangles[2]},
    {0x005c5ad8u,1,3,&pharaoh_info_program_native_generic[3],&pharaoh_info_program_rectangles[3]},
    {0x005c5ba8u,1,4,&pharaoh_info_program_native_generic[4],&pharaoh_info_program_rectangles[4]},
    {0x005c5bc8u,1,5,&pharaoh_info_program_native_generic[5],&pharaoh_info_program_rectangles[5]},
    {0x005c68c8u,36,6,&pharaoh_info_program_native_generic[6],&pharaoh_info_program_rectangles[6]},
    {0x005c5be8u,7,42,&pharaoh_info_program_native_generic[42],&pharaoh_info_program_rectangles[42]},
    {0x005c5cb0u,1,49,&pharaoh_info_program_native_generic[49],&pharaoh_info_program_rectangles[49]},
    {0x005c66d8u,5,50,&pharaoh_info_program_native_generic[50],&pharaoh_info_program_rectangles[50]},
    {0x005c6768u,1,55,&pharaoh_info_program_native_generic[55],&pharaoh_info_program_rectangles[55]},
    {0x005c6788u,5,56,&pharaoh_info_program_native_generic[56],&pharaoh_info_program_rectangles[56]},
    {0x005c6818u,5,61,&pharaoh_info_program_native_generic[61],&pharaoh_info_program_rectangles[61]},
    {0x005c5448u,1,66,&pharaoh_info_program_native_generic[66],&pharaoh_info_program_rectangles[66]},
    {0x005c5188u,1,67,&pharaoh_info_program_native_generic[67],&pharaoh_info_program_rectangles[67]},
};

static const PharaohImageButtonRecord initial_help_close[2]={
    {14,0,27,27,4,403,0,{0,0,80,28,81,0,128,247,81,0},1,0,0,0,{0,0,0,0,0,0,0,0}},
    {424,3,24,24,4,403,4,{0,0,192,113,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,0,0,0,0}},
};

static const PharaohImageButtonRecord initial_limit_buttons[60]={
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,0,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,0,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,1,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,1,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,2,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,2,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,3,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,3,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,4,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,4,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,5,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,5,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,6,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,6,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,7,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,7,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,8,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,8,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,9,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,9,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,10,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,10,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,11,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,11,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,12,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,12,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,13,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,13,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,14,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,14,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,15,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,15,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,16,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,16,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,17,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,17,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,18,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,18,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,19,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,19,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,20,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,20,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,21,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,21,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,22,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,22,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,23,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,23,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,24,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,24,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,25,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,25,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,26,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,26,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,27,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,27,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,28,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,28,0,0,0}},
    {0,0,17,17,4,285,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,29,0,0,0}},
    {17,0,17,17,4,481,0,{0,0,96,100,64,0,128,247,81,0},1,0,0,0,{0,0,0,0,29,0,0,0}},
};

static const PharaohImageButtonRecord initial_tutorials[15]={
    {0,0,18,27,4,403,0,{0,0,96,35,78,0,128,247,81,0},1,0,0,0,{1,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,0,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{1,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,3,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{2,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,3,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{3,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,6,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{4,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,9,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{5,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,12,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{6,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,15,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{7,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,18,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{8,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,21,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{9,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,24,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{10,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,27,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{11,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,30,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{12,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,33,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{13,0,0,0,0,0,0,0}},
    {0,0,27,27,4,375,36,{0,0,224,160,64,0,128,247,81,0},1,0,0,0,{14,0,0,0,0,0,0,0}},
};

const PharaohInfoProgramImageSeed pharaoh_info_program_image_seeds[17]={
    {0x005c7b10u,2,4,initial_help_close},
    {0x005c81f8u,60,0,initial_limit_buttons},
    {0x005c7e48u,1,27,&initial_tutorials[0]},
    {0x005c7ec0u,1,27,&initial_tutorials[1]},
    {0x005c7ee8u,1,27,&initial_tutorials[2]},
    {0x005c7f10u,1,27,&initial_tutorials[3]},
    {0x005c7f38u,1,27,&initial_tutorials[4]},
    {0x005c7f60u,1,27,&initial_tutorials[5]},
    {0x005c7f88u,1,27,&initial_tutorials[6]},
    {0x005c7fb0u,1,27,&initial_tutorials[7]},
    {0x005c7fd8u,1,27,&initial_tutorials[8]},
    {0x005c8000u,1,27,&initial_tutorials[9]},
    {0x005c8028u,1,27,&initial_tutorials[10]},
    {0x005c8050u,1,27,&initial_tutorials[11]},
    {0x005c8078u,1,27,&initial_tutorials[12]},
    {0x005c80a0u,1,27,&initial_tutorials[13]},
    {0x005c80c8u,1,24,&initial_tutorials[14]},
};

const PharaohInfoProgramSquareSeed pharaoh_info_program_square_seeds[2]={
    {0x005c5b48u,{0,0,18,24,0x0040c6b0u,0,0,0,4,0x00000001u,0x00000001u}},
    {0x005c5b60u,{24,0,16,24,0x0040c6b0u,0,0,0,4,0x00000000u,0x00000001u}},
};

const uint32_t pharaoh_info_program_tutorial_addresses[15]={
    0x005c7e48u,0x005c7ec0u,0x005c7ee8u,0x005c7f10u,0x005c7f38u,0x005c7f60u,0x005c7f88u,0x005c7fb0u,0x005c7fd8u,0x005c8000u,0x005c8028u,0x005c8050u,0x005c8078u,0x005c80a0u,0x005c80c8u
};

const uint32_t pharaoh_info_program_action_tokens[23]={
    0x004062d0u,0x004062f0u,0x00406300u,0x00406460u,0x00406620u,0x00406680u,0x00406700u,0x00406720u,0x00406b40u,0x00406c90u,0x00406de0u,0x00406e80u,0x00406fb0u,0x00407110u,0x004071c0u,0x0040a0e0u,0x0040aa50u,0x0040ad30u,0x0040c6b0u,0x004e2360u,0x00511c50u,0x0051f780u,0x0053d730u
};

static const uint32_t PROGRAM_INITIALIZED=0x494e464fu;

static PharaohInfoProgramResult program_result(enum PharaohInfoProgramSupport support,
    uint32_t token,const char *field)
{ const PharaohInfoProgramResult r={support,token,field};return r; }

static int32_t action_index(uint32_t token)
{
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_ACTION_COUNT;++i)
        if(pharaoh_info_program_action_tokens[i]==token)return i;
    return -1;
}

PharaohGenericButtonAction pharaoh_building_info_program_data_action(
    const PharaohBuildingInfoProgramData *d,uint32_t token)
{
    if(!d||d->initialized!=PROGRAM_INITIALIZED)return NULL;
    const int32_t index=action_index(token);
    return index<0?NULL:d->actions[index];
}

PharaohInfoProgramResult pharaoh_building_info_program_data_support(
    const PharaohBuildingInfoProgramData *d)
{
    if(!d)return program_result(PHARAOH_INFO_PROGRAM_MISSING_CONTEXT,0,"program data");
    if(d->initialized==0)return program_result(PHARAOH_INFO_PROGRAM_NOT_INITIALIZED,0,"initialize_once");
    if(d->initialized!=PROGRAM_INITIALIZED)
        return program_result(PHARAOH_INFO_PROGRAM_STATE_MISMATCH,0,"program data initialization marker");
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_ACTION_COUNT;++i)
        if(!d->actions[i])return program_result(PHARAOH_INFO_PROGRAM_MISSING_ACTION,
            pharaoh_info_program_action_tokens[i],"resolved genuine C action");
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_GENERIC_COUNT;++i) {
        const PharaohInfoNativeGenericButton *n=&pharaoh_info_program_native_generic[i];
        const PharaohGenericButton *b=&d->generic[i];
        if(b->left_action!=pharaoh_building_info_program_data_action(d,n->left_token))
            return program_result(PHARAOH_INFO_PROGRAM_STATE_MISMATCH,n->left_token,"generic left action");
        if(b->right_action!=pharaoh_building_info_program_data_action(d,n->right_token))
            return program_result(PHARAOH_INFO_PROGRAM_STATE_MISMATCH,n->right_token,"generic right action");
        /* The six native policy padding records have no callable trigger.
         * Reject later activation of a missing callback before entering input. */
        if((b->trigger==PHARAOH_GENERIC_BUTTON_TRIGGER_PRESS ||
            b->trigger==PHARAOH_GENERIC_BUTTON_TRIGGER_HOLD ||
            b->trigger==PHARAOH_GENERIC_BUTTON_TRIGGER_RELEASE) &&
            (!b->left_action||!b->right_action))
            return program_result(PHARAOH_INFO_PROGRAM_MISSING_ACTION,0,"generic callable trigger without action");
        if(b->trigger==PHARAOH_GENERIC_BUTTON_TRIGGER_LEFT_PRESS&&!b->left_action)
            return program_result(PHARAOH_INFO_PROGRAM_MISSING_ACTION,0,"generic left trigger without action");
        if(memcmp(&b->bounds,&pharaoh_info_program_rectangles[i],sizeof(b->bounds))!=0)
            return program_result(PHARAOH_INFO_PROGRAM_STATE_MISMATCH,0,"generic bounds / dense focus rectangle");
    }
    return program_result(PHARAOH_INFO_PROGRAM_READY,0,NULL);
}

PharaohInfoProgramResult pharaoh_building_info_program_data_initialize_once(
    PharaohBuildingInfoProgramData *d,PharaohInfoProgramActionResolver resolve,void *user_data)
{
    if(!d)return program_result(PHARAOH_INFO_PROGRAM_MISSING_CONTEXT,0,"program data");
    if(d->initialized!=0)return pharaoh_building_info_program_data_support(d);
    if(!resolve)return program_result(PHARAOH_INFO_PROGRAM_MISSING_RESOLVER,0,"genuine C action resolver");
    PharaohGenericButtonAction actions[PHARAOH_INFO_PROGRAM_ACTION_COUNT];
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_ACTION_COUNT;++i) {
        actions[i]=resolve(user_data,pharaoh_info_program_action_tokens[i]);
        if(!actions[i])return program_result(PHARAOH_INFO_PROGRAM_MISSING_ACTION,
            pharaoh_info_program_action_tokens[i],"genuine C action resolver result");
    }
    /* Nothing in the mutable owner is initialized until all actions exist. */
    memcpy(d->actions,actions,sizeof(d->actions));
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_GENERIC_COUNT;++i) {
        const PharaohInfoNativeGenericButton *n=&pharaoh_info_program_native_generic[i];
        PharaohGenericButton *b=&d->generic[i];
        const int32_t left=action_index(n->left_token),right=action_index(n->right_token);
        b->bounds=n->bounds;
        b->left_action=left<0?NULL:actions[left];
        b->right_action=right<0?NULL:actions[right];
        b->activated=n->activated;b->trigger=(PharaohGenericButtonTrigger)n->trigger;
        b->primary_parameter=n->primary_parameter;b->secondary_parameter=n->secondary_parameter;
    }
    memcpy(d->help_close,initial_help_close,sizeof(d->help_close));
    memcpy(d->limits,initial_limit_buttons,sizeof(d->limits));
    memcpy(d->tutorials,initial_tutorials,sizeof(d->tutorials));
    for(int32_t i=0;i<PHARAOH_INFO_PROGRAM_TUTORIAL_COUNT;++i) {
        d->tutorial_rows[i].buttons=&d->tutorials[i];
        d->tutorial_rows[i].trailing_trigger=pharaoh_info_program_image_seeds[i+2].trailing_trigger;
    }
    d->scroll_down=pharaoh_info_program_square_seeds[0].record;
    d->scroll_up=pharaoh_info_program_square_seeds[1].record;
    memset(&d->borrowed,0,sizeof(d->borrowed));d->ui=NULL;
    d->initialized=PROGRAM_INITIALIZED;
    return pharaoh_building_info_program_data_support(d);
}

#define BORROW(x) do { if(!(x))return program_result(PHARAOH_INFO_PROGRAM_MISSING_BORROWED_DATA,0,#x); } while(0)

static PharaohInfoProgramResult borrowed_support(const PharaohInfoProgramBorrowed *p)
{
    if(!p)return program_result(PHARAOH_INFO_PROGRAM_MISSING_CONTEXT,0,"borrowed live program/host state");
    BORROW(p->display_mode);BORROW(p->focus_primary);BORROW(p->focus_secondary);
    BORROW(p->focus_building);BORROW(p->button_parameter);BORROW(p->button_secondary_parameter);
    BORROW(p->text_width);BORROW(p->resource_images);BORROW(p->panel_button_images);
    BORROW(p->font_black);BORROW(p->font_green);BORROW(p->font_white);
    BORROW(p->font_red);BORROW(p->font_title);BORROW(p->tutorial_flags);
    BORROW(p->phrase_counts);BORROW(p->sound_channels);BORROW(p->sound_volume);
    BORROW(p->sound_enabled);BORROW(p->square_input);BORROW(p->square_input->clock);
    BORROW(p->square_input->dispatch);BORROW(p->square_input->repeat_schedule);
    BORROW(p->image_input);BORROW(p->image_input->dispatch);BORROW(p->image_focus);
    BORROW(p->generic_input);BORROW(p->generic_focus);BORROW(p->button_layout);
    BORROW(p->button_layout_operations);
    BORROW(p->button_layout_operations->set_clip_rectangle);
    BORROW(p->button_layout_operations->highlight_actor_panel);
    BORROW(p->button_layout_operations->draw_highlight_rectangle);
    return program_result(PHARAOH_INFO_PROGRAM_READY,0,NULL);
}

PharaohInfoProgramResult pharaoh_building_info_program_data_bind(
    PharaohBuildingInfoProgramData *d,PharaohBuildingInfoLiveContext *ui,
    const PharaohInfoProgramBorrowed *p)
{
    PharaohInfoProgramResult r=pharaoh_building_info_program_data_support(d);
    if(r.support!=PHARAOH_INFO_PROGRAM_READY)return r;
    if(!ui)return program_result(PHARAOH_INFO_PROGRAM_MISSING_CONTEXT,0,"actual building info UI");
    r=borrowed_support(p);if(r.support!=PHARAOH_INFO_PROGRAM_READY)return r;
    for(int32_t i=0;i<PHARAOH_INFO_ROW_COUNT;++i) {
        ui->rows[i].buttons=&d->generic[pharaoh_info_program_generic_rows[i].first_record];
        ui->rows[i].rectangles=pharaoh_info_program_generic_rows[i].rectangles;
    }
    ui->help_close.buttons=d->help_close;
    ui->help_close.trailing_trigger=pharaoh_info_program_image_seeds[0].trailing_trigger;
    ui->limit_buttons=d->limits;
    ui->limit_trailing_trigger=pharaoh_info_program_image_seeds[1].trailing_trigger;
    ui->tutorial_buttons=d->tutorial_rows;
    ui->scroll_down=&d->scroll_down;ui->scroll_up=&d->scroll_up;
    ui->display_mode=p->display_mode;ui->focus_primary=p->focus_primary;
    ui->focus_secondary=p->focus_secondary;ui->focus_building=p->focus_building;
    ui->button_parameter=p->button_parameter;ui->text_width=p->text_width;
    ui->resource_images=p->resource_images;ui->panel_button_images=p->panel_button_images;
    ui->font_black=p->font_black;ui->font_green=p->font_green;
    ui->font_white=p->font_white;ui->font_red=p->font_red;ui->font_title=p->font_title;
    ui->tutorial_flags=p->tutorial_flags;ui->phrase_counts=p->phrase_counts;
    ui->sound_channels=p->sound_channels;ui->sound_volume=p->sound_volume;
    ui->sound_enabled=p->sound_enabled;ui->square_input=p->square_input;
    ui->image_input=p->image_input;ui->image_focus=p->image_focus;
    ui->generic_input=p->generic_input;ui->generic_focus=p->generic_focus;
    ui->button_layout=p->button_layout;ui->button_layout_operations=p->button_layout_operations;
    d->borrowed=*p;d->ui=ui;
    return program_result(PHARAOH_INFO_PROGRAM_READY,0,NULL);
}

#undef BORROW
