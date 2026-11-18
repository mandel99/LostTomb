#ifndef PHARAOH_RUNTIME_LOADING_TEXT_H
#define PHARAOH_RUNTIME_LOADING_TEXT_H

#include "pharaoh/rendering/primitives.h"
#include "pharaoh/runtime/graphics_startup.h"

void pharaoh_loading_text_draw(const char *text,
    const PharaohGraphicsStartupState *graphics,
    PharaohGraphicsPrimitives *primitives);

#endif
