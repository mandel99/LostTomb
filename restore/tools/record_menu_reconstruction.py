"""Record a partial menu checkpoint, rejecting stale evidence or failed checks."""
from pathlib import Path
from datetime import datetime,timezone
import hashlib,json,re
ROOT=Path(__file__).resolve().parent.parent
def sha(path):return hashlib.sha256((ROOT/path).read_bytes()).hexdigest()
def read(path):return json.loads((ROOT/path).read_text(encoding='utf-8-sig'))
def stamp(path):return {'path':path,'sha256':sha(path)}
pairs={'menu-input':'menu_input','file-list':'file_list','menu-layout':'menu_layout','menu-render':'menu_render','campaign-profile':'campaign_profile','family-menu':'family_menu','text-templates':'text_templates','family-render':'family_render','menu-dialog':'menu_dialog','family-delete':'family_delete','text-wrap':'text_wrap','family-options':'family_options','campaign-menu':'campaign_menu','mission-menu':'mission_menu','town-menu':'town_menu','compression':'compression','chunk-io':'chunk_io','mission-chunks':'mission_chunks','mission-routes':'mission_routes','mission-loaders':'mission_loaders','mission-map':'mission_map','numeric-text':'numeric_text','legacy-mission':'legacy_mission','mission-save':'mission_save','load-save-menu':'load_save_menu'}
pairs['city-prepare']='city_prepare'
pairs['terrain-update']='terrain_update'
pairs['terrain-redraw']='terrain_redraw'
pairs['city-prices']='city_prices'
pairs['music-tick']='music_tick'
pairs['music-manager']='music'
pairs['flood-simulation']='flood_simulation'
pairs['actor-update']='actor_update'
pairs['actor-prey']='actor_prey'
pairs['animation-update']='animation_update'
pairs['minimap']='minimap'
pairs['graphics-53f250']='graphics_53f250'
pairs['actor-predicate']='actor_predicate'
pairs['high-scores-menu']='high_scores_menu'
pairs['viewport-controller']='viewport_controller'
pairs['city-ui']='city_ui'
pairs['message-panel']='message_panel'
pairs['terrain-diamond']='terrain_diamond'
pairs['world-render']='world_render'
pairs['event-publisher']='event_publisher'
pairs['upper-raster']='upper_raster'
pairs['rich-message-text']='rich_message_text'
pairs['message-panel-input']='message_panel_input'
pairs['ground-render']='ground_render'
pairs['ambient-sound']='ambient_sound'
pairs['lookup-sentinel']='lookup_sentinel'
pairs['camera-bounds']='camera_bounds'
pairs['city-ui-layout']='city_ui_layout'
pairs['actor-render']='actor_render'
pairs['city-top-chrome']='city_top_chrome'
pairs['mirrored-raster']='mirrored_raster'
pairs['preview-footprints']='preview_footprints'
pairs['mirrored-clip']='mirrored_clip'
pairs['placement-preview']='placement_preview'
pairs.update({'region-validation':'region_validation','tool-selector':'tool_selector','actor-coordinates':'actor_coordinates','camera-navigation':'camera_navigation'})

pairs.update({'actor-human':'actor_human','actor-callbacks-small':'actor_callbacks_small'})

pairs.update({'raw-tile-raster':'raw_tile_raster','actor-callbacks-chain':'actor_callbacks_chain','tool-selector-input':'tool_selector_input','actor-496140':'actor_496140'})

test_count=106
suites=[];source_paths=set();total=0
for name,target in pairs.items():
 path=f'metadata/{name}-emulation.json';report=read(path)
 assert report['passed'],name+' failed'
 binary=f'build/c-only-debug/Debug/restore_{target}_verify.exe'
 assert report['compiled_C_sha256']==sha(binary),name+' binary evidence is stale'
 if name=='actor-predicate':assert report['compiled_C_variants_sha256']['restore_actor_predicate_plain_verify.exe']==sha('build/c-only-debug/Debug/restore_actor_predicate_plain_verify.exe'),'Actor predicate plain binary is stale'
 if name=='viewport-controller':assert report['compiled_C_variants_sha256']['restore_viewport_controller_plain_verify.exe']==sha('build/c-only-debug/Debug/restore_viewport_controller_plain_verify.exe'),'Viewport controller plain binary is stale'
 for source,digest in report['source_hashes'].items():
  assert sha(source)==digest,name+' source evidence is stale: '+source
  source_paths.add(source)
 for source,digest in report.get('native_exports',{}).items():
  assert sha(source)==digest,name+' native export is stale: '+source
  source_paths.add(source)
 total+=report['checked'];suites.append({**stamp(path),'cases':report['checked'],'compiled_C':stamp(binary),'limitations':report.get('limitations',[])})
debug='build/c-only-debug/Debug/restore_link_check.exe';release='build/c-only/Release/restore_link_check.exe'
builds=[]
for kind,directory,binary in [('debug','c-only-debug',debug),('release','c-only',release)]:
 log=f'artifacts/menu-routes-{kind}-build.log';text=(ROOT/log).read_text(errors='replace')
 assert f'100% tests passed, 0 tests failed out of {test_count}' in text and 'C source build completed.' in text,kind+' build not confirmed'
 cache=(ROOT/f'build/{directory}/CMakeCache.txt').read_text()
 assert 'RESTORE_BUILD_HYBRID:BOOL=OFF' in cache
 assert re.search(r'^RESTORE_ORIGINAL_EXE:FILEPATH=$',cache,re.M) and re.search(r'^RESTORE_GAME_EXE:FILEPATH=$',cache,re.M)
 builds.append({'configuration':kind,'tests_passed':test_count,'binary':stamp(binary),'log':stamp(log),'original_EXE_build_dependency':False})
frames=[]
for name,mode in [('family',47),('options',48),('campaign',23)]:
 manifest=f'metadata/menu-{name}-frame.json';frame=read(manifest)
 assert frame['child_exe_sha256']==sha(debug) and frame['completed_frame_copy'] and frame['mode']==mode,'Frame is stale or incomplete: '+name
 image=f'artifacts/menu-{name}-frame.bmp'
 frames.append({'screen':name,'mode':mode,'manifest':stamp(manifest),'image':stamp(image),'raw_frame_sha256':frame['raw_sha256'],'visually_inspected':True})
