#ifndef PHARAOH_MONUMENTS_PYRAMID_IMAGES_H
#define PHARAOH_MONUMENTS_PYRAMID_IMAGES_H

#include "pharaoh/map/rotated_buildings.h"
#include "pharaoh/monuments/stages.h"

typedef struct PharaohPyramidImageShape { int32_t shape; uint32_t native_04; }
    PharaohPyramidImageShape;
typedef struct PharaohPyramidWorkerOffset { uint16_t x; int32_t y; }
    PharaohPyramidWorkerOffset;
typedef struct PharaohPyramidWorkerPoint { uint16_t x, y; }
    PharaohPyramidWorkerPoint;
typedef struct PharaohPyramidImageData {
    PharaohPyramidImageShape regular[5][144], mastaba[3][36];
    uint8_t roof_layers[30];
    PharaohPyramidWorkerOffset worker[9][6];
    PharaohPyramidWorkerPoint worker_style3[9][6], worker_base[9];
    uint32_t corners[4][4], quad[4];
} PharaohPyramidImageData;

typedef struct PharaohPyramidCoordinateTableStorage {
    PharaohMonumentCoordinateTables tables;
    int32_t style_one_heights[30];
    int32_t regular_offsets[9*6*2];
    int32_t style_three_offsets[9*6*4];
    int32_t main_part_offsets[9*2];
} PharaohPyramidCoordinateTableStorage;

extern const PharaohPyramidImageData pharaoh_pyramid_image_initial_data;
extern const PharaohPyramidSpecialStageTable pharaoh_pyramid_regular_special[5];
extern const PharaohPyramidSpecialStageTable pharaoh_pyramid_mastaba_special[3];
extern const int32_t pharaoh_pyramid_complex_temple_offsets[5];
void pharaoh_pyramid_coordinate_tables_init(
    PharaohPyramidCoordinateTableStorage *storage,
    const PharaohPyramidImageData *data);

typedef struct PharaohPyramidImageContext {
    PharaohMapSurfaceContext *surface;
    const PharaohPyramidImageData *data; /* initial data or live owned copy */
    const uint32_t *regular_counts, *mastaba_counts;
} PharaohPyramidImageContext;

void pharaoh_pyramid_update_piece_image(PharaohPyramidImageContext *context,
    int32_t building,int32_t rotation,int32_t kind); /* 004EDF50 */
void pharaoh_pyramid_update_finished_piece(PharaohPyramidImageContext *context,
    int32_t building,int32_t rotation,int32_t kind); /* 004F4160 */
void pharaoh_pyramid_refresh_part_images(PharaohPyramidImageContext *context,
    int32_t root); /* 004EE4A0 */
void pharaoh_rotated_bind_pyramid_image(PharaohRotatedBuildingOperations *ops,
    PharaohPyramidImageContext *context);

#endif
