#ifndef RESTORE_GOD_BUILDING_EFFECTS_H
#define RESTORE_GOD_BUILDING_EFFECTS_H
#include <stdint.h>
struct restore_god_building_frame;
typedef void (*restore_god_building_call)(uint32_t,uint32_t,struct restore_god_building_frame *);
struct restore_god_building_frame {uint32_t r[8],flags;restore_god_building_call boundary;};
void restore_god_C_0043fd00(struct restore_god_building_frame *);
void restore_god_C_0045b2a0(struct restore_god_building_frame *);
void restore_god_C_0045b700(struct restore_god_building_frame *);
void restore_god_C_0045fe20(struct restore_god_building_frame *);
void restore_god_C_004695d0(struct restore_god_building_frame *);
void restore_god_C_0046d0f0(struct restore_god_building_frame *);
void restore_god_C_0046d100(struct restore_god_building_frame *);
void restore_god_C_004bfee0(struct restore_god_building_frame *);
void restore_god_C_004c04a0(struct restore_god_building_frame *);
void restore_god_C_004c3950(struct restore_god_building_frame *);
void restore_god_C_004c3ac0(struct restore_god_building_frame *);
void restore_god_C_004c3ed0(struct restore_god_building_frame *);
void restore_god_C_004c4020(struct restore_god_building_frame *);
void restore_god_C_004c41b0(struct restore_god_building_frame *);
void restore_god_C_004c42b0(struct restore_god_building_frame *);
void restore_god_C_004c45a0(struct restore_god_building_frame *);
void restore_god_C_004c45e0(struct restore_god_building_frame *);
void restore_god_C_004e9170(struct restore_god_building_frame *);
void restore_god_C_00562069(struct restore_god_building_frame *);
void restore_god_C_005621bd(struct restore_god_building_frame *);
void restore_god_C_0056220b(struct restore_god_building_frame *);
/* Private frame composition. Incidental volatile outputs of typed providers
   are not an API; new callers must audit their consumed registers. */
void restore_god_building_raw(uint32_t target,uint32_t registers[8],uint32_t *flags);
uint32_t restore_god_building_004c3950(void);
void restore_god_building_004c3ac0(void);
uint32_t restore_god_building_004c3ed0(void);
void restore_god_building_004c4020(int32_t mode);
uint32_t restore_god_building_004c41b0(void);
void restore_god_building_004c42b0(void);
void restore_god_building_004c45e0(void);
int32_t restore_god_building_0045b2a0(int32_t player,int32_t building,int32_t resource,int32_t amount);
void restore_god_building_0045b700(int32_t building,int32_t amount);
int32_t restore_god_building_0045fe20(int32_t building,int32_t amount,int32_t resource);
void restore_god_building_004695d0(int32_t building,int32_t destroy);
void restore_god_building_0046d100(int32_t building,uint8_t mode);
void restore_god_building_0043fd00(int32_t building,int32_t kind);
void restore_god_building_004c04a0(int32_t building);
uint32_t restore_god_building_0046d0f0(uint32_t building);
void restore_god_building_004e9170(int32_t player,int32_t people);
void restore_god_building_004bfee0(int32_t building);
#endif