log='artifacts/menu-campaign-integration.log';trace=(ROOT/log).read_text(errors='replace')
assert 'mode=47' in trace and 'mode=48' in trace and 'mode=23' in trace and 'family selection: confirm through C buttons' in trace and 'DEBUG TIMEOUT:' in trace
exceptions=re.findall(r'EXCEPTION ([0-9a-fA-F]+)',trace)
assert exceptions==['80000003'],'Unexpected exception in final owned C run: '+repr(exceptions)
assert 'Unreconstructed' not in trace and 'EXIT ' not in trace
profile='build/debug-startup-game/Save/Restore.dat';assert (ROOT/profile).stat().st_size==0x4a9c
bindings=read('metadata/string-binding-verification.json');assert bindings['passed']
for source,digest in bindings['source_hashes'].items():assert sha(source)==digest,'String-binding evidence is stale: '+source
source_paths.update(['README.md','CMakeLists.txt','runtime/standalone_link_check.c','runtime/startup_probe.c','include/startup_probe.h','runtime/menu_callbacks.c','runtime/text_templates.c','runtime/file_list_platform_win32.c','include/file_list.h','include/family_menu.h','src/unit_00223.c','tools/Debug-Standalone.ps1','tools/capture_restore_framebuffer.py','metadata/string-binding-retirements.json'])
source_paths.update(['src/unit_00021.c','src/unit_00225.c','src/unit_00226.c','src/unit_00265.c','src/unit_00274.c','src/unit_00312.c','src/unit_00313.c','src/unit_01835.c','src/unit_01867.c'])
source_paths.update(['src/unit_00064.c','src/unit_02431.c','tools/record_menu_dialogs.py','tools/record_menu_reconstruction.py','tools/record_campaign_menu.py','tools/refresh_menu_native_evidence.py','tools/Test-MenuIntegration.ps1','src/unit_01868.c','metadata/CAMPAIGN_MENU_RECONSTRUCTION.md'])
source_paths.update(['metadata/MISSION_CHUNKS_RECONSTRUCTION.md','metadata/MISSION_ROUTE_RECONSTRUCTION.md','metadata/mission-route-builder-native.json','tools/record_mission_map.py'])
source_paths.update(['metadata/CITY_PREPARATION_RECONSTRUCTION.md','tools/Test-CityConfirm.ps1'])
source_paths.add('metadata/terrain-redraw-next-native.json')
source_paths.update(['metadata/TERRAIN_REDRAW_RECONSTRUCTION.md','metadata/city-price-next-native.json','metadata/water-render-next-native.json'])
source_paths.update(['metadata/CITY_PRICE_MUSIC_RECONSTRUCTION.md','metadata/city-prices-helpers-native.json','metadata/town-trade-callers-native.json','metadata/music-tick-native.json','metadata/city-simulation-next-native.json'])
source_paths.update(['metadata/FLOOD_SIMULATION_RECONSTRUCTION.md','metadata/actor-update-next-native.json','metadata/actor-prey-next-native.json'])
source_paths.update(['metadata/ACTOR_UPDATE_RECONSTRUCTION.md','metadata/animation-state-next-native.json','tools/record_actor_callback_inventory.py','tools/export_actor_callback_evidence.mjs'])
source_paths.update(['metadata/ANIMATION_UPDATE_RECONSTRUCTION.md','metadata/water-ground-render-next-native.json'])
source_paths.update(['metadata/MINIMAP_RECONSTRUCTION.md','metadata/city-draw-next-native.json'])
source_paths.update(['metadata/PARALLEL_RECONSTRUCTION.md','metadata/discarded-return-multiline-repairs.json','metadata/render-dialog-discarded-returns-native.json','metadata/render-dialog-guard-review.json','tools/verify_render_dialog_guard_flow.py'])
source_paths.update(['metadata/city-ui-next-native.json','metadata/message-panel-next-native.json'])
source_paths.update(['metadata/city-ui-children-native.json','metadata/viewport_controller_facade_return_audit.json','metadata/viewport-controller-facade-callers-native.json','metadata/city-ui-reconstruction.json','metadata/message-panel-restoration.json','metadata/message-panel-native-byte-audit.json'])
viewport=read('metadata/viewport-controller-emulation.json')
assert viewport['native_instruction_coverage']==[76,76]
hud=read('metadata/city-ui-emulation.json')
assert len(hud['instruction_coverage'])==10 and all(v['total']==v['executed'] and not v['missing'] for v in hud['instruction_coverage'].values())
message=read('metadata/message-panel-emulation.json')
assert message['complete_case_set'] and all(v['reached'] for v in message['branch_probes'].values())
message_rea=read('metadata/message-panel-rea-function.json')['data']
assert message_rea['operation']=='analyze_function' and message_rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
assert message_rea['normalized_result']['procedure']['address']=='0x4c5e60' and len(message_rea['normalized_result']['assembly'])==987
source_paths.add('metadata/message-panel-rea-function.json')
world=read('metadata/world-render-emulation.json')
assert world['case_selection']=={'start':0,'limit':None,'single_case':False} and world['checked']==11080
assert world['reachable_native_instruction_coverage']==[1398,1398] and world['helper_instruction_coverage']==[12,12]
assert {'src/unit_00547.c','src/unit_00548.c','src/unit_00549.c'}<=world['source_hashes'].keys()
diamond=read('metadata/terrain-diamond-emulation.json')
assert diamond['production_sources'] is True and diamond['total_instruction_coverage']=={'total':8466,'executed':8466}
publisher=read('metadata/event-publisher-emulation.json')
assert publisher['complete_case_set'] and all(v['reached'] for v in publisher['branch_probes'].values())
for source,digest in publisher['fixture_hashes'].items():
 assert sha(source)==digest,'Publisher corpus fixture is stale: '+source
 source_paths.add(source)
