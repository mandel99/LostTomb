#ifndef PHARAOH_RUNTIME_APPLICATION_GAME_SHUTDOWN_H
#define PHARAOH_RUNTIME_APPLICATION_GAME_SHUTDOWN_H

#include "pharaoh/assets/sprite_auxiliary.h"
#include "pharaoh/assets/sprite_libraries.h"

#include <stdint.h>

typedef struct PharaohShutdownOwnedInterface PharaohShutdownOwnedInterface;

typedef struct PharaohApplicationGameShutdownOperations {
    void *user_data;
    void (*dispose_interface)(void *user_data,
        PharaohShutdownOwnedInterface *interface_object, int32_t flags);
} PharaohApplicationGameShutdownOperations;

typedef struct PharaohApplicationGameShutdownContext {
    PharaohShutdownOwnedInterface **first_interface_slot;
    PharaohShutdownOwnedInterface **second_interface_slot;
    const PharaohApplicationGameShutdownOperations *interface_operations;
    PharaohSpriteAuxiliary **sprite_auxiliary_slot;
    const PharaohSpriteAuxiliaryReleaseOperations *auxiliary_operations;
    PharaohSpriteLibraryResetState *sprite_libraries;
    const PharaohSpritePixelLoadContext *sprite_pixel_context;
} PharaohApplicationGameShutdownContext;

/* Typed translation of 0x004E63B0. The two native interface types are
   unidentified; only their first vtable method and its flag are established. */
void pharaoh_application_game_shutdown(
    const PharaohApplicationGameShutdownContext *context);

#endif
