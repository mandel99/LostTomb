#include "pharaoh/ui/city_input_action_bindings.h"
#include "pharaoh/runtime/platform_measure_speed.h"

typedef enum InputSource { INPUT_GENERIC,INPUT_IMAGE } InputSource;

static PharaohCityInputActionSupport support(
    const PharaohCityInputActionBindings *b,uint32_t token)
{
    if(!b||!b->providers.button_parameter||!b->providers.button_secondary_parameter)
        return PHARAOH_CITY_INPUT_ACTION_MISSING_CONTEXT;
#define NEED(x) return (x)?PHARAOH_CITY_INPUT_ACTION_READY:PHARAOH_CITY_INPUT_ACTION_MISSING_CONTEXT
    switch(token){
    case 0x00403d70u:NEED(b->providers.build);
    case 0x00406010u:NEED(b->providers.undo);
    case 0x00406020u:case 0x00406030u:NEED(b->providers.pending_messages);
    case 0x00407320u:NEED(b->providers.message_help&&b->providers.pending_messages);
    case 0x00409d10u:case 0x00409d40u:NEED(b->providers.sidebar);
    case 0x00409eb0u:NEED(b->providers.rotation);
    case 0x0040b520u:NEED(b->providers.advisor);
    case 0x0040b590u:NEED(b->providers.editor_route);
    case 0x00418390u:NEED(b->providers.timer&&b->providers.timer->start_timer);
    case 0x0051f780u:return PHARAOH_CITY_INPUT_ACTION_READY;
    default:return PHARAOH_CITY_INPUT_ACTION_UNKNOWN_TOKEN;
    }
#undef NEED
}

static void run(PharaohCityInputActionBindings *b,uint32_t token)
{
    PharaohCityInputActionProviders *p=&b->providers;
    switch(token){
    case 0x00403d70u:pharaoh_city_sidebar_build_click(p->build);break;
    case 0x00406010u:pharaoh_city_sidebar_undo(p->undo);break;
    case 0x00406020u:pharaoh_city_sidebar_messages(p->pending_messages);break;
    case 0x00406030u:pharaoh_city_sidebar_problem(p->pending_messages);break;
    case 0x00407320u:pharaoh_city_button_message_help(p->message_help,
        p->button_secondary_parameter,p->pending_messages->redraw);break;
    case 0x00409d10u:pharaoh_city_sidebar_collapse(p->sidebar);break;
    case 0x00409d40u:pharaoh_city_overlay_menu_open(p->sidebar);break;
    case 0x00409eb0u:pharaoh_city_rotate_command(p->rotation);break;
    case 0x0040b520u:(void)pharaoh_advisors_show_last(p->advisor);break;
    case 0x0040b590u:pharaoh_editor_city_return(p->editor_route);break;
    case 0x00418390u:pharaoh_small_button_start_timer(p->timer);break;
    case 0x0051f780u:pharaoh_actor_walking_no_operation();break;
    }
}

static void invoke(void *user,uint32_t token,InputSource source)
{
    PharaohCityInputActionBindings *b=user;
    if(!b)return;
    PharaohCityInputActionProviders *p=&b->providers;
    if(source==INPUT_IMAGE){
        *p->button_parameter=p->image_input->primary_parameter;
        *p->button_secondary_parameter=p->image_input->secondary_parameter;
    }else{
        *p->button_parameter=(uint32_t)p->generic_input->selected_primary_parameter;
        *p->button_secondary_parameter=(uint32_t)p->generic_input->selected_secondary_parameter;
    }
    b->last_token=token;b->last_support=support(b,token);
    if(b->last_support==PHARAOH_CITY_INPUT_ACTION_READY)run(b,token);
    p->image_input->primary_parameter=*p->button_parameter;
    p->image_input->secondary_parameter=*p->button_secondary_parameter;
    p->generic_input->selected_primary_parameter=(int32_t)*p->button_parameter;
    p->generic_input->selected_secondary_parameter=(int32_t)*p->button_secondary_parameter;
}

#define ACTION(name,token) static void name(void *b){invoke(b,token,INPUT_GENERIC);}
ACTION(a403d70,0x00403d70u) ACTION(a406010,0x00406010u)
ACTION(a406020,0x00406020u) ACTION(a406030,0x00406030u)
ACTION(a407320,0x00407320u) ACTION(a409d10,0x00409d10u)
ACTION(a409d40,0x00409d40u) ACTION(a409eb0,0x00409eb0u)
ACTION(a40b520,0x0040b520u) ACTION(a40b590,0x0040b590u)
ACTION(a418390,0x00418390u) ACTION(a51f780,0x0051f780u)
#undef ACTION

PharaohGenericButtonAction pharaoh_city_input_action_bindings_resolve_compiled(
    void *user,uint32_t token)
{
    (void)user;
    switch(token){
#define RESOLVE(t,n) case t:return n
    RESOLVE(0x00403d70u,a403d70);RESOLVE(0x00406010u,a406010);
    RESOLVE(0x00406020u,a406020);RESOLVE(0x00406030u,a406030);
    RESOLVE(0x00407320u,a407320);RESOLVE(0x00409d10u,a409d10);
    RESOLVE(0x00409d40u,a409d40);RESOLVE(0x00409eb0u,a409eb0);
    RESOLVE(0x0040b520u,a40b520);RESOLVE(0x0040b590u,a40b590);
    RESOLVE(0x00418390u,a418390);RESOLVE(0x0051f780u,a51f780);
#undef RESOLVE
    default:return NULL;
    }
}

void pharaoh_city_input_action_bindings_image_dispatch(void *user,uint32_t token)
{invoke(user,token,INPUT_IMAGE);}

PharaohCityInputActionSupport pharaoh_city_input_action_bindings_bind(
    PharaohCityInputActionBindings *b,const PharaohCityInputActionProviders *p)
{
    if(!b||!p||!p->image_input||!p->generic_input||!p->button_parameter||
       !p->button_secondary_parameter)return PHARAOH_CITY_INPUT_ACTION_MISSING_CONTEXT;
    b->providers=*p;b->last_support=PHARAOH_CITY_INPUT_ACTION_READY;b->last_token=0;
    p->image_input->user_data=b;
    p->image_input->dispatch=pharaoh_city_input_action_bindings_image_dispatch;
    p->generic_input->action_context=b;
    return PHARAOH_CITY_INPUT_ACTION_READY;
}