source_paths.update(['metadata/world_render_phase_manifest.json','metadata/world_render_contract.json','metadata/world_render_member_binding_audit.json','metadata/event-publisher-restoration.json','metadata/city-confirm-event-publisher-failure.json','artifacts/city-confirm-event-publisher-failure.log','metadata/city-confirm-upper-raster-failure.json','artifacts/city-confirm-upper-raster-failure.log'])
publisher_rea=read('metadata/event-publisher-rea-function.json')['data']
assert publisher_rea['operation']=='analyze_function' and publisher_rea['subject']['digest']['sha256']==message_rea['subject']['digest']['sha256']
assert publisher_rea['normalized_result']['procedure']['address']=='0x4e0cf0' and len(publisher_rea['normalized_result']['assembly'])==170
source_paths.add('metadata/event-publisher-rea-function.json')
upper=read('metadata/upper-raster-emulation.json')
assert upper['total_instruction_coverage']=={'total':1095,'executed':1095}
rich=read('metadata/rich-message-text-emulation.json')
assert rich['complete_case_set'] and rich['native_instructions_verified']==669
assert len(rich['branch_probes'])==40 and all(v['reached'] for v in rich['branch_probes'].values())
source_paths.update(['metadata/upper-raster-reconstruction.json','metadata/rich-message-text-restoration.json','metadata/rich-message-text-asset-markup-audit.json','metadata/rich-message-text-terminal-marker-native.json','tools/record_parallel_phase_status.py'])
markup=read('metadata/rich-message-text-asset-markup-audit.json')
assert markup['MM_sha256']==sha('build/debug-startup-game/Pharaoh_MM.eng') and markup['fields_checked']==1988
assert all(item['field']==60 for item in markup['terminal_numeric_link_markers'])
message_input=read('metadata/message-panel-input-emulation.json')
assert message_input['complete_case_set'] and message_input['stage']=='production' and message_input['newly_restored_native_instructions']==631
assert len(message_input['branch_probes'])==40 and all(v['reached'] for v in message_input['branch_probes'].values())
input_rea=read('metadata/message-panel-input-rea-function.json')['data']
assert input_rea['operation']=='analyze_function' and input_rea['subject']['digest']['sha256']==message_rea['subject']['digest']['sha256']
assert input_rea['normalized_result']['procedure']['address']=='0x4c7770' and len(input_rea['normalized_result']['assembly'])==431
source_paths.update(['metadata/message-panel-input-restoration.json','metadata/message-panel-input-rea-function.json'])
ground=read('metadata/ground-render-emulation.json')
assert ground['case_selection']=={'start':0,'limit':None,'single_case':False} and ground['checked']==5077
assert ground['native_instruction_coverage']==[940,940] and ground['helper_instruction_coverage']==[43,43]
assert len(ground['mutated_endpoint_addresses'])==19 and 'src/unit_00470.c' in ground['source_hashes']
source_paths.update(['metadata/ground_render_contract.json','metadata/ground_render_phase_manifest.json'])
ambient=read('metadata/ambient-sound-emulation.json')
assert ambient['checked']==5015 and sum(v['total'] for v in ambient['instruction_coverage'].values())==357
assert all(v['executed']==v['total'] and not v['unexecuted'] for v in ambient['instruction_coverage'].values())
source_paths.update(['metadata/ambient-sound-reconstruction.json','metadata/city-ambient-x87-failure.json','artifacts/city-ambient-x87-failure.log'])
lookup=read('metadata/lookup-sentinel-emulation.json')
assert lookup['checked']==473 and lookup['case_selection']=={'limit':None,'single_case':False}
assert lookup['native_instruction_coverage']==[178,178] and lookup['full_grid_cells']==105567 and lookup['source_image_cells']==51984
assert not lookup['controlled_algorithm_endpoints'] and 'src/unit_00457.c' in lookup['source_hashes']
source_paths.add('metadata/viewport_endpasses_readonly_audit.json')
source_paths.update(['metadata/lookup_sentinel_contract.json','metadata/lookup_sentinel_phase_manifest.json'])
camera=read('metadata/camera-bounds-emulation.json')
assert camera['checked']==1266 and camera['case_selection']=={'limit':None}
assert camera['native_instruction_coverage']==[189,189] and camera['controlled_algorithm_endpoints']==[]
camera_rea=read('metadata/camera-bounds-rea-function.json')
assert camera_rea['ok'] and camera_rea['data']['operation']=='analyze_function'
assert camera_rea['data']['subject']['digest']['sha256']==camera['native_sha256']
assert camera_rea['data']['normalized_result']['procedure']['address']=='0x4ddf90'
assert len(camera_rea['data']['normalized_result']['assembly'])==69
source_paths.add('metadata/camera-bounds-rea-function.json')
layout=read('metadata/city-ui-layout-emulation.json')
assert layout['production'] and layout['checked']==3689 and layout['native_functions']==12
assert layout['instructions_executed']==layout['instructions_total']==1544 and layout['frozen_HUD_source_drift']==[]
source_paths.update(['metadata/city-ui-layout-reconstruction.json','metadata/city-ui-layout-apply-map.json'])
chrome=read('metadata/city-top-chrome-emulation.json')
assert chrome['production'] and chrome['checked']==4626 and chrome['native_functions']==14
assert chrome['instructions_executed']==chrome['instructions_total']==1661 and chrome['frozen_HUD_source_drift']==[]
source_paths.update(['metadata/city-top-chrome-reconstruction.json','metadata/city-top-chrome-apply-map.json'])
actor_render=read('metadata/actor-render-emulation.json')
assert actor_render['case_selection']=={'start':0,'limit':None,'single_case':False} and actor_render['checked']==7441
assert actor_render['native_instruction_coverage']==[830,833] and actor_render['native_reachable_instruction_coverage']==[830,830]
assert len(actor_render['mutated_endpoint_addresses'])==14
assert {'src/unit_00561.c','src/unit_00562.c'}<=actor_render['source_hashes'].keys()
mirrored=read('metadata/mirrored-raster-emulation.json')
assert mirrored['checked']==mirrored['plain_checked']==1066 and mirrored['case_selection']=={'start':0,'limit':None,'single_case':False}
assert mirrored['native_instruction_coverage']==[64,64] and mirrored['helper_instruction_coverage']==[32,32]
assert mirrored['controlled_algorithm_endpoints']==[] and mirrored['helper_mutation_phases']==[0,1]
assert {'src/unit_00131.c','src/unit_00149.c'}<=mirrored['source_hashes'].keys()
source_paths.update(['metadata/upper-raster-before-mirrored-carry.json','artifacts/mirrored-raster-before-unit-00149.c',
 'metadata/mirrored-raster-helper-carry-failure.json'])
source_paths.update(['metadata/mirrored_raster_contract.json','metadata/mirrored_raster_phase_manifest.json'])
mirrored_clip=read('metadata/mirrored-clip-emulation.json')
assert mirrored_clip['checked']==mirrored_clip['plain_checked']==3258
assert mirrored_clip['case_selection']=={'start':0,'limit':None,'single_case':False}
assert mirrored_clip['native_instruction_coverage']==[154,154] and mirrored_clip['helper_instruction_coverage']==[32,32]
assert mirrored_clip['controlled_algorithm_endpoints']==[]
source_paths.update(['metadata/city-mirrored-clip-failure.json','artifacts/city-raster-repair.log',
 'artifacts/city-raster-repair-frame-capture.log','artifacts/mirrored-clip-before-unit-00133.c',
 'metadata/mirrored_clip_contract.json','metadata/mirrored_clip_phase_manifest.json'])
seeds=read('metadata/preview-footprints-emulation.json')
assert seeds['production'] and seeds['checked']==96 and seeds['record_count']==1327 and seeds['seed_DWORD_count']==5308
assert seeds['native_prefix_instruction_count']==17497 and seeds['native_geometry_instruction_coverage']==[99,99]
assert 'runtime/preview_footprint_seeds.c' in seeds['source_hashes']
source_paths.update(['metadata/preview-footprints-reconstruction.json','metadata/preview-footprints-apply-map.json'])
preview=read('metadata/placement-preview-emulation.json')
assert preview['stage']=='production' and preview['passed'] and preview['complete_case_set'] and preview['checked']==3938
assert preview['controller_instruction_coverage']=={'executed':1963,'total':1963,'unreached':[]}
assert preview['newly_restored_native_instructions']==2860 and preview['existing_geometry_native_instructions']==99
assert all(v['executed']==v['total'] and v['unreached']==[] for v in preview['native_function_instruction_coverage'].values())
assert preview['startup_profiles']==72 and preview['seed_records_independently_recovered']==1327 and preview['seed_native_prefix_instructions']==17497
assert len(preview['global_mask_receiver_bindings'])==34 and len(preview['unknown_or_truncated_tokens_rejected'])==6
assert not preview['original_machine_code_fallback_used'] and preview['unqualified_missing_conditional_outcomes']==[]
outcomes=preview['native_conditional_branch_coverage']
assert sum(v[k] for v in outcomes.values() for k in ['taken','fallthrough'])==684 and len(outcomes)*2==686
assert {(a,k) for a,v in outcomes.items() for k in ['taken','fallthrough'] if not v[k]}=={('0x4e5582','taken'),('0x5461bf','taken')}
assert {'src/unit_00565.c','src/unit_02115.c','src/unit_02527.c','runtime/placement_preview_predicates.c'}<=preview['source_hashes'].keys()
source_paths.update(['src/unit_00177.c','metadata/placement-preview-reconstruction.json','metadata/placement-preview-apply-map.json'])
source_paths.update(['metadata/actor_render_contract.json','metadata/actor_render_phase_manifest.json',
 'metadata/city-actor-retry.json','artifacts/city-actor-retry.log'])
