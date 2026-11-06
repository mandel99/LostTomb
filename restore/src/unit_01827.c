#include "final_state_actions.h"
#include "mission_loaders.h"
#include "sound_streams.h"
#include "crt_lowio.h"
extern void FUN_0046fa40(void);
extern void FUN_004ce2d0(void);
extern int32_t FUN_00568eab(const char *path,uint32_t flags,...);
extern void FUN_004d41e0(int32_t descriptor);
extern uint32_t FUN_004cde40(uint32_t value,uint32_t limit);
extern void FUN_004d52a0(void);
extern void FUN_0046b320(void);
extern void FUN_0046f030(void);
extern void FUN_0046a320(void);
uint32_t restore_mission_load_file(const char *path)
{
 uint32_t saved=VIEW_U(0xe39080u),city,limit,value,scan,remaining,length,source,target,words,bytes;
 int32_t descriptor;
 FUN_004bd330(RESTORE_LOAD_SETTINGS);FUN_0046fa40();FUN_004ce2d0();
 VIEW_B(0xd3b1b8u)=0;
 descriptor=FUN_00568eab(path,0x8000);
 if (descriptor==-1) return 1;
 FUN_0055f561(RESTORE_SOUND,2);VIEW_U(0xf55b94u)=0;
 FUN_004d41e0(descriptor);(void)FUN_005647eb((uint32_t)descriptor);
 city=VIEW_U(0xeaac28u)*0x49d8u;limit=VIEW_U(0x5d1960u+saved*4u);
 VIEW_U(0xe39080u)=saved;value=VIEW_U(0xea187cu+city);
 value=FUN_004cde40(value,limit);
 city=VIEW_U(0xeaac28u)*0x49d8u;
 VIEW_B(0xd3b1b8u)=1;VIEW_U(0xea187cu+city)=value;
 FUN_004d52a0();FUN_0046b320();FUN_0046f030();FUN_0046a320();
 scan=0xe38e74u;remaining=0xffffffffu;
 while (remaining!=0) {uint8_t character=VIEW_B(scan);++scan;--remaining;if (character==0) break;}
 length=~remaining;source=scan-length;target=0xea184cu;
 for (words=length>>2;words!=0;--words) {VIEW_U(target)=VIEW_U(source);target+=4u;source+=4u;}
 for (bytes=length&3u;bytes!=0;--bytes) {VIEW_B(target)=VIEW_B(source);++target;++source;}
 return 0;
}
uint32_t FUN_004d51b0(const char *path){return restore_mission_load_file(path);}
