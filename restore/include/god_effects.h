#ifndef RESTORE_GOD_EFFECTS_H
#define RESTORE_GOD_EFFECTS_H
#include <stdint.h>
/* Native 4C47A0: three cdecl DWORDs, RET0, no incoming ECX. Source is unused.
   Native message substitution initializes only WORDs 0,1,8 of this scratch;
   the publisher copies all thirty bytes, including its unspecified prestate. */
void restore_god_effect_core_004c47a0(int32_t god,uint32_t source,int32_t effect,uint8_t scratch[30]);
void restore_god_effect_004c47a0(int32_t god,uint32_t source,int32_t effect);
void FUN_004c47a0(int32_t god,uint32_t source,int32_t effect);
void restore_god_city_004e6eb0(void);
void restore_god_city_004e6ed0(void);
void restore_god_city_004e70b0(int32_t duration);
void restore_god_city_004ea000(void);
void restore_god_city_004b4710(void);
void restore_god_city_004e8e80(int32_t amount);
void restore_god_settings_004bd5e0(uint32_t self,int32_t amount);
void restore_god_city_00461950(int32_t amount);
void restore_god_actor_patrol_004b1df0(int32_t actor);
#endif