for private_path,private_binary in [
 ('metadata/camera-bounds-private-optimized.json','build/agent-camera-bounds/Release/camera_bounds_verify.exe'),
 ('metadata/actor-render-private-optimized.json','build/agent-actor-render/Release/actor_render_verify.exe'),
 ('metadata/mirrored-raster-private-optimized.json','build/agent-mirrored-raster/Release/mirrored_raster_verify.exe'),
 ('metadata/mirrored-clip-private-optimized.json','build/agent-mirrored-clip/Release/mirrored_clip_verify.exe'),
 ('artifacts/city_ui_layout_phase/metadata/emulation-debug.json','build/agent-city-ui-layout/Debug/restore_city_ui_layout_verify.exe'),
 ('artifacts/city_ui_layout_phase/metadata/emulation-release.json','build/agent-city-ui-layout/Release/restore_city_ui_layout_verify.exe'),
 ('artifacts/city_top_chrome_phase/metadata/emulation-debug.json','build/agent-city-top-chrome/Debug/restore_city_top_chrome_verify.exe'),
 ('artifacts/city_top_chrome_phase/metadata/emulation-release.json','build/agent-city-top-chrome/Release/restore_city_top_chrome_verify.exe'),
 ('artifacts/preview_footprints_phase/metadata/emulation-debug.json','build/agent-preview-footprints/Debug/restore_preview_footprints_verify.exe'),
 ('artifacts/preview_footprints_phase/metadata/emulation-release.json','build/agent-preview-footprints/Release/restore_preview_footprints_verify.exe'),
 ('metadata/placement-preview-return32-private-debug.json','build/placement-preview-return32/Debug/placement_preview_return32_verify.exe'),
 ('metadata/placement-preview-return32-private-release.json','build/placement-preview-return32/Release/placement_preview_return32_verify.exe'),
 ('artifacts/region_validation_phase/metadata/emulation-debug.json','build/agent-region-validation/Debug/restore_region_validation_verify.exe'),
 ('artifacts/region_validation_phase/metadata/emulation-release.json','build/agent-region-validation/Release/restore_region_validation_verify.exe'),
 ('metadata/tool-selector-staged-debug-full-emulation.json','build/agent-tool-selector/out/Debug/restore_tool_selector_verify.exe'),
 ('metadata/tool-selector-staged-release-full-emulation.json','build/agent-tool-selector/out/Release/restore_tool_selector_verify.exe'),
 ('metadata/actor-human-staged-debug-full-emulation.json','build/agent-actor-human/out/Debug/restore_actor_human_verify.exe'),
 ('metadata/actor-human-staged-release-full-emulation.json','build/agent-actor-human/out/Release/restore_actor_human_verify.exe'),
 ('artifacts/actor_callbacks_small_phase/metadata/emulation-debug.json','build/agent-actor-callbacks-small/Debug/restore_actor_callbacks_small_verify.exe'),
 ('artifacts/actor_callbacks_small_phase/metadata/emulation-release.json','build/agent-actor-callbacks-small/Release/restore_actor_callbacks_small_verify.exe'),
 ('artifacts/actor_callbacks_chain_phase/metadata/emulation-debug.json','build/agent-actor-callbacks-chain/Debug/restore_actor_callbacks_chain_verify.exe'),
 ('artifacts/actor_callbacks_chain_phase/metadata/emulation-release.json','build/agent-actor-callbacks-chain/Release/restore_actor_callbacks_chain_verify.exe'),
 ('metadata/actor-496140-staged-debug-full-emulation.json','build/agent-actor-496140/out/Debug/restore_actor_496140_verify.exe'),
 ('metadata/actor-496140-staged-release-full-emulation.json','build/agent-actor-496140/out/Release/restore_actor_496140_verify.exe')]:
 private=read(private_path);assert private['passed'],private_path
 assert sha(private_binary)==private['compiled_C_sha256'],private_path+' stale binary'
 for source,digest in private['source_hashes'].items():assert sha(source)==digest,private_path+' stale source: '+source
 source_paths.add(private_path)
for stem,target,expected,coverage in [('actor-coordinates','actor_coordinates',4771,[403,403]),('camera-navigation','camera_navigation',1246,[126,126])]:
 proof=read('metadata/'+stem+'-emulation.json')
 assert proof['passed'] and proof['checked']==expected and proof['native_instruction_coverage']==coverage
 assert proof['controlled_algorithm_endpoints']==[]
 source_paths.update(['metadata/'+stem.replace('-','_')+'_contract.json','metadata/'+stem.replace('-','_')+'_phase_manifest.json'])
 private_path='metadata/'+stem+'-private-debug-release-emulation.json';private=read(private_path)
 assert private['passed'] and private['checked']==private['comparison_checked']==expected
 private_target=target+'_verify.exe'
 for configuration,key in [('Debug','compiled_C_sha256'),('Release','comparison_C_sha256')]:
  assert sha('build/agent-'+stem+'/'+configuration+'/'+private_target)==private[key],private_path+' stale '+configuration+' binary'
 for source,digest in private['source_hashes'].items():assert sha(source)==digest,private_path+' stale source: '+source
 source_paths.add(private_path)
source_paths.update(['metadata/actor-human-reconstruction.json','metadata/actor-human-apply-map.json','metadata/actor-callbacks-small-reconstruction.json','metadata/actor-callbacks-small-apply-map.json','metadata/actor-callback-c-contracts.json'])
source_paths.update(['metadata/region-validation-reconstruction.json','metadata/region-validation-apply-map.json','metadata/region-validation-return-contract.json','metadata/tool-selector-reconstruction.json','metadata/tool-selector-apply-map.json'])

for stem,count,new,composed in [('tool-selector',3171,639,32),('actor-human',3726,751,17),('actor-496140',3168,289,31)]:
 proof=read('metadata/'+stem+'-emulation.json')
 assert proof['passed'] and proof['complete_case_set'] and proof['checked']==count
 assert proof['newly_restored_native_instructions']==new and proof['unchanged_composed_native_instructions']==composed
 assert all(v['executed']==v['total'] and not v['unreached'] for v in proof['native_function_instruction_coverage'].values())
 assert proof['unqualified_missing_edges']==[]
for stem,count,expected in [('region-validation',3079,329),('actor-callbacks-small',4545,494),('actor-callbacks-chain',1487,308)]:
 proof=read('metadata/'+stem+'-emulation.json')
 assert proof['passed'] and proof['checked']==count and proof['instructions_executed']==proof['instructions_total']==expected
raw=read('metadata/raw-tile-raster-emulation.json')
assert raw['passed'] and raw['checked']==raw['comparison_checked']==1000 and raw['native_instruction_coverage']==[291,291]
assert raw['comparison_C_sha256']==sha('build/c-only/Release/restore_raw_tile_raster_verify.exe')
assert raw['controlled_algorithm_endpoints']==[] and raw['watched_bytes']==65600 and raw['native_writes_per_case']==128
inp=read('metadata/tool-selector-input-emulation.json')
assert inp['passed'] and inp['checked']==4506 and inp['native_instruction_coverage']==[217,217]
assert inp['composed_instruction_coverage']==[477,722] and inp['owned_conditional_outcomes']==[58,58]
assert inp['missing_owned_conditional_outcomes']==[]
private_path='metadata/tool-selector-input-private-debug-release-emulation.json';private=read(private_path)
assert private['passed'] and private['checked']==private['comparison_checked']==4506
for configuration,key in [('Debug','compiled_C_sha256'),('Release','comparison_C_sha256')]:
 assert sha('build/agent-tool-selector-input/'+configuration+'/tool_selector_input_verify.exe')==private[key]
for source,digest in private['source_hashes'].items():assert sha(source)==digest,private_path+' stale source:'+source
source_paths.add(private_path)
source_paths.update(['metadata/tool_selector_input_contract.json','metadata/tool_selector_input_phase_manifest.json',
 'metadata/actor-callbacks-chain-apply-map.json','metadata/actor-callbacks-chain-reconstruction.json',
 'metadata/actor-496140-apply-map.json','metadata/actor-496140-reconstruction.json',
 'metadata/menu-load-raw-tile-failure.json','artifacts/raw-tile-before-unit-00151.c','artifacts/menu-load-before-raw-tile-repair.log',
 'metadata/raw-tile-raster-reconstruction.json','tools/placement_preview_return32_project/CMakeLists.txt'])

