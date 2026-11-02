"""Emit test-only aliases; production algorithms are handwritten staged C."""
from pathlib import Path
P=Path(__file__).resolve().parent;D=P/'tests';D.mkdir(exist_ok=True)
callbacks=['00402c10','00402c40','00402cc0','00402f70','00403060','00403130','004031a0','00403210','00403270','004032f0','00403390','004ea760']
for variant in ['trace','plain']:
 out=['/* Test-only source aliases; includes exact candidate or applied C. */']
 if variant=='trace':out+=['#define SETTINGS_CALLBACKS_TRACE']
 out+=['#include "settings_callbacks.h"']
 def aliases(names):out.extend('#define FUN_'+x+' settings_'+variant+'_'+x for x in names)
 def undo(names):out.extend('#undef FUN_'+x for x in names)
 def include(staged,production):out.extend(['#ifdef SETTINGS_CALLBACKS_STAGED','#include "../'+staged+'"','#else','#include "../../../'+production+'"','#endif'])
 aliases(callbacks);include('runtime/settings_callbacks.c','runtime/settings_callbacks.c');undo(callbacks)
 for name,file in [('00403320','unit_00027.c'),('00416ba0','unit_00191.c')]:
  aliases([name]);include('src/'+file,'src/'+file);undo([name])
 aliases(['00402e50']);out+=['#define SETTINGS_CALLBACKS_DIFFICULTY_ONLY'];include('runtime/mission_menu.c','runtime/mission_menu.c');out+=['#undef SETTINGS_CALLBACKS_DIFFICULTY_ONLY'];undo(['00402e50'])
 for address in ['55f32f','55f3eb']:out+=['#define restore_settings_sound_'+address+' settings_'+variant+'_00'+address]
 include('runtime/settings_callbacks_audio.c','runtime/settings_callbacks_audio.c')
 for address in ['55f32f','55f3eb']:out+=['#undef restore_settings_sound_'+address]
 aliases(['0055f38d']);out+=['#include "../../../src/unit_02705.c"'];undo(['0055f38d'])
 out+=['#define restore_sound_close_stream settings_'+variant+'_close_stream']
 aliases(['0055f561']);out+=['#include "../../../runtime/sound_streams.c"','#include "../../../src/unit_02708.c"'];undo(['0055f561']);out+=['#undef restore_sound_close_stream']
 aliases(['0055f639']);out+=['#include "../../../src/unit_02711.c"'];undo(['0055f639'])
 aliases(['0051f780']);out+=['#include "../../../src/unit_02351.c"'];undo(['0051f780'])
 (D/('settings_actual_'+variant+'.c')).write_text('\n'.join(out)+'\n')
