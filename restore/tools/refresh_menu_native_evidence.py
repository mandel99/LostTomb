"""Rerun only menu native comparisons whose source or compiled C evidence changed."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
import hashlib,json,subprocess,sys
ROOT=Path(__file__).resolve().parent.parent
names=['menu_input', 'file_list', 'menu_layout', 'menu_render', 'campaign_profile', 'family_menu', 'text_templates', 'family_render', 'menu_dialog', 'family_delete', 'text_wrap', 'family_options', 'campaign_menu', 'mission_menu', 'town_menu', 'compression', 'chunk_io', 'mission_chunks', 'mission_routes', 'mission_loaders', 'mission_map', 'numeric_text', 'legacy_mission', 'mission_save', 'load_save_menu', 'city_prepare', 'terrain_update', 'terrain_redraw', 'city_prices', 'music_tick', 'music_manager', 'flood_simulation', 'actor_update', 'actor_prey', 'animation_update', 'minimap', 'graphics_53f250', 'actor_predicate', 'high_scores_menu', 'viewport_controller', 'city_ui', 'message_panel', 'terrain_diamond', 'world_render', 'event_publisher', 'upper_raster', 'rich_message_text', 'message_panel_input', 'ground_render', 'ambient_sound', 'lookup_sentinel', 'camera_bounds', 'city_ui_layout', 'actor_render', 'city_top_chrome', 'mirrored_raster', 'preview_footprints', 'mirrored_clip', 'placement_preview', 'sprite_shutdown', 'scalar_binding_batch_001', 'actor_combat', 'camera_pan', 'dropdown_menu', 'dropdown_tiler', 'map_cells', 'actor_496140', 'tool_selector_input', 'raw_tile_raster', 'actor_callbacks_chain', 'actor_human', 'actor_callbacks_small', 'region_validation', 'tool_selector', 'actor_coordinates', 'camera_navigation']
assert len(names)==len(set(names))
names.append('dropdown_actions')
names.append('dropdown_system_actions')
names.append('dropdown_start_actions')
names.extend(['settings_callbacks','remaining_menu_actions'])
names.append('remaining_menu_click')
names.append('selection_popup')
names.extend(['editor_validation','display_confirmation'])
names.append('replay_mission')
names.extend(['initial_menu_dispatch','menu_existing_actions'])
names.append('training_panel')
names.extend(['menu_keyboard_modal','menu_cursor','training_queries'])
names.append('refresh_menu_dispatch')
names.append('menu_window_cursor_consumer')
names.extend(['menu_text_glyph','actor_order_flood'])
names.append('actor_order_los')
assert len(names)==len(set(names))

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def refresh(name):
 report_path=ROOT/('metadata/'+name.replace('_','-')+'-emulation.json')
 target='music' if name=='music_manager' else name
 binary=ROOT/f'build/c-only-debug/Debug/restore_{target}_verify.exe'
 stale=True
 if report_path.exists():
  report=json.loads(report_path.read_text())
  stale=not report.get('passed') or report.get('compiled_C_sha256')!=sha(binary) or any(not (ROOT/source).is_file() or sha(ROOT/source)!=digest for source,digest in {**report.get('source_hashes',{}),**report.get('native_exports',{})}.items())
  if name=='refresh_menu_dispatch':
   stale=stale or not report.get('production') or not report.get('complete_case_set') or report.get('checked')!=11633 or report.get('comparisons')!=23266
   stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_refresh_menu_dispatch_verify.exe')
   stale=stale or report.get('native_instruction_coverage')!=[960,960] or report.get('controller_instruction_coverage')!=[917,917] or report.get('conditional_outcomes')!=[250,250]
   stale=stale or len(report.get('mutated_endpoint_phases',[]))!=192
   stale=stale or any(sha(ROOT/p)!=h for p,h in report.get('binary_hashes',{}).items())
  if name=='menu_text_glyph':
   stale=stale or not report.get('production') or not report.get('complete_case_set') or report.get('checked')!=3062 or report.get('comparisons')!=6124
   stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_menu_text_glyph_verify.exe')
   stale=stale or report.get('native_instruction_coverage')!=[367,367] or report.get('conditional_outcomes')!=[78,78] or len(report.get('mutated_fields_and_phases',[]))!=54
   stale=stale or any(sha(ROOT/p)!=h for p,h in report.get('binary_hashes',{}).items())
  if name in ['map_cells','scalar_binding_batch_001','dropdown_actions','dropdown_system_actions','dropdown_start_actions']:
   for variant in report.get('variants',{}).values():
    stale=stale or sha(ROOT/variant['path'])!=variant['sha256']
  if name=='raw_tile_raster':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_raw_tile_raster_verify.exe')
  if name=='settings_callbacks':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_settings_callbacks_verify.exe') or report.get('comparison_checked')!=8625
  if name in ['selection_popup','display_confirmation']:
   for variant in report.get('variants',{}).values():stale=stale or sha(ROOT/variant['path'])!=variant['sha256']
  if name=='editor_validation':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_editor_validation_verify.exe') or report.get('comparison_count')!=24600
  if name=='replay_mission':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_replay_mission_verify.exe') or report.get('comparison_count')!=40552 or report.get('case_selection')!={'limit':None,'start':0,'single_case':False}
  if name=='initial_menu_dispatch':
   stale=stale or not report.get('production') or not report.get('complete_case_set') or report.get('checked')!=5795
   for variant in report.get('variants',{}).values():stale=stale or sha(ROOT/variant['path'])!=variant['sha256']
  if name=='menu_existing_actions':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_menu_existing_actions_verify.exe') or report.get('comparison_count')!=8386 or report.get('case_selection')!={'limit':None,'start':0,'single_case':False}
  if name=='training_panel':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_training_panel_verify.exe') or report.get('comparison_count')!=15460 or report.get('case_selection')!={'limit':None,'start':0,'single_case':False} or 'src/unit_02637.c' not in report.get('source_hashes',{})
  if name in ['menu_keyboard_modal','menu_cursor','menu_window_cursor_consumer','actor_order_flood','actor_order_los']:
   stale=stale or not report.get('production') or not report.get('complete_case_set')
   for variant in report.get('variants',{}).values():stale=stale or sha(ROOT/variant['path'])!=variant['sha256']
  if name=='training_queries':stale=stale or report.get('comparison_C_sha256')!=sha(ROOT/'build/c-only/Release/restore_training_queries_verify.exe') or report.get('comparison_count')!=18618 or report.get('case_selection')!={'limit':None,'start':0,'single_case':False} or 'src/unit_00625.c' not in report.get('source_hashes',{})
  if name in ['remaining_menu_actions','remaining_menu_click']:
   for variant in report.get('variants',{}).values():stale=stale or sha(ROOT/variant['path'])!=variant['sha256']
  if name=='remaining_menu_click':
   for linked in report.get('linked_libraries',{}).values():stale=stale or sha(ROOT/linked['path'])!=linked['sha256']
   for path,digest in report.get('artifact_hashes_before_run',{}).items():stale=stale or not (ROOT/path).is_file() or sha(ROOT/path)!=digest
  if name=='actor_predicate':stale=stale or report.get('compiled_C_variants_sha256',{}).get('restore_actor_predicate_plain_verify.exe')!=sha(ROOT/'build/c-only-debug/Debug/restore_actor_predicate_plain_verify.exe')
  if name=='viewport_controller':stale=stale or report.get('compiled_C_variants_sha256',{}).get('restore_viewport_controller_plain_verify.exe')!=sha(ROOT/'build/c-only-debug/Debug/restore_viewport_controller_plain_verify.exe')
 if stale:
  with (ROOT/f'artifacts/{name.replace("_","-")}-native-refresh.log').open('w') as output:
   command=[sys.executable,str(ROOT/f'tools/verify_{name}_emulation.py')]
   if name=='dropdown_actions':command.extend(['--production','--exe',str(binary),'--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_dropdown_actions_plain_verify.exe'),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_dropdown_actions_verify.exe'),'--optimized-plain-exe',str(ROOT/'build/c-only/Release/restore_dropdown_actions_plain_verify.exe')])
   if name=='dropdown_system_actions':command.extend(['--production','--exe',str(binary),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_dropdown_system_actions_verify.exe')])
   if name=='dropdown_start_actions':command.extend(['--production','--exe',str(binary),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_dropdown_start_actions_verify.exe')])
   if name=='settings_callbacks':command.extend(['--production','--exe',str(binary),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_settings_callbacks_verify.exe')])
   if name=='remaining_menu_actions':command.extend(['--production','--exe',str(binary),'--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_remaining_menu_actions_plain_verify.exe'),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_remaining_menu_actions_verify.exe'),'--optimized-plain-exe',str(ROOT/'build/c-only/Release/restore_remaining_menu_actions_plain_verify.exe')])
   if name=='remaining_menu_click':command.extend(['--production','--exe',str(binary),'--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_remaining_menu_click_plain_verify.exe'),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_remaining_menu_click_verify.exe'),'--optimized-plain-exe',str(ROOT/'build/c-only/Release/restore_remaining_menu_click_plain_verify.exe')])
   if name=='selection_popup':command.extend(['--production','--exe',str(binary),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_selection_popup_verify.exe')])
   if name in ['editor_validation','display_confirmation','replay_mission','initial_menu_dispatch','menu_existing_actions','training_panel','menu_keyboard_modal','menu_cursor','training_queries','refresh_menu_dispatch','menu_window_cursor_consumer','menu_text_glyph','actor_order_flood','actor_order_los']:command.extend(['--production','--exe',str(binary),'--optimized-exe',str(ROOT/f'build/c-only/Release/restore_{name}_verify.exe')])
   if name in ['graphics_53f250','actor_predicate','high_scores_menu','viewport_controller','city_ui','message_panel','terrain_diamond','world_render','event_publisher','upper_raster','rich_message_text','message_panel_input','ground_render','ambient_sound','lookup_sentinel','camera_bounds','city_ui_layout','actor_render','city_top_chrome','mirrored_raster','preview_footprints','mirrored_clip','placement_preview','sprite_shutdown','scalar_binding_batch_001','actor_combat','camera_pan','dropdown_menu','dropdown_tiler','map_cells','actor_496140','tool_selector_input','raw_tile_raster','actor_callbacks_chain','actor_human','actor_callbacks_small','region_validation','tool_selector','actor_coordinates','camera_navigation']:command.extend(['--exe',str(binary)])
   if name in ['terrain_diamond','world_render','rich_message_text','message_panel_input','ground_render','lookup_sentinel','city_ui_layout','actor_render','city_top_chrome','mirrored_raster','preview_footprints','mirrored_clip','placement_preview','sprite_shutdown','scalar_binding_batch_001','actor_combat','camera_pan','dropdown_menu','dropdown_tiler','map_cells','actor_496140','tool_selector_input','raw_tile_raster','actor_callbacks_chain','actor_human','actor_callbacks_small','region_validation','tool_selector','actor_coordinates','camera_navigation']:command.append('--production')
   if name=='scalar_binding_batch_001':command.extend(['--optimized-exe',str(ROOT/'build/c-only/Release/restore_scalar_binding_batch_001_verify.exe')])
   if name=='map_cells':command.extend(['--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_map_cells_plain_verify.exe'),'--optimized-exe',str(ROOT/'build/c-only/Release/restore_map_cells_verify.exe'),'--optimized-plain-exe',str(ROOT/'build/c-only/Release/restore_map_cells_plain_verify.exe')])
   if name=='raw_tile_raster':command.extend(['--optimized-exe',str(ROOT/'build/c-only/Release/restore_raw_tile_raster_verify.exe')])
   if name=='actor_predicate':command.extend(['--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_actor_predicate_plain_verify.exe')])
   if name=='viewport_controller':command.extend(['--plain-exe',str(ROOT/'build/c-only-debug/Debug/restore_viewport_controller_plain_verify.exe')])
   result=subprocess.run(command,cwd=ROOT,stdout=output,stderr=subprocess.STDOUT,timeout=1200 if name in ['refresh_menu_dispatch','actor_order_flood','actor_order_los'] else 1800 if name in ['city_prepare','terrain_redraw','animation_update','minimap','actor_predicate','viewport_controller','city_ui','message_panel','terrain_diamond','world_render','event_publisher','ground_render','placement_preview','dropdown_menu','map_cells','actor_combat','camera_pan','sprite_shutdown','scalar_binding_batch_001'] else (1200 if name=='legacy_mission' else (600 if name in ['chunk_io','mission_chunks','mission_save','graphics_53f250','high_scores_menu','menu_text_glyph'] else 300)))
  assert result.returncode==0,name+' native comparison failed; see its refresh log'
 report=json.loads(report_path.read_text());assert report['passed']
 return {'suite':name,'rerun':stale,'cases':report['checked']}
# The selector's retired consumer-gap note depends on the fresh whole85 proof.
# Complete that dependency before scheduling its consumer; do not race reports.
consumer_result=refresh('menu_window_cursor_consumer')
with ThreadPoolExecutor(max_workers=3) as pool:
 for result in pool.map(refresh,[n for n in names if n!='menu_window_cursor_consumer']):print(json.dumps(result),flush=True)
print(json.dumps(consumer_result),flush=True)