guard_review=read('metadata/render-dialog-guard-review.json')
assert guard_review['passed'] and guard_review['checked']==512 and guard_review['complete_dialog_reconstruction'] is False
guard_supersession=read('metadata/render-dialog-guard-supersession.json')
assert guard_supersession['passed'] and guard_supersession['checked']==0
assert guard_supersession['historical_review']['sha256']==sha('metadata/render-dialog-guard-review.json')
assert guard_supersession['historical_review']['source_patterns_current'] is False
assert guard_supersession['superseding_native_proof']['sha256']==sha('metadata/message-panel-emulation.json')
for source,digest in guard_supersession['source_hashes'].items():assert sha(source)==digest,'Dialog supersession evidence is stale: '+source
source_paths.update(guard_supersession['source_hashes']);source_paths.add('metadata/render-dialog-guard-supersession.json')
pixel_regression=read('metadata/minimap-menu-frame-regression.json')
assert pixel_regression['compared']==20 and pixel_regression['changed']==[]
assert pixel_regression['passed'] and pixel_regression['child_exe_sha256']==sha(debug)
assert pixel_regression['current_owned_C_images']==19 and len(pixel_regression['current_baseline_images'])==15
assert len(pixel_regression['retained_historical_images'])==5
for source,digest in pixel_regression['source_hashes'].items():assert sha(source)==digest,'Pixel evidence is stale: '+source
source_paths.update(pixel_regression['source_hashes'])
for image in read(pixel_regression['prior_hashes']):assert sha(image['path'])==image['sha256'],'Menu pixels differ from the accepted baseline: '+image['path']
source_paths.update(['metadata/minimap-menu-frame-regression.json',pixel_regression['prior_hashes']])
rea=read('metadata/menu-dialog-rea-function.json')['data']
assert rea['operation']=='analyze_function' and rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
legacy_rea=read('metadata/legacy-mission-rea-function.json')['data']
assert legacy_rea['operation']=='analyze_function' and legacy_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
source_paths.update(['include/audio_state.h','metadata/record-state-callers-native.json','metadata/LEGACY_MISSION_RECONSTRUCTION.md','tools/repair_discarded_returns.py'])
state_callers=read('metadata/record-state-callers-native.json')
assert len(state_callers['callers'])==41 and sum(len(c['calls']) for c in state_callers['callers'])==65
source_paths.update(c['file'] for c in state_callers['callers'])

dialogs=read('metadata/menu-dialog-integration.json')
assert dialogs['passed'] and dialogs['child_exe_sha256']==sha(debug),'Dialog integration is stale'
campaign=read('metadata/campaign-menu-integration.json')
assert campaign['passed'] and campaign['child_exe_sha256']==sha(debug),'Campaign integration is stale'
for source,digest in campaign['source_hashes'].items():assert sha(source)==digest,'Campaign integration source is stale: '+source
mission=read('metadata/mission-menu-integration.json')
assert mission['passed'] and mission['child_exe_sha256']==sha(debug),'Mission integration is stale'
for source,digest in mission['source_hashes'].items():assert sha(source)==digest,'Mission integration source is stale: '+source
source_paths.update(mission['source_hashes'])
chunks_rea=read('metadata/mission-chunks-rea-function.json')['data']
assert chunks_rea['operation']=='analyze_function' and chunks_rea['subject']['digest']['sha256']=='fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d'
map_integration=read('metadata/mission-map-integration.json')
assert map_integration['passed'] and map_integration['child_exe_sha256']==sha(debug),'Nonempty map preview integration is stale'
for source,digest in map_integration['source_hashes'].items():assert sha(source)==digest,'Nonempty map source evidence is stale: '+source
source_paths.update(map_integration['source_hashes'])
load_save=read('metadata/load-save-menu-integration.json')
assert load_save['passed'] and load_save['child_exe_sha256']==sha(debug),'Load/save integration is stale'
for source,digest in load_save['source_hashes'].items():assert sha(source)==digest,'Load/save source evidence is stale: '+source
source_paths.update(load_save['source_hashes']);source_paths.add('metadata/LOAD_SAVE_MENU_RECONSTRUCTION.md')
scores=read('metadata/high-scores-menu-integration.json')
assert scores['passed'] and scores['child_exe_sha256']==sha(debug),'Scores integration is stale'
for source,digest in scores['source_hashes'].items():assert sha(source)==digest,'Scores integration source is stale: '+source
source_paths.update(scores['source_hashes'])
city_diagnostic_path='metadata/city-confirm-team.json'
city_diagnostic=read(city_diagnostic_path)
assert city_diagnostic['child_exe_sha256']==sha(debug),'City diagnostic is stale'
assert city_diagnostic['confirm_reached'] and city_diagnostic['profile_preserved'] and city_diagnostic['fixture_preserved']
assert city_diagnostic['exceptions']==['80000003'] and not city_diagnostic['standalone_gameplay_proven']
city_trace=(ROOT/city_diagnostic['log']).read_text(errors='replace')
assert 'STARTUP frame mode=1 ' in city_trace and 'STARTUP frame mode=5 ' in city_trace and 'DEBUG TIMEOUT:' in city_trace
assert 'EXIT ' not in city_trace and '_RTC_UninitUse' not in city_trace and 'Unresolved return register' not in city_trace
assert city_diagnostic['completed_city_frame_proven'] and city_diagnostic['completed_frame_manifest']
city_frame_path=city_diagnostic['completed_frame_manifest'];city_frame=read(city_frame_path)
assert city_frame['child_exe_sha256']==sha(debug) and city_frame['completed_frame_copy'] and city_frame['mode']==5
assert city_frame['width']==1024 and city_frame['height']==768 and city_frame['unique_16bit_colors']>100
city_image='artifacts/city-confirm-team-frame.bmp'
assert city_diagnostic['passed']
dismissal_path='metadata/city-message-dismiss.json';dismissal=read(dismissal_path)
assert dismissal['passed'] and dismissal['child_exe_sha256']==sha(debug)
assert dismissal['message_dismissal_requested'] and dismissal['message_OK_dispatched'] and dismissal['completed_gameplay_frame_proven']
assert dismissal['profile_preserved'] and dismissal['fixture_preserved'] and dismissal['exceptions']==['80000003']
dismissal_trace=(ROOT/dismissal['log']).read_text(errors='replace')
assert 'message: dismiss OK through C small buttons' in dismissal_trace and 'message: returned to gameplay through C input' in dismissal_trace
assert 'DEBUG TIMEOUT:' in dismissal_trace and all(x not in dismissal_trace for x in ['EXIT ','_RTC_UninitUse','Unreconstructed','Unresolved return register'])
gameplay_frame_path=dismissal['gameplay_frame_manifest'];gameplay_frame=read(gameplay_frame_path)
assert gameplay_frame['completed_frame_copy'] and gameplay_frame['mode']==1 and gameplay_frame['child_exe_sha256']==sha(debug)
assert gameplay_frame['width']==1024 and gameplay_frame['height']==768 and gameplay_frame['unique_16bit_colors']>100
expected_preview_classes={0x6fd5d8:0x56f48c,0x6de650:0x56f438,0x6fd368:0x56f4dc,0x6f9320:0x570160,
 0x6f5648:0x570268,0x6de9e0:0x570228,0x6e9188:0x5701e8,0x6de548:0x56f6e8,
 0x6f9370:0xa6,0x6f5698:0xa,0x6dea30:0x1b4,0x6e91d8:0x1f}
