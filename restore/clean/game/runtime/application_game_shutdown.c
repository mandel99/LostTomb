#include "pharaoh/runtime/application_game_shutdown.h"

enum {
    APPLICATION_GAME_SHUTDOWN_INTERFACE_FLAGS = 1
};

static void dispose_owned_interface(
    PharaohShutdownOwnedInterface **interface_slot,
    const PharaohApplicationGameShutdownOperations *operations)
{
    PharaohShutdownOwnedInterface *const interface_object = *interface_slot;

    if (interface_object != 0) {
        operations->dispose_interface(operations->user_data,
            interface_object, APPLICATION_GAME_SHUTDOWN_INTERFACE_FLAGS);
        *interface_slot = 0;
    }
}

void pharaoh_application_game_shutdown(
    const PharaohApplicationGameShutdownContext *context)
{
    dispose_owned_interface(context->first_interface_slot,
        context->interface_operations);
    dispose_owned_interface(context->second_interface_slot,
        context->interface_operations);
    pharaoh_sprite_auxiliary_release_slot(context->sprite_auxiliary_slot,
        context->auxiliary_operations);
    pharaoh_sprite_libraries_reset(context->sprite_libraries,
        context->sprite_pixel_context);
}
