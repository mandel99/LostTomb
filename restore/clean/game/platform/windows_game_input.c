#include "pharaoh/platform/windows_game_input.h"
#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/runtime/sound_initialization.h"
#include <mmsystem.h>
#include <share.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(PharaohSoundNameEntry)==sizeof(PharaohDigitalSoundFileEntry),
    "native sound filename entry stride");
_Static_assert(offsetof(PharaohSoundNameEntry,buffer)==
    offsetof(PharaohDigitalSoundFileEntry,name),"native shared filename pointer");

static PharaohSoundNameArray *input_allocate_names(void *data,int32_t count)
{
    (void)data;
    return calloc(1,sizeof(PharaohSoundNameArray)+
        (size_t)count*sizeof(PharaohSoundNameEntry));
}
static void input_free_names(void *data,PharaohSoundNameArray *array)
{ (void)data;free(array); }
static void input_free_string(void *data,void *buffer)
{ (void)data;free(buffer); }
static char *input_copy_string(void *data,const char *text)
{ (void)data;return _strdup(text); }
static void *input_open_sound(void *data,const char *path,const char *mode,uint32_t flags)
{
    (void)data;
    if(path[0]=='\0')return NULL; /* Native empty-path fopen failure. */
    return _fsopen(path,mode,(int)flags);
}
static void input_close_sound(void *data,void *file)
{ (void)data;(void)fclose(file); }

static const PharaohSoundFileMemoryOperations input_file_memory={
    NULL,input_allocate_names,input_free_names,input_free_string,input_copy_string};
static const PharaohSoundFileIdFileOperations input_file_operations={
    NULL,input_open_sound,input_close_sound};

/* Reuse the actual500-entry table and19 interface registrations. Other sound
 * categories remain with their respective owners; this is not5394F0 credit. */
static void input_bind_sound_files(PharaohWindowsGameInput *input,
    PharaohWindowsStartupHost *host)
{
    if(!input->files_initialized){
        const PharaohSoundFileIdLoadContext load={
            &input->files,&input_file_memory,&input_file_operations,0};
        pharaoh_sound_file_table_construct(&input->files,&input->table_bytes);
        (void)pharaoh_sound_file_table_resize(&input->files,
            PHARAOH_SOUND_INITIALIZATION_FILE_CAPACITY,&input_file_memory);
        const PharaohBuildingSoundFileContext buildings={input->building_sounds,&load};
        pharaoh_building_sound_files_initialize(&buildings);
        pharaoh_sound_effect_files_initialize(&input->effects,&load);
        input->files_initialized=1;
    }
    host->sound.state.sound_file_count=input->files.count;
    host->sound.state.sound_files=input->files.storage?
        (PharaohDigitalSoundFileEntry *)(void *)input->files.storage->entries:NULL;
}
static PharaohSoundEffectRequestState input_sound_state(
    PharaohWindowsGameInput *input,PharaohWindowsStartupHost *host)
{
    const PharaohSoundEffectRequestState state={host->sound.ready,
        (uint8_t)(host->settings.sound_effects_enabled!=0),
        (const int32_t *)(const void *)input->effects.values,
        host->settings.sound_effects_volume,&input->request};
    return state;
}
static void input_play_sound(void *data,const PharaohSoundEffectPlayRequest *request)
{
    PharaohWindowsStartupHost *const host=data;
    const PharaohDigitalSoundRequest actual={request->sample_match_tag,
        request->sound_file_id,request->volume,request->pan,request->loop_count};
    (void)pharaoh_digital_sound_play_sample(&host->sound.state,
        &host->sound.device_operations,&actual);
}
static void input_cursor_position(void *data,PharaohMousePoint *point)
{
    POINT actual;
    (void)data;
    /* Native4CF210 ignores GetCursorPos's return. Its valid-sample contract
     * is the same as the existing complete C owner. */
    (void)GetCursorPos(&actual);
    point->x=actual.x;point->y=actual.y;
}

void pharaoh_windows_game_input_message(PharaohWindowsGameInput *input,
    uint32_t message,intptr_t parameter)
{
    if(message!=WM_MOUSEMOVE&&message!=WM_LBUTTONDOWN&&message!=WM_LBUTTONUP&&
        message!=WM_RBUTTONDOWN&&message!=WM_RBUTTONUP)return;
    if(message==WM_LBUTTONDOWN){input->left_message=1;input->mouse.raw_button_bits|=1;}
    if(message==WM_LBUTTONUP)input->mouse.raw_button_bits&=UINT8_C(0xfe);
    if(message==WM_RBUTTONDOWN)input->mouse.raw_button_bits|=2;
    if(message==WM_RBUTTONUP)input->mouse.raw_button_bits&=UINT8_C(0xfd);
    input->mouse.cursor_sample_x=(int16_t)(uint16_t)(uint32_t)parameter;
    input->mouse.cursor_sample_y=(int16_t)(uint16_t)((uint32_t)parameter>>16);
}

PharaohMouseUpdateStatus pharaoh_windows_game_input_update(
    PharaohWindowsGameInput *input,PharaohWindowsStartupHost *host)
{
    input_bind_sound_files(input,host);
    PharaohMouseUpdateState *const mouse=&input->mouse;
    mouse->cursor_sample_enabled=host->city_scroll_requested; /*E38DCC*/
    mouse->cursor_scale_x=host->graphics.render_width; /*E38D9C*/
    mouse->cursor_scale_y=host->graphics.render_height; /*E38D98*/
    mouse->viewport_left=host->video_screen_bounds.left;
    mouse->viewport_top=host->video_screen_bounds.top;
    mouse->viewport_right=host->video_screen_bounds.right;
    mouse->viewport_bottom=host->video_screen_bounds.bottom;
    mouse->current_time=timeGetTime();
    PharaohSoundEffectRequestState sound=input_sound_state(input,host);
    const PharaohSoundEffectRequestOperations sound_ops={host,input_play_sound};
    const PharaohMouseUpdateOperations mouse_ops={NULL,input_cursor_position};
    return pharaoh_mouse_update(mouse,&mouse_ops,&sound,&sound_ops);
}

void pharaoh_windows_game_input_effect(PharaohWindowsGameInput *input,
    PharaohWindowsStartupHost *host,int32_t slot)
{
    input_bind_sound_files(input,host);
    PharaohSoundEffectRequestState sound=input_sound_state(input,host);
    pharaoh_windows_sound_effect(&host->sound,&sound,slot);
}
void pharaoh_windows_game_input_bind_sound(PharaohWindowsGameInput *input,
    PharaohWindowsStartupHost *host,PharaohSoundEffectRequestState *state,
    PharaohDigitalSoundDeviceContext *player)
{
    input_bind_sound_files(input,host);
    *state=input_sound_state(input,host);
    *player=(PharaohDigitalSoundDeviceContext){&host->sound.state,&host->sound.device_operations};
}
void pharaoh_windows_game_input_close(PharaohWindowsGameInput *input,
    PharaohWindowsStartupHost *host)
{
    if(input->files_initialized){
        host->sound.state.sound_files=NULL;host->sound.state.sound_file_count=0;
        pharaoh_sound_file_table_destroy(&input->files,&input->table_bytes,&input_file_memory);
        input->files_initialized=0;
    }
    host->game_input=NULL;
}