assert all(gameplay_frame['live_render_state_at_capture'][hex(a)]['u32']==v for a,v in expected_preview_classes.items()),'Owned startup class tokens/selectors differ'
gameplay_image='artifacts/city-message-dismiss-gameplay-frame.bmp'
message_input_integration={'passed':True,'diagnostic':stamp(dismissal_path),'log':stamp(dismissal['log']),
 'confirmed_flow':['verified SAV load','completed Housing and Roads message in mode5','native-layout OK through actual C small-button callback','restored prior mode1','subsequent completed gameplay frame','bounded45-second run without unexpected exceptions'],
 'frame':stamp(gameplay_frame_path),'image':stamp(gameplay_image),'visually_inspected':True,
 'standalone_gameplay_proven':False,'city_visual_fidelity_proven':False,
 'limitations':['One loaded fixture and one completed active frame do not establish general playable gameplay.','HUD placement and other visible rendering defects remain.']}
source_paths.update(['tools/record_high_scores_integration.py','tools/Test-HighScoresIntegration.ps1'])
assert 'FUN_0053f4a0+' not in city_trace and 'FUN_0043e810+' not in city_trace
assert 'FUN_0041fdb0+' not in city_trace and 'FUN_0041ff20+' not in city_trace
assert 'Unreconstructed actor callback' not in city_trace and 'at 0049BD40 ' not in city_trace
city_rea=read('metadata/city-prepare-rea-function.json')['data']
assert city_rea['operation']=='analyze_function' and city_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
terrain_rea=read('metadata/terrain-redraw-rea-function.json')['data']
assert terrain_rea['operation']=='analyze_function' and terrain_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert terrain_rea['normalized_result']['procedure']['address']=='0x4767d0' and len(terrain_rea['normalized_result']['assembly'])==635
price_rea=read('metadata/city-price-rea-function.json')['data']
assert price_rea['operation']=='analyze_function' and price_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert price_rea['normalized_result']['procedure']['address']=='0x443330' and len(price_rea['normalized_result']['assembly'])==136
trade_callers=read('metadata/town-trade-callers-native.json')
assert trade_callers['global_iterator_calls']==26 and trade_callers['record_predicate_calls']==10 and len(trade_callers['C_record_bindings'])==10
source_paths.update(c['file'] for c in trade_callers['C_record_bindings'])
simulation=read('metadata/city-simulation-next-native.json')
assert simulation['native_sha256']==rea['subject']['digest']['sha256'] and len(simulation['function']['items'])==303 and not simulation['function']['has_more']
flood_rea=read('metadata/flood-simulation-rea-function.json')['data']
assert flood_rea['operation']=='analyze_function' and flood_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert flood_rea['normalized_result']['procedure']['address']=='0x4be770' and len(flood_rea['normalized_result']['assembly'])==303
actors=read('metadata/actor-update-next-native.json')
assert actors['native_sha256']==rea['subject']['digest']['sha256'] and len(actors['function']['items'])==74 and not actors['function']['has_more']
actor_rea=read('metadata/actor-update-rea-function.json')['data']
assert actor_rea['operation']=='analyze_function' and actor_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert actor_rea['normalized_result']['procedure']['address']=='0x489b70' and len(actor_rea['normalized_result']['assembly'])==74
prey_rea=read('metadata/actor-prey-rea-function.json')['data']
assert prey_rea['operation']=='analyze_function' and prey_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert prey_rea['normalized_result']['procedure']['address']=='0x49bd40' and len(prey_rea['normalized_result']['assembly'])==668
animation_rea=read('metadata/animation-update-rea-function.json')['data']
assert animation_rea['operation']=='analyze_function' and animation_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert animation_rea['normalized_result']['procedure']['address']=='0x41fdb0' and len(animation_rea['normalized_result']['assembly'])==144
animations=read('metadata/animation-update-native.json')
assert animations['native_sha256']==rea['subject']['digest']['sha256'] and len(animations['functions'])==10
assert len(animations['update_xrefs']['items'])==32 and not animations['update_xrefs']['has_more']
selector=read('metadata/animation-building-selector-native.json')
assert len(selector['function']['items'])==35 and len(selector['xrefs']['items'])==len(selector['C_consumers'])==4
minimap_rea=read('metadata/minimap-rea-function.json')['data']
assert minimap_rea['operation']=='analyze_function' and minimap_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert minimap_rea['normalized_result']['procedure']['address']=='0x43e810' and len(minimap_rea['normalized_result']['assembly'])==444
predicate_rea=read('metadata/actor-predicate-rea-function.json')['data']
assert predicate_rea['operation']=='analyze_function' and predicate_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert predicate_rea['normalized_result']['procedure']['address']=='0x4387d0' and len(predicate_rea['normalized_result']['assembly'])==436
graphics_rea=read('metadata/graphics_53f250_rea_function.json')['data']
assert graphics_rea['operation']=='analyze_function' and graphics_rea['subject']['digest']['sha256']==rea['subject']['digest']['sha256']
assert graphics_rea['normalized_result']['procedure']['address']=='0x53f250' and len(graphics_rea['normalized_result']['assembly'])==165
inventory=read('metadata/actor-callback-inventory.json')
assert inventory['distinct_callback_bodies']==102 and inventory['implemented_distinct_bodies']==11 and inventory['missing_distinct_bodies']==91 and not inventory['all_callbacks_complete']
for source,digest in inventory['source_hashes'].items():assert sha(source)==digest,'Actor inventory is stale: '+source
source_paths.update(inventory['source_hashes'])
export_path='metadata/actor-callbacks-rea/index.json';export=read(export_path);exported=[];batches=[]
assert export['completed'] and not export['C_imported'] and export['native_sha256']==rea['subject']['digest']['sha256']
for entry in export['batches']:
 path='metadata/actor-callbacks-rea/'+entry['file'];batch=read(path);result=batch['normalized_result']
 assert batch['operation']=='batch_decompile' and batch['subject']['digest']['sha256']==export['native_sha256']
 assert batch['evidence_id']==entry['evidence_id'] and result['failed']==0 and result['succeeded']==entry['count']
 assert len(result['items'])==entry['count'] and all(x['status']=='ok' and isinstance(x['pseudocode'],str) for x in result['items'])
 exported.extend(x['address'] for x in result['items']);batches.append(stamp(path))
