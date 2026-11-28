#ifndef PHARAOH_UI_CITY_INPUT_PROGRAM_DATA_H
#define PHARAOH_UI_CITY_INPUT_PROGRAM_DATA_H

#include "pharaoh/ui/city_ingame_dispatch.h"

typedef PharaohGenericButtonAction (*PharaohCityInputActionResolver)(
    void *user_data,uint32_t token);

typedef enum PharaohCityInputProgramStatus {
    PHARAOH_CITY_INPUT_PROGRAM_READY,
    PHARAOH_CITY_INPUT_PROGRAM_MISSING_ACTION
} PharaohCityInputProgramStatus;

typedef struct PharaohCityInputProgramData {
    PharaohImageButtonRecord compass[1];
    PharaohImageButtonRecord expanded_extra[2];
    PharaohImageButtonRecord collapsed_expand[1];
    PharaohImageButtonRecord expanded_build[16];
    PharaohImageButtonRecord collapsed_build[12];
    PharaohImageButtonRecord expanded_top[2];
    PharaohGenericButton problem[1];
    int16_t expanded_extra_trailing,collapsed_expand_trailing;
    int16_t expanded_build_trailing,collapsed_build_trailing;
    int16_t expanded_top_trailing;
    uint8_t initialized;
} PharaohCityInputProgramData;

/* Exact mutable copies of Native5C7300..5C79EB and generic row5C6CB8.
 * Callback DWORDs remain dispatch tokens; only the generic row is converted
 * to typed callbacks through the supplied complete-action resolver. */
PharaohCityInputProgramStatus pharaoh_city_input_program_data_initialize_once(
    PharaohCityInputProgramData *,PharaohCityInputActionResolver,void *user_data);
void pharaoh_city_input_program_data_bind_tables(PharaohCityInputProgramData *,
    PharaohCityIngameButtonTables *);

#endif
