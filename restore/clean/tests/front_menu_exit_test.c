#include "pharaoh/ui/front_menu_action.h"

#include <stdio.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d\n", __LINE__); \
    return 1; } } while (0)

typedef struct TestContext { int32_t result, step, failed; } TestContext;
#define EXPECT(context, x) do { if (!(x)) (context)->failed = 1; } while (0)
static void stop(void *data)
{ TestContext *const c = data; EXPECT(c, ++c->step == 1); }
static const char *body(void *data, int32_t group, int32_t entry)
{ TestContext *const c = data; EXPECT(c, ++c->step == 2); EXPECT(c, group == 5 && entry == 0); return "body"; }
static const char *title(void *data, int32_t group, int32_t entry)
{ TestContext *const c = data; EXPECT(c, ++c->step == 3); EXPECT(c, group == 5 && entry == 1); return "title"; }
static int32_t show(void *data, const char *title_text,
    const char *body_text, uint32_t type)
{
    TestContext *const context = data;
    EXPECT(context, ++context->step == 4); EXPECT(context, type == 4);
    EXPECT(context, title_text[0] == 't' && body_text[0] == 'b');
    return context->result;
}

int main(void)
{
    uint8_t mouse = 3, close = 0, redraw = 0;
    PharaohFrontMenuExitState state = {&mouse, &close, &redraw};
    TestContext context = {7, 0, 0};
    const PharaohFrontMenuExitOperations operations = {
        &context, stop, body, title, show
    };
    pharaoh_front_menu_confirm_exit(&state, &operations);
    CHECK(mouse == 0 && close == 0 && redraw == 0 && context.step == 4 &&
        context.failed == 0);
    mouse = 3; context.result = 6; context.step = 0; context.failed = 0;
    pharaoh_front_menu_confirm_exit(&state, &operations);
    CHECK(mouse == 0 && close == 1 && redraw == 1 && context.step == 4 &&
        context.failed == 0);
    return 0;
}