assert exported==export['addresses'] and len(exported)==len(set(exported))==102
assert set(exported)=={x['address'] for x in inventory['entries']}
report={'format':1,'generated_at':datetime.now(timezone.utc).isoformat(),'complete_menu_reconstruction':False,'standalone_gameplay_proven':False,
 'city_confirmation_diagnostic':{**stamp(city_diagnostic_path),'post_load_preparation_passed':True,'flood_controller_passed':True,'actor_update_passed_for_fixture':True,'animation_update_passed_for_fixture':True,'minimap_update_passed_for_fixture':True,'main_map_renderer_returned_for_fixture':True,'gameplay_mode_1_reached':True,'mission_dialog_mode_5_reached':True,'bounded_45_second_run_survived':True,'completed_city_frame_proven':True,'owned_C_entry_and_OK_flow_passed':True,'preview_class_bootstrap_observed':True,'city_visual_fidelity_proven':False,'city_entry_passed':False,
 'next_failure':'Tool selector00405200, region validator00482720, projection fallback004f0ab0, actor coordinate004f0d30 and remaining scrolling/overlay/message consumers need reconstruction. HUD origins, actor drawing and mirrored pixels are restored; top-menu highlights, world-space fidelity and complete gameplay remain unproved.',
 'frame':stamp(city_frame_path),'image':stamp(city_image),'visually_inspected':True,'log':stamp(city_diagnostic['log'])},
 'message_panel_input_integration':message_input_integration,
 'city_preparation_REA_evidence':stamp('metadata/city-prepare-rea-function.json'),
 'terrain_redraw_REA_evidence':stamp('metadata/terrain-redraw-rea-function.json'),
 'terrain_redraw_REA_evidence_id':terrain_rea['evidence_id'],
 'city_prices_REA_evidence':stamp('metadata/city-price-rea-function.json'),
 'city_prices_REA_evidence_id':price_rea['evidence_id'],
 'flood_simulation_REA_evidence':stamp('metadata/flood-simulation-rea-function.json'),
 'flood_simulation_REA_evidence_id':flood_rea['evidence_id'],
 'actor_update_REA_evidence':stamp('metadata/actor-update-rea-function.json'),'actor_update_REA_evidence_id':actor_rea['evidence_id'],
 'actor_prey_REA_evidence':stamp('metadata/actor-prey-rea-function.json'),'actor_prey_REA_evidence_id':prey_rea['evidence_id'],
 'animation_update_REA_evidence':stamp('metadata/animation-update-rea-function.json'),'animation_update_REA_evidence_id':animation_rea['evidence_id'],
 'menu_pixel_regression':stamp('metadata/minimap-menu-frame-regression.json'),
 'minimap_REA_evidence':stamp('metadata/minimap-rea-function.json'),'minimap_REA_evidence_id':minimap_rea['evidence_id'],
 'graphics_53f250_REA_evidence':stamp('metadata/graphics_53f250_rea_function.json'),'graphics_53f250_REA_evidence_id':graphics_rea['evidence_id'],
 'actor_predicate_REA_evidence':stamp('metadata/actor-predicate-rea-function.json'),'actor_predicate_REA_evidence_id':predicate_rea['evidence_id'],
 'high_scores_integration':stamp('metadata/high-scores-menu-integration.json'),'high_scores_REA_evidence':scores['REA_evidence'],'high_scores_REA_evidence_id':scores['REA_evidence_id'],
 'parallel_restoration_notes':stamp('metadata/PARALLEL_RECONSTRUCTION.md'),'bounded_dialog_guard_review':stamp('metadata/render-dialog-guard-review.json'),
 'minimap_restored':['0043e810 complete padded minimap scan, full-width fields and correct BYTE/WORD grids, terrain priority and actor color pairs.','00439980 all overlay dispatch, 264-byte building stride, actual 0043e770 lookup and four-sprite footprints.','0043e670 wrapped geometry, native clamping/halving/mask and all three linked-actor classifier loops.','0053f4a0 global/explicit sprite manager plus actual 0040fbe0/0040fc10 paired RGB565 pixel writes. Deep metric, actor predicate, large-sprite and camera-outline endpoints remain controlled.'],
 'animation_update_restored':['0041fdb0 explicit record, native field widths/frame wrap, transition selection, actual RNG and state setter.','All 32 native receiver bindings through eight registries; complete 0041ff60 ordering covers up to 253 records.','004e59c0 actual building ID/field widths, all four consumed-return bindings and conditional temple registry.','Complete native/C algorithms are composed without controlled deep endpoints; record initialization and later rendering remain outside this proof.'],
 'actor_callback_inventory':stamp('metadata/actor-callback-inventory.json'),'actor_callback_bodies_missing':inventory['missing_distinct_bodies'],
 'actor_callback_decompiler_reference':{**stamp(export_path),'batch_reports':batches,'decompiled_entries':102,'imported_into_C':False},
 'actor_update_restored':['00489b70 full-width profile/actor counters, 388-byte stride, signed links and all native callback/reload ordering.','C registry routes the original RET, prey, two human and seven additional actor controllers; other callbacks abort explicitly.','Complete 668-instruction prey state, RNG/history, destination/formation strides and sprite selection; scratch EAX is unused by the void table caller.','622 controller and 3,048 prey native cases, with all six/26 selected probes reached. Deep endpoints are controlled; another 91 callback bodies remain.'],
 'flood_simulation_restored':['Omitted 004bd310 annual-period initializer before C game startup.','Complete 004be770 cycle/phase/growth controller, all dropped x87 inputs and full-width clock globals.','004bd850 explicit region/list traversal, cursor reload and actual 004be0d0 growth helper.','004be490/004be580 seasonal receiver bindings and actual 004bd380/004bd550/004bd580/004bd590 quality algorithms.','4,564 native cases with all 21 selected probes; warning, payload, relocation and person endpoints remain controlled. 004bd950 receiver binding alone does not prove its remaining deep algorithm.'],
 'city_prices_restored':['0044baf0 explicit global/alternate list receiver and 26 verified native global callers.','0044d4c0 explicit record receiver at all ten C call sites.','00443330 real 36-element accumulator, full-width player/population state, wrapped quantities and actual building/clamp helpers.','1,691 native cases; all eight selected probes reached; complex resource-demand helper remains controlled.'],
 'music_tick_restored':['00539820 complete four-phase update with explicit manager receiver and native clock comparison.','0055f38d explicit sound receiver and stream-handle reload after status callback.','00562348 DWORD RNG state, native multiply/add wrap and 15-bit return.','1,668 native cases; all five selected probes reached; time/playback/Miles API endpoints controlled. Existing 324-case constructor/parser suite refreshed.'],
 'terrain_redraw_restored':['Complete 004767d0 clipping, building-retention cases, floodplain selection and both ground passes.','Actual 0047c140 footprint writes, 0047c880 availability, 0046f9a0 complete predicate EAX and 0047db00/00484f40 neighbor data.','4,624 native cases; all 17 selected branch probes reached.','0047a1c0/0047a1f0 boundary widths, defined Boolean return and nineteen one-argument caller bindings; 882 additional cases.'],
 'city_preparation_restored':['Native geometry/predicate, complete clear/borders/RNG/ground initialization.','Explicit world and region receivers, constructor/destructor/sorted insertion and region propagation/build.','Ordered saved-game 004d5100/004d52a0 controllers and actual city coverage/household/bin helpers.','Full-width terrain neighborhood counters, byte grids and rectangle/refresh pass contracts.','2,449 city-preparation and 2,131 terrain-update/boundary native cases; deep endpoint limitations retained per suite.'],
 'reference_project':str(ROOT.parent/'PharaohRewrite'),'reference_role':'Address/function map and hypotheses; native executable instructions remain the behavioral authority.',
 'original_game_process_executed':False,'computer_UI_automation_used':False,'original_machine_code_fallback_used':False,
 'native_reference_sha256':'fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d',
 'REA_evidence':stamp('metadata/menu-dialog-rea-function.json'),'REA_operation':rea['operation'],'REA_evidence_id':rea['evidence_id'],
 'native_cases_passed':total,'native_suites':suites,'builds':builds,'owned_C_integration':{'log':stamp(log),'expected_bounded_timeout':True,'confirmed_flow':campaign['confirmed_flow'],'frames':frames,'created_fixture_profile':stamp(profile)},
 'dialog_integration':stamp('metadata/menu-dialog-integration.json'),
 'mission_integration':stamp('metadata/mission-menu-integration.json'),'mission_REA_evidence':mission['REA_evidence'],'mission_REA_evidence_id':mission['REA_evidence_id'],
 'mission_chunks_REA_evidence':stamp('metadata/mission-chunks-rea-function.json'),'mission_chunks_REA_evidence_id':chunks_rea['evidence_id'],
 'legacy_mission_REA_evidence':stamp('metadata/legacy-mission-rea-function.json'),'legacy_mission_REA_evidence_id':legacy_rea['evidence_id'],
 'mission_chunk_safety_differences':read('metadata/chunk-io-emulation.json')['safety_differences'],
 'mission_route_safety_differences':read('metadata/mission-routes-emulation.json')['safety_differences'],
 'nonempty_mission_integration':stamp('metadata/mission-map-integration.json'),'mission_route_REA_evidence':map_integration['REA_evidence'],'mission_route_REA_evidence_id':map_integration['REA_evidence_id'],
 'load_save_integration':stamp('metadata/load-save-menu-integration.json'),'load_save_REA_evidence':load_save['REA_evidence'],'load_save_REA_evidence_id':load_save['REA_evidence_id'],
 'campaign_integration':stamp('metadata/campaign-menu-integration.json'),'campaign_REA_evidence':campaign['REA_evidence'],'campaign_REA_evidence_id':campaign['REA_evidence_id'],
 'implemented':['Full-width input, hover and button metadata; explicit C callback routing for recovered actions.','Explicit file-list receivers, allocation/sort contracts and a native duplicate-delete repair.','Text-input registration/editing/caret and layout/coordinate registry contracts.','Panel and small-icon rendering coordinates and complete sprite/font IDs.','Native profile persistence, old/partial profile fallback, explicit receivers at all reviewed callers.','Family creation, registry selection/scrolling and options rendering.','Template expansion/lookup contexts, local wrappers, missing constructor and greeting/player-name C callbacks.','Modal layout, measured text selection, yes/no callbacks and callback-before-return-mode ordering.','Family deletion through original .sav enumeration and file/directory lifetime order.','Correct glyph/metric strides for word widths and full-width wrapping state.'],
 'safety_differences':['Old-profile unread stack bytes have defined zero initialization. Malformed active-name/count data is rejected.','The native file-list duplicate free is removed.','Deleted-save paths exceeding the native 260-byte stack buffer are skipped.','Unreconstructed callbacks abort explicitly instead of jumping to original code.'],
 'family_options_restored':['004cb800 history/resume','004cb860 mission selection/confirmation','004cb910 load game','004cb940 custom missions','004cb970 return to registry','0041ba80 explicit completion predicate'],
 'campaign_selector_restored':['0041be10 foreground and 0041c2d0 background with full state widths and native 28-byte rectangle stride','0041bb30 availability, 0041bb10 choose, 0041bb70 confirm/movie selection','0041c310/0041c350 tabs and 0041f650 back callback'],
 'mission_selector_restored':['Explicit 0041dfc0/0041dfe0 receivers and full-width local metadata','0041e5c0 foreground, 0041f220 rows, 0041e1b0 scrollbar, 0041e470 score','0041f460 scroll, 0041f4d0 drag, 0041f670 activate and 0041f870 initialize','0041e0e0 goals/results toggle, 0041f5c0 confirmation and 00402e50 difficulty callback','Mode 61 input and foreground dispatch with all native arguments'],
 'town_menu_restored':['0044bb90 explicit iterator and all 24 native global-receiver callers','0041e270 native recursive/tail-link connectivity walk and 0041e3b0 unconnected count','Complete 150-record list and 24-byte metadata tail, total 18628 bytes'],
 'mission_chunks_restored':['0055bdb0/0055be50 real C codec callbacks and 52-byte context','0055c0f0 CRC32, 0055c000 decoder wrapper and 0055bcf0 block envelope','004d0e10 native descriptor resizing and full-width versions/counts; return consumed as AL','004d0c60 and 004d0d40 old-record migrations through explicit C routing','Complete C chunk/codec pipeline agrees for 712 chunks across four complete v147/v160 mission/save streams'],
 'mission_loaders_restored':['004d4720 explicit route receiver, native interpolation/float conversions and 64-byte graphics stride','004d4ae0/004d41e0 full-width controller contracts and explicit route/settings receivers','004d5820/004d58b0 wrapper ordering, signed mission IDs/years, full difficulty and correct player stride','004bd280/004bd2d0/004bd330/004bd360 settings receivers and local 004bd650 record initialization','Complete v160 MAP path agrees across two derived fixtures, including four real routes with 240 points'],
 'load_save_menu_restored':['00402450 load entry, omitted 00402540 save entry and 00402630 delete entry','00402720 confirmation, 00402980 scroll, 004029d0 drag and omitted 00402a80 row selection','00532340/005325f0 native file-menu renderers and 00532830 scrollbar with full-width state','004d8310 mode-13 branch and actual callback routing','1,968 native cases, three viewport offsets and a real C saved-file selection/cancel flow'],
 'legacy_mission_restored':['004d0a40 all ten 820-byte history snapshots and explicit 28-byte object receiver','004f67e0 native metadata-only import with resize/free; 004f6810 export and signed index normalization','004d19e0 complete 98-byte city conversion, 004d40c0 town links and byte-stride 004493c0','004d32f0 exact walker/building strides, byte frame fields and full-width counters','004d3d00 byte strides; six defaults and building compatibility helpers','0041fca0 explicit receiver, 40/48-byte table strides, selection and 65 native caller bindings','1,566 bounded compatibility comparisons and four complete SAV loads with 724 post-load block hashes plus 106 state cases'],
 'numeric_text_restored':['004ccc30/004cccb0 full-width advances and actual negative-year draw arguments','004cce00/004cce90/004ccf20 byte-addressed suffix/prefix composition; all digits retained','004cc5c0 signed negation has defined native wrap behavior'],
 'remaining_work':['Integrate builtin v147 mission selection into the owned C preview; complete SAV loading now matches four native streams.',
 'Reconstruct tool selection00405200, deep region validation00482720, projection fallback004f0ab0, actor coordinates004f0d30 and remaining scrolling/overlay consumers. The restored preview dispatcher and1327 seed tuples do not establish actual toolbar selection or complete construction gameplay. Entry, real OK and a completed mode1 frame survive the bounded45-second run; top-menu/world-space pixel fidelity remains unproved.',
 'Reconstruct the remaining91 actor callback bodies and deep actor/flood/model algorithms, including pre-v147 migrations.',
 'Recover Go to city, mission preparation and briefing after campaign confirmation, including004181e0.',
 'Verify editor, settings and remaining root menu branches; high scores has native and owned-C entry/scroll/drag/back evidence.',
 'Complete message game-action callbacks40A0E0,4E2360/4E2380,44EE30/44EEA0/44EEF0 and other deeper endpoints. Their selection is verified but their full behaviors are outside the current input proof.',
 'Recover other dropped floating-point expressions as their functions are reconstructed; the ambient fix deliberately leaves the shared legacy x87 ABI unchanged.',
 'Extend actual C framebuffer acceptance beyond1024x768. General playable independent gameplay has not been established.'],
 'source_hashes':{source:sha(source) for source in sorted(source_paths)}}
(ROOT/'metadata/menu-reconstruction-progress.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'checkpoint':'metadata/menu-reconstruction-progress.json','native_cases':total,'debug_tests':test_count,'release_tests':test_count,'complete_menu_reconstruction':False},indent=2))
