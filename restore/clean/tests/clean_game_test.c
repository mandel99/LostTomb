#include "pharaoh/core/record_geometry.h"
#include "pharaoh/runtime/frame_gate.h"
#include "pharaoh/ui/view_invalidation.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdint.h>

typedef struct FrameProbe {
    bool front_lost;
    bool back_lost;
    bool restore_ok;
    bool quit;
    unsigned int front_checks;
    unsigned int back_checks;
    unsigned int restore_calls;
    unsigned int pause_calls;
    unsigned int quit_checks;
    unsigned int dispatch_calls;
} FrameProbe;

static bool probe_surface_lost(void *context, PharaohDisplaySurface surface)
{
    FrameProbe *probe = context;
    if (surface == PHARAOH_DISPLAY_SURFACE_FRONT) {
        ++probe->front_checks;
        return probe->front_lost;
    }
    ++probe->back_checks;
    return probe->back_lost;
}

static bool probe_restore(void *context)
{
    FrameProbe *probe = context;
    ++probe->restore_calls;
    return probe->restore_ok;
}

static void probe_pause(void *context)
{
    FrameProbe *probe = context;
    ++probe->pause_calls;
}

static bool probe_quit(void *context)
{
    FrameProbe *probe = context;
    ++probe->quit_checks;
    return probe->quit;
}

static void probe_dispatch(void *context)
{
    FrameProbe *probe = context;
    ++probe->dispatch_calls;
}

static PharaohFrameGateHooks probe_hooks(FrameProbe *probe)
{
    PharaohFrameGateHooks hooks;
    hooks.context = probe;
    hooks.surface_is_lost = probe_surface_lost;
    hooks.restore_surfaces = probe_restore;
    hooks.pause_game = probe_pause;
    hooks.quit_requested = probe_quit;
    hooks.dispatch_frame = probe_dispatch;
    return hooks;
}

static void test_projection_copy(void)
{
    PharaohProjectionRecord source = {
        1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u
    };
    PharaohProjectionRecord destination = {0};

    pharaoh_projection_record_copy(&destination, &source);
    assert(destination.map_x == 1u);
    assert(destination.preserved_word_3 == 4u);
    assert(destination.reverse_sum_step == 9u);
    assert(destination.difference_scale == 13u);
}

static void test_projection_update(void)
{
    const PharaohProjectionSettings settings = {4u, 2u};
    PharaohProjectionRecord records[2] = {
        {.map_x = 10u, .map_y = 3u},
        {.map_x = 1u, .map_y = 2u}
    };

    pharaoh_projection_records_update(records, 2, &settings);
    assert(records[0].map_tile_offset == 694u);
    assert(records[0].difference_step == 21u);
    assert(records[0].sum_step == 39u);
    assert(records[0].reverse_difference_step == 0u - 21u);
    assert(records[0].reverse_sum_step == 0u - 39u);
    assert(records[0].sum_scale == 52u);
    assert(records[0].reverse_difference_scale == 0u - 28u);
    assert(records[0].reverse_sum_scale == 0u - 52u);
    assert(records[0].difference_scale == 28u);
    assert(records[1].map_tile_offset == 457u);

    records[0].map_tile_offset = 123u;
    pharaoh_projection_records_update(records, -1, &settings);
    assert(records[0].map_tile_offset == 123u);
}

static void test_frame_gate(void)
{
    unsigned int mask;

    /* Exhaust the six independent binary inputs of the reconstructed gate. */
    for (mask = 0u; mask < 64u; ++mask) {
        const bool have_front = (mask & 1u) != 0u;
        const bool have_back = (mask & 2u) != 0u;
        const bool front_lost = (mask & 4u) != 0u;
        const bool back_lost = (mask & 8u) != 0u;
        const bool restore_ok = (mask & 16u) != 0u;
        const bool quit = (mask & 32u) != 0u;
        const bool observed_front_loss = have_front && front_lost;
        const bool check_back = !observed_front_loss && have_back;
        const bool surface_lost = observed_front_loss || (check_back && back_lost);
        const bool restore_failed = surface_lost && !restore_ok;
        const bool expected_result = !restore_failed && !quit;
        const PharaohFrameGateState state = {have_front, have_back};
        FrameProbe probe = {0};
        PharaohFrameGateHooks hooks;

        probe.front_lost = front_lost;
        probe.back_lost = back_lost;
        probe.restore_ok = restore_ok;
        probe.quit = quit;
        hooks = probe_hooks(&probe);

        assert(pharaoh_frame_gate_run(&state, &hooks) == expected_result);
        assert(probe.front_checks == (have_front ? 1u : 0u));
        assert(probe.back_checks == (check_back ? 1u : 0u));
        assert(probe.restore_calls == (surface_lost ? 1u : 0u));
        assert(probe.pause_calls == (restore_failed ? 1u : 0u));
        assert(probe.quit_checks == (restore_failed ? 0u : 1u));
        assert(probe.dispatch_calls == (expected_result ? 1u : 0u));
    }
}

static void test_view_invalidation(void)
{
    PharaohViewState state = {false, PHARAOH_REDRAW_NONE};
    pharaoh_view_invalidate_if_editing(&state);
    assert(state.redraw_request == PHARAOH_REDRAW_NONE);

    state.redraw_request = (PharaohRedrawRequest)7;
    pharaoh_view_invalidate_if_editing(&state);
    assert(state.redraw_request == (PharaohRedrawRequest)7);

    state.layout_edit_active = true;
    pharaoh_view_invalidate_if_editing(&state);
    assert(state.redraw_request == PHARAOH_REDRAW_ALL);
}

int main(void)
{
    test_projection_copy();
    test_projection_update();
    test_frame_gate();
    test_view_invalidation();
    return 0;
}
