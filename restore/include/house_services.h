#ifndef RESTORE_HOUSE_SERVICES_H
#define RESTORE_HOUSE_SERVICES_H
#include <stdint.h>
/* Internal shared rectangle traversal. Publication bit0 writes the advanced
   tile before cells; bit1 writes it after each row, otherwise after all rows. */
typedef uint32_t (*RestoreHouseCellEffect)(int32_t building,uint32_t value);
int32_t restore_house_service_scan(int32_t x,int32_t y,uint32_t radius,
    RestoreHouseCellEffect effect,uint32_t value,uint32_t publication);
int32_t FUN_00484990(int32_t x,int32_t y);
int32_t FUN_00483e90(int32_t x,int32_t y);
int32_t FUN_00483fb0(int32_t x,int32_t y,int32_t deity);
uint32_t FUN_00484890(int32_t x,int32_t y);
uint32_t FUN_00483160(int32_t x,int32_t y);
void FUN_00483260(int32_t x,int32_t y,int32_t amount);
void FUN_00484670(int32_t x,int32_t y);
#endif
